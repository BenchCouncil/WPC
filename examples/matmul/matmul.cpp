// g++ block_matrix_mul.cpp -O3 -o block_matrix_mul
#include <bits/stdc++.h>
#include <sys/time.h>

void Multiply(int n, double **a, double **b, double **c) {
    int blockSize = floor(sqrt(n));

    for (int bi = 0; bi < n; bi += blockSize)
        for (int bj = 0; bj < n; bj += blockSize)
            for (int bk = 0; bk < n; bk += blockSize)
                for (int i = 0; i < blockSize; i++)
                    for (int j = 0; j < blockSize; j++)
                        for (int k = 0; k < blockSize; k++)
                            c[bi + i][bj + j] += a[bi + i][bk + k] * b[bk + k][bj + j];
}

int main(int argc, char *argv[]) {
    int n = 32 * 32;
    int numreps = 1; // 1

    if (argc >= 2) {
        n = atoi(argv[1]);
    }
    if (argc >= 3) {
        numreps = atoi(argv[2]);
    }

    ///////////////////// Matrix A //////////////////////////

    double **A = (double **)malloc(n * sizeof(double *));
    A[0] = (double *)malloc(n * n * sizeof(double));
    if (!A) {
        printf("memory failed \n");
        exit(1);
    }
    for (int i = 1; i < n; i++) {
        A[i] = A[0] + i * n;
        if (!A[i]) {
            printf("memory failed \n");
            exit(1);
        }
    }

    ///////////////////// Matrix B //////////////////////////

    double **B = (double **)malloc(n * sizeof(double *));
    B[0] = (double *)malloc(n * n * sizeof(double));
    if (!B) {
        printf("memory failed \n");
        exit(1);
    }
    for (int i = 1; i < n; i++) {
        B[i] = B[0] + i * n;
        if (!B[i]) {
            printf("memory failed \n");
            exit(1);
        }
    }

    ///////////////////// Matrix C //////////////////////////
    double **C = (double **)malloc(n * sizeof(double *));
    C[0] = (double *)malloc(n * n * sizeof(double));
    if (!C) {
        printf("memory failed \n");
        exit(1);
    }
    for (int i = 1; i < n; i++) {
        C[i] = C[0] + i * n;
        if (!C[i]) {
            printf("memory failed \n");
            exit(1);
        }
    }

    // initialize the matrices
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = 1.0 * rand() / RAND_MAX;
            B[i][j] = 1.0 * rand() / RAND_MAX;
        }
    }

    // multiply matrices
    struct timeval tv1, tv2;
    double elapsed;
    printf("Multiply matrices %d times...\n", numreps);
    for (int i = 0; i < numreps; i++) {
        gettimeofday(&tv1, NULL);
        Multiply(n, A, B, C);
        gettimeofday(&tv2, NULL);
        double delay =
            (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
        printf("time = %lf\n", delay);
        elapsed += (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
    }
    printf("Total time = %lf, %d runs, avg time = %lf\n", elapsed, numreps, elapsed / numreps);

    // deallocate memory

    free(A[0]);
    free(A);
    free(B[0]);
    free(B);
    free(C[0]);
    free(C);
    return 0;
}
