#pragma once

#include <string>
#include <vector>
#include "csv_data.h"

class PBM_Creator {
   public:
    PBM_Creator(int width, int height);
    ~PBM_Creator();
    void plot(CsvData* csv_data);
    void overwrite(CsvData* csv_data);
    void output_P1();
    void output_P4();
    int getWidth() { return width; }
    int getHeight() { return height; }
    Data resize(Data data);

   private:
    double getSpan(double base);
    void set_black(int x, int y);
    void set_white(int x, int y);
    void dot(int x, int y, int size);
    void dot(Data data, int size);
    void draw_line(CsvData* csv_data, int s_index, int line_size, bool put_dot);
    void draw_line(Data data1, Data data2, int line_size, bool put_dot);
    void draw_splitX(double x, int count);
    void draw_splitY(double y, int count);
    void draw_string(const char* str, int x, int y);
    void draw_string(const char* str, double x, double y);
    void draw_string(const char* str, double x, double y, int dx, int dy);
    void draw_font(unsigned char c, int x, int y);
    void draw_font(unsigned char c, double x, double y);
    void draw_font(unsigned char c, double x, double y, int dx, int dy);
    std::string filename;
    std::vector<std::vector<char>> pixels;
    int width, height;
    int w_margin, h_margin;
    int w_gap, h_gap;
    double dataW, dataH;
    double biasX, biasY;
};