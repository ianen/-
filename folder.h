// folder.h
#ifndef FOLDER_H
#define FOLDER_H

#include "note.h"
#include "tag.h"

typedef enum Status
{
	ERROR = 0,
	SUCCESS = 1
} Status;

//�����ļ��нṹ��
typedef struct folder {
	char name[100]; //�ļ�����
	char path[1000];//�ʼ�ʵ��·��    
	struct folder* parent; //���ļ���
	struct folder* child; //��һ�����ļ���
	struct folder* sibling; //��һ���ֵ��ļ���
	Note* note; //�ļ����бʼ��б�
	Tag* tag; //�ļ��б�ǩ�б�
} Folder;

//��ʾ��ǰĿ¼�����еıʼǱ��ļ���
void showFolders(Folder* root);

//������״�ṹ��ʾ���еıʼ��ļ���
void showFolderTree(Folder* root, int depth);

//��ʾ����ָ���ļ������е��ļ���
void showFoldersInDirectory(char* path);

//��ʾ����ָ���ļ������еĴ����������ݵ��ļ�����
void showFoldersWithSearchTerm(char* path, char* searchTerm);

//��·���л�Ϊ��ǰĿ¼�ĸ�Ŀ¼. ���Ǹ�Ŀ¼���򲻽����л�
Folder* moveUpDirectory(Folder* currentFolder);

//��·���л�Ϊ������������ļ���·��
Folder* moveDirectory(Folder* root, char* path);

//��ָ���ıʼ��ļ����ƶ���Ŀ���ļ���Ŀ¼��
Status moveFolderToDirectory(Folder* sourceFolder, Folder* targetFolder);

//ɾ��ָ���ļ����Լ����ڲ���������
Status deleteFolderRecursive(Folder* folder);

// �����ļ���
Folder* createFolder(char* name);

// ɾ���ļ���
Status deleteFolder(Folder* folder);

// �����ļ���
Folder* findFolder(Folder* root, char* path);

// ����ļ���
Status addFolder(Folder* root, char* path);

// �ƶ��ļ���
Status moveFolder(Folder* root, char* oldPath, char* newPath);

// �������ļ���
Status renameFolder(Folder* folder, char* name);

//��������
void saveFolderToFile(Folder* folder, FILE* fp);

//��ȡ����
Folder* readFolderFromFile(FILE* fp);

#endif