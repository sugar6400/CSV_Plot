#include <stdio.h>
#include <stdlib.h>
#include "MyUtil.h"
#include "approx.h"
#include "const.h"
#include "csv_data.h"
#include "pbm.h"

int main(int argc, char* argv[]) {
    CsvData csv_data;
    int approx_dim;
    PBM_Creator pbm_creator(DEF_WIDTH, DEF_HEIGHT);

    if (argc != 2) {
        printf("usage: %s input.csv", argv[0]);
        getchar();
        return EXIT_FAILURE;
    }

    // csv��ǂݍ���
    if (!csv_data.load(argv[1])) {
        printf("csv�̓ǂݍ��݂Ɏ��s���܂���\n");
        return EXIT_FAILURE;
    }
    //�ǂݍ���csv�̊m�F
    csv_data.print();
    printf("loaded\n");
    //�f�[�^��x�Ń\�[�g
    csv_data.sort();

    // pbm�t�@�C�����쐬
    pbm_creator.plot(&csv_data);

    printf("�ߎ��Ȑ��̎��������(0:���� 1->:�ߎ��Ȑ�����):");
    approx_dim = getInputNum();
    if (approx_dim > 0) {
        Approximation appro(&csv_data, approx_dim);
        appro.calc_choeff();
        pbm_creator.overwrite(&appro);
    }

    // pbm�t�@�C�����o��
    pbm_creator.output_P4();

    getchar();

    return EXIT_SUCCESS;
}
