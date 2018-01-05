#include "csv_data.h"
#include <limits.h>
#include <stdio.h>
#include <algorithm>
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
    int max_x = 0, max_y = 0;
    int min_x = 0, min_y = 0;
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

        //最大値, 最小値を更新
        if (new_data.x > max_x) max_x = new_data.x;
        if (new_data.y > max_y) max_y = new_data.y;
        if (new_data.x < min_x) min_x = new_data.x;
        if (new_data.y < min_y) min_y = new_data.y;

        //データリストに追加
        data.push_back(new_data);
    }
    //データの存在する幅と高さを指定
    width = max_x - min_x;
    height = max_y - min_y;

    //ファイル名を記録
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

bool operator<(const Data& left, const Data& right) { return left.x < right.x; }

void CsvData::sort() { std::sort(data.begin(), data.end()); }
