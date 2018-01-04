#include "csv_loader.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

CsvData::CsvData() { filename = NULL; }

CsvData::~CsvData() {}

void CsvData::load(char* in_filename) {
    fstream ifs(in_filename);
    string line;
    string buf;
    if (ifs.fail()) {
        cerr << "ファイルオープンエラー";
        return;
    }
    //各行で
    while (getline(ifs, line)) {
        Data new_data;
        stringstream ss(line);

        //カンマ区切りで2つ読み込む
        getline(ss, buf, ',');
        new_data.x = stoi(buf);
        getline(ss, buf, ',');
        new_data.y = stoi(buf);

        //データリストに追加
        data.push_back(new_data);
    }
    filename = in_filename;
}

void CsvData::print() {
    unsigned int i;
    if (filename != NULL) {
        cout << "File:" << filename << endl;
        cout << "size:" << data.size() << endl;
        for (i = 0; i < data.size(); i++) {
            printf("%d: %d, %d\n", i, data[i].x, data[i].y);
        }
    }
}

char* CsvData::getFileName() { return filename; }
