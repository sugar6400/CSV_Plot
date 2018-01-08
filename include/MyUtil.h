#ifndef MYUTIL
#define MYUTIL

//エラーメッセージをファイル名と行数を含めて表示する
#define ERRORLOG(msg) printf("FILE: %s  Line:%d\nlog:%s\n",__FILE__,__LINE__,msg)

//数値以外を無視するオーバーフローしない数値入力
int getInputNum();

#endif // !MYUTIL
