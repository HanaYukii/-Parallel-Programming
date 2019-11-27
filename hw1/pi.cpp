#include<bits/stdc++.h>
#include<random>
#include<unistd.h>
#include<pthread.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<semaphore.h>
#define double float
using namespace std;
sem_t s,se[105];
unsigned long long number_in_circle;
vector<unsigned long long>tt;
void *sample(void* arg){
    int cur = *((int *)arg);
    std::default_random_engine generator (time(NULL)*cur);
    std::uniform_real_distribution<double>unif(0.0,1.0);
    unsigned long long cnt = 0;
    unsigned long long times = tt[cur];
    double x,y,dis;
    while(times--){
        x = unif(generator);
        y = unif(generator);
        dis = x*x + y*y;
        if (dis <= 1)
            cnt++;
    }
    sem_wait(&s);
    number_in_circle += cnt;
    sem_post(&s);
    sem_post(&se[cur]);
}
int main(int argc, char **argv)
{
    double pi_estimate, distance_squared, x, y;
    unsigned long long  number_of_cpu, number_of_tosses,toss;
    if ( argc < 2) {
        exit(-1);
    }
    number_of_cpu = atoi(argv[1]);
    number_of_tosses = atoi(argv[2]);
    if (( number_of_cpu < 1) || ( number_of_tosses < 0)) {
        exit(-1);
    }
    unsigned long long per = number_of_tosses / number_of_cpu ;
    unsigned long long last = number_of_tosses - per * (number_of_cpu - 1);
    pthread_t thread[number_of_cpu+5];
    number_in_circle = 0;
    sem_init(&s,1,0);
    for(int i=0;i<number_of_cpu;i++){
        if(i==0){
            tt.push_back(last);
        }
        else{
            tt.push_back(per);
        }
    }
    sem_post(&s);
    for(int i=0;i<number_of_cpu;i++){
        sem_init(&se[i],1,0);
        int *k = (int *)malloc(sizeof(int));
        *k = i;

        pthread_create(&thread[i],NULL,sample,(void *)k);
    }
    for(int i=0;i<number_of_cpu;i++){
        sem_wait(&se[i]);
    }
    pi_estimate = 4*number_in_circle/((double) number_of_tosses);

    printf("%.10lf\n",pi_estimate);
    return 0;
}
