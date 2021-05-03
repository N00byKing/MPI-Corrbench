#include <mpi.h>
#include <stddef.h>
#include <stdio.h>
/*
 * Wrong MPI_Type specified for send buffer. line 17.
 *
 */
int main(int argc, char *argv[]) {
  int myRank, numProcs;

  int local_sum = 4;
  int global_sum[2] = {0};

  MPI_Init(&argc, &argv);

  int root = 0;

  MPI_Gather(&local_sum, 1, MPI_UNSIGNED, global_sum, 1, MPI_UNSIGNED, root, MPI_COMM_WORLD);

  MPI_Finalize();

  return 0;
}