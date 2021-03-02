// g++ sort.cpp -O3 -o sort
#include <bits/stdc++.h>
#include <sys/time.h>

void merge_sort(int arr[], int len) {
    int *a = arr;
    int *b = new int[len];
    for (int seg = 1; seg < len; seg += seg) {
        for (int start = 0; start < len; start += seg + seg) {
            int low = start, mid = std::min(start + seg, len), high = std::min(start + seg + seg, len);
            int k = low;
            int start1 = low, end1 = mid;
            int start2 = mid, end2 = high;
            while (start1 < end1 && start2 < end2)
                b[k++] = a[start1] < a[start2] ? a[start1++] : a[start2++];
            while (start1 < end1)
                b[k++] = a[start1++];
            while (start2 < end2)
                b[k++] = a[start2++];
        }
        std::swap(a, b);
    }
    if (a != arr) {
        // deep copy
        for (int i = 0; i < len; i++)
            b[i] = a[i];
        b = a;
    }
    delete[] b;
}

int main(int argc, char *argv[]) {
    int n = 0, numreps = 0;

    // initialize the array
    FILE* fp = fopen("./input/data.in", "r");
    fscanf(fp, "%d %d", &n, &numreps);
    // allocate memory for the arrays
    int *I = (int *)malloc(n * sizeof(int));
    int *A = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        fscanf(fp, "%d", &I[i]);
    }

    // multiply calculation
    struct timeval tv1, tv2;
    double elapsed = 0;
    printf("Multiply calculation %d times...\n", numreps);
    for (int i = 0; i < numreps; i++) {
        gettimeofday(&tv1, NULL);
        memcpy(A, I, n * sizeof(int));
        merge_sort(A, n);
        // std::sort(A, A + n);
        gettimeofday(&tv2, NULL);
        double delay =
            (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
        printf("time = %lf\n", delay);
        elapsed += (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
        // for(int j = 0; j < 10; ++j) {
        //     printf("%d, ", A[j]);
        // }
        // printf("\n");
    }
    printf("Total time = %lf, %d runs, array size=%d, avg time = %lf\n", elapsed, numreps, n,
           elapsed / numreps);

    // deallocate memory
    free(A);
    return 0;
}
