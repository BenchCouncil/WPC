#!/bin/bash

c_bench_list=(505.mcf_r  523.xalancbmk_r  531.deepsjeng_r  544.nab_r  502.gcc_r)
# bench_list=(505.mcf_r  523.xalancbmk_r  531.deepsjeng_r  507.cactuBSSN_r  544.nab_r  549.fotonik3d_r)
# bench_list=(500.perlbench_r  505.mcf_r  510.parest_r  520.omnetpp_r  525.x264_r  531.deepsjeng_r  544.nab_r  554.roms_r
#             502.gcc_r  507.cactuBSSN_r  511.povray_r  521.wrf_r  526.blender_r  538.imagick_r  548.exchange2_r  557.xz_r
#             503.bwaves_r  508.namd_r  519.lbm_r  523.xalancbmk_r  527.cam4_r  541.leela_r  549.fotonik3d_r)

for bench_name in ${c_bench_list[@]}
do
    echo -e "bench started:  $bench_name"
    ./run_speccpu_llvm_mem_alloc.sh $bench_name &
    echo -e "bench finished:  $bench_name \n"
    # sleep 30
done
