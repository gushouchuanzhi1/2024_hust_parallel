#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>


int isPerfectSquare(int num) {
    int root = (int)sqrt(num);
    return root * root == num;
}

void printMatrix(int** matrix, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int* matrix_multiply_block(int* A, int size_A, int* B, int size_B) {
    int size_convolution = size_A - size_B + 1;
    int* res = (int*)malloc(size_convolution * size_convolution * sizeof(int));


    for (int i = 0; i < size_convolution; i++) {
        for (int j = 0; j < size_convolution; j++) {
            int calc = 0;
            int row, col;
            for (row = 0; row < size_B / 2; row += 2)
                for (col = 0; col < size_B / 2; col += 2) {
                    calc += A[(row + 0 + i) * size_A + col + 0 + j] * B[(row + 0) * size_B + col + 0];
                    calc += A[(row + 0 + i) * size_A + col + 1 + j] * B[(row + 0) * size_B + col + 1];
                    calc += A[(row + 1 + i) * size_A + col + 0 + j] * B[(row + 1) * size_B + col + 0];
                    calc += A[(row + 1 + i) * size_A + col + 1 + j] * B[(row + 1) * size_B + col + 1];
                }
            if (size_B % 2) {
                for (col = 0; col < size_B / 2; col += 2) {
                    calc += A[(size_B - 1 + i) * size_A + col + 0 + j] * B[(size_B - 1 + 0) * size_B + col + 0];
                    calc += A[(size_B - 1 + i) * size_A + col + 1 + j] * B[(size_B - 1 + 0) * size_B + col + 1];
                }
                calc += A[(size_B - 1 + i) * size_A + size_B - 1 + j] * B[size_B * size_B - 1];
            }
            res[i * size_convolution + j] = calc;
        }
    }
    return res;
}

int main() {
    int a[100];
    int b[100];
    int num1 = 0, num2 = 0;
    char c;

    // 读取第一行数字
    do {
        scanf("%d", &a[num1++]);
    } while ((c = getchar()) != '\n');

    // 读取第二行数字
    do {
        scanf("%d", &b[num2++]);
    } while ((c = getchar()) != '\n');

    // 检查是否为完全平方数
    if (!isPerfectSquare(num1) || !isPerfectSquare(num2)) {
        printf("输入的数字个数不是完全平方数。\n");
        return 1;
    }

    int size1 = sqrt(num1);
    int size2 = sqrt(num2);

    // 分配第一个矩阵的内存
    int** matrix1 = (int**)malloc(size1 * sizeof(int*));
    for (int i = 0; i < size1; ++i) {
        matrix1[i] = (int*)malloc(size1 * sizeof(int));
    }

    // 分配第二个矩阵的内存
    int** matrix2 = (int**)malloc(size2 * sizeof(int*));
    for (int i = 0; i < size2; ++i) {
        matrix2[i] = (int*)malloc(size2 * sizeof(int));
    }

    // 将第一个数组的元素填充到第一个矩阵中
    for (int i = 0; i < num1; ++i) {
        matrix1[i / size1][i % size1] = a[i];
    }

    // 将第二个数组的元素填充到第二个矩阵中
    for (int i = 0; i < num2; ++i) {
        matrix2[i / size2][i % size2] = b[i];
    }

    //// 打印第一个矩阵
    //printf("第一个矩阵是：\n");
    //printMatrix(matrix1, size1);

    //// 打印第二个矩阵
    //printf("第二个矩阵是：\n");
    //printMatrix(matrix2, size2);

    // 将矩阵转换为一维数组
    int* A = (int*)malloc(num1 * sizeof(int));
    int* B = (int*)malloc(num2 * sizeof(int));

    for (int i = 0; i < num1; ++i) {
        A[i] = a[i];
    }
    for (int i = 0; i < num2; ++i) {
        B[i] = b[i];
    }

    // 计时开始
    double start_time = omp_get_wtime();

    // 执行卷积运算
    int* result = matrix_multiply_block(A, size1, B, size2);

    // 计算执行时间
    double end_time = omp_get_wtime();
    double elapsed_time = end_time - start_time;

    // 打印卷积结果
    int size_convolution = size1 - size2 + 1;
    for (int i = 0; i < size_convolution; ++i) {
        for (int j = 0; j < size_convolution; ++j) {
            printf("%d ", result[i * size_convolution + j]);
        }
        printf("\n");
    }

    // 打印执行时间
    printf("卷积运算耗时：%.6f 秒\n", elapsed_time);

    // 释放内存
    for (int i = 0; i < size1; ++i) {
        free(matrix1[i]);
    }
    free(matrix1);

    for (int i = 0; i < size2; ++i) {
        free(matrix2[i]);
    }
    free(matrix2);

    free(A);
    free(B);
    free(result);

    return 0;
}
