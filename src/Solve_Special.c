#include <stdio.h>
#include <stdlib.h>
#include "MyUtil.h"
#include "MatrixUtil.h"

int main() {
	Matrix* A;
	Matrix* b;
	Matrix* x;

	int dim;
	printf("ŸŒ³”‚ğ“ü—Í‚µ‚Ä‚­‚¾‚³‚¢\n");
	dim = getInputNum();

	if (dim <= 0) {
		ERRORLOG("•s³‚ÈŸŒ³”");
		return EXIT_FAILURE;
	}

	A = Matrix_Alloc(dim, dim);
	b = Matrix_Alloc(1, dim);
	x = Matrix_Alloc(1, dim);

	printf("ŒW”s—ñ‚ğ“ü—Í\n");
	Matrix_Input(A);
	printf("‰ğs—ñ‚ğ“ü—Í\n");
	Matrix_Input(b);

	Matrix_Solve(A, b, x);

	Matrix_Print(x);

	return EXIT_SUCCESS;
}