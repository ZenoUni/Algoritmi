import unittest
import numpy as np
import time
import sys
import random
# import timeit

def linear_search(array, x, eq = lambda x, y: x == y):
    for i in range(len(array)):
        if(eq(array[i],x)):
            return i
    return -1

def binary_search_recur(array, x):
    return _binary_search_recur(array, x, 0, len(array)-1)

def _binary_search_recur(array, x, start, to, eq = lambda x, y: x == y, less = lambda x, y: x < y):
    if to < start: return -1
    if start == to:
        return start if eq(array[start], x)  else -1
    mid = start + (to - start)//2
    if eq(array[mid], x): return mid
    elif less(array[mid], x): return _binary_search_recur(array, x, mid+1, to)
    else: return _binary_search_recur(array, x, start, mid-1)

def binary_search_iter(array, x):
    return _binary_search_iter(array, x, 0, len(array)-1)

def _binary_search_iter(array, x, start, to, eq = lambda x, y: x == y, less = lambda x, y: x < y):
    while start <= to:
        m = (start + to) // 2
        if eq(array[m], x): return m
        elif less(array[m], x): start = m+1
        else: to = m-1
    return -1

search_algorithms = [linear_search, binary_search_recur, binary_search_iter]

def random_array(fromN, toN, n, seed = None):
    random.seed(seed)
    return [random.randint(fromN, toN) for i in range(n)]

sizes = [5,20,50,100,1000,2000,5000]
inputs = {}
for n in sizes:
    arr = random_array(0, n*10, n)
    arr.sort()
    inputs[n] = arr

class TestSortingAlgorithms(unittest.TestCase):

    def test_on_sorted_lists(self):
        """
        Test searching on sorted lists of increasing size.
        """
        for f in search_algorithms:
            with self.subTest(algo=f.__name__):
                for n in sizes:
                    arr = inputs[n]
                    for elem in [arr[0], arr[n//2], arr[n-1], -1]:
                        with self.subTest(elem_to_find=elem):
                            try:
                                expected = arr.index(elem)
                            except ValueError:
                                expected = -1
                            start_time = time.perf_counter()
                            actual = f(arr, elem)
                            end_time = time.perf_counter()
                            print(f"Algorithm {f.__name__:20}: on sorted array of {n:20} elements took: {end_time-start_time:20} s\n")
                            self.assertEqual(expected, actual, f"({f.__name__:20}, search {elem} in array of {n} elems) Expected {expected} but got {actual}")

if __name__ == '__main__':
    unittest.main()(base) 