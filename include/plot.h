#pragma once

#include <string>
#include <vector>
#include "csv_data.h"

class PBM_Creator {
   public:
    PBM_Creator(int width, int height);
    ~PBM_Creator();
    void plot(CsvData* csv_data);
    void output_P1();
    void output_P4();
    Data resize(Data data);
    int getWidth() { return width; }
    int getHeight() { return height; }

   private:
    void set_black(int x, int y);
    void set_white(int x, int y);
    void dot(int x, int y);
    void dot(Data data);
    void draw_line(CsvData* csv_data, int s_index);
    void mid_point();
    std::string filename;
    std::vector<std::vector<char>> pixels;
    int width, height;
    int rawW, rawH;
};