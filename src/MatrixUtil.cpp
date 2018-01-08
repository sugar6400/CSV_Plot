#include "MatrixUtil.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "MyUtil.h"

Matrix* Matrix_Alloc(int w, int h) {
    int i;
    Matrix* out;

    // Matrixサイズのメモリを確保
    out = (Matrix*)malloc(sizeof(Matrix));
    if (out == NULL) {
        ERRORLOG("Can't allocate memory");
        return NULL;
    }
    //ベクトル本体のサイズのメモリを確保
    out->v = (double**)malloc(h * sizeof(double*));
    if (out->v == NULL) {
        ERRORLOG("Can't allocate memory");
        return NULL;
    }

    for (i = 0; i < h; i++) {
        //行ごとに1次元配列のサイズのメモリを確保(ゼロ初期化)
        out->v[i] = (double*)calloc(w, sizeof(double));
        if (out->v[i] == NULL) {
            ERRORLOG("Can't allocate memory");
            printf("out->v[%d]\n", i);
            //確保したメモリ分だけ開放する
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
    // Nullポインター判定
    if (vec == NULL) {
        ERRORLOG("null pointer");
        return;
    }
    // Nullポインター判定
    if (vec->v == NULL) {
        ERRORLOG("null pointer");
        // Matrixのメモリ開放
        free(vec);
        return;
    }

    //各行ごとにメモリ開放
    for (i = 0; i < vec->h; i++) {
        if (vec->v[i]) {
            free(vec->v[i]);
            vec->v[i] = NULL;
        } else {
            ERRORLOG("null pointer");
            printf("vector->vec[%d]\n", i);
        }
    }
    //ベクトル本体のメモリ開放
    free(vec->v);
    vec->v = NULL;
    // Matrixのメモリ開放
    free(vec);
    vec = NULL;
}

//ベクトルのコピーを作る
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

// vecに単位行列を設定する
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

// vecに零行列を設定する
void Matrix_SetZero(Matrix* vec) {
    int i, j;
    for (i = 0; i < vec->h; i++) {
        for (j = 0; j < vec->w; j++) {
            vec->v[i][j] = 0;
        }
    }
}

// vecの対角成分を1にする
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

// vecの対角成分を0にする
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

//ベクトルに値を代入する
void Matrix_Input(Matrix* vec) {
    int i, j;
    char buffer[50];

    for (i = 0; i < vec->h; i++) {
        for (j = 0; j < vec->w; j++) {
            printf("要素%d,%dを入力\n", i + 1, j + 1);
            //安全な入力(数値以外はatofによって無視される)
            fgets(buffer, sizeof(buffer), stdin);
            vec->v[i][j] = atof(buffer);
        }
    }
    //出力の区切りをはっきりさせる
    putchar('\n');
}

//ベクトルを表示する
void Matrix_Print(const Matrix* vec) {
    int i, j;
    for (i = 0; i < vec->h; i++) {
        for (j = 0; j < vec->w; j++) {
            //整数部3桁 小数部2桁でそろえる
            printf(" %6.2f", vec->v[i][j]);
        }
        putchar('\n');
    }
    putchar('\n');
}

//逆行列を計算してretに入れる
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

//転置ベクトルの取得
void Matrix_GetTrans(const Matrix* vec, Matrix* tr) {
    int i, j;
    if (vec->h != tr->w || vec->w != tr->h) {
        ERRORLOG("転置ベクトルの保存先のベクトルサイズが不正");
        return;
    }
    for (i = 0; i < vec->h; i++) {
        for (j = 0; j < vec->w; j++) {
            tr->v[j][i] = vec->v[i][j];
        }
    }
}

//[vec]を正規化したベクトルを[normal]に設定する(要素ごとに独立vec==normalでの動作可)
void Matrix_GetNormal(const Matrix* vec, Matrix* normal) {
    int i, j;
    double norm = Matrix_GetNorm(vec);
    for (i = 0; i < vec->h; i++) {
        for (j = 0; j < vec->w; j++) {
            normal->v[i][j] = vec->v[i][j] / norm;
        }
    }
}

// row1行とrow2行を入れ替える
void Matrix_Swap(Matrix* vec, int row1, int row2) {
    double* tmp;
    tmp = vec->v[row1];
    vec->v[row1] = vec->v[row2];
    vec->v[row2] = tmp;
}

// col列目で最大値を持つ行を取得する
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

//ベクトルvec1とベクトルvec2の積を求める(outは入力と同じものにできない)
void Matrix_Mul(const Matrix* vec1, const Matrix* vec2, Matrix* out) {
    int i, j, k;

    for (i = 0; i < vec1->h; i++) {
        for (j = 0; j < vec2->w; j++) {
            //初期化
            out->v[i][j] = 0;
            for (k = 0; k < vec1->w; k++) {
                out->v[i][j] += vec1->v[i][k] * vec2->v[k][j];
            }
        }
    }
}

// LU行列を計算する
void Matrix_GetLUP(const Matrix* vec, Matrix* L, Matrix* U, Matrix* P) {
    int i, j, k;
    int pivot;
    Matrix_Copy(vec, U);
    Matrix_SetDiag1(L);
    for (k = 0; k < U->h - 1; k++) {
        //ピボット交換
        pivot = Matrix_GetPivot(U, k);
        if (pivot != k) {
            Matrix_Swap(U, pivot, k);
            Matrix_Swap(P, pivot, k);
            Matrix_SetDiag0(L);
            Matrix_Swap(L, pivot, k);
            Matrix_SetDiag1(L);
        }
        //前進消去
        for (i = k + 1; i < U->h; i++) {
            assert(U->v[k][k] != 0);
            L->v[i][k] = U->v[i][k] / U->v[k][k];
            for (j = k; j < U->h; j++) {
                U->v[i][j] -= U->v[k][j] * L->v[i][k];
            }
        }

            //デバッグ用出力
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

//前進代入
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

//後退代入
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

//連立方程式Ax=bを解きxに代入する
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

//ベクトルノルムを計算する
double Matrix_GetNorm(const Matrix* vec) {
    int i;
    double sum = 0;
    if (vec->h > 1 && vec->w > 1) {
        printf("ベクトルではない入力\n");
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

// vec1とvec2とその内積から角度を計算する
double Matrix_GetVectorAngle(Matrix* vec1, Matrix* vec2, double inner) {
    double norm1 = Matrix_GetNorm(vec1);
    printf("norm1:%.3f\n", norm1);
    double norm2 = Matrix_GetNorm(vec2);
    printf("norm2:%.3f\n", norm2);
    return acos(inner / (norm1 * norm2)) * 180 / PI;
}

//最大固有値を計算するA:ベクトル eps:目標とする誤差 max_loop:最大ループ数
double Matrix_GetMaxEugen(const Matrix* A, double eps, int max_loop) {
    int k;
    double delta;
    double lambda = 100000.0;
    double old_lambda = 100000.0;
    Matrix* X = Matrix_Alloc(1, A->h);
    Matrix* Y = Matrix_Alloc(1, A->h);
    // Yの転置行列
    Matrix* Yt = Matrix_Alloc(A->h, 1);
    //分母, 分子の内積
    Matrix* inner1 = Matrix_Alloc(1, 1);
    Matrix* inner2 = Matrix_Alloc(1, 1);
    //単位ベクトルで初期化することとする
    Matrix_SetIdentity(X);

    for (k = 0; k < max_loop; k++) {
        delta = 0.0;
        // Y = AX;
        Matrix_Mul(A, X, Y);

        Matrix_GetTrans(Y, Yt);

        Matrix_Mul(Yt, Y, inner1);
        Matrix_Mul(Yt, X, inner2);

        lambda = inner1->v[0][0] / inner2->v[0][0];

        // Yを正規化
        Matrix_GetNormal(Y, Y);
        // X=[Y]
        Matrix_Copy(Y, X);

        delta = fabs(lambda - old_lambda) / fabs(old_lambda);
        if (delta < eps) {
            //収束した
            break;
        }

        old_lambda = lambda;
    }

    ERRORLOG("収束しませんでした");

    Matrix_Delete(X);
    Matrix_Delete(Y);
    Matrix_Delete(Yt);
    Matrix_Delete(inner1);
    Matrix_Delete(inner2);

    return lambda;
}

//最小固有値を計算する　[[[未完成]]]
double Matrix_GetMinEugen(const Matrix* A, double eps, int max_loop) {
    int k, j, i;
    double delta;
    double lambda = 100000.0;
    double old_lambda = 100000.0;
    double sum;
    //計算上の一時保存行列
    Matrix* X = Matrix_Alloc(1, A->h);
    Matrix* Y = Matrix_Alloc(1, A->h);
    Matrix* z = Matrix_Alloc(1, A->h);
    // L,U行列
    Matrix* U = Matrix_Alloc(A->h, A->w);
    Matrix* L = Matrix_Alloc(A->h, A->w);
    Matrix* P = Matrix_Alloc(A->h, A->w);
    // Yの転置行列
    Matrix* Yt = Matrix_Alloc(A->h, 1);
    //分母, 分子の内積
    Matrix* inner1 = Matrix_Alloc(1, 1);
    Matrix* inner2 = Matrix_Alloc(1, 1);

    for (i = 0; i < X->h; i++) {
        X->v[i][0] = 1;
    }

    Matrix_GetLUP(A, L, U, P);

    for (k = 0; k < max_loop; k++) {
        delta = 0.0;
        //任意のzを求める
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

        // Yを正規化
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

    ERRORLOG("収束しませんでした");
    return lambda;
}