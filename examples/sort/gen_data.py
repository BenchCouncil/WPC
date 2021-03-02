# encoding: utf-8
import random
from tqdm import tqdm

RAND_MAX = 2147483647

if __name__ == "__main__":
    random.seed(2333)
    n, numreps = (1 << 20), 100
    
    fd = open("data.in", "w+")
    fd.write("%d %d\n" % (n, numreps))
    for i in tqdm(range(n)):
        fd.write("%d\n" % random.randint(0, RAND_MAX))
    fd.close()
