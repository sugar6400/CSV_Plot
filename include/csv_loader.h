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
    char* getFileName();

   private:
    char* filename;
    //データリスト
    std::vector<Data> data;
};
