/** 
 * @file object.c
 * @brief 这份源文件实现了有关 Object 结构体的一系列重要函数。以下单位均为像素。
 * @author 陆营
 * @date 2025-12-03
 * @version v1.0
 */

#include <stdio.h>
#include <stdbool.h>
#include "object.h"

#ifndef OBJECT_H
#define OBJECT_H

typedef enum ObjectType {
	PLAYER,
	ENEMY,
	BULLET
} ObjectType;

/**
 * @brief 游戏对象，表示游戏中的各种实体。无需单独储存每个对象的宽高，而通过对象的类型推导，使用预定义的常量。
 */
typedef struct Object {
	int x, y; // 游戏对象左上角位置坐标，单位：像素
	ObjectType type;
} Object;

#endif /* OBJECT_H */

/**
 * @brief 判断游戏对象是否碰撞。
 */
bool object_collision(const Object *obj1, const Object *obj2) {
	if (!obj1 || !obj2) {
		fprintf(stderr, "Invalid object pointer.\n");
		return false;
	}

	// 推导对象的宽高
	int width1, height1, width2, height2;
	switch (obj1->type) {
		case PLAYER:
			width1 = PLAYER_WIDTH;
			height1 = PLAYER_HEIGHT;
			break;
		case ENEMY:
			width1 = ENEMY_WIDTH;
			height1 = ENEMY_HEIGHT;
			break;
		case BULLET:
			width1 = BULLET_WIDTH;
			height1 = BULLET_HEIGHT;
			break;
		default:
			fprintf(stderr, "Object type error.\n");
			return false;
	}

	switch (obj2->type) {
		case PLAYER:
			width2 = PLAYER_WIDTH;
			height2 = PLAYER_HEIGHT;
			break;
		case ENEMY:
			width2 = ENEMY_WIDTH;
			height2 = ENEMY_HEIGHT;
			break;
		case BULLET:
			width2 = BULLET_WIDTH;
			height2 = BULLET_HEIGHT;
			break;
		default:
			fprintf(stderr, "Object type error.\n");
			return false;
	}

	if (obj1->x < obj2->x + width2 &&
		obj1->x + width1 > obj2->x &&
		obj1->y < obj2->y + height2 &&
		obj1->y + height1 > obj2->y) {
		return true;
	}

	return false;
}
