#include <stdio.h>
#include <stdlib.h>
#include "MyUtil.h"

//���l�ȊO�𖳎�����I�[�o�[�t���[���Ȃ����l����
int getInputNum() {
	char buf[50];
	int out;
	fgets(buf, sizeof(buf), stdin);
	out = atoi(buf);
	return out;
}