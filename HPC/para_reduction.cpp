// Implement Min, Max, Sum and Average operations using Parallel Reduction.
// Measure the performance of sequential and parallel algorithms.
#include <iostream>
#include <climits>
#include <omp.h>

using namespace std;

const int N = 1e7;  // Size of the array

void generateData(int* arr, int size) {
    for (int i = 0; i < size; ++i)
        arr[i] = rand() % 1000;  // Random values between 0 and 999
}

int main() {
    int* arr = new int[N];
    generateData(arr, N);

    int sum_seq = 0, min_seq = INT_MAX, max_seq = INT_MIN;
    float avg_seq = 0;

    // --- Sequential Version ---
    double start_seq = omp_get_wtime();

    for (int i = 0; i < N; ++i) {
        sum_seq += arr[i];
        min_seq = min(min_seq, arr[i]);
        max_seq = max(max_seq, arr[i]);
    }
    avg_seq = (float)sum_seq / N;

    double end_seq = omp_get_wtime();

    // --- Parallel Version ---
    int sum_par = 0, min_par = INT_MAX, max_par = INT_MIN;
    float avg_par = 0;

    double start_par = omp_get_wtime();
// 
    #pragma omp parallel for reduction(+:sum_par)
    for (int i = 0; i < N; ++i)
        sum_par += arr[i];

    #pragma omp parallel for reduction(min:min_par)
    for (int i = 0; i < N; ++i)
        min_par = min(min_par, arr[i]);

    #pragma omp parallel for reduction(max:max_par)
    for (int i = 0; i < N; ++i)
        max_par = max(max_par, arr[i]);

    avg_par = (float)sum_par / N;

    double end_par = omp_get_wtime();

    // --- Output Results ---
    cout << "\n--- Sequential Results ---" << endl;
    cout << "Sum: " << sum_seq << ", Min: " << min_seq << ", Max: " << max_seq << ", Avg: " << avg_seq << endl;
    cout << "Time Taken: " << (end_seq - start_seq) * 1000 << " ms" << endl;

    cout << "\n--- Parallel Results ---" << endl;
    cout << "Sum: " << sum_par << ", Min: " << min_par << ", Max: " << max_par << ", Avg: " << avg_par << endl;
    cout << "Time Taken: " << (end_par - start_par) * 1000 << " ms" << endl;

    // Clean up
    delete[] arr;
    return 0;
}