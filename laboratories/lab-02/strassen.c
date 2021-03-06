#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct data_t {
  unsigned int N;
  int **a;
  int **b;
  int **c;
  int **M1;
  int **AUXM11;
  int **AUXM12;
  int **M2;
  int **AUXM21;
  int **M3;
  int **AUXM31;
  int **M4;
  int **AUXM41;
  int **M5;
  int **AUXM51;
  int **M6;
  int **AUXM61;
  int **AUXM62;
  int **M7;
  int **AUXM71;
  int **AUXM72;
};

void free_matrix(int **a, size_t n) {
  size_t i;
  for (i = 0; i < n; i++) {
    free(a[i]);
  }
  free(a);
}

int **alloc_matrix(size_t n, size_t m) {
  int **a;
  size_t i;
  a = calloc(n, sizeof(int *));
  if (a == NULL) {
    return NULL;
  }
  for (i = 0; i < n; i++) {
    a[i] = calloc(m, sizeof(int));
    if (a[i] == NULL) {
      free_matrix(a, i);
    }
  }
  return a;
}

void init(struct data_t *data) {
  size_t i, j;
  data->a = alloc_matrix(data->N, data->N);
  data->b = alloc_matrix(data->N, data->N);
  data->c = alloc_matrix(data->N, data->N);

  data->M1     = alloc_matrix(data->N, data->N);
  data->AUXM11 = alloc_matrix(data->N, data->N);
  data->AUXM12 = alloc_matrix(data->N, data->N);

  data->M2     = alloc_matrix(data->N, data->N);
  data->AUXM21 = alloc_matrix(data->N, data->N);

  data->M3     = alloc_matrix(data->N, data->N);
  data->AUXM31 = alloc_matrix(data->N, data->N);

  data->M4     = alloc_matrix(data->N, data->N);
  data->AUXM41 = alloc_matrix(data->N, data->N);

  data->M5     = alloc_matrix(data->N, data->N);
  data->AUXM51 = alloc_matrix(data->N, data->N);

  data->M6     = alloc_matrix(data->N, data->N);
  data->AUXM61 = alloc_matrix(data->N, data->N);
  data->AUXM62 = alloc_matrix(data->N, data->N);

  data->M7     = alloc_matrix(data->N, data->N);
  data->AUXM71 = alloc_matrix(data->N, data->N);
  data->AUXM72 = alloc_matrix(data->N, data->N);

  for (i = 0; i < data->N; i++) {
    for (j = 0; j < data->N; j++) {
      data->c[i][j] = 0;

      data->M1[i][j] = 0;
      data->AUXM11[i][j] = 0;
      data->AUXM12[i][j] = 0;

      data->M2[i][j] = 0;
      data->AUXM21[i][j] = 0;

      data->M3[i][j] = 0;
      data->AUXM31[i][j] = 0;

      data->M4[i][j] = 0;
      data->AUXM41[i][j] = 0;

      data->M5[i][j] = 0;
      data->AUXM51[i][j] = 0;

      data->M6[i][j] = 0;
      data->AUXM61[i][j] = 0;
      data->AUXM62[i][j] = 0;

      data->M7[i][j] = 0;
      data->AUXM71[i][j] = 0;
      data->AUXM72[i][j] = 0;

      if (i <= j) {
        data->a[i][j] = 1;
        data->b[i][j] = 1;
      } else {
        data->a[i][j] = 0;
        data->b[i][j] = 0;
      }
    }
  }
}

void print(int **mat, size_t N) {
  size_t i, j;

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      printf("%i\t", mat[i][j]);
    }
    printf("\n");
  }
}

void mul_matrix(int **C, int startCi, int startCj, int **A, int startAi,
                int startAj, int **B, int startBi, int startBj, size_t N) {
  // NU paralelizati aceasta functie
  size_t i, j, k;
  for (i = 0; i < N / 2; i++)
    for (j = 0; j < N / 2; j++)
      for (k = 0; k < N / 2; k++)
        C[startCi + i][startCj + j] +=
            A[startAi + i][startAj + k] * B[startBi + k][startBj + j];
}

void add_matrix(int **C, int startCi, int startCj, int **A, int startAi,
                int startAj, int **B, int startBi, int startBj, size_t N) {
  // NU paralelizati aceasta functie
  size_t i, j;
  for (i = 0; i < N / 2; i++)
    for (j = 0; j < N / 2; j++)
      C[startCi + i][startCj + j] =
          A[startAi + i][startAj + j] + B[startBi + i][startBj + j];
}

void sub_matrix(int **C, int startCi, int startCj, int **A, int startAi,
                int startAj, int **B, int startBi, int startBj, size_t N) {
  // NU paralelizati aceasta functie
  size_t i, j;
  for (i = 0; i < N / 2; i++)
    for (j = 0; j < N / 2; j++)
      C[startCi + i][startCj + j] =
          A[startAi + i][startAj + j] - B[startBi + i][startBj + j];
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: ./strassen N\n");
    exit(1);
  }

  struct data_t data;
  data.N = atoi(argv[1]);

  init(&data);

  // compute matrix M1
  add_matrix(data.AUXM11, 0, 0, data.a, 0, 0, data.a, data.N / 2, data.N / 2, data.N);
  add_matrix(data.AUXM12, 0, 0, data.b, 0, 0, data.b, data.N / 2, data.N / 2, data.N);
  mul_matrix(data.M1, 0, 0, data.AUXM11, 0, 0, data.AUXM12, 0, 0, data.N);

  // compute matrix M2
  add_matrix(data.AUXM21, 0, 0, data.a, data.N / 2, 0, data.a, data.N / 2, data.N / 2, data.N);
  mul_matrix(data.M2, 0, 0, data.AUXM21, 0, 0, data.b, 0, 0, data.N);

  // compute matrix M3
  sub_matrix(data.AUXM31, 0, 0, data.b, 0, data.N / 2, data.b, data.N / 2, data.N / 2, data.N);
  mul_matrix(data.M3, 0, 0, data.a, 0, 0, data.AUXM31, 0, 0, data.N);

  // compute matrix M4
  sub_matrix(data.AUXM41, 0, 0, data.b, data.N / 2, 0, data.b, 0, 0, data.N);
  mul_matrix(data.M4, 0, 0, data.a, data.N / 2, data.N / 2, data.AUXM41, 0, 0, data.N);

  // compute matrix M5
  add_matrix(data.AUXM51, 0, 0, data.a, 0, 0, data.a, 0, data.N / 2, data.N);
  mul_matrix(data.M5, 0, 0, data.AUXM51, 0, 0, data.b, data.N / 2, data.N / 2, data.N);

  // compute matrix M6
  sub_matrix(data.AUXM61, 0, 0, data.a, data.N / 2, 0, data.a, 0, 0, data.N);
  add_matrix(data.AUXM62, 0, 0, data.b, 0, 0, data.b, 0, data.N / 2, data.N);
  mul_matrix(data.M6, 0, 0, data.AUXM61, 0, 0, data.AUXM62, 0, 0, data.N);

  // compute matrix M7
  sub_matrix(data.AUXM71, 0, 0, data.a, 0, data.N / 2, data.a, data.N / 2, data.N / 2, data.N);
  add_matrix(data.AUXM72, 0, 0, data.b, data.N / 2, 0, data.b, data.N / 2, data.N / 2, data.N);
  mul_matrix(data.M7, 0, 0, data.AUXM71, 0, 0, data.AUXM72, 0, 0, data.N);

  // compute submatrix C1,1
  add_matrix(data.c, 0, 0, data.M1, 0, 0, data.M4, 0, 0, data.N);
  sub_matrix(data.c, 0, 0, data.c, 0, 0, data.M5, 0, 0, data.N);
  add_matrix(data.c, 0, 0, data.c, 0, 0, data.M7, 0, 0, data.N);

  // compute submatrix C1,2
  add_matrix(data.c, 0, data.N / 2, data.M3, 0, 0, data.M5, 0, 0, data.N);

  // compute submatrix C2,1
  add_matrix(data.c, data.N / 2, 0, data.M2, 0, 0, data.M4, 0, 0, data.N);

  // compute submatrix C2,2
  sub_matrix(data.c, data.N / 2, data.N / 2, data.M1, 0, 0, data.M2, 0, 0, data.N);
  add_matrix(data.c, data.N / 2, data.N / 2, data.c, data.N / 2, data.N / 2, data.M3, 0, 0, data.N);
  add_matrix(data.c, data.N / 2, data.N / 2, data.c, data.N / 2, data.N / 2, data.M6, 0, 0, data.N);

  print(data.c, data.N);

  return 0;
}
