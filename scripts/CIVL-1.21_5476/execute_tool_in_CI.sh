#!/bin/bash

TIMEOUT_CMD="/usr/bin/timeout -k 120 120"

$TIMEOUT_CMD java -jar $MPI_CORRECTNESS_BM_DIR/scripts/CIVL-1.21_5476/lib/civl-1.21_5476.jar verify testcase.c 2> output.txt
