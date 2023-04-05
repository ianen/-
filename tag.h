// tag.h
#ifndef TAG_H
#define TAG_H

#include "folder.h"

typedef struct Tag {
    char name[50];//��ǩ��
    struct Tag* next;
} Tag;

// ��ӱʼǱ�ǩ(ע���ǩ��������⣬�ɹ�����SUCCESS��ʧ�ܷ���ERROR)
Status addTagToNote(Note* note, char* tagName);

// ����ļ��б�ǩ(ע���ǩ��������⣬�ɹ�����SUCCESS��ʧ�ܷ���ERROR)
Status addTagToFolder(Folder* folder, char* tagName);

// ɾ���ʼǱ�ǩ(�ɹ�����SUCCESS��ʧ�ܷ���ERROR,����ͬ��)
Status deleteTagFromNote(Note* note, char* tagName);

// ɾ���ļ��б�ǩ
Status deleteTagFromFolder(Folder* folder, char* tagName);

// ��ʾָ���ʼǵı�ǩ
Status viewTagsInNote(Note* note);

// ��ʾָ���ļ��еı�ǩ
Status viewTagsInFolder(Folder* folder);

// ���ݱ�ǩ���ݣ��ڵ�ǰĿ¼�½��бʼ����������ƥ��ı�ǩ��Ӧ�ıʼ��ļ���
Status searchNotesInFolderByTag(Folder* folder, char* tagName);

// ���ݱ�ǩ���ݣ������бʼǽ������������ƥ��ı�ǩ����Ӧ�ıʼǵ��ļ����Լ�����·��
Status searchNotesByTag(char* tagName);

#endif