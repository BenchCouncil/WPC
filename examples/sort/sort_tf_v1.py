# encoding: utf-8
import sys
import time
import tensorflow as tf

RAND_MAX = 2147483647

if __name__ == "__main__":
    fd = open("input/data.in", "r")
    lines = fd.readlines()
    fd.close()

    tf.compat.v1.disable_eager_execution()
    config = tf.compat.v1.ConfigProto()
    config.graph_options.optimizer_options.global_jit_level = tf.compat.v1.OptimizerOptions.ON_1

    n, numreps = map(int, lines[0].split())
    # numreps = 10
    I = tf.convert_to_tensor(list(map(int, lines[1:])), dtype=tf.int32)

    print("Multiply calculation %d times...\n" % numreps)

    total_elapsed = 0
    for epoch in range(numreps):
        start = time.time()
        A = tf.sort(I, axis=-1, direction='ASCENDING', name=None)
        
        with tf.compat.v1.Session(config=config) as sess:
            A = sess.run(A)
            print(A)
        # A = tf.keras.backend.eval(A)
        # print(A, type(A))
        elapsed = time.time() - start
        print("Sort Time = %f, epoch = %d/%d" % (elapsed, epoch, numreps))
        total_elapsed += elapsed
    print("Total time = %f, %d runs, avg time = %f\n" %
          (total_elapsed, numreps, total_elapsed / numreps))
