#!/bin/bash

TIMEOUT_CMD="/usr/bin/timeout -k 120 120"

clang-13 -c -g -emit-llvm -I/usr/lib/x86_64-linux-gnu/mpich/include/ testcase.c -o testcase.bc

$TIMEOUT_CMD opt-13 -enable-new-pm=0 -load $MPI_CORRECTNESS_BM_DIR/scripts/PARCOACH/parcoach/build/src/aSSA/aSSA.* -parcoach -check-mpi < testcase.bc > /dev/null 2> output.txt