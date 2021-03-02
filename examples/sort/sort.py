# encoding: utf-8
import copy
import time

RAND_MAX = 2147483647

def merge_sort(arr: list, length: int) -> None:
    a = arr
    b = [0 for _ in range(length)]
    seg = 1
    while seg < length:
        for start in range(0, length, seg + seg):
            low, mid, high = start, min(start + seg, length), min(start + seg + seg, length)
            k = low
            start1, end1 = low, mid
            start2, end2 = mid, high
            while start1 < end1 and start2 < end2:
                if a[start1] < a[start2]:
                    b[k] = a[start1]
                    start1 += 1
                else:
                    b[k] = a[start2]
                    start2 += 1
                k += 1
            while start1 < end1:
                b[k] = a[start1]
                start1 += 1
                k += 1
            while start2 < end2:
                b[k] = a[start2]
                start2 += 1
                k += 1
        a, b = b, a  # swap(a, b)
        seg += seg
    if a != arr:
        for i in range(length):
            b[i] = a[i]
        b = a

if __name__ == "__main__":
    fd = open("input/data.in", "r")
    lines = fd.readlines()
    fd.close()

    n, numreps = map(int, lines[0].split())
    numreps = 5
    I = list(map(int, lines[1:n+1]))
    
    print("Multiply calculation %d times...\n" % numreps)
    
    total_elapsed = 0
    for epoch in range(numreps):
        start = time.time()
        # A = sorted(I)
        A = copy.deepcopy(I)
        merge_sort(A, n)
        # for j in range(10):
        #     print(A[j], end=", ")
        # print("\n")
        elapsed = time.time() - start
        print("Sort Time = %f, epoch = %d/%d" % (elapsed, epoch, numreps))
        total_elapsed += elapsed
    print("Total time = %f, %d runs, avg time = %f\n" % (total_elapsed, numreps, total_elapsed / numreps))
