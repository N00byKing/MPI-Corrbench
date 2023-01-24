#include "nondeterminism.h"
#include <mpi.h>
#include <omp.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * Only call finalize after all threads have finished MPI
 * we assume thread 0 is the master thread (but this may be OpenMP implementation dependant)
 */
int main(int argc, char *argv[]) {
  int myRank;
  int buffer_out[10], buffer_in[10];
  int provided;
  const int requested = MPI_THREAD_MULTIPLE;

  MPI_Init_thread(&argc, &argv, requested, &provided);
  if (provided < requested) {
    has_error_manifested(false);
    exit(EXIT_FAILURE);
  }

  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

  DEF_ORDER_CAPTURING_VARIABLES
#pragma omp parallel num_threads(NUM_THREADS)
  {
#pragma omp for
    for (int i = 0; i < 10; i++) {
      buffer_out[i] = i * 10;
    }
// implicit OpenMP barrier
#pragma omp sections
    {
#pragma omp section
      {
        if (myRank == 0) {
          ENTER_CALL_A
          MPI_Send(buffer_out, 10, MPI_INT, 1, 123, MPI_COMM_WORLD);
          EXIT_CALL_A
        } else if (myRank == 1) {
#ifdef USE_DISTURBED_THREAD_ORDER
          us_sleep(20);
#endif
          ENTER_CALL_A
          MPI_Recv(buffer_in, 10, MPI_INT, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          EXIT_CALL_A
        }
      }
#pragma omp section
      {
#ifndef USE_DISTURBED_THREAD_ORDER
        us_sleep(20);
#endif
        ENTER_CALL_A
        MPI_Finalize();
        EXIT_CALL_B
      }
    }
  }
  // end of omp parallel

  has_error_manifested(!CHECK_FOR_EXPECTED_ORDER);
  return 0;
}
