#include <mpi.h>
#include <stddef.h>
#include <stdio.h>

#define MSG_TAG_A 124523
#define N 1000

/*
 * negative count in recv call. (line 21)
 */
int main(int argc, char *argv[]) {
  int rank;
  int buffer[N];

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    MPI_Send(buffer, N, MPI_INT, 1, MSG_TAG_A, MPI_COMM_WORLD);
  } else if (rank == 1) {
    int invalid_count = N;
    if (argc == 1) {
      invalid_count = -1;
    } else {
      invalid_count = N;
    }
    MPI_Recv(buffer, invalid_count, MPI_INT, 0, MSG_TAG_A, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

  MPI_Finalize();

  return 0;
}