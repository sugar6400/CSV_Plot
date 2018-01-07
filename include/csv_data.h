#pragma once

#include <vector>

// 1点の�?ータ
struct Data {
    double x;
    double y;
};

// CSVから読み込んだ�?ータ
class CsvData {
   public:
    CsvData();
    ~CsvData();
    bool load(char* filename);
    void print();
    void sort();
    char* getFileName() { return filename; }
    double getDataWidth() { return max_x - min_x; }
    double getDataHeight() { return max_y - min_y; }
    double getMin_x() { return min_x; }
    double getMin_y() { return min_y; }
    double getMax_x() { return max_x; }
    double getMax_y() { return max_y; }
    double getDataNum() { return data.size(); }
    Data* get_data(int index) { return &data[index]; }

   private:
    char* filename;
    //�?ータリス�?
    std::vector<Data> data;
    double max_x, max_y;
    double min_x, min_y;
};
