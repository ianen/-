#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "note.h"
//#include "folder.h"

// �����ʼ��ļ��ڵ�
Note* createNote(char *creater,char* noteTitle,char* currentPath) {
	Note* newNote = (Note*)malloc(sizeof(Note));//�±ʼ��ļ��ڵ�
	char newPath[MAX_NOTE_PATH_LENGTH];//�±ʼ�·��
	
	//�����ڴ�ʧ��
	if (!newNote) {
		printf("�ڴ����ʧ�ܣ��޷������ʼ��ļ���\n");
		return NULL;
	}

	// ���ñʼǵĴ�����
	strncpy(newNote->creater, creater, MAX_NOTE_CREATER_LENGTH);
	newNote->title[MAX_NOTE_NAME_LENGTH - 1] = '\0'; // ȷ���ַ����� null ��ֹ

	// ���ñʼǵ�����
	strncpy(newNote->title, noteTitle, MAX_NOTE_NAME_LENGTH);
	newNote->title[MAX_NOTE_NAME_LENGTH - 1] = '\0'; // ȷ���ַ����� null ��ֹ

	// ���ñʼǵ�·��
	strncpy(newPath, currentPath, MAX_NOTE_PATH_LENGTH);
	strncat(newPath, "\\", MAX_NOTE_PATH_LENGTH);
	strncat(newPath, noteTitle, MAX_NOTE_PATH_LENGTH);
	strncpy(newNote->path, newPath, MAX_NOTE_PATH_LENGTH);
	newNote->path[MAX_NOTE_PATH_LENGTH - 1] = '\0'; // ȷ���ַ����� null ��ֹ

	// ��ʼ����������
	newNote->tag = NULL;
	newNote->next = NULL;

	return newNote;
}

// ��ӱʼǵ��ļ���
Status addNoteToFolder(Folder* folder, Note* newNote) {
	//�������Ϊ��ʱ
	if (!folder || !newNote)
		return ERROR;

	Note* note = folder->note;

	//�������ļ��е��ļ��б��β�ڵ�
	while (note->next)
		note = note->next;
	note->next = newNote;
	
	return SUCCESS;
}

//�ڵ�ǰ�ļ��д����±ʼ��ļ�����Ӧ mkdir <�ʼ��ļ���>
Status addNewNoteToFolder(Folder* currentfolder, char* creater, char* noteTitle) {
	//�ȴ����½ڵ�
	Note* newNote = createNote(creater, noteTitle, currentfolder->path);
	//���½ڵ�ָ�벻ΪNULL�������ܳɹ����ڵ���ӵ��ļ���ʱ
	if (addNoteToFolder(currentfolder, newNote))
		return SUCCESS;
	else
		return ERROR;
}

// ��ָ���ʼ��ļ�����ɾ��
Status deleteNote(Note* note) {
	//����սڵ�ʱ
	if (!note)
		return ERROR;

	//ɾ���ʼ��ļ���tag�б�
	Tag* preTag = note->tag;
	Tag* tag = preTag;
	while (tag) {
		preTag = tag->next;
		free(tag);
		tag = preTag;
	}

	//�ͷűʼǽڵ�
	free(note);

	return SUCCESS;
}

// �ӵ�ǰ�ļ�����ɾ���ʼ�,��Ӧ rm <�ʼ��ļ���>
Status deleteNoteFromFolder(Folder* folder, char* noteTitle) {

}

//���ʼ����������ʼ��ļ�
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

	// ���û���ҵ�ƥ��ıʼǣ����ؿ�ָ��
	return NULL;
}

// ���ʼ��ļ��ƶ���ָ�����ļ���Ŀ¼��,��Ӧ mv <�ʼ��ļ���> <�ʼ��ļ���Ŀ¼>
Status moveNoteToFolder(char* noteName, Folder* folder, Folder* root) {
	Note* note = searchNoteByName(root, noteName);
	if (note) {
		
		return SUCCESS;
	}
	else
		return ERROR;

}

// ���ʼ��ļ���������������ԭ�ļ�����ΪĿ���ļ��� ��Ӧ mv <�ʼ�ԭ�ļ���> <�ʼ�Ŀ���ļ���>
Status renameNote(Note* note, char* newName) {
	if (!note||!newName) {
		return ERROR;
	}

	return SUCCESS;
}

// ��ָ���ļ����ڵıʼ��ļ����ļ�������ĸ��������,��Ӧ sort <�ʼ��ļ�����>
Status sortNotesInFolder(Folder* folder) {

}