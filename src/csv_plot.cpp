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

    // csvを読み込む
    csv_data.load(argv[1]);
    //読み込んだcsvの確認
    csv_data.print();
    //データをxでソート
    csv_data.sort();
    printf("Sorted\n");
    //ソートしたcsvの確認
    csv_data.print();

    // pbmファイルを作成
    pbm_creator.plot(&csv_data);

    pbm_creator.output_P4();

    getchar();

    return 0;
}
