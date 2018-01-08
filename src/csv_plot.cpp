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

    // csvを読み込む
    if (!csv_data.load(argv[1])) {
        printf("csvの読み込みに失敗しました\n");
        return EXIT_FAILURE;
    }
    //読み込んだcsvの確認
    csv_data.print();
    printf("loaded\n");
    //データをxでソート
    csv_data.sort();

    // pbmファイルを作成
    pbm_creator.plot(&csv_data);

    printf("近似曲線の次数を入力(0:無し 1->:近似曲線次数):");
    approx_dim = getInputNum();
    if (approx_dim > 0) {
        Approximation appro(&csv_data, approx_dim);
        appro.calc_choeff();
        pbm_creator.overwrite(&appro);
    }

    // pbmファイルを出力
    pbm_creator.output_P4();

    getchar();

    return EXIT_SUCCESS;
}
