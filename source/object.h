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
#define ENIMY_WIDTH 40
#define ENIMY_HEIGHT 40
#define BULLET_WIDTH 10
#define BULLET_HEIGHT 20

typedef enum ObjectType {
	PLAYER,
	ENIMY,
	BULLET
} ObjectType;

typedef struct Object {
	int x, y; // 游戏对象左上角位置坐标，单位：像素
	ObjectType type;
} Object;


bool object_collide(const Object *obj1, const Object *obj2);

#endif /* OBJECT_H */
