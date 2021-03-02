import math
import time
import sys
import random
# import numba
# from numba.typed import List

# @numba.jit(nopython=True)
def matrix_mul(n, matrix_A, matrix_B, matrix_C):
    block_size = int(math.floor(math.sqrt(n)))
    for bi in range(0, n, block_size):
        for bj in range(0, n, block_size):
            for bk in range(0, n, block_size):
                for i in range(block_size):
                    for j in range(block_size):
                        for k in range(block_size):
                            matrix_C[bi + i][bj + j] += matrix_A[bi + i][bk + k] * matrix_B[bk + k][bj + j]

def block_matrix_mul(n, numreps):
    # matrix_A = List([List([random.random() for i in range(n)]) for j in range(n)])
    # matrix_B = List([List([random.random() for i in range(n)]) for j in range(n)])
    # matrix_C = List([List([0.0 for i in range(n)]) for j in range(n)])
    matrix_A = [[random.random() for i in range(n)] for j in range(n)]
    matrix_B = [[random.random() for i in range(n)] for j in range(n)]
    matrix_C = [[0.0 for i in range(n)] for j in range(n)]

    print('Multiply calculation %d times' % numreps)
    total_elapsed = 0
    for epoch in range(numreps):
        start = time.time()
        matrix_mul(n, matrix_A, matrix_B, matrix_C)
        elapsed = time.time() - start
        print("Matmul Time = %f, epoch = %d/%d" % (elapsed, epoch, numreps))
        total_elapsed += elapsed
    print("Total time = %f, %d runs, avg time = %f\n" %
          (total_elapsed, numreps, total_elapsed / numreps));

if __name__ == "__main__":
    n = 32 * 32
    numreps = 1

    if len(sys.argv) > 1:
        n = int(sys.argv[1])
    if len(sys.argv) > 2:
        numreps = int(sys.argv[2])

    block_matrix_mul(n, numreps)
