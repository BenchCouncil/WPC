# encoding: utf-8
import sys
import time
import random
import string
from hashlib import sha256

if __name__ == "__main__":
    n = 20 * (1 << 20)
    numreps = 100

    if len(sys.argv) > 1:
        n = int(sys.argv[1])
    if len(sys.argv) > 2:
        numreps = int(sys.argv[2])
    
    I = ''.join(random.choices(string.ascii_letters + string.digits, k=n)).encode("utf-8")

    print("Multiply calculation %d times..." % numreps)
    
    start = time.time()
    for _ in range(numreps):
        sha256(I).hexdigest()
    elapsed = time.time() - start
    print("Total time = %f, %d runs, avg time = %f" %
          (elapsed, numreps, elapsed / numreps));
