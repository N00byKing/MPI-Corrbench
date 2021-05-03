#include <mpi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
/*
 * invalid target rank in get call. line 25
 */

#define N 10

int main(int argc, char *argv[]) {
  int rank, numProcs;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int *buffer = malloc(N * sizeof(int));

  MPI_Win win;
  MPI_Win_create(&buffer, N * sizeof(int), 1, MPI_INFO_NULL, MPI_COMM_WORLD, &win);

  if (rank == 0) {
    int rank = 1;
    MPI_Win_fence(0, win);
    int local_buf[N] = {0};
    if (argc == 1) {
      rank = -1;
    } else {
      rank = 1;
    }
    MPI_Get(&local_buf, N, MPI_INT, rank, 0, N, MPI_INT, win);

    MPI_Win_fence(0, win);
  } else {
    MPI_Win_fence(0, win);
    MPI_Win_fence(0, win);
  }

  MPI_Win_free(&win);

  free(buffer);

  MPI_Finalize();

  return 0;
}