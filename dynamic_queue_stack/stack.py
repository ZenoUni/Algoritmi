import ctypes
import sys

GROWING_DELTA = 10
SHRINKING_DELTA = 20
GROWING_FACTOR = 2.5
SHRINKING_FACTOR = 3.0

class DArray:
    def __init__(self, initial_size=0, initial_capacity=None):
        if initial_capacity is None:
            initial_capacity = initial_size
        assert initial_capacity >= initial_size
        self.item = (ctypes.py_object * initial_capacity)()
        self.capacity = initial_capacity
        self.size = initial_size

    def set(self, pos, value):
        self.item[pos] = value

    def get(self, pos):
        return self.item[pos]

    def print(self):
        print(f"[{self.size}/{self.capacity}]{{", end="")
        if self.size > 0:
            for i in range(self.size - 1):
                print(f"{self.item[i]}, ", end="")
            print(f"{self.item[self.size - 1]}", end="")
        print("}")

    def destroy(self):
        self.item = None
        self.capacity = 0
        self.size = 0

    def resize_linear(self, new_size):
        if new_size < 0:
            return
        print(f"[LINEAR] Resize darray to new_size={new_size}. ", end="")
        if new_size > self.capacity or (self.capacity - new_size) > SHRINKING_DELTA:
            new_capacity = new_size + GROWING_DELTA
            print(f"Setting capacity to {new_capacity}.", end="")
            new_items = (ctypes.py_object * new_capacity)()
            for i in range(self.size):
                new_items[i] = self.item[i]
            self.item = new_items
            self.capacity = new_capacity
        print("")
        self.size = new_size

    def resize_geometric(self, new_size):
        if new_size < 0:
            return
        print(f"[GEOMETRIC] Resize darray to new_size={new_size}. ", end="")
        if new_size > self.capacity or (self.capacity / new_size) > SHRINKING_FACTOR:
            new_capacity = int(new_size * GROWING_FACTOR)
            print(f"Setting capacity to {new_capacity}.", end="")
            new_items = (ctypes.py_object * new_capacity)()
            for i in range(self.size):
                new_items[i] = self.item[i]
            self.item = new_items
            self.capacity = new_capacity
        print("")
        self.size = new_size

    def resize(self, new_size):
        self.resize_linear(new_size)

    def append(self, value):
        curr_size = self.size
        self.resize(curr_size + 1)
        self.item[curr_size] = value

    def remove_last(self):
        self.resize(self.size - 1)

    def expand(self, arr, sz):
        curr_size = self.size
        self.resize(curr_size + sz)
        for i in range(sz):
            self.set(curr_size + i, arr[i])

    def insert(self, insert_pos, value):
        """Inserisce un valore in una posizione specifica."""
        if insert_pos < 0 or insert_pos > self.size:
            raise IndexError("Indice fuori dai limiti")

        # Espandiamo l'array se necessario
        if self.size >= self.capacity:
            self.resize(self.size + 1)

        # Spostiamo gli elementi a destra per fare spazio
        for i in range(self.size, insert_pos, -1):
            self.item[i] = self.item[i - 1]

        # Inseriamo il valore nella posizione desiderata
        self.item[insert_pos] = value
        self.size += 1

    def assert_equals(self, expected, expected_len):
        """Verifica che l'array contenga gli stessi valori attesi."""
        test_passed = True  # Variabile booleana per determinare se il test è riuscito

        if self.size != expected_len:
            print(f"Errore: lunghezza non corrispondente. Atteso {expected_len}, trovato {self.size}")
            test_passed = False

        for i in range(self.size):
            if self.item[i] != expected[i]:
                print(f"Errore: elemento {i} non corrisponde. Atteso {expected[i]}, trovato {self.item[i]}")
                test_passed = False

        if test_passed:
            print("Test Passed!")
        else:
            print("Test Failed!")

class Stack:
    def __init__(self):
        self.array = DArray(0)

    def push(self, value):
        self.array.append(value)

    def pop(self):
        assert self.array.size > 0
        value = self.array.get(self.array.size - 1)
        self.array.remove_last()
        return value

    def print(self):
        print("STACK: ", end="")
        self.array.print()

def main():
    s = Stack()
    s.push(5)
    s.push(7)
    s.print()
    print("")
    for _ in range(3):
        try:
            print(f"Pop: {s.pop()}.")
        except AssertionError:
            print("Pop: Stack is empty.")
        s.print()
        print("")
    print("")

if __name__ == "__main__":
    main()
