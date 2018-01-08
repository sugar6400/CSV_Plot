#include <stdio.h>
#include <stdlib.h>
#include "MyUtil.h"
#include "MatrixUtil.h"

int main() {
	Matrix* A;
	Matrix* b;
	Matrix* x;

	int dim;
	printf("����������͂��Ă�������\n");
	dim = getInputNum();

	if (dim <= 0) {
		ERRORLOG("�s���Ȏ�����");
		return EXIT_FAILURE;
	}

	A = Matrix_Alloc(dim, dim);
	b = Matrix_Alloc(1, dim);
	x = Matrix_Alloc(1, dim);

	printf("�W���s������\n");
	Matrix_Input(A);
	printf("���s������\n");
	Matrix_Input(b);

	Matrix_Solve(A, b, x);

	Matrix_Print(x);

	return EXIT_SUCCESS;
}