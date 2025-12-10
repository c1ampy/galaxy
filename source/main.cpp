#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include "list.h"
#include "object.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 800
#define FPS 60

Object *player;
List *enemy_list = NULL, *bullet_list = NULL;
double min_fire_gap, min_enemy_spawn_gap; // 单位：秒
clock_t last_bullet_spawn_time, last_enemy_spawn_time;
int player_speed, enemy_speed, bullet_speed; // 单位：像素每帧
// 上面的各类 speed 与 gap 参数不设置为常量，因为后续可能推出动态难度系统。

/**
 * @brief 处理玩家移动。
 */
void player_move();

/**
 * @brief 处理玩家开火。
 */
void player_fire();

/**
 * @brief 处理敌机生成。
 */
void enemy_spawn();

/**
 * @brief 处理敌机向下移动，并删除超出屏幕底端的敌机。
 */
void enemy_move();

/**
 * @brief 处理子弹向上移动，并删除超出屏幕顶端的子弹。
 */
void bullet_move();

/**
 * @brief 对当前帧，处理所有游戏对象的移动、添加、删除等操作。
 */
void all_object_update();

/**
 * @brief 对当前帧，渲染窗口。
 */
void all_object_render();

int main() {
	
	// 各类其他初始化。

	player = (Object *)malloc(sizeof(Object));

	player->x = 0, player->y = SCREEN_HEIGHT - PLAYER_HEIGHT, player->type = PLAYER; // 位置坐标为测试用值，正式游戏请修改。
	
	enemy_list = list_init(), bullet_list = list_init();
	min_enemy_spawn_gap = min_fire_gap = 0.5;
	player_speed = 4, enemy_speed = 3, bullet_speed = 6;
	// 开火间隔、敌机生成间隔、速度这里都是乱写的，等待难度系统开发与测试。

	bool running = true;
	while (running) {
		all_object_update();
		all_object_render();
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
 * @brief 处理玩家移动。
 */
void player_move() {
	if (GetAsyncKeyState('W') & 0x8000 || GetAsyncKeyState(VK_UP) & 0x8000) {
		player->y -= player_speed;
	}
	if (GetAsyncKeyState('S') & 0x8000 || GetAsyncKeyState(VK_DOWN) & 0x8000) {
		player->y += player_speed;
	}
	if (GetAsyncKeyState('A') & 0x8000 || GetAsyncKeyState(VK_LEFT) & 0x8000) {
		player->x -= player_speed;
	}
	if (GetAsyncKeyState('D') & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		player->x += player_speed;
	}

	// 超出边界时拉回
	if (player->y < 0) {
		player->y = 0;
	}
	if (player->y + PLAYER_HEIGHT > SCREEN_HEIGHT) {
		player->y = SCREEN_HEIGHT - PLAYER_HEIGHT;
	}
	if (player->x < 0) {
		player->x = 0;
	}
	if (player->x + PLAYER_WIDTH > SCREEN_WIDTH) {
		player->x = SCREEN_WIDTH - PLAYER_WIDTH;
	}
}

/**
 * @brief 处理玩家开火。
 */
void player_fire() {
	Object *new_bullet = (Object *)malloc(sizeof(Object));
	if (!new_bullet) {
		fprintf(stderr, "内存分配失败。\n");
		exit(EXIT_FAILURE);
	}

	new_bullet->x = player->x + PLAYER_WIDTH / 2 - BULLET_WIDTH / 2;
	new_bullet->y = player->y - BULLET_HEIGHT;
	new_bullet->type = BULLET;

	list_append(bullet_list, new_bullet);
}

/**
 * @brief 处理敌机生成。
 */
void enemy_spawn() {
	Object *new_enemy = (Object *)malloc(sizeof(Object));
	if (!new_enemy) {
		fprintf(stderr, "内存分配失败。\n");
		exit(EXIT_FAILURE);
	}

	new_enemy->x = rand() % (SCREEN_WIDTH - ENEMY_WIDTH);
	new_enemy->y = -ENEMY_HEIGHT;
	new_enemy->type = ENEMY;

	list_append(enemy_list, new_enemy);
}

/**
 * @brief 处理敌机向下移动，并删除超出屏幕底端的敌机。
 */
void enemy_move() {
	for (Node *enemy_node = enemy_list->head->next; enemy_node; ) {
		/**
		 * 此处不能在 for 循环语句中写入 enemy_node = enemy_node->next，
		 * 因为在进入下一次循环之前，当前的 enemy_node 指针所指向的结点可能已经被释放，
		 * 若再次访问 enemy_node->next 会引发段错误。
		 * 应该额外用一个指针变量 next_enemy_node 提前记录后继节点的地址。
		 */
		Node *next_enemy_node = enemy_node->next;
		Object *enemy = (Object *)enemy_node->data;

		enemy->y += enemy_speed;
		if (enemy->y > SCREEN_HEIGHT) {
			list_random_erase(enemy_list, enemy_node);

			fprintf(stdout, "An enemy has been erased. (out of bound)");
		}

		enemy_node = next_enemy_node;
	}
}

/**
 * @brief 处理子弹向上移动，并删除超出屏幕顶端的子弹。
 */
void bullet_move() {
	for (Node *bullet_node = bullet_list->head->next; bullet_node; ) {
		/**
		 * 创建指针变量 next_bullet_node 原因：
		 * 见函数 enemy_move() 中 next_enemy_node 定义处的注释。
		 */
		Node *next_bullet_node = bullet_node->next;
		Object *bullet = (Object *)bullet_node->data;

		bullet->y -= bullet_speed;
		if (bullet->y < -BULLET_HEIGHT) {
			list_random_erase(bullet_list, bullet_node);

			fprintf(stdout, "A bullet has been erased. (out of bound)");
		}

		bullet_node = next_bullet_node;
	}
}

/**
 * @brief 对当前帧，处理所有游戏对象的移动、添加、删除等操作。
 */
void all_object_update() {

	player_move();

	// 检查开火键（空格键）是否被按下。
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		const clock_t fire_time = clock();
		const double fire_gap = (double)(fire_time - last_bullet_spawn_time) / CLOCKS_PER_SEC;
		
		// 检查本次开火与上次开火的时间间隔是否足够。
		if (fire_gap >= min_fire_gap) {
			player_fire();
			last_bullet_spawn_time = fire_time; // 更新最后一次子弹生成时间。

			fprintf(stdout, "A bullet has been fired.\n");
			// 给出开火成功成功的反馈，如音效等。
		}
		else {
			fprintf(stdout, "Firing event has been denied. (not enough gap)\n");
			// 给出开火失败的反馈，如音效等。
		}
	}

	const clock_t enemy_spawn_time = clock();
	const double enemy_spawn_gap = (double)(enemy_spawn_time - last_enemy_spawn_time) / CLOCKS_PER_SEC;
	
	// 检查本次敌机生成与上次敌机生成的时间间隔是否足够。
	if (enemy_spawn_gap >= min_enemy_spawn_gap) {
		enemy_spawn();
		last_enemy_spawn_time = enemy_spawn_time;

		fprintf(stdout, "An enemy has been spawned.\n");
	}

	enemy_move();
	bullet_move();

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

				fprintf(stdout, "A bullet has been erased. (collision)");
				fprintf(stdout, "A enemy has been erased. (collision)");
			}

			bullet_node = next_bullet_node;
		}

		if (object_collide(enemy, player)) {
			// --hp 或结束游戏等
		}
		
		enemy_node = next_enemy_node;
	}
}

/**
 * @brief 对当前帧，渲染窗口。
 */
void all_object_render() {

}
