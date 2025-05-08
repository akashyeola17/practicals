#include <iostream>
#include <omp.h>
#include <vector>
using namespace std;

// Merge two sorted subarrays
void merge(vector<int> &arr, int l, int m, int r) {
    vector<int> left(arr.begin() + l, arr.begin() + m + 1);
    vector<int> right(arr.begin() + m + 1, arr.begin() + r + 1);

    int i = 0, j = 0, k = l;
    while (i < left.size() && j < right.size())
        arr[k++] = (left[i] <= right[j]) ? left[i++] : right[j++];
    while (i < left.size()) arr[k++] = left[i++];
    while (j < right.size()) arr[k++] = right[j++];
}

// Sequential Merge Sort
void mergeSort(vector<int> &arr, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}

// Parallel Merge Sort using OpenMP
void parallelMergeSort(vector<int> &arr, int l, int r, int depth = 0) {
    if (l >= r) return;
    int m = l + (r - l) / 2;

    if (depth < 4) {
#pragma omp parallel sections
        {
#pragma omp section
            parallelMergeSort(arr, l, m, depth + 1);
#pragma omp section
            parallelMergeSort(arr, m + 1, r, depth + 1);
        }
    } else {
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
    }

    merge(arr, l, m, r);
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter the elements:\n";
    for (int &x : arr) cin >> x;

    vector<int> arrSeq = arr;

    double start = omp_get_wtime();
    mergeSort(arrSeq, 0, n - 1);
    double seqTime = omp_get_wtime() - start;

    start = omp_get_wtime();
    parallelMergeSort(arr, 0, n - 1);
    double parTime = omp_get_wtime() - start;

    cout << "\nSorted array:\n";
    for (int x : arr) cout << x << " ";

    cout << "\n\nSequential Time: " << seqTime << " seconds";
    cout << "\nParallel Time  : " << parTime << " seconds\n";

    return 0;
}
