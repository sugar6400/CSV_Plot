#pragma once

#include <vector>

// 1点のデータ
struct Data {
    int x;
    int y;
};

// CSVから読み込んだデータ
class CsvData {
   public:
    CsvData();
    ~CsvData();
    void load(char* filename);
    void print();
    void sort();
    char* getFileName() { return filename; }
    int getDataWidth() { return width; }
    int getDataHeight() { return height; }
    int getDataNum() { return data.size(); }
    Data* get_data(int index) { return &data[index]; }

   private:
    char* filename;
    //データリスト
    std::vector<Data> data;
    int width, height;
};
