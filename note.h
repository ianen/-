// note.h
#ifndef NOTE_H
#define NOTE_H

#define MAX_NOTE_NAME_LENGTH    100
#define MAX_NOTE_PATH_LENGTH    100
#define MAX_NOTE_CREATER_LENGTH 100

#include "folder.h"

typedef struct note {
    char creater[MAX_NOTE_CREATER_LENGTH];//创建者名字
    char title[MAX_NOTE_NAME_LENGTH];//笔记主题
    char path[MAX_NOTE_PATH_LENGTH];//笔记实际路径
    Tag* tag; //文件夹标签列表
    struct note* next;
} Note;

// 创建笔记文件节点
Note* createNote(char* creater, char* noteTitle, char* currentPath);

// 添加笔记到文件夹
Status addNoteToFolder(Folder* folder, Note* newNote);

//在当前文件夹创建新笔记文件，对应mkdir <笔记文件名>
Status addNewNoteToFolder(Folder* currentfolder, char* creater, char* noteTitle);

// 对指定笔记文件进行删除
Status deleteNote(Note* note);

// 从当前文件夹中删除笔记,对应 rm <笔记文件名>
Status deleteNoteFromFolder(Folder* folder, char* noteTitle);

//按笔记名称搜索笔记文件
Note* searchNoteByName(Folder* root, char* noteName);

// 将笔记文件移动到指定的文件夹目录下,对应 mv <笔记文件名> <笔记文件夹目录>
Status moveNoteToFolder(char* noteName, Folder* folder, Folder* root);

// 将笔记文件进行重命名，从原文件名改为目标文件名 对应 mv <笔记原文件名> <笔记目标文件名>
Status renameNote(Note* note, char* newName);

// 对指定文件夹内的笔记文件按文件名首字母进行排序,对应 sort <笔记文件夹名>
Status sortNotesInFolder(Folder* folder);

#endif