#include<iostream>
#include<vector>
#include<omp.h>

using namespace std;


//BUBBLE SORT
void bubblesort(vector<int>& arr){
    int n=arr.size();
    for(int i=0;i<n;i++){
        for(int j=0;j<n-i-1;j++){
            if(arr[j]>arr[j+1]){
                swap(arr[j],arr[j+1]);
            }
        }
    }

// PARALLEL VERSION 
void P_bubblesort(vector<int>& arr){
    int n=arr.size();
    for(int i=0;i<n;i++){
        //even phase
        #pragma omp parallel for
        for(int j=0;j<n-1;j+=2){
            if(arr[j]>arr[j+1]){
                swap(arr[j],arr[j+1]);
            }

            //odd phase
            #pragma omp parallel for
            for(int j=1;j<n-1;j+=2){
                if(arr[j]>arr[j+1])
                swap(arr[j],arr[j+1]);
            }
        }
    }
}






}

//MERGE SORT
void merge(int left, int mid, int right, vector<int>& arr){
    vector<int> Leftarr(arr.begin(),arr.begin()+mid+1);
    vector<int> Rightarr(arr.begin()+mid+1,arr.begin+right+1);

    int i=0,j=0,k=left;
    while(i<Leftarr.size() && j<Rightarr.size()){
        if(Leftarr[i]<=Rightarr[j])
        arr[k++]=Leftarr[i++];
        else
        arr[k++]=Rightarr[j++];

    }

    while(i<=Leftarr.size()){
        arr[k++]=Leftarr[i++];
    }

    while(j<=Rightarr.size()){
        arr[k++]=Rightarr[j++];
    }
}


void rec_mergesort(int start,int end, vector<int>& arr){
    if(start>=end) return;
    int mid=start+(end-start)/2;
    
    rec_mergesort(start,mid,arr);
    rec_mergesort(mid+1,end,arr);

    merge(start,mid,end,arr);

}

void MergeSort(vector<int>& arr){
    rec_mergesort(0,arr.size()-1,arr);
}

//Parallel Mergesort
void P_rec_mergesort(int start,int end,vector<int>& arr){
    if(start>=end) return;
    int mid=start+(end-start)/2;
    
    #pragma omp task shared(arr)
    P_rec_mergesort(start,mid,arr);

    #pragma omp task shared(arr)
    P_rec_mergesort(mid+1,end,arr);

    #pragma omp taskwait
    merge(start,mid,end,arr);
}


void P_MergeSort(vector<int>& arr){
    #pragma omp parallel{
        #pragma omp single{
            P_rec_mergesort(0,arr.size()-1,arr);
        }
    }
}

int main(){
    int l;
    cout<<"Enter the length of the array: "<<endl;
    cin>>l;
    vector<int> arr(n);
    for(int i=0;i<l;i++) cin>>arr[i];

    vector<int> arr1=arr;
    vector<int> arr2=arr;

    cout<<"Sequential Merge Sort"<<" ";
    double start_seq_m=omp_get_wtime();
    MergeSort(arr1);
    double end=omp_get_wtime();
    cout<<" Time Taken: "<<(end-start_seq_m)<<endl;

    for(int i=0;i<arr1.size();i++){
        cout<<arr1[i]<<" ";
    }


}