#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 300

int** create_matrix(int);
int** create_matrix_zero(int n);
int** strassen(int**, int**, int n);
int** add(int**, int**, int);
int** sub(int**, int**, int);
int** divide(int** matrixA, int n, int row, int col);
void combine(int**, int**, int, int, int);


int main() {
	//지정된 크기의 매트릭스 생성
	int n = MAX;
	int** matrixA = create_matrix(MAX);
	int** matrixB = create_matrix(MAX);

	time_t start = time(NULL);

	//행렬 곱셈
	int** strassensRes = strassen(matrixA, matrixB, n);

	time_t end = time(NULL);
	printf("소요 시간 : %lf\n", (double)(end - start));

	return 0;
}

//랜덤한 값을 가지는 매트릭스 생성
int** create_matrix(int n) {
	srand(time(NULL));

	int** array = (int**)malloc(n * sizeof(int*));

	for (int i = 0; i < n; i++) {
		array[i] = (int*)malloc(n * sizeof(int));
		for (int j = 0; j < n; j++) {
			array[i][j] = rand()%11;
		}
	}

	return array;
}

//0으로 초기화된 매트릭스 생성
int** create_matrix_zero(int n) {
	int** array = (int**)malloc(n * sizeof(int*));

	for (int i = 0; i < n; i++) {
		array[i] = (int*)malloc(n * sizeof(int));
		for (int j = 0; j < n; j++) {
			array[i][j] = 0;
		}
	}

	return array;
}

//슈트라센 알고리즘
int** strassen(int** matrixA, int** matrixB, int n) {
	int** result = create_matrix_zero(n);

	if (n > 1) {
		//매트릭스 분할
		int** a11 = divide(matrixA, n, 0, 0);
		int** a12 = divide(matrixA, n, 0, (n / 2));
		int** a21 = divide(matrixA, n, (n / 2), 0);
		int** a22 = divide(matrixA, n, (n / 2), (n / 2));
		int** b11 = divide(matrixB, n, 0, 0);
		int** b12 = divide(matrixB, n, 0, n / 2);
		int** b21 = divide(matrixB, n, n / 2, 0);
		int** b22 = divide(matrixB, n, n / 2, n / 2);

		//슈트라센 분할 정복
		int** m1 = strassen(add(a11, a22, n / 2), add(b11, b22, n / 2), n / 2);
		int** m2 = strassen(add(a21, a22, n / 2), b11, n / 2);
		int** m3 = strassen(a11, sub(b12, b22, n / 2), n / 2);
		int** m4 = strassen(a22, sub(b21, b11, n / 2), n / 2);
		int** m5 = strassen(add(a11, a12, n / 2), b22, n / 2);
		int** m6 = strassen(sub(a21, a11, n / 2), add(b11, b12, n / 2), n / 2);
		int** m7 = strassen(sub(a12, a22, n / 2), add(b21, b22, n / 2), n / 2);

		//슈트라센 계산
		int** c11 = add(sub(add(m1, m4, n / 2), m5, n / 2), m7, n / 2);
		int** c12 = add(m3, m5, n / 2);
		int** c21 = add(m2, m4, n / 2);
		int** c22 = add(sub(add(m1, m3, n / 2), m2, n / 2), m6, n / 2);
		
		//결과 생성
		combine(c11, result, n / 2, 0, 0);
		combine(c12, result, n / 2, 0, n / 2);
		combine(c21, result, n / 2, n / 2, 0);
		combine(c22, result, n / 2, n / 2, n / 2);
	}
	else
		result[0][0] = matrixA[0][0] * matrixB[0][0];

	return result;
}

//매트릭스 덧셈
int** add(int** matrixA, int** matrixB, int n) {
	int** result = create_matrix_zero(n);

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			result[i][j] = matrixA[i][j] + matrixB[i][j];

	return result;
}

//매트릭스 뺄셈
int** sub(int** matrixA, int** matrixB, int n) {
	int** result = create_matrix_zero(n);

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			result[i][j] = matrixA[i][j] - matrixB[i][j];

	return result;
}

//매트릭스 분할
int** divide(int** matrix, int n, int row, int col) {
	int div_n = n / 2;
	int r = row, c = col;
	int** array = create_matrix_zero(div_n);

	for (int i = 0; i < div_n; i++) {
		c = col;
		for (int j = 0; j < div_n; j++) {
			array[i][j] = matrix[r][c];
			c++;
		}
		r++;
	}

	return array;
}

//결과 매트릭스 생성
void combine(int** matrix, int** result, int n, int row, int col) {
	int r = row, c = col;
	for (int i = 0; i < n; i++) {
		c = col;
		for (int j = 0; j < n; j++) {
			result[r][c] = matrix[i][j];
			c++;
		}
		r++;
	}
}


