#include <stdio.h>
#include <stdlib.h>
#include "MyUtil.h"

//数値以外を無視するオーバーフローしない数値入力
int getInputNum() {
	char buf[50];
	int out;
	fgets(buf, sizeof(buf), stdin);
	out = atoi(buf);
	return out;
}