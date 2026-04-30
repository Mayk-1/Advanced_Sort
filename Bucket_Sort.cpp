#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <string>

using namespace std;

typedef chrono::high_resolution_clock Clock;

// bucket sort
void bucketSort(vector<int>& arr) {
    if (arr.empty()) return;

    int n = arr.size();
    int maxVal = *max_element(arr.begin(), arr.end());
    int minVal = *min_element(arr.begin(), arr.end());
    
    int range = maxVal - minVal + 1;
    vector<vector<int>> buckets(n);

    for (int i = 0; i < n; i++) {
        int bucketIndex = (1LL * (arr[i] - minVal) * (n - 1)) / range;
        buckets[bucketIndex].push_back(arr[i]);
    }

    arr.clear();
    for (int i = 0; i < n; i++) {
        sort(buckets[i].begin(), buckets[i].end()); 
        for (int x : buckets[i]) {
            arr.push_back(x);
        }
    }
}

void runBucketSort(vector<int>& arr) {
    bucketSort(arr);
}

void test(vector<int> data, void (*func)(vector<int>&), const string& name) {
    auto t1 = Clock::now();
    func(data);
    auto t2 = Clock::now();
    cout << name << " took " << chrono::duration_cast<chrono::microseconds>(t2 - t1).count() << " us" << endl;
}

int main() {
    const int N = 100000;
    
    //unifrme
    vector<int> uniformData(N);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> uniformDis(0, 1000000);
    for (int i = 0; i < N; i++) uniformData[i] = uniformDis(gen);

    //sesgada
    vector<int> skewedData(N);
    normal_distribution<> normalDis(500000, 1000); 
    for (int i = 0; i < N; i++) {
        skewedData[i] = (int)normalDis(gen);
    }

    cout << "Prueba Bucket Sort con N = " << N << endl;
    cout << "------------------------------------------" << endl;
    test(uniformData, runBucketSort, "Bucket Sort (Uniforme)");
    test(skewedData, runBucketSort, "Bucket Sort (Sesgado/Normal)");

    return 0;
}