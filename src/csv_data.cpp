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

// CSVを読み込む
bool CsvData::load(char* in_filename) {
    fstream ifs(in_filename);
    string line;
    string buf;
    max_x = INT_MIN, max_y = INT_MIN;
    min_x = INT_MAX, min_y = INT_MAX;
    if (ifs.fail()) {
        printf("ファイルオープンエラー:%s\n", in_filename);
        return false;
    }
    //各行で
    while (getline(ifs, line)) {
        //コメント行は読み飛ばす
        if (line.c_str()[0] == '#') continue;
        Data new_data;
        stringstream ss(line);

        //カンマ区切りで2つ読み込む
        getline(ss, buf, ',');
        new_data.x = stod(buf);
        getline(ss, buf, ',');
        new_data.y = stod(buf);

        //最大値, 最小値を更新
        if (new_data.x > max_x) max_x = new_data.x;
        if (new_data.y > max_y) max_y = new_data.y;
        if (new_data.x < min_x) min_x = new_data.x;
        if (new_data.y < min_y) min_y = new_data.y;

        //データリストに追加
        data.push_back(new_data);
    }
    //データの存在する幅と高さを指定

    //ファイル名を記録
    filename = in_filename;
    return true;
}

//全要素を出力
void CsvData::print() {
    unsigned int i;
    if (filename != NULL) {
        cout << "File:" << filename << endl;
        cout << "size:" << data.size() << endl;
        for (i = 0; i < data.size(); i++) {
            printf("%d: %.3lf, %.3lf\n", i, data[i].x, data[i].y);
        }
    }
}

bool operator<(const Data& left, const Data& right) { return left.x < right.x; }

void CsvData::sort() { std::sort(data.begin(), data.end()); }
