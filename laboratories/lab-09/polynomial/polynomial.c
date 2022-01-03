#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0

int main(int argc, char *argv[]) {
  int rank;
  int nProcesses;
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
  printf("Hello from %i/%i.\n", rank, nProcesses);

  if (rank == MASTER) { // This code is run by a single process
    int polynomialSize, n;
    float x = 5,
          sum; // the value on which the polynomial function is applied - f(5)

    /*
            the input file has the following format:
            number_of_coefficients
            coefficient for x^0
            coefficient for x^1
            etc.
    */

    FILE *polFunctionFile = fopen(argv[1], "rt");
    fscanf(polFunctionFile, "%d", &polynomialSize);
    /*
            array a stores the polynomial coefficients
            e.g. a = {1, 4, 4} => 1 * (x ^ 2) + 4 * (x ^ 1) + 4 * (x ^ 0)
    */
    float *a = malloc(sizeof(float) * polynomialSize);
    if (a == NULL) {
      exit(1);
    }
    for (int i = 0; i < polynomialSize - 1; i++) {
      fscanf(polFunctionFile, "%f", &a[i]);
      printf("Read value %f.\n", a[i]);
      /*
              Send the coefficients for x^0, x^1, etc. to processes
              polynomialSize - 1, polynomialSize - 2, etc.
              Process 0 stores the coefficient for x^0.
      */
      printf(
          "Process with rank [%d], sent x, %f, and coefficient, %f, to [%d].\n",
          rank, x, a[i], polynomialSize - 1 - i);
      MPI_Send(&x, 1, MPI_FLOAT, polynomialSize - 1 - i, 1, MPI_COMM_WORLD);
      MPI_Send(&a[i], 1, MPI_FLOAT, polynomialSize - 1 - i, 1, MPI_COMM_WORLD);
    }
    fscanf(polFunctionFile, "%f", &a[polynomialSize - 1]);
    printf("Read value %f.\n\n", a[polynomialSize - 1]);
    fclose(polFunctionFile);

    MPI_Status status;
    MPI_Recv(&sum, 1, MPI_FLOAT, rank + 1, 1, MPI_COMM_WORLD, &status);
    printf("Process with rank [%d], received sum %f from [%d].\n", rank, sum,
           status.MPI_SOURCE);
    sum = sum * x + a[polynomialSize - 1];
    free(a);

    printf("Polynomial function application result is: %f.\n", sum);
  } else {
    MPI_Status status;
    float x, coef, sum;

    /*
            receive:
            - the partial sum
            - the value of x in f(x)
            - the coefficient
    */
    if (rank != nProcesses - 1) {
      MPI_Recv(&sum, 1, MPI_FLOAT, rank + 1, 1, MPI_COMM_WORLD, &status);
      printf("Process with rank [%d], received sum %f from [%d].\n", rank, sum,
             status.MPI_SOURCE);
    } else {
      sum = 0;
    }
    MPI_Recv(&x, 1, MPI_FLOAT, MASTER, 1, MPI_COMM_WORLD, &status);
    sum *= x;
    printf("Process with rank [%d], received x %f from [%d].\n", rank, x,
           status.MPI_SOURCE);
    MPI_Recv(&coef, 1, MPI_FLOAT, MASTER, 1, MPI_COMM_WORLD, &status);
    sum += coef;
    printf("Process with rank [%d], received coef %f from [%d].\n", rank, coef,
           status.MPI_SOURCE);

    MPI_Send(&sum, 1, MPI_FLOAT, rank - 1, 1, MPI_COMM_WORLD);
    printf("Process with rank [%d], sent sum %f to [%d].\n", rank, sum,
           rank - 1);
  }

  MPI_Finalize();
  return 0;
}
