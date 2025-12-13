#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include "list.h"
#include "object.h"
#include "render.h"
#include "control.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 800
#define FPS 60

extern RenderTextures g_renderTextures;

Object *player = NULL;
List *enemy_list = NULL, *bullet_list = NULL;

#define DIFFICULTY_COUNT 3

int difficulty;
const double min_fire_gap[DIFFICULTY_COUNT] = { 0.5, 0.5, 0.5 }; // 单位：秒
const double min_enemy_spawn_gap[DIFFICULTY_COUNT] = { 1, 1, 1 }; // 单位：秒
const int player_speed[DIFFICULTY_COUNT] = { 4, 4, 4 }; // 单位：像素每帧
const int enemy_speed[DIFFICULTY_COUNT] = { 3, 3, 3 }; // 单位：像素每帧
const int bullet_speed[DIFFICULTY_COUNT] = { 6, 6, 6 }; // 单位：像素每帧

clock_t last_bullet_spawn_time, last_enemy_spawn_time;

#define POINTS_PER_HIT 10

GameControlData game_control_data;

/**
 * @brief 初始化游戏对象。
 */
void object_init();

/**
 * @brief 释放游戏对象的内存。
 */
void object_free();

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
 * @brief 对所有敌机，判断其是否被子弹击中。
 */
void enemy_bullet_collision();

/**
 * @brief 对所有敌机，判断其是否与玩家碰撞。
 */
void enemy_player_collision();

/**
 * @brief 对当前帧，处理所有游戏对象的移动、添加、删除等操作。
 */
void all_object_update();

int main() {

	render_init(SCREEN_WIDTH, SCREEN_HEIGHT, L"飞机大战");

	render_load_gameplay_textures(
		L"",
		L"D:\\coding\\galaxy_test\\image\\player.png",
		L"D:\\coding\\galaxy_test\\image\\enemy.png",
		L"D:\\coding\\galaxy_test\\image\\bullet.png"
	);

	game_control_data.running = true;
	game_to_menu(&game_control_data);
	while (game_control_data.running) {
		switch (game_control_data.state) {
		case MENU:
			const int high_score = 0; // 等待加入文件读取最高分。
			const int button_id = render_draw_main_menu(SCREEN_WIDTH, SCREEN_HEIGHT, high_score, FPS);

			switch (button_id) {
			case 0:
				game_start(&game_control_data, 1);
			}
		}
		Sleep(1000 / FPS);
	}

	return 0;
}

/**
 * @brief 初始化游戏对象。
 */
void object_init() {
	player = (Object *)malloc(sizeof(Object));
	if (!player) {
		fprintf(stderr, "malloc() failed.\n");
		exit(EXIT_FAILURE);
	}

	player->x = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2;
	player->y = SCREEN_HEIGHT - PLAYER_HEIGHT - 100;
	player->type = PLAYER;

	enemy_list = list_init();
	bullet_list = list_init();
}

/**
 * @brief 释放游戏对象的内存。
 */
void object_free() {
	list_free(enemy_list);
	list_free(bullet_list);
	free(player);
}

/**
 * @brief 处理玩家移动。
 */
void player_move() {
	if (GetAsyncKeyState('W') & 0x8000 || GetAsyncKeyState(VK_UP) & 0x8000) {
		player->y -= player_speed[difficulty];
	}
	if (GetAsyncKeyState('S') & 0x8000 || GetAsyncKeyState(VK_DOWN) & 0x8000) {
		player->y += player_speed[difficulty];
	}
	if (GetAsyncKeyState('A') & 0x8000 || GetAsyncKeyState(VK_LEFT) & 0x8000) {
		player->x -= player_speed[difficulty];
	}
	if (GetAsyncKeyState('D') & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		player->x += player_speed[difficulty];
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
		fprintf(stderr, "malloc() failed.\n");
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
		fprintf(stderr, "malloc() failed.\n");
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

		enemy->y += enemy_speed[difficulty];
		if (enemy->y > SCREEN_HEIGHT) {
			list_random_erase(enemy_list, enemy_node);

			fprintf(stdout, "An enemy has been erased. (out of bound)\n");
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

		bullet->y -= bullet_speed[difficulty];
		if (bullet->y < -BULLET_HEIGHT) {
			list_random_erase(bullet_list, bullet_node);

			fprintf(stdout, "A bullet has been erased. (out of bound)\n");
		}

		bullet_node = next_bullet_node;
	}
}

/**
 * @brief 对所有敌机，判断其是否被子弹击中。
 */
void enemy_bullet_collision() {
	for (Node *enemy_node = enemy_list->head->next; enemy_node; ) {
		/**
		 * 创建指针变量 next_enemy_node 原因：
		 * 见函数 enemy_move() 中 next_enemy_node 定义处的注释。
		 */
		Node *next_enemy_node = enemy_node->next;
		Object *enemy = (Object *)enemy_node->data;

		for (Node *bullet_node = bullet_list->head->next; bullet_node; ) {
			/**
			 * 创建指针变量 next_bullet_node 原因：
			 * 见函数 enemy_move() 中 next_enemy_node 定义处的注释。
			 */
			Node *next_bullet_node = bullet_node->next;
			Object *bullet = (Object *)bullet_node->data;

			if (object_collision(enemy, bullet)) {
				add_score(&game_control_data, POINTS_PER_HIT);

				list_random_erase(enemy_list, enemy_node);
				list_random_erase(bullet_list, bullet_node);

				fprintf(stdout, "A bullet has been erased. (collision with enemy)\n");
				fprintf(stdout, "An enemy has been erased. (collision with bullet)\n");

				break; // 不再与其他子弹进行碰撞判断。
			}

			bullet_node = next_bullet_node;
		}

		enemy_node = next_enemy_node;
	}
}

/**
 * @brief 对所有敌机，判断其是否与玩家碰撞。
 */
void enemy_player_collision() {
	for (Node *enemy_node = enemy_list->head->next; enemy_node; ) {
		/**
		 * 创建指针变量 next_enemy_node 原因：
		 * 见函数 enemy_move() 中 next_enemy_node 定义处的注释。
		 */
		Node *next_enemy_node = enemy_node->next;
		Object *enemy = (Object *)enemy_node->data;

		if (object_collision(enemy, player)) {
			reduce_hp(&game_control_data, 1);

			list_random_erase(enemy_list, enemy_node);
			
			fprintf(stdout, "An enemy has been erased. (collision with player)\n");
		}

		enemy_node = next_enemy_node;
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
		if (fire_gap >= min_fire_gap[difficulty]) {
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
	if (enemy_spawn_gap >= min_enemy_spawn_gap[difficulty]) {
		enemy_spawn();
		last_enemy_spawn_time = enemy_spawn_time; // 更新最后一次敌机生成时间。

		fprintf(stdout, "An enemy has been spawned.\n");
	}

	enemy_move();
	bullet_move();
	enemy_bullet_collision();
	enemy_player_collision();
}
