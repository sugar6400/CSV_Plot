#pragma once

#include <string>
#include "csv_loader.h"

class PPM_Creator {
   public:
    PPM_Creator();
    ~PPM_Creator();
    void plot(CsvData* csv_data);

   private:
    std::string filename;
};