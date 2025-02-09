import unittest
import numpy as np
import time
import sys

sizes = [10,100,1000,2000,5000]
sys.setrecursionlimit(2 * max(sizes))

def selection_sort(arr):
    n=len(arr)
    for i in range(n):
        min_idx=i
        for j in range(i+1, n):
            if (arr[j]<arr[min_idx]):
                min_idx=j
        if (min_idx!=i):
            arr[i], arr[min_idx]=arr[min_idx], arr[i]

def insertion_sort(a):
    for i in range(1,len(a)):
        e = a[i]
        j = i-1
        while j >= 0 and a[j] > e:
            a[j+1] = a[j]
            j = j-1
        a[j+1] = e

def bubble_sort(a):
    swap = True
    n = len(a)
    i = 0
    while swap and i < n-1:
        swap = False
        for j in range(n-1-i):
            if a[j] > a[j+1]:
                swap = True
                a[j], a[j+1] = a[j+1], a[j]
        i += 1

def merge_sort(a):
    if len(a)<=1: return
    m = len(a)//2
    left = a[:m]
    right = a[m:]
    merge_sort(left)
    merge_sort(right)
    merge(left, right, a)

def merge(a, b, r):
    na = 0
    nb = 0
    k = 0
    # print(f"merge {a} and {b} into {r}")
    while na < len(a) and nb < len(b):
        if a[na] <= b[nb]:
            r[k] = a[na]
            na = na + 1
        else:
            r[k] = b[nb]
            nb = nb + 1
        k = k + 1
    for i in range(na,len(a)):
        r[k] = a[i]
        k = k + 1
    for i in range(nb,len(b)):
        r[k] = b[i]
        k = k + 1

def quick_sort(array, start=None, to=None):
    start = 0 if start==None else start
    to = len(array) if to==None else to
    if start < to:
        pivot_index = partition(array,start,to)
        quick_sort(array,start,pivot_index)
        quick_sort(array,pivot_index+1,to)

def partition(a,start,to):
    pivot = a[start]
    k = start+1
    for i in range(start+1,to):
        if a[i] < pivot:
            a[i], a[k] = a[k], a[i]
            k += 1
    a[start] = a[k-1]
    a[k-1] = pivot
    return k-1

sorting_algorithms = [selection_sort, insertion_sort, bubble_sort, merge_sort, quick_sort]

class TestSortingAlgorithms(unittest.TestCase):

    def test_on_empty_list(self):
        """
        Test sorting on empty list.
        """
        for f in sorting_algorithms:
            with self.subTest(algo=f.__name__):
                a = []
                f(a)
                self.assertEqual([], a)

    def test_on_singleton_list(self):
        """
        Test sorting on singleton list.
        """
        for f in sorting_algorithms:
            with self.subTest(algo=f.__name__):
                a = [7]
                f(a)
                self.assertEqual([7], a)

    def test_on_random_lists(self):
        """
        Test sorting on random lists of increasing size.
        """
        for n in sizes:
            arr = np.random.randint(0, 1_000_000, n)
            expected = np.sort(arr).tolist() # return a sorted copy
            arr = arr.tolist()
            for f in sorting_algorithms:
                with self.subTest(algo=f.__name__):
                    a = arr.copy()
                    start_time = time.perf_counter()
                    f(a)
                    end_time = time.perf_counter()
                    print(f"Algorithm {f.__name__:20} on random array of {n:12} elements took: {end_time-start_time:20.2} s")
                    self.assertEqual(expected, a)
            print("---")

    def test_on_sorted_lists(self):
        """
        Test sorting on sorted lists of increasing size (best case).
        """
        for n in sizes:
            arr = list(range(0,n))
            expected = arr.copy()
            expected.sort()
            for f in sorting_algorithms:
                with self.subTest(algo=f.__name__):
                    a = arr.copy()
                    start_time = time.perf_counter()
                    f(a)
                    end_time = time.perf_counter()
                    print(f"Algorithm {f.__name__:20} on *sorted* array of {n:20} elements took: {end_time-start_time:20} s")
                    self.assertEqual(expected, a)
            print("---")

    def test_on_reverse_ordered_lists(self):
        """
        Test sorting on reverse-ordered lists of increasing size (worst case).
        """
        for n in sizes:
            arr = list(range(n,0,-1))
            expected = arr.copy()
            expected.sort()
            for f in sorting_algorithms:
                with self.subTest(algo=f.__name__):
                    a = arr.copy()
                    start_time = time.perf_counter()
                    f(a)
                    end_time = time.perf_counter()
                    print(f"Algorithm {f.__name__:20} on *reverse-sorted* array of {n:20} elements took: {end_time-start_time:20} s")
                    self.assertEqual(expected, a)
            print("---")

if __name__ == '__main__':
    unittest.main()
