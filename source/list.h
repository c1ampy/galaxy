/** 
 * @file list.h
 * @brief 这份头文件声明了泛型链表类型以及一些对其进行操作的函数。
 * @author 陆营
 * @date 2025-12-02
 * @version v1.0
 */

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

/**
 * @brief 初始化链表，最初链表只有一个结点，其即为头结点又为尾结点。
 * @return 指向新创建链表的指针。
 */
List *list_init();

/**
 * @brief O(1) 从链表尾部插入元素。
 */
void list_append(List *list, void *data);

/**
 * @brief O(1) 随机删除链表元素，需要保证传入的结点在链表中，且不为头结点。
 */
void list_random_erase(List *list, Node *node);

/**
 * @brief 释放链表。
 */
void list_free(List *list);

#endif /* LIST_H */
