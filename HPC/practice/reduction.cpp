// Implement Min, Max, Sum and Average operations using Parallel Reduction.
// Measure the performance of sequential and parallel algorithms.

#include<iostream>
#include<climits>
#include<omp.h>

using namespace std;

const int N=1e7;

void generateData(int* arr,int size){
    for(int i=0;i<size;i++){
        arr[i]=rand()%1000;
    }
}


void Sequential_operations(int* arr, int size){
    long sum_seq=0;
    int min_seq=INT_MAX, max_seq=INT_MIN, avg_seq=0;
    
    double start=omp_get_wtime();
    for(int i=0;i<size;i++){
        sum_seq+=arr[i];
        min_seq=min(min_seq,arr[i]);
        max_seq=max(max_seq,arr[i]);        
    }
    avg_seq=(float)sum_seq/N;
    double end=omp_get_wtime();
    cout<<"*********"<<" Sequential Results "<<"***********"<<endl;
    cout<<"Sum:"<<" "<<sum_seq<<", ";
    cout<<"Min: "<<min_seq<<" ";
    cout<<"Max: "<<max_seq<<" ";
    cout<<"Avg: "<<avg_seq<<endl;

    cout<<"Time Taken: "<<(end-start)*1000<<"ms"<<endl;


}

void Parallel_operations(int* arr, int size){
    long sum_para=0;
    int min_para=INT_MAX,max_para=INT_MIN,avg_para=0;

    double start=omp_get_wtime();
    #pragma omp parallel for reduction(min:min_para)
    for(int i=0;i<size;i++){
        min_para=min(min_para,arr[i]);
    }

    #pragma omp parallel for reduction(+:sum_para)
    for(int i=0;i<size;i++){
        sum_para+=arr[i];
    }

    #pragma omp parallel for reduction(max:max_para)
    for(int i=0;i<size;i++){
        max_para=max(max_para,arr[i]);
    }



    avg_para = (float)sum_para / N;

    double end = omp_get_wtime();
    cout<<"*********"<<" Parallel Results "<<"***********"<<endl;
    cout<<"Sum:"<<" "<<sum_para<<", ";
    cout<<"Min: "<<min_para<<" ";
    cout<<"Max: "<<max_para<<" ";
    cout<<"Avg: "<<avg_para<<endl;

    cout<<"Time Taken: "<<(end-start)*1000<<"ms"<<endl;

}

int main(){
int* arr=new int[N];
generateData(arr,N);

Sequential_operations(arr,N);
Parallel_operations(arr,N);
return 0;
}
