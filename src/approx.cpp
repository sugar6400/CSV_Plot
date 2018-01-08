#include "approx.h"
#include <math.h>
#include "MatrixUtil.h"
#include "csv_data.h"

Approximation::Approximation(CsvData* csv_data, int dim) {
    setXMatrix(csv_data, dim);
    setYMatrix(csv_data, dim);
    result = Matrix_Alloc(1, dim + 1);
}

Approximation::~Approximation() {
    Matrix_Delete(X);
    Matrix_Delete(Y);
    Matrix_Delete(result);
}

void Approximation::setXMatrix(CsvData* csv_data, int dim) {
    //ƒ[ƒ‰Šú‰»‚Åƒƒ‚ƒŠŠm•Û
    X = Matrix_Alloc(dim + 1, dim + 1);
    //‰Šúİ’è
    for (int i = 0; i < X->h; i++) {
        for (int j = 0; j < X->w; j++) {
            for (int k = 0; k < csv_data->getDataNum(); k++) {
                X->v[i][j] += pow(csv_data->get_data(k)->x, i + j);
            }
        }
    }
    Matrix_Print(X);
}

void Approximation::setYMatrix(CsvData* csv_data, int dim) {
    Data* data;
    //ƒ[ƒ‰Šú‰»‚Åƒƒ‚ƒŠŠm•Û
    Y = Matrix_Alloc(1, dim + 1);
    //‰Šúİ’è
    for (int i = 0; i < Y->h; i++) {
        for (int k = 0; k < csv_data->getDataNum(); k++) {
            data = csv_data->get_data(k);
            Y->v[i][0] += pow(data->x, i) * data->y;
        }
    }
}

void Approximation::calc_choeff() { Matrix_Solve(X, Y, result); }

double Approximation::approx_func(double x) {
    double ret = 0;
    for (int i = 0; i < result->h; i++) {
        ret += result->v[i][0] * pow(x, i);
    }
    return ret;
}