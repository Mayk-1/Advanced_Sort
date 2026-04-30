#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void countingSort(vector<int>& arr) {
    if (arr.empty()) return;

    int maxVal = *max_element(arr.begin(), arr.end());
    vector<int> count(maxVal + 1, 0);
    vector<int> output(arr.size());

    for (int num : arr) {
        count[num]++;
    }

    for (int i = 1; i <= maxVal; i++) {
        count[i] += count[i - 1];
    }

    for (int i = arr.size() - 1; i >= 0; i--) {
        output[count[arr[i]] - 1] = arr[i];
        count[arr[i]]--;
    }

    arr = output;
}

int main() {
    vector<int> arr = {5, 2, 1, 7, 3, 2, 5, 0};
    
    cout << "Original: ";
    for (int n : arr) cout << n << " ";
    
    countingSort(arr);
    
    cout << "\nResultado Counting Sort: ";
    for (int n : arr) cout << n << " ";
    cout << endl;

    return 0;
}