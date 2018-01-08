#pragma once

#define PI 3.1415926535897832384626433832795

typedef struct Matrix {
    // w��
    int w;
    // h�s
    int h;
    //�x�N�g���{��
    double** v;
} Matrix;

// h�s, w���2�����x�N�g���̃��������m�ۂ���.
Matrix* Matrix_Alloc(int w, int h);

//�������J��
void Matrix_Delete(Matrix* vec);

//�x�N�g���̃R�s�[�����
void Matrix_Copy(const Matrix* master, Matrix* out);

// vec�ɒP�ʍs���ݒ肷��
void Matrix_SetIdentity(Matrix* vec);

// vec�ɗ�s���ݒ肷��
void Matrix_SetZero(Matrix* vec);

// vec�̑Ίp������1�ɂ���
void Matrix_SetDiag1(Matrix* vec);

void Matrix_SetDiag1_Reverse(Matrix* vec);

// vec�̑Ίp������0�ɂ���
void Matrix_SetDiag0(Matrix* vec);

//�x�N�g���ɒl��������
void Matrix_Input(Matrix* vec);

//�x�N�g����\������
void Matrix_Print(const Matrix* vec);

//�]�u�x�N�g���̎擾
void Matrix_GetTrans(const Matrix* vec, Matrix* tr);

//[vec]�𐳋K�������x�N�g����[normal]�ɐݒ肷��
void Matrix_GetNormal(const Matrix* vec, Matrix* normal);

// row1�s��row2�s�����ւ���
void Matrix_Swap(Matrix* vec, int row1, int row2);

// col��ڂōő�l�����s���擾����
int Matrix_GetPivot(const Matrix* vec, const int col);

void Matrix_Mul(const Matrix* vec1, const Matrix* vec2, Matrix* out);

// LU�s����v�Z����
void Matrix_GetLUP(const Matrix* vec, Matrix* L, Matrix* U, Matrix* P);

//�O�i���
void Matrix_FSolve(Matrix* y, const Matrix* b, const Matrix* L);

//��ޑ��
void Matrix_BSolve(Matrix* x, const Matrix* y, const Matrix* U);

//�A��������Ax=b������x�ɑ������
void Matrix_Solve(const Matrix* A, const Matrix* b, Matrix* x);

//�x�N�g���m�������v�Z����
double Matrix_GetNorm(const Matrix* vec);

// vec1��vec2�̊p�x���v�Z����
double Matrix_GetVectorAngle(Matrix* vec1, Matrix* vec2, double inner);

//�ő�ŗL�l���v�Z����A:�x�N�g�� eps:�ڕW�Ƃ���덷 max_loop:�ő僋�[�v��
double Matrix_GetMaxEugen(const Matrix* A, double eps, int max_loop);

//�ŏ��ŗL�l���v�Z����A:�x�N�g�� eps:�ڕW�Ƃ���덷 max_loop:�ő僋�[�v��
double Matrix_GetMinEugen(const Matrix* A, double eps, int max_loop);