#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "note.h"
//#include "folder.h"

// 创建笔记文件节点
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

	return newNote;
}

// 添加笔记到文件夹
Status addNoteToFolder(Folder* folder, Note* newNote) {
	//传入参数为空时
	if (!folder || !newNote)
		return ERROR;

	Note* note = folder->note;

	//遍历到文件夹的文件列表的尾节点
	while (note->next)
		note = note->next;
	note->next = newNote;
	
	return SUCCESS;
}

//在当前文件夹创建新笔记文件，对应 mkdir <笔记文件名>
Status addNewNoteToFolder(Folder* currentfolder, char* creater, char* noteTitle) {
	//先创建新节点
	Note* newNote = createNote(creater, noteTitle, currentfolder->path);
	//若新节点指针不为NULL，而且能成功将节点添加到文件夹时
	if (addNoteToFolder(currentfolder, newNote))
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

	//释放笔记节点
	free(note);

	return SUCCESS;
}

// 从当前文件夹中删除笔记,对应 rm <笔记文件名>
Status deleteNoteFromFolder(Folder* folder, char* noteTitle) {

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

// 将笔记文件移动到指定的文件夹目录下,对应 mv <笔记文件名> <笔记文件夹目录>
Status moveNoteToFolder(char* noteName, Folder* folder, Folder* root) {
	Note* note = searchNoteByName(root, noteName);
	if (note) {
		
		return SUCCESS;
	}
	else
		return ERROR;

}

// 将笔记文件进行重命名，从原文件名改为目标文件名 对应 mv <笔记原文件名> <笔记目标文件名>
Status renameNote(Note* note, char* newName) {
	if (!note||!newName) {
		return ERROR;
	}

	return SUCCESS;
}

// 对指定文件夹内的笔记文件按文件名首字母进行排序,对应 sort <笔记文件夹名>
Status sortNotesInFolder(Folder* folder) {

}