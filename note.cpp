#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "note.h"
#include "folder.h"

// �����ʼ��ļ��ڵ��ʵ�ʱʼ��ļ�
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

	//������ʵ�ʼ��ļ�
	FILE* file = fopen(newPath, "w");; // �ļ�ָ��
	if (file == NULL) 
		return NULL;

	return newNote;
}

// ��ӱʼǽڵ㵽�ļ���
Status addNoteToFolder(Folder* folder, Note* newNote) {
	//�������Ϊ��ʱ
	if (!folder || !newNote)
		return ERROR;

	//�������ļ��е��ļ��б��β�ڵ�
	Note* note = folder->note;
	while (note->next)
		note = note->next;
	note->next = newNote;
	
	return SUCCESS;
}

//�ڵ�ǰ�ļ��д����±ʼ��ļ�����Ӧ mkdir <�ʼ��ļ���>
Status addNewNoteToFolder(Folder* currentFolder, char* creater, char* noteTitle) {
	//�ȴ����½ڵ�
	Note* newNote = createNote(creater, noteTitle, currentFolder->path);
	//���½ڵ�ָ�벻ΪNULL�������ܳɹ����ڵ���ӵ��ļ���ʱ
	if (addNoteToFolder(currentFolder, newNote))
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

	//ɾ��ʵ�ʱʼ��ļ�
	remove(note->path);

	//�ͷűʼǽڵ�
	free(note);

	return SUCCESS;
}

// �ӵ�ǰ�ļ�����ɾ���ʼ�,��Ӧ rm <�ʼ��ļ���>
Status deleteNoteFromFolder(Folder* currentFolder, char* noteTitle) {
	//������ΪNULL
	if (!currentFolder || !noteTitle)
		return ERROR;

	// ������������ָ���ʼ�
	Note* note = currentFolder->note, * preNote = NULL;
	while(note != NULL) {
		if (!strcmp(note->title, noteTitle)) {
			if (preNote == NULL) {
				// ɾ������ͷ�ڵ�
				currentFolder->note = note->next;
			}
			else {
				// ɾ���м�ڵ��β�ڵ�
				preNote->next = note->next;
			}
			deleteNote(note);
			return SUCCESS;
		}
		preNote = note, note = note->next;
	}

	return ERROR;
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

//���ʼ��ļ������洢�ļ���
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

	// ���û���ҵ�ƥ����ļ��У����ؿ�ָ��
	return NULL;
}

// ���ʼ��ļ��ƶ���ָ�����ļ���Ŀ¼��,��Ӧ mv <�ʼ��ļ���> <�ʼ��ļ���Ŀ¼>
Status moveNoteToFolder(char* noteName, Folder* folder, Folder* root) {
	Note* note = searchNoteByName(root, noteName);
	if (note) {
		// ���ʼ��ļ��ڵ��ƶ���ָ�����ļ���Ŀ¼��
		if (!addNewNoteToFolder(folder, note->creater, note->title)) {
			return ERROR;
		}
		Folder* newFolder = searchFolderByNote(root, note);

		// �ƶ���ʵ�ļ�
		char newPath[MAX_NOTE_PATH_LENGTH];
		snprintf(newPath, MAX_NOTE_PATH_LENGTH, "%s/%s", newFolder->path, note->title);
		if (rename(note->path, newPath) != 0) {
			return ERROR;
		}

		// ��ԭ�ļ�����ɾ���ʼ��ļ��ڵ�
		if (!deleteNoteFromFolder(searchFolderByNote(root, note), note->title)) {
			return ERROR;
		}
		return SUCCESS;
	}
	else
		return ERROR;
}

// ���ʼ��ļ���������������ԭ�ļ�����ΪĿ���ļ��� ��Ӧ mv <�ʼ�ԭ�ļ���> <�ʼ�Ŀ���ļ���>
Status renameNote(Note* note, char* newName) {
	//����ΪNULLʱ
	if (!note||!newName) {
		return ERROR;
	}

	//���ʼ��ļ��ڵ�������
	strncpy(note->title, newName, MAX_NOTE_NAME_LENGTH);
	note->title[MAX_NOTE_NAME_LENGTH - 1] = '\0'; // ȷ���ַ����� null ��ֹ

	//��ʵ�ʱʼ��ļ�������
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

// ��ָ���ļ����ڵıʼ��ļ����ļ�������ĸ��������,��Ӧ sort <�ʼ��ļ�����>
Status sortNotesInFolder(Folder* folder) {
	//����ΪNULL�����ļ���û�бʼ�ʱ
	if (!folder || !folder->note)
		return ERROR;

	Note* pHead = NULL;  // ��������ͷ�ڵ�ָ�룬����ʼ��Ϊ NULL
	Note* cur = folder->note;
	while (cur) {
		Note* next = cur->next;  // ��¼��һ���ڵ��ָ��
		cur->next = NULL;  // �Ͽ���ǰ�ڵ���ԭ���������
		Note** pp = &pHead;
		while (*pp && (*pp)->title[0] < cur->title[0]) {
			pp = &((*pp)->next);
		}
		cur->next = *pp;
		*pp = cur;  // ���뵱ǰ�ڵ㵽��������
		cur = next;  // ������һ���ڵ�
	}
	folder->note = pHead ? pHead->next : NULL;  // �����������ͷ�ڵ�
}