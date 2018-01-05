#pragma once

#include <vector>

// 1点のデータ
struct Data {
    double x;
    double y;
};

// CSVから読み込んだデータ
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
    double getDataNum() { return data.size(); }
    Data* get_data(int index) { return &data[index]; }

   private:
    char* filename;
    //データリスト
    std::vector<Data> data;
    double max_x, max_y;
    double min_x, min_y;
};
