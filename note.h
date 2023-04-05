// note.h
#ifndef NOTE_H
#define NOTE_H

#define MAX_NOTE_NAME_LENGTH    100
#define MAX_NOTE_PATH_LENGTH    100
#define MAX_NOTE_CREATER_LENGTH 100

#include "folder.h"

typedef struct note {
    char creater[MAX_NOTE_CREATER_LENGTH];//����������
    char title[MAX_NOTE_NAME_LENGTH];//�ʼ�����
    char path[MAX_NOTE_PATH_LENGTH];//�ʼ�ʵ��·��
    Tag* tag; //�ļ��б�ǩ�б�
    struct note* next;
} Note;

// �����ʼ��ļ��ڵ�
Note* createNote(char* creater, char* noteTitle, char* currentPath);

// ��ӱʼǵ��ļ���
Status addNoteToFolder(Folder* folder, Note* newNote);

//�ڵ�ǰ�ļ��д����±ʼ��ļ�����Ӧmkdir <�ʼ��ļ���>
Status addNewNoteToFolder(Folder* currentfolder, char* creater, char* noteTitle);

// ��ָ���ʼ��ļ�����ɾ��
Status deleteNote(Note* note);

// �ӵ�ǰ�ļ�����ɾ���ʼ�,��Ӧ rm <�ʼ��ļ���>
Status deleteNoteFromFolder(Folder* folder, char* noteTitle);

//���ʼ����������ʼ��ļ�
Note* searchNoteByName(Folder* root, char* noteName);

// ���ʼ��ļ��ƶ���ָ�����ļ���Ŀ¼��,��Ӧ mv <�ʼ��ļ���> <�ʼ��ļ���Ŀ¼>
Status moveNoteToFolder(char* noteName, Folder* folder, Folder* root);

// ���ʼ��ļ���������������ԭ�ļ�����ΪĿ���ļ��� ��Ӧ mv <�ʼ�ԭ�ļ���> <�ʼ�Ŀ���ļ���>
Status renameNote(Note* note, char* newName);

// ��ָ���ļ����ڵıʼ��ļ����ļ�������ĸ��������,��Ӧ sort <�ʼ��ļ�����>
Status sortNotesInFolder(Folder* folder);

#endif