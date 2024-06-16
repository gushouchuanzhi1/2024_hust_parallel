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

    // ��ȡ��һ������
    do {
        scanf("%d", &a[num1++]);
    } while ((c = getchar()) != '\n');

    // ��ȡ�ڶ�������
    do {
        scanf("%d", &b[num2++]);
    } while ((c = getchar()) != '\n');

    // ����Ƿ�Ϊ��ȫƽ����
    if (!isPerfectSquare(num1) || !isPerfectSquare(num2)) {
        printf("��������ָ���������ȫƽ������\n");
        return 1;
    }

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

    //// ��ӡ��һ������
    //printf("��һ�������ǣ�\n");
    //printMatrix(matrix1, size1);

    //// ��ӡ�ڶ�������
    //printf("�ڶ��������ǣ�\n");
    //printMatrix(matrix2, size2);

    // ������ת��Ϊһά����
    int* A = (int*)malloc(num1 * sizeof(int));
    int* B = (int*)malloc(num2 * sizeof(int));

    for (int i = 0; i < num1; ++i) {
        A[i] = a[i];
    }
    for (int i = 0; i < num2; ++i) {
        B[i] = b[i];
    }

    // ��ʱ��ʼ
    double start_time = omp_get_wtime();

    // ִ�о������
    int* result = matrix_multiply_block(A, size1, B, size2);

    // ����ִ��ʱ��
    double end_time = omp_get_wtime();
    double elapsed_time = end_time - start_time;

    // ��ӡ������
    int size_convolution = size1 - size2 + 1;
    for (int i = 0; i < size_convolution; ++i) {
        for (int j = 0; j < size_convolution; ++j) {
            printf("%d ", result[i * size_convolution + j]);
        }
        printf("\n");
    }

    // ��ӡִ��ʱ��
    printf("��������ʱ��%.6f ��\n", elapsed_time);

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
    free(result);

    return 0;
}
