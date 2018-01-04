#include "plot.h"
#include <iostream>
#include <string>
#include "string.h"

using namespace std;

PPM_Creator::PPM_Creator() {}

PPM_Creator::~PPM_Creator() {}

void PPM_Creator::plot(CsvData *csv_data) {
    int split;
    filename += csv_data->getFileName();
    split = filename.find(".csv");
    filename.replace(split, strlen(".csv"), ".ppm");
    cout << "ppm file: " << filename << endl;
}