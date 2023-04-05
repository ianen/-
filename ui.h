#ifndef UI_H
#define UI_H

#include "user.h"
#include "note.h"
#include "folder.h"
#include "tag.h"

//识别用户输入的指令并执行相应的功能
Status input(char* command);

//登录注册界面
void loginScreen();

//主界面
void mainScreen();
#endif