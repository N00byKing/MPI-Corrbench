#!/bin/bash

clang-tidy testcase.c -checks='*mpi*' -- -I/usr/lib/x86_64-linux-gnu/mpich/include/ > output.txt
