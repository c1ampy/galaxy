/** 
 * @file list.c
 * @brief 这份源文件实现了一个泛型链表，支持了开发飞机大战主程序所需的必要功能。
 * @author 陆营
 * @date 2025-12-02
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
	void *data; // 泛型数据
	struct Node *next, *prev; // 双向链表
} Node;

/**
 * @brief 链表本身，头结点总不储存数据而尾结点储存数据。
 */
typedef struct List {
	Node *head, *tail;
} List;

#endif // LIST_H

/**
 * @brief 初始化链表，最初链表只有一个结点，其即为头结点又为尾结点，头结点总不储存数据而尾结点储存数据。
 * @return 指向新创建链表的指针。
 */
List *list_init() {
	List *list = (List *)malloc(sizeof(List));
	if (!list) {
		fprintf(stderr, "内存分配失败。\n");
		exit(EXIT_FAILURE);
	}
	                                   
	list->head = (Node *)malloc(sizeof(Node));
	list->head->data = NULL;
	list->head->next = NULL;
	list->head->prev = NULL;
	list->tail = list->head;
	return list;
}

/**
 * @brief 从链表尾部插入元素。
 */
void list_append(List *list, void *data) {
	if (!list) {
		fprintf(stderr, "无效的链表。\n");
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

/**
 * @brief O(1) 随机删除链表元素，需要保证传入的结点在链表中，且不为头结点。
 */
void list_random_erase(List *list, Node *node) {
	if (!list) {
		fprintf(stderr, "无效的链表。\n");
		return;
	}
	if (!node || node == list->head) {
		fprintf(stderr, "无效的结点。\n");
		return;
	}

	if (node->prev)
		node->prev->next = node->next;
	if (node->next)
		node->next->prev = node->prev;
	else
		list->tail = node->prev; // 如果删除的是尾结点，更新链表的尾指针

	free(node->data);
	free(node);
}

/**
 * @brief 释放链表。
 */
void list_free(List *list) {
	if (!list)
		return;

	Node *cur = list->head;
	while (cur) {
		Node *next = cur->next;
		free(cur->data);
		free(cur);
		cur = next;
	}
	free(list);
}
