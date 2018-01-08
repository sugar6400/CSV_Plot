#include <stdio.h>
#include <stdlib.h>
#include "MyUtil.h"
#include "MatrixUtil.h"

int main() {
	Matrix* A;
	Matrix* b;
	Matrix* x;

	int dim;
	printf("次元数を入力してください\n");
	dim = getInputNum();

	if (dim <= 0) {
		ERRORLOG("不正な次元数");
		return EXIT_FAILURE;
	}

	A = Matrix_Alloc(dim, dim);
	b = Matrix_Alloc(1, dim);
	x = Matrix_Alloc(1, dim);

	printf("係数行列を入力\n");
	Matrix_Input(A);
	printf("解行列を入力\n");
	Matrix_Input(b);

	Matrix_Solve(A, b, x);

	Matrix_Print(x);

	return EXIT_SUCCESS;
}