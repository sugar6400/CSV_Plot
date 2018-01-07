#include "plot.h"
#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <string>
#include "bitmap.h"
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
    int split;
    double min_x = csv_data->getMin_x();
    double min_y = csv_data->getMin_y();
    //出力ファイルの名前を設定する
    filename += csv_data->getFileName();
    //.csvを.pbmに置き換え
    split = filename.find(".csv");
    filename.replace(split, strlen(".csv"), ".pbm");

    //データ範囲を記録
    dataW = csv_data->getDataWidth();
    dataH = csv_data->getDataHeight();
    //マイナスの要素を除くためのバイアス
    biasX = -min_x;
    biasY = -min_y;

    //余白設定
    w_margin = DEF_MARGIN;
    h_margin = DEF_MARGIN;

    //最小データまでのピクセルを計算
    if (min_x > 0) {
        w_gap = min_x * (width - 2 * w_margin) / (dataW + 2 * min_x);
    } else {
        w_gap = 0;
    }
    if (min_y > 0) {
        h_gap = min_y * (height - 2 * h_margin) / (dataH + 2 * min_y);
    } else {
        h_gap = 0;
    }

    cout << "output pbm file: " << filename << endl;
    //それぞれの点を結ぶ
    int i;
    for (i = 0; i < csv_data->getDataNum() - 1; i++) {
        draw_line(csv_data, i, DEF_LINE_SIZE, true);
    }
    //終点に点を打つ
    dot(resize(*csv_data->get_data(i)), DATA_POINT_SIZE);

    //  目盛間隔の計算
    double x_span;
    double y_span;
    x_span = getSpan(dataW);
    y_span = getSpan(dataH);

    Data s_axisX, e_axisX;
    Data s_axisY, e_axisY;
    // x軸の設定
    s_axisX.x = -biasX - w_gap - x_span;  // 1メモリ分長く
    e_axisX.x = -biasX + dataW + x_span;
    s_axisX.y = 0;
    e_axisX.y = 0;

    // y軸の設定
    s_axisY.x = 0;
    e_axisY.x = 0;
    s_axisY.y = -biasY - h_gap - y_span;  // 1メモリ分長く
    e_axisY.y = -biasY + dataH + y_span;

    // x軸を引く
    draw_line(s_axisX, e_axisX, DEF_AXIS_SIZE, false);
    // y軸を引く
    draw_line(s_axisY, e_axisY, DEF_AXIS_SIZE, false);

    //軸にメモリをつける
    double x = ((int)(s_axisX.x / x_span)) * x_span;
    double y = ((int)(s_axisY.y / y_span)) * y_span;
    for (; x <= e_axisX.x; x += x_span) {
        draw_splitX(x, round(x / x_span));
    }
    for (; y <= e_axisY.y; y += y_span) {
        draw_splitY(y, round(y / y_span));
    }

    // 1目盛あたりの増加量を描く
    char buf[10];
    snprintf(buf, sizeof(buf), "X[%5.4g]", x_span);
    draw_string(buf, e_axisX.x, e_axisX.y, -8 * 6, 4);
    snprintf(buf, sizeof(buf), "X[%5.4g]", y_span);
    draw_string(buf, e_axisY.x, e_axisY.y, -8 * 5, 8);

    //原点
    draw_font('0', 0.0, 0.0, -4, -12);
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
            // 1byteずつ保存
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
    //(1データ座標移動あたりの画像座標移動)
    //= (画像サイズ - 両端の余白 - 最小データまでの画像座標)/データの存在範囲

    //画像座標　=
    // (データ座標+バイアス) * (1データ座標移動あたりの画像座標移動)+ 余白 +
    // 最小データまでの画像座標
    ret.x = (data.x + biasX) * (width - 2 * w_margin - w_gap) / dataW +
            w_margin + w_gap;
    ret.y = (data.y + biasY) * (height - 2 * h_margin - h_gap) / dataH +
            h_margin + h_gap;
    return ret;
}

double PBM_Creator::getSpan(double base) {
    double shift;

    // shiftの桁合わせ
    for (shift = 1; base / shift < 10; shift /= 10) {
        printf("shift1\n");
    }
    for (; base / shift >= 100; shift *= 10) {
    }
    //おおよそ10splitになるように
    for (; base / shift < 10; shift /= 2) {
    }
    for (; base / shift > 10; shift *= 2) {
    }
    return shift;
}

//指定座標を黒にする
void PBM_Creator::set_black(int x, int y) {
    if (x >= 0 && y > 0 && x < width && y <= height)
        pixels[height - y][x] = BLACK;
}

//指定座標を白にする
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
        //傾きが45度より大きい時にx,yを交換
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

// X軸目盛を描く
void PBM_Creator::draw_splitX(double in_x, int count) {
    Data data = {in_x, 0};
    data = resize(data);
    set_black(data.x, data.y);
    set_black(data.x, data.y + 1);
    set_black(data.x, data.y - 1);
    if (count) {
        //数値を文字列に変換
        char buf[5];
        snprintf(buf, sizeof(buf), "%3d", count);
        draw_string(buf, (int)data.x - 8, (int)data.y - 12);
    }
}

// Y軸目盛を描く
void PBM_Creator::draw_splitY(double in_y, int count) {
    Data data = {0, in_y};
    data = resize(data);
    set_black(data.x, data.y);
    set_black(data.x + 1, data.y);
    set_black(data.x - 1, data.y);
    if (count) {
        //数値を文字列に変換
        char buf[5];
        snprintf(buf, sizeof(buf), "%3d", count);
        draw_string(buf, (int)data.x - 8 * 3, (int)data.y);
    }
}

//指定された文字列を指定画像座標を先頭にして描く
void PBM_Creator::draw_string(const char *str, int x, int y) {
    for (int i = 0; str[i] != '\0'; i++) {
        draw_font(str[i], x + i * 8, y);
    }
}

//指定された文字列を指定データ座標を先頭にして描く
void PBM_Creator::draw_string(const char *str, double x, double y) {
    Data data = {x, y};
    data = resize(data);
    draw_string(str, (int)data.x, (int)data.y);
}

//指定された文字列を指定データ座標からdx,dyずらした位置を先頭にして描く
void PBM_Creator::draw_string(const char *str, double x, double y, int dx,
                              int dy) {
    Data data = {x, y};
    data = resize(data);
    draw_string(str, (int)data.x + dx, (int)data.y + dy);
}

//文字cを画像座標x,yの位置に描く
void PBM_Creator::draw_font(unsigned char c, int x, int y) {
    int shift;

    for (shift = 63; shift >= 0; shift--) {
        if (((FONT[c] >> shift) & 1) == 1) {
            set_black(x - shift % 8, y + shift / 8);
        }
    }
}

//文字cをデータ座標x,yの位置に描く
void PBM_Creator::draw_font(unsigned char c, double x, double y) {
    Data data = {x, y};
    data = resize(data);
    draw_font(c, (int)data.x, (int)data.y);
}

//文字cをデータ座標x,yの位置に描く
void PBM_Creator::draw_font(unsigned char c, double x, double y, int dx,
                            int dy) {
    Data data = {x, y};
    data = resize(data);
    draw_font(c, (int)data.x + dx, (int)data.y + dy);
}
