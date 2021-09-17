/*
Editors: Kelsey McRae, Malachi Parks
Section: CISC361-010
Assignment: Programming Assignment 4
Due Date: 10/?/2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct LinkedList{
	char* data;
	char** synon;
	struct LinkedList *next;
};

typedef struct LinkedList *node;
node add(node head, char* d, char** s);
void printlist(node head);
node removelist(node head);
void printpartial(node head, int size, int num);

node add(node head, char* d, char** s){
	node tmp;
	node t;
	tmp = malloc(sizeof(node));
	int len = strlen(d);
	tmp->data = (char*)malloc(len*sizeof(char));
	strcpy(tmp->data, d);
	
	int count = 0;
	/*if(s != NULL) {
		while(s[count]!=NULL)
			count++;
	
	tmp->synon = (char**)malloc(count*sizeof(char*));
	for(int j = 0; j<count; j++)
		strcpy(tmp->synon[j], s[j]); 
	} else*/
	tmp->synon = s;

	tmp->next = NULL;

	if (head == NULL)
		head = tmp;
	else {
		t = head;
		while (t->next != NULL)
			t = t->next;
		t->next = tmp;
	}
	return head;
}

void printlist(node head){
	node t;
	t = malloc(sizeof(node));
	if (head != NULL) {
		t = head;
		while (t->next != NULL){
			if (t->synon !=  NULL) {
				printf("%s      ",t->data);
				char *tmp = *(t->synon);
				int i = 1;
				while(tmp) {
					printf("%s ", tmp);
					tmp = *(t->synon+i);
					i ++;
				}
				printf("\n");
			}
			else
				printf("%s\n",t->data);	
			t = t->next;
		}
		if (t->synon !=  NULL) {
			printf("%s      ",t->data);
			char *tmp = *(t->synon);
			int i = 1;
			while(tmp) {
				printf("%s ", tmp);
				tmp = *(t->synon+i);
				i ++;
			}
			printf("\n");
		}
		else
			printf("%s\n",t->data); 
	}
}
void printpartial(node head, int size, int num){
	int findNode = size - num;
	if (findNode > 0) {
		int count = 0;
		node t;
		t = head;
		while (t->next != NULL && count < findNode){
			count++;
			t = t->next;	
		}
		while (t->next != NULL) {
			printf("%s\n", t->data);
			t = t->next;
		}
		printf("%s\n", t->data);
	} else 
		printlist(head);
		
}		
node removelist(node head){
	node t;
	if (head != NULL) {
		t = head;
		head = head->next;
		free(t->data);
		free(t->synon);
		free(t);
	}
	return head;
}	

