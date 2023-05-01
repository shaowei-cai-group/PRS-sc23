#!/bin/bash

mpirun --mca btl_tcp_if_include eth0 --allow-run-as-root \
    --hostfile $1 --bind-to none --allow-run-as-root \
    /light -i $2 --share=1 --threads=16 --times=1000 --share_method=0