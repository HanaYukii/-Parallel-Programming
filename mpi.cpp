#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#ifndef W
#define W 20
#endif

int main(int argc, char **argv){
    MPI_Init(NULL,NULL);
    MPI_Barrier(MPI_COMM_WORLD);
    int sz,cur,len;
    char name[MPI_MAX_PROCESSOR_NAME];
    MPI_Comm_size(MPI_COMM_WORLD, &sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &cur);
    MPI_Get_processor_name(name, &len);
    MPI_Barrier(MPI_COMM_WORLD);

    int L,iteration,seed;
    float d;
    int *temp;
    int *next;
    if(cur == 0){
        L = atoi(argv[1]);
        iteration = atoi(argv[2]);
        seed = atoi(argv[3]);
        for(int i = 1 ; i < sz ; i++){
            MPI_Send(&L,1,MPI_INT,i,0,MPI_COMM_WORLD);
            MPI_Send(&iteration,1,MPI_INT,i,0,MPI_COMM_WORLD);
            MPI_Send(&seed,1,MPI_INT,i,0,MPI_COMM_WORLD);
        }
    }
    else{
        MPI_Recv(&L,1,MPI_INT,0,0,MPI_COMM_WORLD);
        MPI_Recv(&iteration,1,MPI_INT,0,0,MPI_COMM_WORLD);
        MPI_Recv(&seed,1,MPI_INT,0,0,MPI_COMM_WORLD);
    }
    srand(seed);
    d = (float) random() / RAND_MAX * 0.2;
    temp = (int *)malloc(L*W*sizeof(int));
    next = (int *)malloc(L*W*sizeof(int));
    for(int i = 0 ; i < L ; i++){
        for(int j = 0 ; j < W ; j++){
            temp[i * W + j] = random() >> 3;
        }
    }
    int f1 = 0, f2 = 0, cnt = 0;
    int st = L / sz * cur;
    int ed = L / sz * (cur + 1)
    while(iteration--){
        f1 = 0;
        cnt++;
        for(int i = st ; i < ed ; i++){
            for(int j = 0 ; j < W ; j++){
                float t = temp[i * W + j] / d;
                t += temp[i * W + j] * (-4);
                t += temp[(i - 1 <  0 ? 0 : i - 1) * W + j];
                t += temp[(i + 1 >= L ? i : i + 1) * W + j];
                t += temp[i * W + (j - 1 <  0 ? 0 : j - 1)];
                t += temp[i * W + (j + 1 >= W ? j : j + 1)];
                t *= d;
                next[i * W + j] = t;
                if(next[i * W + j] != temp[i * W + j]){
                    f1 = 1;
                }
            }
        }

        MPI_Barrier(MPI_COMM_WORLD);
        if(cur > 0){
            MPI_Send(&next[st * W], W, MPI_INT, cur - 1, 0, MPI_COMM_WORLD);
        }
        if(cur < sz - 1){
            MPI_Send(&next[(ed - 1) * W], W, MPI_INT, cur + 1, 0, MPI_COMM_WORLD);
        }
        if(cur > 0){
            MPI_Recv(&next[(st - 1) * W], W, MPI_INT, cur - 1, 0, MPI_COMM_WORLD);
        }
        if(cur < sz - 1){
            MPI_Recv(&next[ed * W], W, MPI_INT, cur + 1, 0, MPI_COMM_WORLD);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Allreduce(&f1,&f2,1,MPI_INT,MPI_MAX,MPI_COMM_WORLD);
        if(f2 == 0){
            break;
        }
        MPI_Barrier(MPI_COMM_WORLD);
        int *tmp = temp;
        temp = next;
        next = tmp;
    }
    int Min1 = 100000000, Min2;
    MPI_Barrier(MPI_COMM_WORLD);
    for(int i = st ; i < ed ; i++){
        for(int j = 0 ; j < W ; j++){
            Min1 = min(Min1,temp[i * W + j]);
        }
    }
    MPI_Reduce(&Min,&Min2,1,MPI_INT,MPI_MIN,0,MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if(cur == 0) {
        printf("Size: %d*%d, Iteration: %d, Min Temp: %d\n", L, W, cnt, Min2);
    }
    MPI_Finalize();
    return 0;
}