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

//������нṹ��
typedef struct queue {
	Folder* data[sizeof(Folder)]; //�洢Ԫ�ص�����
	int front; //��ͷָ��
	int rear; //��βָ��
} Queue;

Folder* root;

Status showFolders(char* path, int depth) {
	// ��Ŀ¼
	HANDLE hFind;
	WIN32_FIND_DATAA findData;
	char pattern[MAX_PATH];
	sprintf_s(pattern, sizeof(pattern), "%s\\*", path);
	hFind = FindFirstFileA(pattern, &findData);

	// ����Ƿ�򿪳ɹ�
	if (hFind == INVALID_HANDLE_VALUE) {
		printf("Error opening directory %s\n", path);
		return ERROR;
	}

	// ����Ŀ¼�е������ļ�����Ŀ¼
	do {
		// ����.��..���������Ŀ¼
		if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0) {
			continue;
		}

		// ��ӡ�ļ���
		printf("%s\n", findData.cFileName);

		// �����һ����Ŀ¼���ݹ�����Լ�
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			char subpath[MAX_PATH];
			sprintf_s(subpath, sizeof(subpath), "%s\\%s", path, findData.cFileName);
			showFolders(subpath, depth + 1);
		}
	} while (FindNextFileA(hFind, &findData));

	// �ر�Ŀ¼
	FindClose(hFind);
	return SUCCESS;
}

// һ���������������ڴ�ӡ����
void print_indent(int depth) {
	for (int i = 0; i < depth; i++) {
		printf(" |-");
	}
}


/*
description: һ���ݹ麯�������ڱ���Ŀ¼����ӡ�ļ���
path: Ҫ������Ŀ¼
depth: ��ǰĿ¼����ȣ����ڴ�ӡ����
*/
Status showFolderTree(char* path, int depth) {
	// ��Ŀ¼
	HANDLE hFind;
	WIN32_FIND_DATAA findData;
	char pattern[MAX_PATH];
	sprintf_s(pattern,sizeof(pattern), "%s\\*", path);
	hFind = FindFirstFileA(pattern, &findData);

	// ����Ƿ�򿪳ɹ�
	if (hFind == INVALID_HANDLE_VALUE) {
		printf("Error opening directory %s\n", path);
		return ERROR;
	}

	// ����Ŀ¼�е������ļ�����Ŀ¼
	do {
		// ����.��..���������Ŀ¼
		if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0) {
			continue;
		}

		// ��ӡ�������ļ���
		print_indent(depth);
		printf("%s\n", findData.cFileName);

		// �����һ����Ŀ¼���ݹ�����Լ�
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			char subpath[MAX_PATH];
			sprintf_s(subpath,sizeof(subpath), "%s\\%s", path, findData.cFileName);
			showFolderTree(subpath, depth + 1);
		}
	} while (FindNextFileA(hFind, &findData));

	// �ر�Ŀ¼
	FindClose(hFind);
	return SUCCESS;
}

////����һ�����ڵ�Ľṹ
//typedef struct Folder {
//    char* name; //�ļ��е�����
//    struct Folder** children; //�ӽڵ���б�
//    int size; //�ӽڵ�ĸ���
//} Folder;

//����һ�����ڵ㣬�������ڴ�
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

////��һ�����ڵ����һ���ӽڵ㣬�������ڴ�
//void addChild(Folder* parent, Folder* child) {
//    parent->child = child;
//    parent->children[parent->size - 1] = child;
//}

//�Զ���ĺ��������ڶ�ȡָ��·���µ������ļ��У��������Ǵ���Ϊ��
Folder* readFolders(char* path) {
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	//����Ҫ�������ļ���ģʽ
	char pattern[MAX_PATH];
	sprintf_s(pattern, sizeof(pattern), "%s\\*", path);
	//����FindFirstFile����
	hFind = FindFirstFileA(pattern, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) {
		printf("FindFirstFile failed (%d)\n", GetLastError());
		return NULL;
	}
	//����һ�����ڵ㣬��ʾ��ǰ·��
	Folder* node = createTreeNode("node", path);
	//ѭ������FindNextFile����
	do {
		//�ж��ҵ����Ƿ���һ���ļ���
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			//����"."��".."������������ļ���
			if (strcmp(FindFileData.cFileName, ".") == 0 || strcmp(FindFileData.cFileName, "..") == 0) {
				continue;
			}
			//ƴ�����ļ��е�����·��
			char subpath[MAX_PATH];
			/*strcpy(subpath, path);
			strcat(subpath, "\\");
			strcat(subpath, FindFileData.cFileName);*/
			sprintf(subpath, "%s\\%ws", path, FindFileData.cFileName);
			//�ݹ�ص����Զ��庯������ȡ���ļ����µ������ļ��У�����Ϊ�ӽڵ���ӵ���ǰ�ڵ�
			//addChild(root, readFolders(subpath));
			node->child = readFolders(subpath);
			node->child->parent = node;
		}
		//�������һ���ļ��У����Ը���Ŀ
	} while (FindNextFile(hFind, &FindFileData));
	//����Ƿ񵽴�����������������
	if (GetLastError() != ERROR_NO_MORE_FILES) {
		printf("FindNextFile failed (%d)\n", GetLastError());
		return NULL;
	}
	//�ر��������
	FindClose(hFind);
	//���ظ��ڵ�
	return node;
}

//Folder* searchTree(char* path) {
//	Folder* current = root->child; //��ʾ��������һ��
//	Folder* cur_sib = current; // cur_sib������¼��ǰ�ڵ���ֵܽڵ��εı���
//	while (current != NULL) { //ѭ��ֱ���ҵ�ƥ����ļ��л��ߵ���ĩβ
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


//��ʼ������
void initQueue(Queue* q) {
	q->front = q->rear = 0;
}

//�ж϶����Ƿ�Ϊ��
int isEmpty(Queue* q) {
	return q->front == q->rear;
}

//��Ӳ���
void enqueue(Queue* q, Folder* f) {
	if ((q->rear + 1) % 100 == q->front) { //��������
		printf("Queue is full!\n");
		return;
	}
	q->data[q->rear] = f; //��Ԫ�ط����β
	q->rear = (q->rear + 1) % 100; //���¶�βָ��
}

//���Ӳ���
Folder* dequeue(Queue* q) {
	if (isEmpty(q)) { //����Ϊ��
		printf("Queue is empty!\n");
		return NULL;
	}
	Folder* f = q->data[q->front]; //ȡ����ͷԪ��
	q->front = (q->front + 1) % 100; //���¶�ͷָ��
	return f;
}

//���������������������Ϊ���ĸ��ڵ�
Folder* findFolder(char* path) {
	if (root == NULL) { //����ֱ�ӷ���
		return;
	}
	if (strcmp(root->path, path) == 0) { //������ļ��о���Ŀ���ļ��У���ӡ��·��������
		return root;
	}
	Queue q; //����һ������
	initQueue(&q); //��ʼ�����У��ڶ�������Ϊ�Զ���ȽϺ��������ﲻ��Ҫ
	//int visited[100] = { 0 }; //����һ�������¼�ѷ��ʹ��Ľڵ㣬��ʼΪȫ0
	enqueue(&q, root); //�����ڵ����
	root->visited = 1; //�����ڵ���Ϊ�ѷ��ʣ��������name�Ǵ�д��ĸA-Z֮һ�����Ը���ʵ������޸ı�Ƿ���
	//visited[root->name[0] - 'A'] = 1; 
	while (!isEmpty(&q)) { //�����в�Ϊ��ʱѭ��
		Folder* f = dequeue(&q); //�Ӷ�ͷȡ��һ���ڵ�
		printf("%s ", f->name); //��ӡ�ýڵ��name������������
		Folder* c = f->child; //��ȡ�ýڵ�ĵ�һ���ӽڵ�
		while (c != NULL) { //�����ýڵ�������ӽڵ�
			if (c->visited == 0) { //����ӽڵ�δ�����ʹ�
				enqueue(&q, c); //���ӽڵ����
				c->visited = 1; //���ӽڵ���Ϊ�ѷ���
			}
			if (strcmp(c->path, path) == 0) {
				return c;
			}
			c = c->sibling; //��ȡ��һ���ֵܽڵ�
		}
	}
	//free(&q); //���ٶ���
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
				printf("��·���Ǹ�Ŀ¼��\n");
				sprintf(&p, "%s\\%s", path, name); //ƴ��·��
				if (_mkdir(&p) == -1) {
					return NULL;
				}

				Folder* new_folder = (Folder*)malloc(sizeof(Folder));
				strcpy(new_folder->name, name); //�����ļ�����
				new_folder->path = (char*)malloc(strlen(path) + strlen(name) + 2); //����·���ռ�
				//sprintf(new_folder->path, "%s\\%s", path, name); //ƴ��·��
				strcpy(new_folder->path, &p);
				new_folder->parent = root; //���ø��ڵ�Ϊ���ڵ�
				new_folder->child = NULL; //�����ӽڵ�Ϊ��
				new_folder->sibling = root->child; //�����ֵܽڵ�Ϊԭ���ĵ�һ���ӽڵ�
				root->child = new_folder; //�������ļ���Ϊ��һ���ӽڵ�
				printf("Create folder Success\n");
				return new_folder;
			}
			else {
				Folder* result = findFolder(root, path);
				Folder* f = malloc(sizeof(Folder));
				f->parent = result;
				f->sibling = result->child;
				result->child = f;
				////char* first = strtok(path, "\\"); //�÷�б�ָܷ�·��
				//Folder* current = root->child; //�Ӹ��ڵ�ĵ�һ���ӽڵ㿪ʼ����
				//Folder* cur_sib = current; // cur_sib������¼��ǰ�ڵ���ֵܽڵ��εı���
				//while (current != NULL) { //ѭ��ֱ���ҵ�ƥ����ļ��л��ߵ���ĩβ
				//	if (strcmp(current->path, path) != 0) { //����ҵ�ƥ���·��
				//		
				//		while (cur_sib->path != p) {
				//			cur_sib = cur_sib->sibling;
				//			if (cur_sib == NULL) {
				//				break;
				//			}
				//		}
				//		current = current->child;

				//		createFolder(current, rest, name); //�ݹ���ú�������ƥ����ļ����´������ļ���
				return NULL;

			}
		}
		else {
			printf("·�������ڡ�\n");
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
		Folder* r = findFolder(root, path); //��ȡ·����Ӧ�Ľ��
		if (r == NULL) {
			printf("·�������ڡ�\n");
			return;
		}
		else {
			Queue q; //����һ������
			initQueue(&q); //��ʼ�����У��ڶ�������Ϊ�Զ���ȽϺ��������ﲻ��Ҫ
			//int visited[100] = { 0 }; //����һ�������¼�ѷ��ʹ��Ľڵ㣬��ʼΪȫ0
			enqueue(&q, root); //�����ڵ����
			root->visited = 1; //�����ڵ���Ϊ�ѷ��ʣ��������name�Ǵ�д��ĸA-Z֮һ�����Ը���ʵ������޸ı�Ƿ���
			//visited[root->name[0] - 'A'] = 1; 
			while (!isEmpty(&q)) { //�����в�Ϊ��ʱѭ��
				Folder* f = dequeue(&q); //�Ӷ�ͷȡ��һ���ڵ�
				printf("%s ", f->name); //��ӡ�ýڵ��name������������
				Folder* c = f->child; //��ȡ�ýڵ�ĵ�һ���ӽڵ�
				while (c != NULL) { //�����ýڵ�������ӽڵ�
					if (c->visited == 0) { //����ӽڵ�δ�����ʹ�
						enqueue(&q, c); //���ӽڵ����
						c->visited = 1; //���ӽڵ���Ϊ�ѷ���
					}
					result = strstr(c->path, searchTerm);//�ж��ļ�·���Ƿ���������
					if (result != NULL) {
						printf(c->path);
						printf("\n");
					}
					c = c->sibling; //��ȡ��һ���ֵܽڵ�
				}
			}
			free(&q); //���ٶ���

			
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


	//��ɾ���ļ����µ������ļ�
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
	//char temp[100]; //�洢�ļ�·��
	//struct _finddata_t fileinfo; //�洢�ļ���Ϣ
	//long handle; //���ڲ����ļ��ľ��
	////strcpy(temp->path, folder->path); //�����ļ���·��
	//strcat(temp->path, "\\test"); //���ͨ���
	//handle = _findfirst(folder->path, &fileinfo); //���ҵ�һ��ƥ����ļ�
	//if (handle == -1L) { //���û���ҵ�������
	//	return ERROR;
	//}
	//do {
	//	if (strcmp(fileinfo.name, ".") == 0 || strcmp(fileinfo.name, "..") == 0) {
	//	/*	if (strcmp(temp->path, folder->path) == 0 ) {
	//			return SUCCESS;
	//		}*/
	//		continue; //������ǰĿ¼���ϼ�Ŀ¼
	//	}
	//	strcpy(temp->path, folder->path); //�����ļ���·��
	//	strcat(temp->path, "\\"); //��ӷָ���
	//	strcat(temp->path, fileinfo.name); //����ļ���
	//	if (fileinfo.attrib & _A_SUBDIR) { //�������Ŀ¼
	//		deleteFolder(temp); //�ݹ�ɾ����Ŀ¼�µ������ļ�
	//		_rmdir(temp->path); //ɾ������Ŀ¼
	//	}
	//	else { //�������ͨ�ļ�
	//		remove(temp->path); //ɾ���ļ�
	//	}
	//} while (_findnext(handle, &fileinfo) == 0); //������һ��ƥ����ļ�
	//_findclose(handle); //�رվ��

	//free(findFolder(folder->path));
	//return SUCCESS;
}

Status renameFolder(Folder* folder, char* name) {

	char temp[MAX_PATH];
	strcpy(temp, folder->path);
	int len = strlen(folder->path); //��ȡ���鳤��
	for (int i = len - 1; i >= 0; i--) { //�Ӻ���ǰ����
		if (folder->path[i] == '\\') { //�������\\�ַ�
			break; //�˳�ѭ��
		}
		folder->path[i] = '\0'; //���򽫸��ַ���ֵΪ'\0'
	}
	int result = rename(temp, strcat(folder->path, name)); //����rename����
	if (result == 0) { //�ж��Ƿ�ɹ�
		printf("�������ɹ�\n");
		return SUCCESS;
	
		//strcat(folder->path, name);
		//printf("%s\n", folder->path); //��ӡ����

	}
	else {
		printf("������ʧ��\n");
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
