#ifndef USER_H
#define USER_H

typedef struct User {
    char username[50];
    char password[50];
} User;

// 用户注册
int registerUser(User* user);

// 用户登陆
int loginUser(User* user);

// 用户退出
void logoutUser();

#endif