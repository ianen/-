#ifndef USER_H
#define USER_H

typedef struct User {
    char username[50];
    char password[50];
} User;

// �û�ע��
int registerUser(User* user);

// �û���½
int loginUser(User* user);

// �û��˳�
void logoutUser();

#endif