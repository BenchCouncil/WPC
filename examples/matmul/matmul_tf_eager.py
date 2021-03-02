# encoding: utf-8
import time
import sys
import tensorflow as tf

if __name__ == "__main__":
    n = 32 * 32
    numreps = 2000
    if len(sys.argv) > 1:
        n = int(sys.argv[1])
    if len(sys.argv) > 2:
        numreps = int(sys.argv[2])
    
    tf.random.set_seed(2333)
    tf.compat.v1.set_random_seed(2333)
    tf.compat.v1.enable_eager_execution()
    matrix_A = tf.random.uniform([n, n], dtype=tf.float32)
    matrix_B = tf.random.uniform([n, n], dtype=tf.float32)

    start = time.time()
    total_elapsed = 0
    
    for epoch in range(numreps):
        start = time.time()    
        matrix_C = tf.matmul(matrix_A, matrix_B)
        matrix_C = tf.keras.backend.eval(matrix_C)
        # print("matrix_A", matrix_A, type(matrix_A))
        # print("matrix_B", matrix_B, type(matrix_B))
        # print("matrix_C", matrix_C, type(matrix_C))
        elapsed = time.time() - start
        # if epoch % 100 == 0:
        print("Sort Time = %f, epoch = %d/%d" % (elapsed, epoch, numreps))
        total_elapsed += elapsed
        del matrix_C
    print("Total time = %f, %d runs, avg time = %f\n" %
          (total_elapsed, numreps, total_elapsed / numreps))
