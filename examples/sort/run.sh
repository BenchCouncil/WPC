#!/bin/bash
source /opt/benchcpu/hadoop-3.2.1-standalone/env.sh
rm -rf output

set -x
eval "echo xingw.xiong | sudo -S perf stat -e 'r04C7,r15C7,r08C7,r3CC7,r81D0,r01C7,r82D0,r03C7,r2AC7,r10C7,r20C7,r003C,r00C0,r010E,r0180,r0280,r02C7,r02C0,r00C4,r00C5,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,cache-misses,cache-references,branch-misses,branch-instructions,instructions,cycles,r00C0:u,r00C0:k' hadoop jar build/sort_fixed_mr.jar sort_fixed_mr" > $log_dir/run.perf.log 2> $log_dir/run.perf.err \
    || (echo "execution failed"; exit 1)
set +x
