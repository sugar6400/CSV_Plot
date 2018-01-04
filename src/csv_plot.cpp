#include <stdio.h>
#include <stdlib.h>
#include "csv_loader.h"
#include "plot.h"

int main(int argc, char* argv[]) {
    CsvData csv_data;
    PPM_Creator ppm_creator;

    if (argc != 2) {
        printf("usage: %s input.csv", argv[0]);
        getchar();
        return EXIT_FAILURE;
    }

    csv_data.load(argv[1]);
    csv_data.print();

    ppm_creator.plot(&csv_data);

    getchar();

    return 0;
}
