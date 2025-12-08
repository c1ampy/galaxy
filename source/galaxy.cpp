#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "object.h"
#include <windows.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 800
#define FPS 60

Object *player;
List *enemy_list = NULL, *bullet_list = NULL;
int player_speed, enemy_speed, bullet_speed; // 单位：像素每帧
// 上面的各类 speed 参数不设置为常量，因为后续可能推出动态难度系统。

/**
 * @brief 对当前帧，处理所有游戏对象的移动、添加、删除等操作。
 */
void game_update();

/**
 * @brief 对当前帧，渲染窗口。
 */
void game_render();

int main() {
	
	// 各类其他初始化。

	player = (Object *)malloc(sizeof(Object));
	enemy_list = list_init(), bullet_list = list_init();
	player_speed = 4;
	enemy_speed = bullet_speed = 0;
	// 速度这里是乱写的，等待难度系统开发与测试。

	bool running = true;
	while (running) {
		game_update();
		game_render();
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			// 之后可以拓展出一个暂停界面。
			fprintf(stdout, "Exiting.\n");
			running = false;
		}
		Sleep(1000 / FPS);
	}

	list_free(enemy_list);
	list_free(bullet_list);
	free(player);

	return 0;
}

/**
 * @brief 对当前帧，处理所有游戏对象的移动、添加、删除等操作。
 */
void game_update() {

	// 处理玩家移动，等待后续开发。
	if (GetAsyncKeyState('W') & 0x8000 || GetAsyncKeyState(VK_UP) & 0x8000) {
		
	}
	if (GetAsyncKeyState('S') & 0x8000 || GetAsyncKeyState(VK_DOWN) & 0x8000) {
		
	}
	if (GetAsyncKeyState('A') & 0x8000 || GetAsyncKeyState(VK_LEFT) & 0x8000) {
		
	}
	if (GetAsyncKeyState('D') & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		
	}

	if (false /* 玩家的部分模型超出边界 */) {
		// 强制拉回。
	}

	// 生成新子弹（如果开火键（暂定为空格键）被按下）。
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		Object *new_bullet = (Object *)malloc(sizeof(Object));
		if (!new_bullet) {
			fprintf(stderr, "内存分配失败。\n");
			exit(EXIT_FAILURE);
		}

		// 设置新子弹初始位置。

		list_append(bullet_list, new_bullet);
	}

	// 生成新敌机（如果条件合适）。
	if (false /* 生成敌机的条件合适 */) {
		Object *new_enemy = (Object *)malloc(sizeof(Object));
		if (!new_enemy) {
			fprintf(stderr, "内存分配失败。\n");
			exit(EXIT_FAILURE);
		}

		// 设置新敌机初始位置。

		list_append(enemy_list, new_enemy);
	}

	// 所有敌机向下移动，删除超出屏幕的敌机。
	for (Node *enemy_node = enemy_list->head->next; enemy_node; ) {
		/**
		 * 此处不能在 for 循环语句中写入 enemy_node = enemy_node->next，
		 * 因为在进入下一次循环之前，当前的 enemy_node 指针所指向的结点可能已经被释放，
		 * 若再次访问 enemy_node->next 会引发段错误。
		 * 应该额外用一个指针变量 next_enemy_node 提前记录后继节点的地址。
		 */
		Node *next_enemy_node = enemy_node->next;
		Object *enemy = (Object *)enemy_node->data;

		enemy->y += enemy_speed; // 如果后续有专门的移动和边界检测一体化 move() 函数就再说。
		if (enemy->y > SCREEN_HEIGHT) {
			list_random_erase(enemy_list, enemy_node);
		}

		enemy_node = next_enemy_node;
	}

	// 所有子弹向上移动，删除超出屏幕的子弹。
	for (Node *bullet_node = bullet_list->head->next; bullet_node; ) {
		Node *next_bullet_node = bullet_node->next; // 同上
		Object *bullet = (Object *)bullet_node->data;

		bullet->y -= bullet_speed; // 如果后续有专门的移动和边界检测一体化 move() 函数就再说。
		if (bullet->y + BULLET_HEIGHT < 0) {
			list_random_erase(bullet_list, bullet_node);
		}

		bullet_node = next_bullet_node;
	}

	// 对于所有敌机，判断其与子弹、玩家是否碰撞。
	for (Node *enemy_node = enemy_list->head->next; enemy_node; ) {
		Node *next_enemy_node = enemy_node->next; // 同上
		Object *enemy = (Object *)enemy_node->data;
		
		for (Node *bullet_node = bullet_list->head->next; bullet_node; ) {
			Node *next_bullet_node = bullet_node->next; // 同上
			Object *bullet = (Object *)bullet_node->data;
			
			if (object_collide(enemy, bullet)) {
				list_random_erase(enemy_list, enemy_node);
				list_random_erase(bullet_list, bullet_node);
			}

			bullet_node = next_bullet_node;
		}

		if (object_collide(enemy, player)) {
			// --hp 或结束游戏等
		}
		
		enemy_node = next_enemy_node;
	}
}
