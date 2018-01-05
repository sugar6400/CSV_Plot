#include "plot.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include "const.h"
#include "string.h"

using namespace std;

PBM_Creator::PBM_Creator(int i_width, int i_height) {
    width = i_width;
    height = i_height;
    pixels = vector<vector<char>>(height, vector<char>(width, 0));
}

PBM_Creator::~PBM_Creator() {}

void PBM_Creator::plot(CsvData *csv_data) {
    int split;
    //出力ファイルの名前を設定する
    filename += csv_data->getFileName();
    //.csvを.pbmに置き換え
    split = filename.find(".csv");
    filename.replace(split, strlen(".csv"), ".pbm");

    rawW = csv_data->getDataWidth();
    rawH = csv_data->getDataHeight();
    cout << "output pbm file: " << filename << endl;
    int i;
    for (i = 0; i < csv_data->getDataNum() - 1; i++) {
        draw_line(csv_data, i);
    }
    //終点に点を打つ
    dot(resize(*csv_data->get_data(i)));
}

void PBM_Creator::output_P1() {
    int x, y;
    FILE *fp = fopen(filename.c_str(), "w");
    if (!fp) {
        printf("can't open file: %s", filename.c_str());
        return;
    }
    //ヘッダ出力
    fprintf(fp, "P1\n");
    fprintf(fp, "%u %u\n", width, height);

    //画像データ出力
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            putc('0' + pixels[y][x], fp);
        }
        putc('\n', fp);
    }
    fclose(fp);
}

void PBM_Creator::output_P4() {
    int x, y;
    uint8_t pix8;
    unsigned char shift;
    FILE *fp = fopen(filename.c_str(), "wb");
    if (!fp) {
        printf("can't open file: %s", filename.c_str());
        return;
    }
    //ヘッダ出力
    fprintf(fp, "P4\n");
    fprintf(fp, "%u %u\n", width, height);

    //画像データ出力
    for (y = 0; y < height; y++) {
        shift = 8;
        pix8 = 0;
        for (x = 0; x < width; x++) {
            // 1byte分ずつ保存
            --shift;
            pix8 |= pixels[y][x] << shift;
            if (shift == 0) {
                putc(pix8, fp);
                shift = 8;
                pix8 = 0;
            }
        }
        // 8bitまでいかなくてもx終端まで行ったら出力
        if (shift != 8) {
            putc(pix8, fp);
        }
    }
    fclose(fp);
}

Data PBM_Creator::resize(Data data) {
    Data ret;
    ret.x = (width - 1) * data.x / rawW;
    ret.y = (height - 1) * data.y / rawH;
    return ret;
}

void PBM_Creator::set_black(int x, int y) {
    if (x >= 0 && y > 0 && x < width && y <= height)
        pixels[height - y][x] = BLACK;
}
void PBM_Creator::set_white(int x, int y) {
    if (x >= 0 && y > 0 && x < width && y <= height)
        pixels[height - y][x] = WHITE;
}

void PBM_Creator::dot(int x, int y) {
    //十字に
    int dx, dy;
    for (dx = -1; dx <= 1; dx++) {
        for (dy = -1; dy <= 1; dy++) {
            set_black(x + dx, y + dy);
        }
    }
}

void PBM_Creator::dot(Data data) { dot(data.x, data.y); }

void PBM_Creator::draw_line(CsvData *csv_data, int s_index) {
    Data data1, data2;
    bool xy_swapped;
    // s_indexとその次のデータを取得
    data1 = *csv_data->get_data(s_index);
    data2 = *csv_data->get_data(s_index + 1);

    //データの正規化
    data1 = resize(data1);
    data2 = resize(data2);

    //始点に点を打つ
    dot(data1.x, data1.y);

    int dx = data2.x - data1.x;
    int dy = data2.y - data2.y;
    //傾き1以上
    if (dy > dx) {
        xy_swapped = true;
    }
}