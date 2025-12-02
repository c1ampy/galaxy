/** 
 * @file list.c
 * @brief 这份源文件实现了一个泛型链表。
 * @author 陆营
 * @date 2025-12-01
 * @version v1.0
 */

#include "list.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef LIST_H
#define LIST_H

/**
* @brief 链表结点
*/
typedef struct Node {
	void *data; // 泛型
	struct Node *next, *prev; // 双向链表
} Node;

/**
* @brief 链表本身
*/
typedef struct List {
	Node *head, *tail; // 头结点与尾结点
} List;

#endif // LIST_H

/**
 * @brief 初始化链表，最初链表只有一个结点，其即为头节点又为尾结点。
 * @param list 指向链表指针的指针，创建的链表的头尾结点储存于 *list 所指向的内存空间。
 */
void list_init(List **list) {
	*list = (List *)malloc(sizeof(List));
	if (!*list) {
		fprintf(stderr, "内存分配失败。\n");
		exit(EXIT_FAILURE);
	}
	                                   
	(*list)->head = (Node *)malloc(sizeof(Node));
	(*list)->head->data = NULL;
	(*list)->head->next = NULL;
	(*list)->head->prev = NULL;
	(*list)->tail = (*list)->head;
}

/**
 * @brief 从链表尾部插入元素。
 */
void list_tail_insert(List *list, void *data) {
	if (!list) {
		fprintf(stderr, "链表未初始化。\n");
		return;
	}

	Node *new_node = (Node *)malloc(sizeof(Node));
	if (!new_node) {
		fprintf(stderr, "内存分配失败。\n");
		exit(EXIT_FAILURE);
	}

	new_node->data = data;
	new_node->next = NULL;
	new_node->prev = list->tail;

	list->tail->next = new_node;
	list->tail = new_node;
}

int main() {
	
	printf("Hello galaxy!\n");

	return 0;
};
