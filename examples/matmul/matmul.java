import java.util.*;
import java.lang.Math;
import java.io.IOException;

public class matmul {
    public static void main(String[] args) {
        int blockSize = 32;
        int numreps = 1;
        if (args.length >= 1) {
            blockSize = Integer.parseInt(args[0]);
        }
        if (args.length >= 2) {
            numreps = Integer.parseInt(args[1]);
        }

        int maSize = blockSize * blockSize;

        // if (maSize > 4096) {
        //     throw new IllegalArgumentException("Matrix size cannot be so large!");
        // }
        double[][] firstMa = iniMatrix(maSize);
        double[][] secondMa = iniMatrix(maSize);
        double[][] c = new double[maSize][maSize];

        double total_elapsed=0;
        for(int i=0; i<numreps; i++) {
            long startTime = System.nanoTime();
            matrixMultiple(blockSize, firstMa, secondMa, c);
            long endTime = System.nanoTime();
            total_elapsed += (endTime - startTime) * 1e-9;
        }
        System.out.println("Iterations " + numreps);
        System.out.println("Total time wihout initialization " + total_elapsed);
        System.out.println("Avg time wihout initialization " + total_elapsed / numreps);
    }

    public static double[][] iniMatrix(int maSize) {
        double[][] matrix = new double[maSize][maSize];
        for(int i=0; i< maSize; i++) {
            for(int j=0; j< maSize; j++) {
                matrix[i][j] = Math.random();
            }
        }
        return matrix;
    }

    public static void matrixMultiple(int blockSize, double[][] a, double[][] b, double[][] c) {
        int n = blockSize*blockSize;
        for(int bi=0; bi<n; bi+=blockSize)
            for(int bj=0; bj<n; bj+=blockSize)
                for(int bk=0; bk<n; bk+=blockSize)
                    for(int i=0; i<blockSize; i++)
                        for(int j=0; j<blockSize; j++)
                            for(int k=0; k<blockSize; k++) {
                                c[bi + i][bj + j] += a[bi + i][bk + k] * b[bk + k][bj + j];
                            }
    }
}
