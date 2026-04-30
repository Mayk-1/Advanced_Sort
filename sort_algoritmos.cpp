#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <string>
using namespace std;
using namespace chrono;

// COUNTING SORT   O(n + k)

vector<int> countingSort(vector<int> arr) {
    int maxVal = *max_element(arr.begin(), arr.end());
    int minVal = *min_element(arr.begin(), arr.end());
    int rango  = maxVal - minVal + 1;
    vector<int> count(rango, 0), output(arr.size());
    for (int num : arr) count[num - minVal]++;
    for (int i = 1; i < rango; i++) count[i] += count[i-1];
    for (int i = (int)arr.size()-1; i >= 0; i--) {
        output[count[arr[i]-minVal]-1] = arr[i];
        count[arr[i]-minVal]--;
    }
    return output;
}

// RADIX SORT   O(d * (n + k))

void countingSortRadix(vector<int>& arr, int exp) {
    int n = (int)arr.size();
    vector<int> output(n), count(10, 0);
    for (int i = 0; i < n; i++) count[(arr[i]/exp)%10]++;
    for (int i = 1; i < 10; i++) count[i] += count[i-1];
    for (int i = n-1; i >= 0; i--) {
        output[count[(arr[i]/exp)%10]-1] = arr[i];
        count[(arr[i]/exp)%10]--;
    }
    for (int i = 0; i < n; i++) arr[i] = output[i];
}
vector<int> radixSort(vector<int> arr) {
    int maxNum = *max_element(arr.begin(), arr.end());
    for (int exp = 1; maxNum/exp > 0; exp *= 10)
        countingSortRadix(arr, exp);
    return arr;
}

// QUICKSORT   O(n log n)

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high], i = low-1;
    for (int j = low; j < high; j++)
        if (arr[j] <= pivot) swap(arr[++i], arr[j]);
    swap(arr[i+1], arr[high]);
    return i+1;
}
void _quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        _quickSort(arr, low, pi-1);
        _quickSort(arr, pi+1, high);
    }
}
vector<int> quickSort(vector<int> arr) {
    _quickSort(arr, 0, (int)arr.size()-1);
    return arr;
}

// MERGESORT   O(n log n)

void _merge(vector<int>& arr, int l, int m, int r) {
    vector<int> L(arr.begin()+l, arr.begin()+m+1);
    vector<int> R(arr.begin()+m+1, arr.begin()+r+1);
    int i=0, j=0, k=l;
    while (i<(int)L.size() && j<(int)R.size())
        arr[k++] = (L[i]<=R[j]) ? L[i++] : R[j++];
    while (i<(int)L.size())  arr[k++]=L[i++];
    while (j<(int)R.size()) arr[k++]=R[j++];
}
void _mergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l+(r-l)/2;
        _mergeSort(arr,l,m); _mergeSort(arr,m+1,r);
        _merge(arr,l,m,r);
    }
}
vector<int> mergeSort(vector<int> arr) {
    _mergeSort(arr, 0, (int)arr.size()-1);
    return arr;
}

vector<int> leerArchivo(const string& ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cerr << "  ERROR: No se pudo abrir '" << ruta << "'\n";
        cerr << "  Asegurate de que datos.txt este en la misma carpeta.\n";
        exit(1);
    }
    vector<int> datos;
    int num;
    while (archivo >> num) datos.push_back(num);
    archivo.close();
    return datos;
}

double memCountingSort(int n, int k) { return (n + k) * 4.0 / 1024.0; }
double memRadixSort(int n)           { return (n + 10) * 4.0 / 1024.0; }
double memQuickSort(int n)           {
    int logn = 0, tmp = n;
    while (tmp > 1) { logn++; tmp /= 2; }
    return logn * 32.0 / 1024.0;
}
double memMergeSort(int n)           { return n * 4.0 / 1024.0; }


void ejecutarCaso(const string& titulo, const vector<int>& arr, int k) {
    int n = (int)arr.size();
    cout << "\n" << string(72,'=') << "\n";
    cout << "  " << titulo << "\n";
    cout << "  N = " << n << " numeros | Rango: "
         << *min_element(arr.begin(),arr.end()) << " - "
         << *max_element(arr.begin(),arr.end()) << "\n";
    cout << string(72,'=') << "\n";
    cout << "  " << left  << setw(22) << "Algoritmo"
         << right << setw(12) << "Tiempo (ms)"
                 << setw(16) << "Memoria (KB)"
         << "   Tipo\n";
    cout << "  " << string(67,'-') << "\n";

    struct Info { string nombre; double t; double mem; };
    vector<Info> rows;

    auto ejecutar = [&](const string& nom,
                        vector<int>(*fn)(vector<int>),
                        double mem_kb,
                        const string& tipo)
    {
        auto ini = high_resolution_clock::now();
        fn(arr);
        auto fin = high_resolution_clock::now();
        double ms = duration<double,milli>(fin-ini).count();
        cout << "  " << left  << setw(22) << nom
             << right << setw(10) << fixed << setprecision(3) << ms    << " ms"
                     << setw(13) << fixed << setprecision(2) << mem_kb << " KB"
             << "   " << tipo << "\n";
        rows.push_back({nom, ms, mem_kb});
    };

    ejecutar("Counting Sort", countingSort, memCountingSort(n, k), "No Comparativo");
    ejecutar("Radix Sort",    radixSort,    memRadixSort(n),        "No Comparativo");
    ejecutar("QuickSort",     quickSort,    memQuickSort(n),        "Comparativo");
    ejecutar("MergeSort",     mergeSort,    memMergeSort(n),        "Comparativo");

    auto ref = countingSort(arr);
    auto rq  = quickSort(arr);
    cout << "\n  Verificacion (todos ordenaron igual): "
         << (ref == rq ? "OK" : "ERROR") << "\n";

    auto mr = min_element(rows.begin(),rows.end(),[](auto& a,auto& b){return a.t<b.t;});
    auto mm = min_element(rows.begin(),rows.end(),[](auto& a,auto& b){return a.mem<b.mem;});
    cout << "  Mas rapido:    " << mr->nombre
         << " (" << fixed << setprecision(3) << mr->t << " ms)\n";
    cout << "  Menos memoria: " << mm->nombre
         << " (" << fixed << setprecision(2) << mm->mem << " KB)\n";
}

int main() {
    cout << "\n" << string(72,'#') << "\n";
    cout << "#   PRACTICA 4 - ORDENAMIENTO NO COMPARATIVO (C++)                   #\n";
    cout << "#   Counting Sort y Radix Sort vs QuickSort y MergeSort              #\n";
    cout << string(72,'#') << "\n";

    string ARCHIVO = "datos.txt"; 
    cout << "\n  Leyendo datos desde: " << ARCHIVO << "\n";
    vector<int> arr = leerArchivo(ARCHIVO);
    cout << "  Total de numeros:  " << arr.size() << "\n";
    cout << "  Rango detectado:   "
         << *min_element(arr.begin(),arr.end()) << " - "
         << *max_element(arr.begin(),arr.end()) << "\n";

    int k = *max_element(arr.begin(),arr.end()) -
            *min_element(arr.begin(),arr.end()) + 1;

    ejecutarCaso("CASO 1: Datos del archivo (rango original)", arr, k);

    vector<int> arr2(arr.size());
    for (int i = 0; i < (int)arr.size(); i++) arr2[i] = arr[i] * 1000;
    int k2 = *max_element(arr2.begin(),arr2.end()) -
             *min_element(arr2.begin(),arr2.end()) + 1;
    ejecutarCaso("CASO 2: Rango grande (valores x1000)", arr2, k2);

    return 0;
}