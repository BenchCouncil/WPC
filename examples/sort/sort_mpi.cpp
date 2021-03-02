#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <mpi.h>
#include <sys/time.h>

void merge_sort(int arr[], int len) {
    int *a = arr;
    int *b = new int[len];
    for (int seg = 1; seg < len; seg += seg) {
        for (int start = 0; start < len; start += seg + seg) {
            int low = start, mid = std::min(start + seg, len),
                high = std::min(start + seg + seg, len);
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

int main(int argc, char **argv) {

    /********** Create and populate the array **********/
    int n = 0, numreps = 0;
    // initialize the array
    FILE *fp = fopen("./input/data.in", "r");
    fscanf(fp, "%d %d", &n, &numreps);
    // allocate memory for the arrays
    int *original = (int *)malloc(n * sizeof(int));
    int *unsorted = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        fscanf(fp, "%d", &original[i]);
    }
    struct timeval tv1, tv2;
    double elapsed = 0;
    MPI_Init(&argc, &argv);
    for (int i = 0; i < numreps; i++) {
        memcpy(unsorted, original, n * sizeof(int));
        gettimeofday(&tv1, NULL);

        /********** Initialize MPI **********/
        int world_rank;
        int world_size;

        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);

        /********** Divide the array in equal-sized chunks **********/
        int size = n / world_size;

        /********** Send each subarray to each process **********/
        int *sub_array = (int *)malloc(size * sizeof(int));
        MPI_Scatter(unsorted, size, MPI_INT, sub_array, size, MPI_INT, 0, MPI_COMM_WORLD);

        /********** Perform the mergesort on each process **********/
        merge_sort(sub_array, size);

        /********** Gather the sorted subarrays into one **********/
        int *sorted = NULL;
        if (world_rank == 0) {
            sorted = (int *)malloc(n * sizeof(int));
        }

        MPI_Gather(sub_array, size, MPI_INT, sorted, size, MPI_INT, 0, MPI_COMM_WORLD);

        /********** Make the final mergeSort call **********/
        if (world_rank == 0) {
            merge_sort(sorted, n);
#ifdef DEBUG
            /********** Display the sorted array **********/
            printf("This is the sorted array: ");
            for (int c = 0; c < std::min(n, 10); c++) {
                printf("%d ", sorted[c]);
            }
            printf("\n");
#endif // DEBUG
            /********** Clean up root **********/
            free(sorted);
        }

        /********** Clean up rest **********/
        free(sub_array);
        gettimeofday(&tv2, NULL);
        double delay =
            (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
        printf("[%d/%d] time = %lf\n", i, numreps, delay);
        elapsed += (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;
    }
    printf("Total time = %lf, %d runs, array size=%d, avg time = %lf\n", elapsed, numreps, n,
           elapsed / numreps);
    /********** Finalize MPI **********/
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    free(original);
}
