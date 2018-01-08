#pragma once

#include "MatrixUtil.h"
#include "csv_data.h"

class Approximation {
   public:
    Approximation(CsvData* data, int dim);
    ~Approximation();
    void calc_choeff();
    double approx_func(double x);

   private:
    void setXMatrix(CsvData* csv_data, int dim);
    void setYMatrix(CsvData* csv_data, int dim);
    int dim;
    Matrix* X;
    Matrix* Y;
    Matrix* result;
};