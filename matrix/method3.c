#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>

// Function to check if a number is a perfect square
int isPerfectSquare(int num) {
    int root = (int)sqrt(num);
    return root * root == num;
}

// Function to perform block matrix multiplication (convolution) without parallelization
int* matrix_multiply_block_serial(int* A, int size_A, int* B, int size_B) {
    int size_convolution = size_A - size_B + 1;
    int* res = (int*)malloc(size_convolution * size_convolution * sizeof(int));

    for (int i = 0; i < size_convolution; i++) {
        for (int j = 0; j < size_convolution; j++) {
            int calc = 0;
            for (int row = 0; row < size_B; row++) {
                for (int col = 0; col < size_B; col++) {
                    calc += A[(row + i) * size_A + col + j] * B[row * size_B + col];
                }
            }
            res[i * size_convolution + j] = calc;
        }
    }
    return res;
}

// Function to perform block matrix multiplication (convolution) with parallelization
int* matrix_multiply_block_parallel(int* A, int size_A, int* B, int size_B) {
    int size_convolution = size_A - size_B + 1;
    int* res = (int*)malloc(size_convolution * size_convolution * sizeof(int));

#pragma omp parallel for collapse(2)
    for (int i = 0; i < size_convolution; i++) {
        for (int j = 0; j < size_convolution; j++) {
            int calc = 0;
            for (int row = 0; row < size_B; row++) {
                for (int col = 0; col < size_B; col++) {
                    calc += A[(row + i) * size_A + col + j] * B[row * size_B + col];
                }
            }
            res[i * size_convolution + j] = calc;
        }
    }
    return res;
}

int main() {
    // 设置矩阵大小
    int size_A = 1200; // 增大问题规模
    int size_B = 64;

    printf("start!\n");

    // 生成随机矩阵
    int* A = (int*)malloc(size_A * size_A * sizeof(int));
    int* B = (int*)malloc(size_B * size_B * sizeof(int));

    srand(123); // 设置随机数种子
    for (int i = 0; i < size_A * size_A; ++i) {
        A[i] = rand() % 100;
    }
    for (int i = 0; i < size_B * size_B; ++i) {
        B[i] = rand() % 100;
    }

    // 计时开始（串行）
    double start_time_serial = omp_get_wtime();

    // 执行卷积运算多次（串行）
    int num_iterations = 10; // 设置一个较大的循环次数以增加计时时间
    int* result_serial;
    for (int iter = 0; iter < num_iterations; ++iter) {
        result_serial = matrix_multiply_block_serial(A, size_A, B, size_B);
        free(result_serial); // 释放每次循环分配的内存
    }

    // 计算执行时间（串行）
    double end_time_serial = omp_get_wtime();
    double elapsed_time_serial = end_time_serial - start_time_serial;

    // 计时开始（并行）
    double start_time_parallel = omp_get_wtime();

    // 执行卷积运算多次（并行）
    int* result_parallel;
    for (int iter = 0; iter < num_iterations; ++iter) {
        result_parallel = matrix_multiply_block_parallel(A, size_A, B, size_B);
        free(result_parallel); // 释放每次循环分配的内存
    }

    // 计算执行时间（并行）
    double end_time_parallel = omp_get_wtime();
    double elapsed_time_parallel = end_time_parallel - start_time_parallel;

    // 打印执行时间
    printf("串行卷积运算（%d 次循环）耗时：%.6f 秒\n", num_iterations, elapsed_time_serial);
    printf("并行卷积运算（%d 次循环）耗时：%.6f 秒\n", num_iterations, elapsed_time_parallel);

    // 释放内存
    free(A);
    free(B);

    return 0;
}
