import java.io.*;
import java.util.*;

public class sort_fixed_sm {
    static final int RAND_MAX = 2147483647;

    public static void merge_sort(int[] arr, int len) {
        int [] a = arr;
        int [] b = new int[len];
        for (int seg = 1; seg < len; seg += seg) {
            for (int start = 0; start < len; start += seg + seg) {
                int low = start, mid = Math.min(start + seg, len), high = Math.min(start + seg + seg, len);
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
            int[] temp = a;
            a = b;
            b = temp;
        }
        if (a != arr) {
            // deep copy
            for (int i = 0; i < len; i++)
                b[i] = a[i];
            b = a;
        }
    }

    public static void main(String[] args) throws IOException {

        FileReader fr = new FileReader("input/data.in");
        BufferedReader br = new BufferedReader(fr);
        String[] split = br.readLine().split("\\s+");
        int n = Integer.parseInt(split[0]), numreps = Integer.parseInt(split[1]);
        int[] I = new int[n];
        for (int i = 0; i < n; i++) {
            I[i] = Integer.parseInt(br.readLine());
        }

        System.out.printf("Multiply calculation %d times...\n", numreps);
        double total_elapsed=0;
        for(int i = 0; i < numreps; i++) {
            int[] A = Arrays.copyOf(I, n);
            long startTime = System.nanoTime();
            // Arrays.sort(A);
            merge_sort(A, n);
            double deltaTime = (System.nanoTime() - startTime) * 1e-9;
            System.out.printf("[%d/%d] time = %f\n", i, numreps, deltaTime);
            total_elapsed += deltaTime;
            // for(int j = 0; j < 20; ++j) {
            //     System.out.printf("%d, ", A[j]);
            // }
            // System.out.printf("\n");
        }
        System.out.println("Array Size " + n);
        System.out.println("Iterations " + numreps);
        System.out.println("Total time wihout initialization " + total_elapsed);
        System.out.println("Avg time wihout initialization " + total_elapsed / numreps);
    }
}