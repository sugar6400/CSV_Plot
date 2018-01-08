#include "approx.h"
#include "MatrixUtil.h"
#include "csv_data.h"

Approximation::Approximation(CsvData* csv_data, int dim) {
    setXMatrix(csv_data, dim);
    setYMatrix(csv_data, dim);
    result = Matrix_Alloc(1, dim);
}

Approximation::~Approximation() {
    Matrix_Delete(X);
    Matrix_Delete(Y);
    Matrix_Delete(result);
}

void Approximation::setXMatrix(CsvData* csv_data, int dim) {
    //�[���������Ń������m��
    X = Matrix_Alloc(dim, dim);
    //�����ݒ�
    for (int i = 0; i < X->h; i++) {
        for (int j = 0; j < X->w; j++) {
            for (int k = 0; k < csv_data->getDataNum(); k++) {
                X->v[i][j] += pow(csv_data->get_data(k)->x, i + j);
            }
        }
    }
}

void Approximation::setXMatrix(CsvData* csv_data, int dim) {
    //�[���������Ń������m��
    Y = Matrix_Alloc(1, dim);
    //�����ݒ�
    for (int i = 0; i < Y->h; i++) {
        for (int j = 0; j < Y->w; j++) {
            for (int k = 0; k < csv_data->getDataNum(); k++) {
                Data* data = csv_data->get_data(k);
                X->v[i][j] += pow(data->x, i) * data->y;
            }
        }
    }
}