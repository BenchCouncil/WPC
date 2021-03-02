#!/bin/bash
# use pin/mica to profile SPECCPU 2017

bench_name=523.xalancbmk_r
bench_type=llvm_memory_alloc

if [ -n "$1" ]; then
    bench_name=$1
fi

cur_dir=$(cd `dirname $0`; pwd)
log_dir=$cur_dir/results/$bench_name/$bench_type
run_dir=$SPEC_HOME/benchspec/CPU/$bench_name/run/run_base_refrate_benchcpu-m64.0000

if [ ! -d $run_dir ]; then
    echo "ERROR: Directory $run_dir not exists"
    exit
fi

echo "INFO: bench_name=$bench_name"
echo "INFO: bench_type=$bench_type"

# sleep 3

rm -rf $log_dir && mkdir -p $log_dir
cd $run_dir

exe_name=$(basename /opt/cpu2017_llvm/$bench_name/src/*.ll .ll)
if [ ! -f /opt/cpu2017_llvm/$bench_name/src/$exe_name.ll ]; then
    echo "ERROR: File /opt/cpu2017_llvm/$bench_name/src/$exe_name.ll not exists"
    exit
fi

echo "INFO: log_dir=$log_dir"

set -x
cp /opt/cpu2017_llvm/$bench_name/src/$exe_name.ll $log_dir
/opt/benchcpu/llvm-profiling/memory-allocation/mem_alloc \
    $log_dir/$exe_name.ll $log_dir/$exe_name-instrumented.bc \
    || (echo "instrumentation failed"; exit 1)
llc -filetype=obj $log_dir/$exe_name-instrumented.bc -o $log_dir/$exe_name-instrumented.o \
    || (echo "llc failed"; exit 1)
clang++ $log_dir/$exe_name-instrumented.o /opt/benchcpu/llvm-profiling/runtime.cpp \
    `llvm-config --cxxflags --ldflags --system-libs --libs` -o $log_dir/$exe_name-instrumented \
    || (echo "clang++ failed"; exit 1)

cmd_line_argument=$(tail -n1 speccmds.cmd \
    | grep "../run_base_refrate_benchcpu-m64.0000.*" -o | sed "s#> \(.*[(out)|(log)]\) 2>> \(.*err\)##" \
    | sed "s#../run_base_refrate_benchcpu-m64.0000.*/*benchcpu-m64 ##g") \
    || (echo "command line arguments parse failed"; exit 1)


time $log_dir/$exe_name-instrumented $cmd_line_argument \
    > $log_dir/run.llvm_memory_alloc.log \
    2>> $log_dir/run.llvm_memory_alloc.err \
    || (echo "execution failed"; exit 1)

