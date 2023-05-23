#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
using namespace std;
// Sequential Bubble Sort
void sequentialBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Parallel Bubble Sort
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        #pragma omp parallel for
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                #pragma omp critical
                {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }
    }
}

// Sequential Merge Sort
void sequentialMergeSort(vector<int>& arr) {
    if (arr.size() <= 1) {
        return;
    }

    int mid = arr.size() / 2;
    vector<int> left(arr.begin(), arr.begin() + mid);
    vector<int> right(arr.begin() + mid, arr.end());

    sequentialMergeSort(left);
    sequentialMergeSort(right);

    merge(left.begin(), left.end(), right.begin(), right.end(), arr.begin());
}

// Parallel Merge Sort
void parallelMergeSort(vector<int>& arr) {
    if (arr.size() <= 1) {
        return;
    }

    int mid = arr.size() / 2;
    vector<int> left(arr.begin(), arr.begin() + mid);
    vector<int> right(arr.begin() + mid, arr.end());

    #pragma omp parallel sections
    {
        #pragma omp section
        parallelMergeSort(left);

        #pragma omp section
        parallelMergeSort(right);
    }

    merge(left.begin(), left.end(), right.begin(), right.end(), arr.begin());
}

int main() {
    int n = 10000;
    vector<int> arr(n);

    // Initialize the array with random values
    for (int i = 0; i < n; ++i) {
        arr[i] = rand() % 100;
    }

    vector<int> arrSequential = arr;
    vector<int> arrParallel = arr;

    // Measure the time for sequential Bubble Sort
    double startTime = omp_get_wtime();
    sequentialBubbleSort(arrSequential);
    double endTime = omp_get_wtime();

    cout << "Sequential Bubble Sort Time: " << endTime - startTime << " seconds" << endl;

    // Measure the time for parallel Bubble Sort
    startTime = omp_get_wtime();
    parallelBubbleSort(arrParallel);
    endTime = omp_get_wtime();

    cout << "Parallel Bubble Sort Time: " << endTime - startTime << " seconds" << endl;

    // Reset the arrays for Merge Sort
    arrSequential = arr;
    arrParallel = arr;

    // Measure the time for sequential Merge Sort
    startTime = omp_get_wtime();
    sequentialMergeSort(arrSequential);
    endTime = omp_get_wtime();

    cout << "Sequential Merge Sort Time: " << endTime - startTime << " seconds" << endl;

    // Measure the time for parallel Merge Sort
    startTime = omp_get_wtime();
    parallelMergeSort(arrParallel);
    endTime = omp_get_wtime();

    cout << "Parallel Merge Sort Time: " << endTime - startTime << " seconds" << endl;

    return 0;
}
