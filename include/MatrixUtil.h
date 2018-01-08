#pragma once

#define PI 3.1415926535897832384626433832795

typedef struct Matrix {
    // w列
    int w;
    // h行
    int h;
    //ベクトル本体
    double** v;
} Matrix;

// h行, w列の2次元ベクトルのメモリを確保する.
Matrix* Matrix_Alloc(int w, int h);

//メモリ開放
void Matrix_Delete(Matrix* vec);

//ベクトルのコピーを作る
void Matrix_Copy(const Matrix* master, Matrix* out);

// vecに単位行列を設定する
void Matrix_SetIdentity(Matrix* vec);

// vecに零行列を設定する
void Matrix_SetZero(Matrix* vec);

// vecの対角成分を1にする
void Matrix_SetDiag1(Matrix* vec);

void Matrix_SetDiag1_Reverse(Matrix* vec);

// vecの対角成分を0にする
void Matrix_SetDiag0(Matrix* vec);

//ベクトルに値を代入する
void Matrix_Input(Matrix* vec);

//ベクトルを表示する
void Matrix_Print(const Matrix* vec);

//転置ベクトルの取得
void Matrix_GetTrans(const Matrix* vec, Matrix* tr);

//[vec]を正規化したベクトルを[normal]に設定する
void Matrix_GetNormal(const Matrix* vec, Matrix* normal);

// row1行とrow2行を入れ替える
void Matrix_Swap(Matrix* vec, int row1, int row2);

// col列目で最大値を持つ行を取得する
int Matrix_GetPivot(const Matrix* vec, const int col);

void Matrix_Mul(const Matrix* vec1, const Matrix* vec2, Matrix* out);

// LU行列を計算する
void Matrix_GetLUP(const Matrix* vec, Matrix* L, Matrix* U, Matrix* P);

//前進代入
void Matrix_FSolve(Matrix* y, const Matrix* b, const Matrix* L);

//後退代入
void Matrix_BSolve(Matrix* x, const Matrix* y, const Matrix* U);

//連立方程式Ax=bを解きxに代入する
void Matrix_Solve(const Matrix* A, const Matrix* b, Matrix* x);

//ベクトルノルムを計算する
double Matrix_GetNorm(const Matrix* vec);

// vec1とvec2の角度を計算する
double Matrix_GetVectorAngle(Matrix* vec1, Matrix* vec2, double inner);

//最大固有値を計算するA:ベクトル eps:目標とする誤差 max_loop:最大ループ数
double Matrix_GetMaxEugen(const Matrix* A, double eps, int max_loop);

//最小固有値を計算するA:ベクトル eps:目標とする誤差 max_loop:最大ループ数
double Matrix_GetMinEugen(const Matrix* A, double eps, int max_loop);