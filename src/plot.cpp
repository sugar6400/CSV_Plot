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

//�f�[�^�����ƂɃO���t��`��
void PBM_Creator::plot(CsvData *csv_data) {
    Data s_axisX, e_axisX;
    Data s_axisY, e_axisY;
    int split;
    double x_span;
    double y_span;
    double min_x = csv_data->getMin_x();
    double min_y = csv_data->getMin_y();
    //�o�̓t�@�C���̖��O��ݒ肷��
    filename += csv_data->getFileName();
    //.csv��.pbm�ɒu������
    split = filename.find(".csv");
    filename.replace(split, strlen(".csv"), ".pbm");

    //�f�[�^�͈͂��L�^
    dataW = csv_data->getDataWidth();
    dataH = csv_data->getDataHeight();
    //�}�C�i�X�̗v�f���������߂̃o�C�A�X
    biasX = -min_x;
    biasY = -min_y;

    //�]�����v�Z
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
    //���ꂼ��̓_������
    int i;
    for (i = 0; i < csv_data->getDataNum() - 1; i++) {
        draw_line(csv_data, i, DEF_LINE_SIZE, true);
    }
    //�I�_�ɓ_��ł�
    dot(resize(*csv_data->get_data(i)), DATA_POINT_SIZE);

    // x���̐ݒ�
    s_axisX.x = -biasX - (w_margin)*dataW / width;
    e_axisX.x = -biasX + dataW + (w_margin)*dataW / width;
    s_axisX.y = 0;
    e_axisX.y = 0;

    // y���̐ݒ�
    s_axisY.x = 0;
    e_axisY.x = 0;
    s_axisY.y = -biasY - (h_margin)*dataH / height;
    e_axisY.y = -biasY + dataH + (h_margin)*dataH / height;

    // x��������
    draw_line(s_axisX, e_axisX, DEF_AXIS_SIZE, false);
    // y��������
    draw_line(s_axisY, e_axisY, DEF_AXIS_SIZE, false);

    x_span = getSpan(dataW);
    y_span = getSpan(dataH);
    double x = ((int)(s_axisX.x / x_span)) * x_span;
    double y = ((int)(s_axisY.y / y_span)) * y_span;
    for (; x <= e_axisX.x; x += x_span) {
        draw_splitX(x);
    }
    for (; y <= e_axisY.y; y += y_span) {
        draw_splitY(y);
    }
}

//�e�L�X�g�`��PBM�ŏo��
void PBM_Creator::output_P1() {
    int x, y;
    FILE *fp = fopen(filename.c_str(), "w");
    if (!fp) {
        printf("can't open file: %s", filename.c_str());
        return;
    }
    //�w�b�_�o��
    fprintf(fp, "P1\n");
    fprintf(fp, "%u %u\n", width, height);

    //�摜�f�[�^�o��
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            putc('0' + pixels[y][x], fp);
        }
        putc('\n', fp);
    }
    fclose(fp);
}

//�o�C�i���`��PBM�ŏo��
void PBM_Creator::output_P4() {
    int x, y;
    uint8_t pix8;
    unsigned char shift;
    FILE *fp = fopen(filename.c_str(), "wb");
    if (!fp) {
        printf("can't open file: %s", filename.c_str());
        return;
    }
    //�w�b�_�o��
    fprintf(fp, "P4\n");
    fprintf(fp, "%u %u\n", width, height);

    //�摜�f�[�^�o��
    for (y = 0; y < height; y++) {
        shift = 8;
        pix8 = 0;
        for (x = 0; x < width; x++) {
            // 1byte���ۑ�
            --shift;
            pix8 |= pixels[y][x] << shift;
            if (shift == 0) {
                putc(pix8, fp);
                shift = 8;
                pix8 = 0;
            }
        }
        // 8bit�܂ł����Ȃ��Ă�x�I�[�܂ōs������o��
        if (shift != 8) {
            putc(pix8, fp);
        }
    }
    fclose(fp);
}

//�f�[�^���摜�̃T�C�Y�ɐ��K��
Data PBM_Creator::resize(Data data) {
    Data ret;
    ret.x = (width - 1 - w_margin * 2) * (data.x + biasX) / dataW + w_margin;
    ret.y = (height - 1 - h_margin * 2) * (data.y + biasY) / dataH + h_margin;
    return ret;
}

double PBM_Creator::getSpan(double base) {
    double shift;

    // shift�̌����킹
    for (shift = 1; base / shift < 10; shift /= 10) {
        printf("shift1\n");
    }
    for (; base / shift >= 100; shift *= 10) {
    }
    //�����悻10split�ɂȂ�悤��
    for (; base / shift < 10; shift /= 2) {
    }
    for (; base / shift > 10; shift *= 2) {
    }
    return shift;
}

void PBM_Creator::set_black(int x, int y) {
    if (x >= 0 && y > 0 && x < width && y <= height)
        pixels[height - y][x] = BLACK;
}

void PBM_Creator::set_white(int x, int y) {
    if (x >= 0 && y > 0 && x < width && y <= height)
        pixels[height - y][x] = WHITE;
}

//�_��ł�
void PBM_Creator::dot(int x, int y, int size) {
    int dx, dy;
    // size x size�̐����`
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

    //�f�[�^�̐��K��
    data1 = resize(data1);
    data2 = resize(data2);

    if (put_dot) {
        //�n�_�ɓ_��ł�
        dot(data1.x, data1.y, DATA_POINT_SIZE);
    }

    //�����ݒ�
    int dx = abs(data2.x - data1.x);
    int dy = abs(data2.y - data1.y);
    int i1, i2, d, x, y;
    int dir;

    x = data1.x;
    y = data1.y;
    if (dy > dx) {
        //�X����45�x���傫������x,y������
        xy_swapped = true;
        d = 2 * dx - dy;
        i1 = 2 * dx;
        i2 = 2 * (dx - dy);
        //�����̕��������߂�
        if (data1.y < data2.y) {
            dir = 1;
        } else {
            dir = -1;
        }
    } else {
        //�X����45�x��菬�������͂��̂܂�
        xy_swapped = false;
        d = 2 * dy - dx;
        i1 = 2 * dy;
        i2 = 2 * (dy - dx);
        //�����̕��������߂�
        if (data1.y < data2.y) {
            dir = 1;
        } else {
            dir = -1;
        }
    }

    if (xy_swapped) {
        // y�����炵�Ȃ���x�����߂�
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
        // x�����炵�Ȃ���y�����߂�
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

// x����Ƀ\�[�g���ꂽ�f�[�^�����Ƃɐ���`��
void PBM_Creator::draw_line(CsvData *csv_data, int s_index, int line_size,
                            bool put_dot) {
    // s_index�Ƃ��̎��̃f�[�^���擾����������
    draw_line(*csv_data->get_data(s_index), *csv_data->get_data(s_index + 1),
              line_size, put_dot);
}

void PBM_Creator::draw_splitX(double in_x) {
    Data data = {in_x, 0};
    data = resize(data);
    set_black(data.x, data.y);
    set_black(data.x, data.y + 1);
    set_black(data.x, data.y - 1);
}

void PBM_Creator::draw_splitY(double in_y) {
    Data data = {0, in_y};
    data = resize(data);
    set_black(data.x, data.y);
    set_black(data.x + 1, data.y);
    set_black(data.x - 1, data.y);
}