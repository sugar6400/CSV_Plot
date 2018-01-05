#include <stdio.h>
#include <stdlib.h>
#include "const.h"
#include "csv_data.h"
#include "plot.h"

int main(int argc, char* argv[]) {
    CsvData csv_data;
    PBM_Creator pbm_creator(DEF_WIDTH, DEF_HEIGHT);

    if (argc != 2) {
        printf("usage: %s input.csv", argv[0]);
        getchar();
        return EXIT_FAILURE;
    }

    // csv��ǂݍ���
    if (csv_data.load(argv[1])) {
        //�ǂݍ���csv�̊m�F
        csv_data.print();
        printf("loaded\n");
        //�f�[�^��x�Ń\�[�g
        csv_data.sort();

        // pbm�t�@�C�����쐬
        pbm_creator.plot(&csv_data);
        // pbm�t�@�C�����o��
        pbm_creator.output_P4();
    }

    getchar();

    return 0;
}
