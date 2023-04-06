#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "note.h"
#include "folder.h"

// 创建笔记文件节点和实际笔记文件
Note* createNote(char *creater,char* noteTitle,char* currentPath) {
	Note* newNote = (Note*)malloc(sizeof(Note));//新笔记文件节点
	char newPath[MAX_NOTE_PATH_LENGTH];//新笔记路径
	
	//分配内存失败
	if (!newNote) {
		printf("内存分配失败，无法创建笔记文件。\n");
		return NULL;
	}

	// 设置笔记的创建者
	strncpy(newNote->creater, creater, MAX_NOTE_CREATER_LENGTH);
	newNote->title[MAX_NOTE_NAME_LENGTH - 1] = '\0'; // 确保字符串以 null 终止

	// 设置笔记的名称
	strncpy(newNote->title, noteTitle, MAX_NOTE_NAME_LENGTH);
	newNote->title[MAX_NOTE_NAME_LENGTH - 1] = '\0'; // 确保字符串以 null 终止

	// 设置笔记的路径
	strncpy(newPath, currentPath, MAX_NOTE_PATH_LENGTH);
	strncat(newPath, "\\", MAX_NOTE_PATH_LENGTH);
	strncat(newPath, noteTitle, MAX_NOTE_PATH_LENGTH);
	strncpy(newNote->path, newPath, MAX_NOTE_PATH_LENGTH);
	newNote->path[MAX_NOTE_PATH_LENGTH - 1] = '\0'; // 确保字符串以 null 终止

	// 初始化其他属性
	newNote->tag = NULL;
	newNote->next = NULL;

	//创建真实笔记文件
	FILE* file = fopen(newPath, "w");; // 文件指针
	if (file == NULL) 
		return NULL;

	return newNote;
}

// 添加笔记节点到文件夹
Status addNoteToFolder(Folder* folder, Note* newNote) {
	//传入参数为空时
	if (!folder || !newNote)
		return ERROR;

	//遍历到文件夹的文件列表的尾节点
	Note* note = folder->note;
	while (note->next)
		note = note->next;
	note->next = newNote;
	
	return SUCCESS;
}

//在当前文件夹创建新笔记文件，对应 mkdir <笔记文件名>
Status addNewNoteToFolder(Folder* currentFolder, char* creater, char* noteTitle) {
	//先创建新节点
	Note* newNote = createNote(creater, noteTitle, currentFolder->path);
	//若新节点指针不为NULL，而且能成功将节点添加到文件夹时
	if (addNoteToFolder(currentFolder, newNote))
		return SUCCESS;
	else
		return ERROR;
}

// 对指定笔记文件进行删除
Status deleteNote(Note* note) {
	//输入空节点时
	if (!note)
		return ERROR;

	//删除笔记文件的tag列表
	Tag* preTag = note->tag;
	Tag* tag = preTag;
	while (tag) {
		preTag = tag->next;
		free(tag);
		tag = preTag;
	}

	//删除实际笔记文件
	remove(note->path);

	//释放笔记节点
	free(note);

	return SUCCESS;
}

// 从当前文件夹中删除笔记,对应 rm <笔记文件名>
Status deleteNoteFromFolder(Folder* currentFolder, char* noteTitle) {
	//若输入为NULL
	if (!currentFolder || !noteTitle)
		return ERROR;

	// 遍历链表，查找指定笔记
	Note* note = currentFolder->note, * preNote = NULL;
	while(note != NULL) {
		if (!strcmp(note->title, noteTitle)) {
			if (preNote == NULL) {
				// 删除链表头节点
				currentFolder->note = note->next;
			}
			else {
				// 删除中间节点或尾节点
				preNote->next = note->next;
			}
			deleteNote(note);
			return SUCCESS;
		}
		preNote = note, note = note->next;
	}

	return ERROR;
}

//按笔记名称搜索笔记文件
Note* searchNoteByName(Folder* root, char* noteName) {
	Folder* folder = root;
	Note* note;
	while (folder->child) {
		while (folder->sibling) {
			note = folder->note;
			while (note->next) {
				if (!strcmp(note->title, noteName))
					return note;
				note = note->next;
			}
			folder = folder->sibling;
		}
		folder = folder->child;
	}

	// 如果没有找到匹配的笔记，返回空指针
	return NULL;
}

//按笔记文件搜索存储文件夹
Folder* searchFolderByNote(Folder* root, Note* note) {
	Folder* folder = root;
	while (folder->child) {
		while (folder->sibling) {
				if (folder->note == note)
					return folder;
			folder = folder->sibling;
		}
		folder = folder->child;
	}

	// 如果没有找到匹配的文件夹，返回空指针
	return NULL;
}

// 将笔记文件移动到指定的文件夹目录下,对应 mv <笔记文件名> <笔记文件夹目录>
Status moveNoteToFolder(char* noteName, Folder* folder, Folder* root) {
	Note* note = searchNoteByName(root, noteName);
	if (note) {
		// 将笔记文件节点移动到指定的文件夹目录下
		if (!addNewNoteToFolder(folder, note->creater, note->title)) {
			return ERROR;
		}
		Folder* newFolder = searchFolderByNote(root, note);

		// 移动真实文件
		char newPath[MAX_NOTE_PATH_LENGTH];
		snprintf(newPath, MAX_NOTE_PATH_LENGTH, "%s/%s", newFolder->path, note->title);
		if (rename(note->path, newPath) != 0) {
			return ERROR;
		}

		// 从原文件夹中删除笔记文件节点
		if (!deleteNoteFromFolder(searchFolderByNote(root, note), note->title)) {
			return ERROR;
		}
		return SUCCESS;
	}
	else
		return ERROR;
}

// 将笔记文件进行重命名，从原文件名改为目标文件名 对应 mv <笔记原文件名> <笔记目标文件名>
Status renameNote(Note* note, char* newName) {
	//输入为NULL时
	if (!note||!newName) {
		return ERROR;
	}

	//给笔记文件节点重命名
	strncpy(note->title, newName, MAX_NOTE_NAME_LENGTH);
	note->title[MAX_NOTE_NAME_LENGTH - 1] = '\0'; // 确保字符串以 null 终止

	//给实际笔记文件重命名
	char* newPath;
	strncpy(newPath, note->path, MAX_NOTE_PATH_LENGTH);
	int i = strlen(newPath);
	while (newPath[i] != '\\') {
		newPath[i--] = '\0';
	}
	strncat(newPath, newName, MAX_NOTE_PATH_LENGTH);
	rename(note->path,newPath);

	return SUCCESS;
}

// 对指定文件夹内的笔记文件按文件名首字母进行排序,对应 sort <笔记文件夹名>
Status sortNotesInFolder(Folder* folder) {
	//输入为NULL或者文件夹没有笔记时
	if (!folder || !folder->note)
		return ERROR;

	Note* pHead = NULL;  // 定义虚拟头节点指针，并初始化为 NULL
	Note* cur = folder->note;
	while (cur) {
		Note* next = cur->next;  // 记录下一个节点的指针
		cur->next = NULL;  // 断开当前节点与原链表的连接
		Note** pp = &pHead;
		while (*pp && (*pp)->title[0] < cur->title[0]) {
			pp = &((*pp)->next);
		}
		cur->next = *pp;
		*pp = cur;  // 插入当前节点到新链表中
		cur = next;  // 处理下一个节点
	}
	folder->note = pHead ? pHead->next : NULL;  // 返回新链表的头节点
}