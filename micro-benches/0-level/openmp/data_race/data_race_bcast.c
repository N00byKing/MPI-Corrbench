#include "nondeterminism.h"

#include <mpi.h>
#include <omp.h>
#include <stdbool.h>
#include <stdlib.h>

// This test is loosely based on a unit test of the MUST correctness checker.
// See https://itc.rwth-aachen.de/must/
//
// Data race in parallel region: Concurrently, (1) buffer is written to
// (marker "A"), and (b) without any synchronization ("omp barrier") passed to a
// broadcast operation (marker "B").

#define NUM_THREADS 8

bool has_error(const int *buffer) {
  for (int i = 0; i < NUM_THREADS; ++i) {
    if (buffer[i] != -1) {
      return true;
    }
  }
  return false;
}

int main(int argc, char *argv[]) {
  int provided;
  const int requested = MPI_THREAD_FUNNELED;

  MPI_Init_thread(&argc, &argv, requested, &provided);
  if (provided < requested) {
    has_error_manifested(false);
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }

  int size;
  int rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int bcast_data[BUFFER_LENGTH_INT];
  fill_message_buffer(bcast_data, BUFFER_LENGTH_BYTE, 6);

#pragma omp parallel
  {
    DISTURB_THREAD_ORDER
    bcast_data[omp_get_thread_num()] = -1; /* A */

// #pragma omp barrier -- this fixes the data race error
#pragma omp master
    { MPI_Bcast(bcast_data, BUFFER_LENGTH_INT, MPI_INT, 0, MPI_COMM_WORLD); /* B */ }
  }

  const bool error = has_error(bcast_data);
  has_error_manifested(error);

  MPI_Finalize();

  return 0;
}
