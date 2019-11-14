#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

#define pb push_back
#define ll long long
#define maxn 300005
#define fr(i,j,k) for(int i=j;i<k;i++)
#define f(n) fr(i,0,n)
#define f1(n) fr(i,1,n+1)
#define ms(i) memset(i,0,sizeof(i));
#define ms1(i) memset(i,-1,sizeof(i));
#define F first
#define S second
const int mod = 1e9 + 7;
void test(int n){
    for(int i = 0 ; i < 10000 ; i++){
        for(int j = 0 ; j < 100000 ; j++){
            int x = i ^ i ^ j ^ j;
        }
    }
    cout << n << endl;
}
int main(){
    //ios_base::sync_with_stdio(0);
    //cin.tie(0);
    #pragma omp parallel 
    {
    #pragma omp for
    for(int i = 0 ; i < 10 ; i++){
        test(i);
    }
    }
}
