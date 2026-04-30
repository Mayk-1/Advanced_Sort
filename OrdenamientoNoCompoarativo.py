import time
import random

#Counting Sort
def counting_sort(arr):
    if not arr: return arr
    max_val = max(arr)
    count = [0] * (max_val + 1)
    
    for num in arr:
        count[num] += 1
    
    sorted_arr = []
    for i in range(len(count)):
        sorted_arr.extend([i] * count[i])
    return sorted_arr

#Radix Sort
def counting_sort_for_radix(arr, exp):
    n = len(arr)
    output = [0] * n
    count = [0] * 10
    
    for i in range(n):
        index = (arr[i] // exp) % 10
        count[index] += 1
        
    for i in range(1, 10):
        count[i] += count[i - 1]
        
    i = n - 1
    while i >= 0:
        index = (arr[i] // exp) % 10
        output[count[index] - 1] = arr[i]
        count[index] -= 1
        i -= 1
        
    for i in range(n):
        arr[i] = output[i]

def radix_sort(arr):
    if not arr: return
    max_num = max(arr)
    exp = 1
    while max_num // exp > 0:
        counting_sort_for_radix(arr, exp)
        exp *= 10

def test_algorithm(algorithm_func, data, name):
    data_copy = data.copy()
    start_time = time.perf_counter()  
    
    if name == "Timsort":
        data_copy.sort()  #Timsort 
    else:
        algorithm_func(data_copy)
        
    end_time = time.perf_counter()
    duration_us = (end_time - start_time) * 1_000_000
    print(f"{name:15} took {duration_us:12.2f} us")

if __name__ == "__main__":
    N = 100000
    data = [random.randint(0, 10000) for _ in range(N)]
    
    print(f"Analizando {N} datos aleatorios (Rango 0-10,000):")
    print("-" * 45)
    
    test_algorithm(counting_sort, data, "Counting Sort")
    test_algorithm(radix_sort, data, "Radix Sort")
    test_algorithm(None, data, "Timsort")