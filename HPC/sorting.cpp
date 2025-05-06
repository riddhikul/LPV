#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n - i - 1; j++) {
            if(arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Parallel version using Odd-Even Transposition Sort
void P_bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n; i++) {
        // Even phase
        #pragma omp parallel for
        for (int j = 0; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
        // Odd phase
        #pragma omp parallel for
        for (int j = 1; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> leftArr(arr.begin() + left, arr.begin() + mid + 1);
    vector<int> rightArr(arr.begin() + mid + 1, arr.begin() + right + 1);
    int i = 0, j = 0, k = left;
    while(i < leftArr.size() && j < rightArr.size()) {
        if(leftArr[i] <= rightArr[j]) arr[k++] = leftArr[i++];
        else arr[k++] = rightArr[j++];
    }
    while(i < leftArr.size()) arr[k++] = leftArr[i++];
    while(j < rightArr.size()) arr[k++] = rightArr[j++];
}

void mergesortRec(int start, int end, vector<int>& arr) {
    if(start >= end) return;
    int mid = start + (end - start) / 2;
    mergesortRec(start, mid, arr);
    mergesortRec(mid + 1, end, arr);
    merge(arr, start, mid, end);
}

void mergesort(vector<int>& arr) {
    mergesortRec(0, arr.size() - 1, arr);
}

void P_mergesortRec(int start, int end, vector<int>& arr) {
    if(start >= end) return;
    int mid = start + (end - start) / 2;
    #pragma omp task shared(arr)
    P_mergesortRec(start, mid, arr);
    #pragma omp task shared(arr)
    P_mergesortRec(mid + 1, end, arr);
    #pragma omp taskwait
    merge(arr, start, mid, end);
}

void P_mergesort(vector<int>& arr) {
    #pragma omp parallel
    {
        #pragma omp single
        P_mergesortRec(0, arr.size() - 1, arr);
    }
}

int main() {
    cout << "Enter length of array: ";
    int n; cin >> n; vector<int> arr(n);
    for(int i = 0; i < n; i++) cin >> arr[i];

    vector<int> arr1 = arr;
    vector<int> arr2 = arr;
    vector<int> arr3 = arr;
    vector<int> arr4 = arr;

    double start, end, lms, pms, lbs, pbs;

    // Sequential Bubble Sort
    start = omp_get_wtime();
    bubbleSort(arr1);
    end = omp_get_wtime();
    lbs = end - start;

    cout << "\nBubble sort (Sequential): ";
    for(int i : arr1) cout << i << " ";
    cout << "\nTime: " << lbs << "s\n";

    // Parallel Bubble Sort
    start = omp_get_wtime();
    P_bubbleSort(arr2);
    end = omp_get_wtime();
    pbs = end - start;

    cout << "\nBubble sort (Parallel): ";
    for(int i : arr2) cout << i << " ";
    cout << "\nTime: " << pbs << "s\n";

    // Sequential Merge Sort
    start = omp_get_wtime();
    mergesort(arr3);
    end = omp_get_wtime();
    lms = end - start;

    cout << "\nMerge sort (Sequential): ";
    for(int i : arr3) cout << i << " ";
    cout << "\nTime: " << lms << "s\n";

    // Parallel Merge Sort
    start = omp_get_wtime();
    P_mergesort(arr4);
    end = omp_get_wtime();
    pms = end - start;

    cout << "\nMerge sort (Parallel): ";
    for(int i : arr4) cout << i << " ";
    cout << "\nTime: " << pms << "s\n";

    cout << "\nSummary:\n";
    cout << "Bubble Sort L: " << lbs << "s, P: " << pbs << "s\n";
    cout << "Merge Sort  L: " << lms << "s, P: " << pms << "s\n";

    return 0;
}
