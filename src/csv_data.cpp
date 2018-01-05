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

// CSV��ǂݍ���
bool CsvData::load(char* in_filename) {
    fstream ifs(in_filename);
    string line;
    string buf;
    max_x = INT_MIN, max_y = INT_MIN;
    min_x = INT_MAX, min_y = INT_MAX;
    if (ifs.fail()) {
        printf("�t�@�C���I�[�v���G���[:%s\n", in_filename);
        return false;
    }
    //�e�s��
    while (getline(ifs, line)) {
        //�R�����g�s�͓ǂݔ�΂�
        if (line.c_str()[0] == '#') continue;
        Data new_data;
        stringstream ss(line);

        //�J���}��؂��2�ǂݍ���
        getline(ss, buf, ',');
        new_data.x = stod(buf);
        getline(ss, buf, ',');
        new_data.y = stod(buf);

        //�ő�l, �ŏ��l���X�V
        if (new_data.x > max_x) max_x = new_data.x;
        if (new_data.y > max_y) max_y = new_data.y;
        if (new_data.x < min_x) min_x = new_data.x;
        if (new_data.y < min_y) min_y = new_data.y;

        //�f�[�^���X�g�ɒǉ�
        data.push_back(new_data);
    }
    //�f�[�^�̑��݂��镝�ƍ������w��

    //�t�@�C�������L�^
    filename = in_filename;
    return true;
}

//�S�v�f���o��
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
