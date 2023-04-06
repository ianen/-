#define _CRT_SECURE_NO_WARNINGS
#include "../Header/Folder.h"
#include <sys/types.h> 
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <direct.h>
#include <io.h>
#include <string.h>

//定义队列结构体
typedef struct queue {
	Folder* data[sizeof(Folder)]; //存储元素的数组
	int front; //队头指针
	int rear; //队尾指针
} Queue;

Folder* root;

Status showFolders(char* path, int depth) {
	// 打开目录
	HANDLE hFind;
	WIN32_FIND_DATAA findData;
	char pattern[MAX_PATH];
	sprintf_s(pattern, sizeof(pattern), "%s\\*", path);
	hFind = FindFirstFileA(pattern, &findData);

	// 检查是否打开成功
	if (hFind == INVALID_HANDLE_VALUE) {
		printf("Error opening directory %s\n", path);
		return ERROR;
	}

	// 遍历目录中的所有文件和子目录
	do {
		// 跳过.和..两个特殊的目录
		if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0) {
			continue;
		}

		// 打印文件名
		printf("%s\n", findData.cFileName);

		// 如果是一个子目录，递归调用自己
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			char subpath[MAX_PATH];
			sprintf_s(subpath, sizeof(subpath), "%s\\%s", path, findData.cFileName);
			showFolders(subpath, depth + 1);
		}
	} while (FindNextFileA(hFind, &findData));

	// 关闭目录
	FindClose(hFind);
	return SUCCESS;
}

// 一个辅助函数，用于打印缩进
void print_indent(int depth) {
	for (int i = 0; i < depth; i++) {
		printf(" |-");
	}
}


/*
description: 一个递归函数，用于遍历目录并打印文件树
path: 要遍历的目录
depth: 当前目录的深度，用于打印缩进
*/
Status showFolderTree(char* path, int depth) {
	// 打开目录
	HANDLE hFind;
	WIN32_FIND_DATAA findData;
	char pattern[MAX_PATH];
	sprintf_s(pattern,sizeof(pattern), "%s\\*", path);
	hFind = FindFirstFileA(pattern, &findData);

	// 检查是否打开成功
	if (hFind == INVALID_HANDLE_VALUE) {
		printf("Error opening directory %s\n", path);
		return ERROR;
	}

	// 遍历目录中的所有文件和子目录
	do {
		// 跳过.和..两个特殊的目录
		if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0) {
			continue;
		}

		// 打印缩进和文件名
		print_indent(depth);
		printf("%s\n", findData.cFileName);

		// 如果是一个子目录，递归调用自己
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			char subpath[MAX_PATH];
			sprintf_s(subpath,sizeof(subpath), "%s\\%s", path, findData.cFileName);
			showFolderTree(subpath, depth + 1);
		}
	} while (FindNextFileA(hFind, &findData));

	// 关闭目录
	FindClose(hFind);
	return SUCCESS;
}

////定义一个树节点的结构
//typedef struct Folder {
//    char* name; //文件夹的名称
//    struct Folder** children; //子节点的列表
//    int size; //子节点的个数
//} Folder;

//创建一个树节点，并分配内存
Folder* createTreeNode(char* name, char* path) {
	Folder* node = (Folder*)malloc(sizeof(Folder));
	node->path = (char*)malloc(sizeof(char) * strlen(path));
	strcpy(node->name, name);
	node->child = NULL;
	node->parent = NULL;
	node->sibling = NULL;
	node->visited = 0;
	strcpy(node->path, path);
	return node;
}

////向一个树节点添加一个子节点，并分配内存
//void addChild(Folder* parent, Folder* child) {
//    parent->child = child;
//    parent->children[parent->size - 1] = child;
//}

//自定义的函数，用于读取指定路径下的所有文件夹，并把它们储存为树
Folder* readFolders(char* path) {
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	//设置要搜索的文件名模式
	char pattern[MAX_PATH];
	sprintf_s(pattern, sizeof(pattern), "%s\\*", path);
	//调用FindFirstFile函数
	hFind = FindFirstFileA(pattern, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) {
		printf("FindFirstFile failed (%d)\n", GetLastError());
		return NULL;
	}
	//创建一个树节点，表示当前路径
	Folder* node = createTreeNode("node", path);
	//循环调用FindNextFile函数
	do {
		//判断找到的是否是一个文件夹
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			//忽略"."和".."这两个特殊的文件夹
			if (strcmp(FindFileData.cFileName, ".") == 0 || strcmp(FindFileData.cFileName, "..") == 0) {
				continue;
			}
			//拼接子文件夹的完整路径
			char subpath[MAX_PATH];
			/*strcpy(subpath, path);
			strcat(subpath, "\\");
			strcat(subpath, FindFileData.cFileName);*/
			sprintf(subpath, "%s\\%ws", path, FindFileData.cFileName);
			//递归地调用自定义函数，获取子文件夹下的所有文件夹，并作为子节点添加到当前节点
			//addChild(root, readFolders(subpath));
			node->child = readFolders(subpath);
			node->child->parent = node;
		}
		//如果不是一个文件夹，忽略该项目
	} while (FindNextFile(hFind, &FindFileData));
	//检查是否到达搜索结束或发生错误
	if (GetLastError() != ERROR_NO_MORE_FILES) {
		printf("FindNextFile failed (%d)\n", GetLastError());
		return NULL;
	}
	//关闭搜索句柄
	FindClose(hFind);
	//返回根节点
	return node;
}

//Folder* searchTree(char* path) {
//	Folder* current = root->child; //表示在树的哪一层
//	Folder* cur_sib = current; // cur_sib用来记录当前节点的兄弟节点层次的遍历
//	while (current != NULL) { //循环直到找到匹配的文件夹或者到达末尾
//
//		while (cur_sib->path != path) {
//			cur_sib = cur_sib->sibling;
//			if (cur_sib == NULL) {
//				current = current->child;
//				cur_sib = current;
//				continue;
//			}
//			if (strcmp(cur_sib->path, path) == 0) {
//				return cur_sib;
//			}
//		}
//	}
//}


//初始化队列
void initQueue(Queue* q) {
	q->front = q->rear = 0;
}

//判断队列是否为空
int isEmpty(Queue* q) {
	return q->front == q->rear;
}

//入队操作
void enqueue(Queue* q, Folder* f) {
	if ((q->rear + 1) % 100 == q->front) { //队列已满
		printf("Queue is full!\n");
		return;
	}
	q->data[q->rear] = f; //将元素放入队尾
	q->rear = (q->rear + 1) % 100; //更新队尾指针
}

//出队操作
Folder* dequeue(Queue* q) {
	if (isEmpty(q)) { //队列为空
		printf("Queue is empty!\n");
		return NULL;
	}
	Folder* f = q->data[q->front]; //取出队头元素
	q->front = (q->front + 1) % 100; //更新队头指针
	return f;
}

//广度优先搜索函数，参数为树的根节点
Folder* findFolder(char* path) {
	if (root == NULL) { //空树直接返回
		return;
	}
	if (strcmp(root->path, path) == 0) { //如果根文件夹就是目标文件夹，打印其路径并返回
		return root;
	}
	Queue q; //创建一个队列
	initQueue(&q); //初始化队列，第二个参数为自定义比较函数，这里不需要
	//int visited[100] = { 0 }; //创建一个数组记录已访问过的节点，初始为全0
	enqueue(&q, root); //将根节点入队
	root->visited = 1; //将根节点标记为已访问，这里假设name是大写字母A-Z之一，可以根据实际情况修改标记方法
	//visited[root->name[0] - 'A'] = 1; 
	while (!isEmpty(&q)) { //当队列不为空时循环
		Folder* f = dequeue(&q); //从队头取出一个节点
		printf("%s ", f->name); //打印该节点的name或者其他操作
		Folder* c = f->child; //获取该节点的第一个子节点
		while (c != NULL) { //遍历该节点的所有子节点
			if (c->visited == 0) { //如果子节点未被访问过
				enqueue(&q, c); //将子节点入队
				c->visited = 1; //将子节点标记为已访问
			}
			if (strcmp(c->path, path) == 0) {
				return c;
			}
			c = c->sibling; //获取下一个兄弟节点
		}
	}
	//free(&q); //销毁队列
	return NULL;
}


Folder* createFolder (char* name, char* path) {
	if (root == NULL || path == NULL || name == NULL) {
		return; 
	}
	else {
		char p[MAX_PATH];
		int len = strlen(path);
		char* a = path + len - 2;
		if (_access(path, 0) == 0) {
			if (strcmp(a, "md") == 0) {
				printf("该路径是根目录。\n");
				sprintf(&p, "%s\\%s", path, name); //拼接路径
				if (_mkdir(&p) == -1) {
					return NULL;
				}

				Folder* new_folder = (Folder*)malloc(sizeof(Folder));
				strcpy(new_folder->name, name); //复制文件夹名
				new_folder->path = (char*)malloc(strlen(path) + strlen(name) + 2); //分配路径空间
				//sprintf(new_folder->path, "%s\\%s", path, name); //拼接路径
				strcpy(new_folder->path, &p);
				new_folder->parent = root; //设置父节点为根节点
				new_folder->child = NULL; //设置子节点为空
				new_folder->sibling = root->child; //设置兄弟节点为原来的第一个子节点
				root->child = new_folder; //设置新文件夹为第一个子节点
				printf("Create folder Success\n");
				return new_folder;
			}
			else {
				Folder* result = findFolder(root, path);
				Folder* f = malloc(sizeof(Folder));
				f->parent = result;
				f->sibling = result->child;
				result->child = f;
				////char* first = strtok(path, "\\"); //用反斜杠分割路径
				//Folder* current = root->child; //从根节点的第一个子节点开始遍历
				//Folder* cur_sib = current; // cur_sib用来记录当前节点的兄弟节点层次的遍历
				//while (current != NULL) { //循环直到找到匹配的文件夹或者到达末尾
				//	if (strcmp(current->path, path) != 0) { //如果找到匹配的路径
				//		
				//		while (cur_sib->path != p) {
				//			cur_sib = cur_sib->sibling;
				//			if (cur_sib == NULL) {
				//				break;
				//			}
				//		}
				//		current = current->child;

				//		createFolder(current, rest, name); //递归调用函数，在匹配的文件夹下创建新文件夹
				return NULL;

			}
		}
		else {
			printf("路径不存在。\n");
			return NULL;
		}
		
	}
}

void showFoldersWithSearchTerm(char* path, char* searchTerm) {
	char* result; 
	if (root == NULL || path == NULL || searchTerm == NULL) {
		return;
	}
	else {
		Folder* r = findFolder(root, path); //获取路径对应的结点
		if (r == NULL) {
			printf("路径不存在。\n");
			return;
		}
		else {
			Queue q; //创建一个队列
			initQueue(&q); //初始化队列，第二个参数为自定义比较函数，这里不需要
			//int visited[100] = { 0 }; //创建一个数组记录已访问过的节点，初始为全0
			enqueue(&q, root); //将根节点入队
			root->visited = 1; //将根节点标记为已访问，这里假设name是大写字母A-Z之一，可以根据实际情况修改标记方法
			//visited[root->name[0] - 'A'] = 1; 
			while (!isEmpty(&q)) { //当队列不为空时循环
				Folder* f = dequeue(&q); //从队头取出一个节点
				printf("%s ", f->name); //打印该节点的name或者其他操作
				Folder* c = f->child; //获取该节点的第一个子节点
				while (c != NULL) { //遍历该节点的所有子节点
					if (c->visited == 0) { //如果子节点未被访问过
						enqueue(&q, c); //将子节点入队
						c->visited = 1; //将子节点标记为已访问
					}
					result = strstr(c->path, searchTerm);//判断文件路径是否有搜索词
					if (result != NULL) {
						printf(c->path);
						printf("\n");
					}
					c = c->sibling; //获取下一个兄弟节点
				}
			}
			free(&q); //销毁队列

			
		}
	}
}

Folder* moveUpDirectory(Folder* currentFolder) {
	currentFolder = currentFolder->parent;
	if (currentFolder == root) {
		printf("It's root\n");
		return currentFolder;
	}
	return currentFolder;
}

Folder* moveDirectory(Folder* currentFolder, char* path) {
	Folder* result = findFolder(root, path);
	if (result != NULL) {
		currentFolder = result;
		return currentFolder;
	}
	else {
		return ERROR;
	}
}

Status IsEmptyFolder(Folder* folder) {
	if (folder->child == NULL) {
		return SUCCESS;
	}
	else {
		return ERROR;
	}
}

Status deleteFolder(Folder* folder) {
	printf("It'll delete all file in this folder!\n");


	//先删除文件夹下的所有文件
	//Folder* temp = malloc(sizeof(Folder));
	//Folder* temp = createTreeNode("temp", root->path);
	//if (temp == ERROR) {
	//	return ERROR;
	//}

	if (IsEmptyFolder(folder) == SUCCESS) {
		_rmdir(folder->path);
		return SUCCESS;
	}
	//else {
	//	while (1) {
	//		if (note->path == NULL) {
	//			break;
	//		}
	//		rmove(note->path);
	//	}
	//	_rmdir(folder->path);
	//}
	//char temp[100]; //存储文件路径
	//struct _finddata_t fileinfo; //存储文件信息
	//long handle; //用于查找文件的句柄
	////strcpy(temp->path, folder->path); //复制文件夹路径
	//strcat(temp->path, "\\test"); //添加通配符
	//handle = _findfirst(folder->path, &fileinfo); //查找第一个匹配的文件
	//if (handle == -1L) { //如果没有找到，返回
	//	return ERROR;
	//}
	//do {
	//	if (strcmp(fileinfo.name, ".") == 0 || strcmp(fileinfo.name, "..") == 0) {
	//	/*	if (strcmp(temp->path, folder->path) == 0 ) {
	//			return SUCCESS;
	//		}*/
	//		continue; //跳过当前目录和上级目录
	//	}
	//	strcpy(temp->path, folder->path); //复制文件夹路径
	//	strcat(temp->path, "\\"); //添加分隔符
	//	strcat(temp->path, fileinfo.name); //添加文件名
	//	if (fileinfo.attrib & _A_SUBDIR) { //如果是子目录
	//		deleteFolder(temp); //递归删除子目录下的所有文件
	//		_rmdir(temp->path); //删除空子目录
	//	}
	//	else { //如果是普通文件
	//		remove(temp->path); //删除文件
	//	}
	//} while (_findnext(handle, &fileinfo) == 0); //查找下一个匹配的文件
	//_findclose(handle); //关闭句柄

	//free(findFolder(folder->path));
	//return SUCCESS;
}

Status renameFolder(Folder* folder, char* name) {

	char temp[MAX_PATH];
	strcpy(temp, folder->path);
	int len = strlen(folder->path); //获取数组长度
	for (int i = len - 1; i >= 0; i--) { //从后往前遍历
		if (folder->path[i] == '\\') { //如果遇到\\字符
			break; //退出循环
		}
		folder->path[i] = '\0'; //否则将该字符赋值为'\0'
	}
	int result = rename(temp, strcat(folder->path, name)); //调用rename函数
	if (result == 0) { //判断是否成功
		printf("重命名成功\n");
		return SUCCESS;
	
		//strcat(folder->path, name);
		//printf("%s\n", folder->path); //打印数组

	}
	else {
		printf("重命名失败\n");
		return ERROR;
	}
}

int main() {
	char curdir[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, curdir);
	printf("Current directory: %s\n", curdir);
	showFolderTree(curdir, 0);
	printf("\n");
	showFolders(curdir, 0);
	root = readFolders(strcat(curdir, "\\md"));


	renameFolder(createFolder("test", "J:\\Programing\\C\\QG_Training\\Note_sys\\Note\\md"), "a");
	//deleteFolder(findFolder("J:\\Programing\\C\\QG_Training\\Note_sys\\Note\\md\\test"));
	return 0;
}
