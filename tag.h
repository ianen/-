// tag.h
#ifndef TAG_H
#define TAG_H

#include "folder.h"

typedef struct Tag {
    char name[50];//标签名
    struct Tag* next;
} Tag;

// 添加笔记标签(注意标签有重名检测，成功返回SUCCESS，失败返回ERROR)
Status addTagToNote(Note* note, char* tagName);

// 添加文件夹标签(注意标签有重名检测，成功返回SUCCESS，失败返回ERROR)
Status addTagToFolder(Folder* folder, char* tagName);

// 删除笔记标签(成功返回SUCCESS，失败返回ERROR,以下同理)
Status deleteTagFromNote(Note* note, char* tagName);

// 删除文件夹标签
Status deleteTagFromFolder(Folder* folder, char* tagName);

// 显示指定笔记的标签
Status viewTagsInNote(Note* note);

// 显示指定文件夹的标签
Status viewTagsInFolder(Folder* folder);

// 根据标签内容，在当前目录下进行笔记搜索，输出匹配的标签对应的笔记文件名
Status searchNotesInFolderByTag(Folder* folder, char* tagName);

// 根据标签内容，对所有笔记进行搜索，输出匹配的标签所对应的笔记的文件名以及绝对路径
Status searchNotesByTag(char* tagName);

#endif