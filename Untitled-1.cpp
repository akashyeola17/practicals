#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

void bubbleSortSequential(vector<int> nums, int n) {
    for(int i=0; i< n-1; i++) {
        for(int j=0; j<n-i-1; j++) {
            if(nums[j] > nums[j+1]) {
                swap(nums[j],nums[j+1]);
            }
        }
    }
}

void bubbleSortParallel(vector<int> nums, int n) {
    for(int i=0; i<n-1; i++) {
        if(i % 2 == 0) {
            #pragma omp parallel for
            for(int j=0; j<n-1; j+=2) {
                if(nums[j] > nums[j+1]) {
                    swap(nums[j], nums[j+1]);
                }
            }
        } else {
            #pragma omp parallel for
            for(int j=1; j<n-1; j+=2) {
                if(nums[j] > nums[j+1]) {
                    swap(nums[j], nums[j+1]);
                }
            }
        }
    }
}

int main() {
    int n;
    cout << "Enter the number of elements";
    cin >>n;

    vector<int> arr(n),arr_seq(n);

    for(int i=0; i<n; i++) {
        cout << "Enter the elements: "<<endl;
        cin >> arr[i];
    }

    arr_seq = arr;

    double start_time = omp_get_wtime();
    bubbleSortSequential(arr_seq,n);
    double end_time = omp_get_wtime();
    double seq_time = end_time-start_time;

    start_time = omp_get_wtime();
    bubbleSortParallel(arr, n);
    end_time = omp_get_wtime();
    double partime = end_time-start_time;

    cout << "\nSorted Array: \n";
    for(int i=0; i<n; i++) {
        cout << arr[i] << "";
    }
    cout << "\nSequential time: " <<seq_time <<" Seconds";
    cout << "\nParalled time: " <<partime <<" seconds";

    return 0;

}