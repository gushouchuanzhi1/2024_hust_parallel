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

// Function to print a matrix
void printMatrix(int** matrix, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Function to perform block matrix multiplication (convolution) without parallelization
int* matrix_multiply_block_serial(int* A, int size_A, int* B, int size_B) {
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

// Function to perform block matrix multiplication (convolution) with parallelization
int* matrix_multiply_block_parallel(int* A, int size_A, int* B, int size_B) {
    int size_convolution = size_A - size_B + 1;
    int* res = (int*)malloc(size_convolution * size_convolution * sizeof(int));

#pragma omp parallel for collapse(2)
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

    // ��ȡ��һ������
    do {
        scanf("%d", &a[num1++]);
    } while ((c = getchar()) != '\n');

    // ��ȡ�ڶ�������
    do {
        scanf("%d", &b[num2++]);
    } while ((c = getchar()) != '\n');

    // ��������Ƿ�Ϊ��ȫƽ����
    if (!isPerfectSquare(num1) || !isPerfectSquare(num2)) {
        printf("��������ָ���������ȫƽ������\n");
        return 1;
    }

    // �������Ĵ�С
    int size1 = sqrt(num1);
    int size2 = sqrt(num2);

    // �����һ��������ڴ�
    int** matrix1 = (int**)malloc(size1 * sizeof(int*));
    for (int i = 0; i < size1; ++i) {
        matrix1[i] = (int*)malloc(size1 * sizeof(int));
    }

    // ����ڶ���������ڴ�
    int** matrix2 = (int**)malloc(size2 * sizeof(int*));
    for (int i = 0; i < size2; ++i) {
        matrix2[i] = (int*)malloc(size2 * sizeof(int));
    }

    // ����һ�������Ԫ����䵽��һ��������
    for (int i = 0; i < num1; ++i) {
        matrix1[i / size1][i % size1] = a[i];
    }

    // ���ڶ��������Ԫ����䵽�ڶ���������
    for (int i = 0; i < num2; ++i) {
        matrix2[i / size2][i % size2] = b[i];
    }

    // ������ת��Ϊһά����
    int* A = (int*)malloc(num1 * sizeof(int));
    int* B = (int*)malloc(num2 * sizeof(int));

    for (int i = 0; i < num1; ++i) {
        A[i] = a[i];
    }
    for (int i = 0; i < num2; ++i) {
        B[i] = b[i];
    }

    // ��ʱ��ʼ�����У�
    double start_time_serial = omp_get_wtime();

    // ִ�о�������Σ����У�
    int num_iterations = 10000000; // ����һ���ϴ��ѭ�����������Ӽ�ʱʱ��
    int* result_serial;
    for (int iter = 0; iter < num_iterations; ++iter) {
        result_serial = matrix_multiply_block_serial(A, size1, B, size2);
        free(result_serial); // �ͷ�ÿ��ѭ��������ڴ�
    }

    // ����ִ��ʱ�䣨���У�
    double end_time_serial = omp_get_wtime();
    double elapsed_time_serial = end_time_serial - start_time_serial;

    // ��ʱ��ʼ�����У�
    double start_time_parallel = omp_get_wtime();

    // ִ�о�������Σ����У�
    int* result_parallel;
    for (int iter = 0; iter < num_iterations; ++iter) {
        result_parallel = matrix_multiply_block_parallel(A, size1, B, size2);
        free(result_parallel); // �ͷ�ÿ��ѭ��������ڴ�
    }

    // ����ִ��ʱ�䣨���У�
    double end_time_parallel = omp_get_wtime();
    double elapsed_time_parallel = end_time_parallel - start_time_parallel;

    // ��ӡִ��ʱ��
    printf("���о�����㣨%d ��ѭ������ʱ��%.6f ��\n", num_iterations, elapsed_time_serial);
    printf("���о�����㣨%d ��ѭ������ʱ��%.6f ��\n", num_iterations, elapsed_time_parallel);

    // �ͷ��ڴ�
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

    return 0;
}
