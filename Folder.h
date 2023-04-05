// folder.h
#ifndef FOLDER_H
#define FOLDER_H

typedef enum Status
{
	ERROR = 0,
	SUCCESS = 1
} Status;

//�����ļ��нṹ��
typedef struct folder {
	char name[100]; //�ļ�����
	char* path;
	struct folder* parent; //���ļ���
	struct folder* child; //��һ�����ļ���
	struct folder* sibling; //��һ���ֵ��ļ���
	int visited; //1����������
	//Note* note; //�ļ����бʼ��б�
	//Tag* tag; //�ļ��б�ǩ�б�
} Folder;

//��ʾ��ǰĿ¼�����еıʼǱ��ļ���
Status showFolders(char* path);

//������״�ṹ��ʾ���еıʼ��ļ���
Status showFolderTree(char* path, int depth);

////��ʾ����ָ���ļ������е��ļ���
//void showFoldersInDirectory(char* path);

//��ʾ����ָ���ļ������еĴ����������ݵ��ļ�����
void showFoldersWithSearchTerm(char* path, char* searchTerm);

//��·���л�Ϊ��ǰĿ¼�ĸ�Ŀ¼. ���Ǹ�Ŀ¼���򲻽����л�
Folder* moveUpDirectory(Folder* currentFolder);

//��·���л�Ϊ������������ļ���·��
Folder* moveDirectory(Folder* currentFolder, char* path);

//��ָ���ıʼ��ļ����ƶ���Ŀ���ļ���Ŀ¼��(δʵ�֣�
//Status moveFolderToDirectory(Folder* sourceFolder, Folder* targetFolder);

//ɾ��ָ���ļ����Լ����ڲ���������
//Status deleteFolderRecursive(Folder* folder);

// �����ļ���
Folder* createFolder(char* name, char* path);

// ɾ���ļ���
Status deleteFolder(Folder* folder);

// �����ļ���
Folder* findFolder(Folder* root, char* path);

// ����ļ���
//Status addFolder(Folder* root, char* path);

// �ƶ��ļ���
Status moveFolder(Folder* root, char* oldPath, char* newPath);

// �������ļ���
Status renameFolder(Folder* folder, char* name);

// ���ļ�����ɾ���ʼ�
Status deleteNoteFromFolder(Folder* folder, char* noteName);

//// ��ӱʼǵ��ļ���
//Status addNoteT	oFolder(Folder* folder, Note* note);
//
////��������
//void saveFolderToFile(Folder* folder, FILE* fp);
//
////��ȡ����
//Folder* readFolderFromFile(FILE* fp);

#endif