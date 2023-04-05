// folder.h
#ifndef FOLDER_H
#define FOLDER_H

typedef enum Status
{
	ERROR = 0,
	SUCCESS = 1
} Status;

//定义文件夹结构体
typedef struct folder {
	char name[100]; //文件夹名
	char* path;
	struct folder* parent; //父文件夹
	struct folder* child; //第一个子文件夹
	struct folder* sibling; //下一个兄弟文件夹
	int visited; //1代表已搜索
	//Note* note; //文件夹中笔记列表
	//Tag* tag; //文件夹标签列表
} Folder;

//显示当前目录下所有的笔记本文件夹
Status showFolders(char* path);

//按照树状结构显示所有的笔记文件夹
Status showFolderTree(char* path, int depth);

////显示命令指定文件下所有的文件夹
//void showFoldersInDirectory(char* path);

//显示命令指定文件下所有的带有搜索内容的文件夹名
void showFoldersWithSearchTerm(char* path, char* searchTerm);

//将路径切换为当前目录的父目录. 若是根目录则则不进行切换
Folder* moveUpDirectory(Folder* currentFolder);

//将路径切换为命令中输入的文件夹路径
Folder* moveDirectory(Folder* currentFolder, char* path);

//将指定的笔记文件夹移动到目标文件夹目录下(未实现）
//Status moveFolderToDirectory(Folder* sourceFolder, Folder* targetFolder);

//删除指定文件夹以及其内部所有内容
//Status deleteFolderRecursive(Folder* folder);

// 创建文件夹
Folder* createFolder(char* name, char* path);

// 删除文件夹
Status deleteFolder(Folder* folder);

// 查找文件夹
Folder* findFolder(Folder* root, char* path);

// 添加文件夹
//Status addFolder(Folder* root, char* path);

// 移动文件夹
Status moveFolder(Folder* root, char* oldPath, char* newPath);

// 重命名文件夹
Status renameFolder(Folder* folder, char* name);

// 从文件夹中删除笔记
Status deleteNoteFromFolder(Folder* folder, char* noteName);

//// 添加笔记到文件夹
//Status addNoteT	oFolder(Folder* folder, Note* note);
//
////保存数据
//void saveFolderToFile(Folder* folder, FILE* fp);
//
////读取数据
//Folder* readFolderFromFile(FILE* fp);

#endif