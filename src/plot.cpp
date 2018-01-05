#include "plot.h"
#include <math.h>
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

//データをもとにグラフを描く
void PBM_Creator::plot(CsvData *csv_data) {
    Data s_axisX, e_axisX;
    Data s_axisY, e_axisY;
    int split;
    //出力ファイルの名前を設定する
    filename += csv_data->getFileName();
    //.csvを.pbmに置き換え
    split = filename.find(".csv");
    filename.replace(split, strlen(".csv"), ".pbm");

    //データ範囲を記録
    dataW = csv_data->getDataWidth();
    dataH = csv_data->getDataHeight();
    //負の要素を除くためのバイアス
    biasX = -csv_data->getMin_x();
    biasY = -csv_data->getMin_y();

    //余白を計算
    if (biasX < 0 && -biasX * width / dataW > DEF_MARGIN) {
        w_margin = -biasX * width / dataW;
    } else {
        w_margin = DEF_MARGIN;
    }
    if (biasY < 0 && -biasY * height / dataH > DEF_MARGIN) {
        h_margin = -biasY * height / dataH;
    } else {
        h_margin = DEF_MARGIN;
    }

    cout << "output pbm file: " << filename << endl;
    int i;
    for (i = 0; i < csv_data->getDataNum() - 1; i++) {
        draw_line(csv_data, i, DEF_LINE_SIZE, true);
    }
    //終点に点を打つ
    dot(resize(*csv_data->get_data(i)), DATA_POINT_SIZE);

    // x軸の設定
    s_axisX.x = -biasX - (w_margin)*dataW / width;
    e_axisX.x = -biasX + dataW + (w_margin)*dataW / width;
    s_axisX.y = 0;
    e_axisX.y = 0;

    // y軸の設定
    s_axisY.x = 0;
    e_axisY.x = 0;
    s_axisY.y = -biasY - (h_margin)*dataH / height;
    e_axisY.y = -biasY + dataH + (h_margin)*dataH / height;

    // x軸を引く
    draw_line(s_axisX, e_axisX, DEF_AXIS_SIZE, false);
    // y軸を引く
    draw_line(s_axisY, e_axisY, DEF_AXIS_SIZE, false);
}

//テキスト形式PBMで出力
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

//バイナリ形式PBMで出力
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

//データを画像のサイズに正規化
Data PBM_Creator::resize(Data data) {
    Data ret;
    ret.x = (width - 1 - w_margin * 2) * (data.x + biasX) / dataW + w_margin;
    ret.y = (height - 1 - h_margin * 2) * (data.y + biasY) / dataH + h_margin;
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

//点を打つ
void PBM_Creator::dot(int x, int y, int size) {
    int dx, dy;
    // size x sizeの正方形
    for (dx = -size / 2; dx <= size / 2; dx++) {
        for (dy = -size / 2; dy <= size / 2; dy++) {
            set_black(x + dx, y + dy);
        }
    }
}

void PBM_Creator::dot(Data data, int size) { dot(data.x, data.y, size); }

void PBM_Creator::draw_line(Data data1, Data data2, int line_size,
                            bool put_dot) {
    bool xy_swapped;

    //データの正規化
    data1 = resize(data1);
    data2 = resize(data2);

    if (put_dot) {
        //始点に点を打つ
        dot(data1.x, data1.y, DATA_POINT_SIZE);
    }

    //初期設定
    int dx = abs(data2.x - data1.x);
    int dy = abs(data2.y - data1.y);
    int i1, i2, d, x, y;
    int dir;

    x = data1.x;
    y = data1.y;
    if (dy > dx) {
        //傾きが45度より大きい時はx,yを交換
        xy_swapped = true;
        d = 2 * dx - dy;
        i1 = 2 * dx;
        i2 = 2 * (dx - dy);
        //増加の方向を決める
        if (data1.y < data2.y) {
            dir = 1;
        } else {
            dir = -1;
        }
    } else {
        //傾きが45度より小さい時はそのまま
        xy_swapped = false;
        d = 2 * dy - dx;
        i1 = 2 * dy;
        i2 = 2 * (dy - dx);
        //増加の方向を決める
        if (data1.y < data2.y) {
            dir = 1;
        } else {
            dir = -1;
        }
    }

    if (xy_swapped) {
        // yをずらしながらxを決める
        while (true) {
            dot(x, y, line_size);
            if (y == (int)data2.y) break;
            y += dir;
            if (d <= 0) {
                d += i1;
            } else {
                x++;
                d += i2;
            }
        }
    } else {
        // xをずらしながらyを決める
        while (true) {
            dot(x, y, line_size);
            if (x == (int)data2.x) break;
            x++;
            if (d <= 0)
                d += i1;
            else {
                d += i2;
                y += dir;
            }
        }
    }
}

// xを基準にソートされたデータをもとに線を描く
void PBM_Creator::draw_line(CsvData *csv_data, int s_index, int line_size,
                            bool put_dot) {
    // s_indexとその次のデータを取得し線を引く
    draw_line(*csv_data->get_data(s_index), *csv_data->get_data(s_index + 1),
              line_size, put_dot);
}