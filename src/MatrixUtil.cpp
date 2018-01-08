#include "MatrixUtil.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "MyUtil.h"

Matrix* Matrix_Alloc(int w, int h) {
    int i;
    Matrix* out;

    // Matrix�T�C�Y�̃��������m��
    out = (Matrix*)malloc(sizeof(Matrix));
    if (out == NULL) {
        ERRORLOG("Can't allocate memory");
        return NULL;
    }
    //�x�N�g���{�̂̃T�C�Y�̃��������m��
    out->v = (double**)malloc(h * sizeof(double*));
    if (out->v == NULL) {
        ERRORLOG("Can't allocate memory");
        return NULL;
    }

    for (i = 0; i < h; i++) {
        //�s���Ƃ�1�����z��̃T�C�Y�̃��������m��(�[��������)
        out->v[i] = (double*)calloc(w, sizeof(double));
        if (out->v[i] == NULL) {
            ERRORLOG("Can't allocate memory");
            printf("out->v[%d]\n", i);
            //�m�ۂ����������������J������
            Matrix_Delete(out);
            return NULL;
        }
    }

    out->w = w;
    out->h = h;

    return out;
}

void Matrix_Delete(Matrix* vec) {
    int i;
    // Null�|�C���^�[����
    if (vec == NULL) {
        ERRORLOG("null pointer");
        return;
    }
    // Null�|�C���^�[����
    if (vec->v == NULL) {
        ERRORLOG("null pointer");
        // Matrix�̃������J��
        free(vec);
        return;
    }

    //�e�s���ƂɃ������J��
    for (i = 0; i < vec->h; i++) {
        if (vec->v[i]) {
            free(vec->v[i]);
            vec->v[i] = NULL;
        } else {
            ERRORLOG("null pointer");
            printf("vector->vec[%d]\n", i);
        }
    }
    //�x�N�g���{�̂̃������J��
    free(vec->v);
    vec->v = NULL;
    // Matrix�̃������J��
    free(vec);
    vec = NULL;
}

//�x�N�g���̃R�s�[�����
void Matrix_Copy(const Matrix* master, Matrix* out) {
    int i, j;
    if (master->h != out->h || master->w != out->w) {
        ERRORLOG("different size");
        return;
    }
    for (i = 0; i < master->h; i++) {
        for (j = 0; j < master->w; j++) {
            out->v[i][j] = master->v[i][j];
        }
    }
}

// vec�ɒP�ʍs���ݒ肷��
void Matrix_SetIdentity(Matrix* vec) {
    int i, j;
    for (i = 0; i < vec->h; i++) {
        for (j = 0; j < vec->h; j++) {
            if (i == j) {
                vec->v[i][j] = 1;
            } else {
                vec->v[i][j] = 0;
            }
        }
    }
}

// vec�ɗ�s���ݒ肷��
void Matrix_SetZero(Matrix* vec) {
    int i, j;
    for (i = 0; i < vec->h; i++) {
        for (j = 0; j < vec->w; j++) {
            vec->v[i][j] = 0;
        }
    }
}

// vec�̑Ίp������1�ɂ���
void Matrix_SetDiag1(Matrix* vec) {
    int i, j;
    for (i = 0; i < vec->h; i++) {
        for (j = 0; j < vec->w; j++) {
            if (i == j) {
                vec->v[i][j] = 1;
            }
        }
    }
}

// vec�̑Ίp������0�ɂ���
void Matrix_SetDiag0(Matrix* vec) {
    int i, j;
    for (i = 0; i < vec->h; i++) {
        for (j = 0; j < vec->w; j++) {
            if (i == j) {
                vec->v[i][j] = 0;
            }
        }
    }
}

//�x�N�g���ɒl��������
void Matrix_Input(Matrix* vec) {
    int i, j;
    char buffer[50];

    for (i = 0; i < vec->h; i++) {
        for (j = 0; j < vec->w; j++) {
            printf("�v�f%d,%d�����\n", i + 1, j + 1);
            //���S�ȓ���(���l�ȊO��atof�ɂ���Ė��������)
            fgets(buffer, sizeof(buffer), stdin);
            vec->v[i][j] = atof(buffer);
        }
    }
    //�o�͂̋�؂���͂����肳����
    putchar('\n');
}

//�x�N�g����\������
void Matrix_Print(const Matrix* vec) {
    int i, j;
    for (i = 0; i < vec->h; i++) {
        for (j = 0; j < vec->w; j++) {
            //������3�� ������2���ł��낦��
            printf(" %6.2f", vec->v[i][j]);
        }
        putchar('\n');
    }
    putchar('\n');
}

//�t�s����v�Z����ret�ɓ����
void Matrix_GetInverse(const Matrix* A, Matrix* ret) {
    Matrix *L, *Li, *U, *Ui, *P, *I, *tmp;

    L = Matrix_Alloc(A->w, A->h);
    Li = Matrix_Alloc(A->w, A->h);
    U = Matrix_Alloc(A->w, A->h);
    Ui = Matrix_Alloc(A->w, A->h);
    P = Matrix_Alloc(A->w, A->h);
    I = Matrix_Alloc(A->w, A->h);
    tmp = Matrix_Alloc(A->w, A->h);

    Matrix_SetDiag1(I);

    Matrix_GetLUP(A, L, U, P);
    Matrix_Solve(L, I, Li);
    Matrix_Solve(U, I, Ui);

    Matrix_Mul(Ui, Li, tmp);
    Matrix_Mul(tmp, P, ret);
}

//�]�u�x�N�g���̎擾
void Matrix_GetTrans(const Matrix* vec, Matrix* tr) {
    int i, j;
    if (vec->h != tr->w || vec->w != tr->h) {
        ERRORLOG("�]�u�x�N�g���̕ۑ���̃x�N�g���T�C�Y���s��");
        return;
    }
    for (i = 0; i < vec->h; i++) {
        for (j = 0; j < vec->w; j++) {
            tr->v[j][i] = vec->v[i][j];
        }
    }
}

//[vec]�𐳋K�������x�N�g����[normal]�ɐݒ肷��(�v�f���ƂɓƗ�vec==normal�ł̓����)
void Matrix_GetNormal(const Matrix* vec, Matrix* normal) {
    int i, j;
    double norm = Matrix_GetNorm(vec);
    for (i = 0; i < vec->h; i++) {
        for (j = 0; j < vec->w; j++) {
            normal->v[i][j] = vec->v[i][j] / norm;
        }
    }
}

// row1�s��row2�s�����ւ���
void Matrix_Swap(Matrix* vec, int row1, int row2) {
    double* tmp;
    tmp = vec->v[row1];
    vec->v[row1] = vec->v[row2];
    vec->v[row2] = tmp;
}

// col��ڂōő�l�����s���擾����
int Matrix_GetPivot(const Matrix* vec, const int col) {
    double max = vec->v[col][col];
    int pivot = col;
    int i;
    for (i = col; i < vec->h; i++) {
        if (vec->v[i][col] > max) {
            max = vec->v[i][col];
            pivot = i;
        }
    }
    return pivot;
}

//�x�N�g��vec1�ƃx�N�g��vec2�̐ς����߂�(out�͓��͂Ɠ������̂ɂł��Ȃ�)
void Matrix_Mul(const Matrix* vec1, const Matrix* vec2, Matrix* out) {
    int i, j, k;

    for (i = 0; i < vec1->h; i++) {
        for (j = 0; j < vec2->w; j++) {
            //������
            out->v[i][j] = 0;
            for (k = 0; k < vec1->w; k++) {
                out->v[i][j] += vec1->v[i][k] * vec2->v[k][j];
            }
        }
    }
}

// LU�s����v�Z����
void Matrix_GetLUP(const Matrix* vec, Matrix* L, Matrix* U, Matrix* P) {
    int i, j, k;
    int pivot;
    Matrix_Copy(vec, U);
    Matrix_SetDiag1(L);
    for (k = 0; k < U->h - 1; k++) {
        //�s�{�b�g����
        pivot = Matrix_GetPivot(U, k);
        if (pivot != k) {
            Matrix_Swap(U, pivot, k);
            Matrix_Swap(P, pivot, k);
            Matrix_SetDiag0(L);
            Matrix_Swap(L, pivot, k);
            Matrix_SetDiag1(L);
        }
        //�O�i����
        for (i = k + 1; i < U->h; i++) {
            assert(U->v[k][k] != 0);
            L->v[i][k] = U->v[i][k] / U->v[k][k];
            for (j = k; j < U->h; j++) {
                U->v[i][j] -= U->v[k][j] * L->v[i][k];
            }
        }

            //�f�o�b�O�p�o��
#ifdef _DEBUG
        printf("k=%d  U\n", k);
        Matrix_Print(U);
        printf("k=%d  L\n", k);
        Matrix_Print(L);
        printf("k=%d  P\n", k);
        Matrix_Print(P);

#endif  // DEBUG
    }
}

//�O�i���
void Matrix_FSolve(Matrix* A, const Matrix* b, const Matrix* L) {
    int i, j;
    double sum;
    for (i = 0; i < A->h; i++) {
        sum = 0;
        for (j = 0; j < i; j++) {
            sum += L->v[i][j] * A->v[j][0];
        }
        A->v[i][0] = (b->v[i][0] - sum) / L->v[i][i];
    }
}

//��ޑ��
void Matrix_BSolve(Matrix* x, const Matrix* y, const Matrix* U) {
    int i, j;
    double sum;

    for (i = x->h - 1; i >= 0; i--) {
        sum = 0;
        for (j = 0; j < x->h; j++) {
            sum += U->v[i][j] * x->v[j][0];
        }
        // assert(U->v[i][i] != 0);
        if (U->v[i][i] != 0) {
            x->v[i][0] = (y->v[i][0] - sum) / U->v[i][i];
        } else {
            x->v[i][0] = 0;
        }
    }
}

//�A��������Ax=b������x�ɑ������
void Matrix_Solve(const Matrix* A, const Matrix* b, Matrix* x) {
    Matrix *L, *U, *P, *tmpA, *y;
    L = Matrix_Alloc(A->w, A->h);
    U = Matrix_Alloc(A->w, A->h);
    P = Matrix_Alloc(A->w, A->h);
    tmpA = Matrix_Alloc(A->w, A->h);
    y = Matrix_Alloc(1, A->h);

    Matrix_Copy(A, tmpA);

    Matrix_Print(A);  //////////////////////////
    Matrix_Print(b);
    Matrix_Print(x);

    Matrix_SetDiag1(P);

    Matrix_GetLUP(tmpA, L, U, P);

    Matrix_Mul(P, b, y);

    Matrix_FSolve(tmpA, y, L);
    Matrix_BSolve(x, tmpA, U);

    Matrix_Delete(L);
    Matrix_Delete(U);
    Matrix_Delete(P);
    Matrix_Delete(tmpA);
    Matrix_Delete(y);
}

//�x�N�g���m�������v�Z����
double Matrix_GetNorm(const Matrix* vec) {
    int i;
    double sum = 0;
    if (vec->h > 1 && vec->w > 1) {
        printf("�x�N�g���ł͂Ȃ�����\n");
        return -1;
    }
    if (vec->h == 1) {
        for (i = 0; i < vec->w; i++) {
            sum += vec->v[0][i] * vec->v[0][i];
        }
    } else {
        for (i = 0; i < vec->h; i++) {
            sum += vec->v[i][0] * vec->v[i][0];
        }
    }
    return sqrt(sum);
}

// vec1��vec2�Ƃ��̓��ς���p�x���v�Z����
double Matrix_GetVectorAngle(Matrix* vec1, Matrix* vec2, double inner) {
    double norm1 = Matrix_GetNorm(vec1);
    printf("norm1:%.3f\n", norm1);
    double norm2 = Matrix_GetNorm(vec2);
    printf("norm2:%.3f\n", norm2);
    return acos(inner / (norm1 * norm2)) * 180 / PI;
}

//�ő�ŗL�l���v�Z����A:�x�N�g�� eps:�ڕW�Ƃ���덷 max_loop:�ő僋�[�v��
double Matrix_GetMaxEugen(const Matrix* A, double eps, int max_loop) {
    int k;
    double delta;
    double lambda = 100000.0;
    double old_lambda = 100000.0;
    Matrix* X = Matrix_Alloc(1, A->h);
    Matrix* Y = Matrix_Alloc(1, A->h);
    // Y�̓]�u�s��
    Matrix* Yt = Matrix_Alloc(A->h, 1);
    //����, ���q�̓���
    Matrix* inner1 = Matrix_Alloc(1, 1);
    Matrix* inner2 = Matrix_Alloc(1, 1);
    //�P�ʃx�N�g���ŏ��������邱�ƂƂ���
    Matrix_SetIdentity(X);

    for (k = 0; k < max_loop; k++) {
        delta = 0.0;
        // Y = AX;
        Matrix_Mul(A, X, Y);

        Matrix_GetTrans(Y, Yt);

        Matrix_Mul(Yt, Y, inner1);
        Matrix_Mul(Yt, X, inner2);

        lambda = inner1->v[0][0] / inner2->v[0][0];

        // Y�𐳋K��
        Matrix_GetNormal(Y, Y);
        // X=[Y]
        Matrix_Copy(Y, X);

        delta = fabs(lambda - old_lambda) / fabs(old_lambda);
        if (delta < eps) {
            //��������
            break;
        }

        old_lambda = lambda;
    }

    ERRORLOG("�������܂���ł���");

    Matrix_Delete(X);
    Matrix_Delete(Y);
    Matrix_Delete(Yt);
    Matrix_Delete(inner1);
    Matrix_Delete(inner2);

    return lambda;
}

//�ŏ��ŗL�l���v�Z����@[[[������]]]
double Matrix_GetMinEugen(const Matrix* A, double eps, int max_loop) {
    int k, j, i;
    double delta;
    double lambda = 100000.0;
    double old_lambda = 100000.0;
    double sum;
    //�v�Z��̈ꎞ�ۑ��s��
    Matrix* X = Matrix_Alloc(1, A->h);
    Matrix* Y = Matrix_Alloc(1, A->h);
    Matrix* z = Matrix_Alloc(1, A->h);
    // L,U�s��
    Matrix* U = Matrix_Alloc(A->h, A->w);
    Matrix* L = Matrix_Alloc(A->h, A->w);
    Matrix* P = Matrix_Alloc(A->h, A->w);
    // Y�̓]�u�s��
    Matrix* Yt = Matrix_Alloc(A->h, 1);
    //����, ���q�̓���
    Matrix* inner1 = Matrix_Alloc(1, 1);
    Matrix* inner2 = Matrix_Alloc(1, 1);

    for (i = 0; i < X->h; i++) {
        X->v[i][0] = 1;
    }

    Matrix_GetLUP(A, L, U, P);

    for (k = 0; k < max_loop; k++) {
        delta = 0.0;
        //�C�ӂ�z�����߂�
        z->v[0][0] = X->v[0][0];
        // Lz=X;
        for (i = 1; i < L->h; i++) {
            sum = X->v[i][0];
            for (j = 0; j < i; j++) {
                sum -= L->v[i][j] * z->v[j][0];
            }
            z->v[i][0] = sum;
        }

        // Uy = z
        Y->v[0][U->w - 1] = z->v[0][U->w - 1] / U->v[U->w - 1][U->w - 1];
        for (i = Y->h - 2; i >= 0; i--) {
            sum = z->v[i][0];
            for (j = i; j < Y->h; j++) {
                sum -= U->v[i][j] * Y->v[j][0];
            }
            Y->v[i][0] = sum / U->v[i][i];
        }

        Matrix_GetTrans(Y, Yt);

        Matrix_Mul(Yt, Y, inner1);
        Matrix_Mul(Yt, X, inner2);

        lambda = inner2->v[0][0] / inner1->v[0][0];

        // Y�𐳋K��
        Matrix_GetNormal(Y, Y);
        // X=[Y]
        Matrix_Copy(Y, X);

        delta = fabs(lambda - old_lambda) / fabs(old_lambda);
        if (delta < eps) {
            return lambda;
        }

        old_lambda = lambda;
    }

    Matrix_Delete(X);
    Matrix_Delete(Y);
    Matrix_Delete(z);
    Matrix_Delete(U);
    Matrix_Delete(L);
    Matrix_Delete(P);
    Matrix_Delete(Yt);
    Matrix_Delete(inner1);
    Matrix_Delete(inner2);

    ERRORLOG("�������܂���ł���");
    return lambda;
}