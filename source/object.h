/** 
 * @file object.h
 * @brief 这份头文件声明了 Object 结构体及其相关常量，用于表示游戏中的各种对象。以下单位均为像素。
 * @author 陆营
 * @date 2025-12-03
 * @version v1.0
 */

#include <stdbool.h>

#ifndef OBJECT_H
#define OBJECT_H

#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 50
#define ENEMY_WIDTH 40
#define ENEMY_HEIGHT 40
#define BULLET_WIDTH 10
#define BULLET_HEIGHT 20

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

/**
 * @brief 判断游戏对象是否碰撞。
 */
bool object_collision(const Object *obj1, const Object *obj2);

#endif /* OBJECT_H */
