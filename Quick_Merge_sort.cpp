#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <string>

using namespace std;

typedef chrono::high_resolution_clock Clock;

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSortRecursive(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSortRecursive(arr, low, pi - 1);
        quickSortRecursive(arr, pi + 1, high);
    }
}

void merge(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSortRecursive(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSortRecursive(arr, l, m);
        mergeSortRecursive(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void runQuickSort(vector<int>& arr) {
    if (!arr.empty()) quickSortRecursive(arr, 0, arr.size() - 1);
}

void runMergeSort(vector<int>& arr) {
    if (!arr.empty()) mergeSortRecursive(arr, 0, arr.size() - 1);
}

void test(vector<int> data, void (*func)(vector<int>&), const string& name) {
    auto t1 = Clock::now();
    func(data);
    auto t2 = Clock::now();
    cout << name << " took " << chrono::duration_cast<chrono::microseconds>(t2 - t1).count() << " us" << endl;
}

int main() {
    const int N = 100000;
    vector<int> original(N);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 1000000);
    for (int i = 0; i < N; i++) original[i] = dis(gen);

    cout << "Analizando rendimiento para n = " << N << "..." << endl;
    cout << "------------------------------------------" << endl;

    test(original, runQuickSort, "QuickSort");
    test(original, runMergeSort, "MergeSort");

    return 0;
}