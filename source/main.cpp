/**
 * @file main.cpp
 * @brief 飞机大战游戏的主程序。
 * @author 陆营、邓皓轩
 * @date 2025-12-13
 * @version v1.0
 */

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "list.h"
#include "object.h"
#include "render.h"
#include "control.h"
#include "high_score_save_load.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 800
#define FPS 60

extern RenderTextures g_renderTextures;

Object* player = NULL;
List* enemy_list = NULL, * bullet_list = NULL;

int difficulty;
const int starting_hp[DIFFICULTY_COUNT] = { 2, 2, 1 };
const int delta_hp[DIFFICULTY_COUNT] = { 1, 1, 1 };
const double min_fire_gap[DIFFICULTY_COUNT] = { 0.4, 0.5, 0.6 }; // 单位：秒
const double min_enemy_spawn_gap[DIFFICULTY_COUNT] = { 0.3, 0.2, 0.1 }; // 单位：秒
const int player_speed[DIFFICULTY_COUNT] = { 8, 12, 12 }; // 单位：像素每帧
const int enemy_speed[DIFFICULTY_COUNT] = { 6, 9, 9 }; // 单位：像素每帧
const int bullet_speed[DIFFICULTY_COUNT] = { 12, 18, 18 }; // 单位：像素每帧

clock_t last_bullet_spawn_time, last_enemy_spawn_time;

#define POINTS_PER_HIT 10

GameControlData game_control_data;

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
 * @brief 初始化游戏对象。
 */
void object_init();

/**
 * @brief 对当前帧，处理所有游戏对象的移动、添加、删除等操作。
 */
void object_update();

/**
 * @brief 释放游戏对象的内存。
 */
void object_free();

int main() {

	window_create(SCREEN_WIDTH, SCREEN_HEIGHT, L"飞机大战");

	render_load_texture(
		L"image\\background.png",
		L"image\\player.png",
		L"image\\enemy.png",
		L"image\\bullet.png"
	);

	game_control_data.running = true;
	game_control_to_menu(&game_control_data);
	while (game_control_data.running) {

		if (game_control_data.state == MENU) {

			int high_score[DIFFICULTY_COUNT] = { 0 };
			high_score_load(high_score);
			const int choice = render_draw_main_menu(SCREEN_WIDTH, SCREEN_HEIGHT, high_score, difficulty, FPS);

			if (choice == 0) {
				object_init();
				game_control_start(&game_control_data, starting_hp[difficulty]);
			}
			else if (choice == 1) {
				game_control_to_settings(&game_control_data);
			}
			else if (choice == 2) {
				game_control_data.running = false;
			}
		}
		else if (game_control_data.state == SETTINGS) {

			difficulty = render_draw_difficulty_menu(SCREEN_WIDTH, SCREEN_HEIGHT, difficulty, FPS);

			fprintf(stdout, "Difficulty set to %d.\n", difficulty);

			game_control_to_menu(&game_control_data);
		}
		else if (game_control_data.state == PLAYING) {

			object_update();
			const GameplayVisualState state{
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				game_control_data.score,
				L"",
				(const Object*)player,
				(const List*)enemy_list,
				(const List*)bullet_list,
				difficulty,
				game_control_data.hp,
				starting_hp[difficulty]
			};
			render_draw_current_frame(&state);

			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
				game_control_pause(&game_control_data);
			}
		}
		else if (game_control_data.state == PAUSED) {

			int high_score[DIFFICULTY_COUNT] = { 0 };
			high_score_load(high_score);
			high_score[difficulty] = game_control_data.score > high_score[difficulty] ? game_control_data.score : high_score[difficulty];
			high_score_save(high_score);

			const int choice = render_draw_pause_menu(SCREEN_WIDTH, SCREEN_HEIGHT, FPS);

			if (choice == 0) {
				game_control_resume(&game_control_data);
			}
			else if (choice == 1) {
				object_free();
				object_init();
				game_control_start(&game_control_data, starting_hp[difficulty]);
			}
			else if (choice == 2) {
				object_free();
				game_control_to_menu(&game_control_data);
			}
			else if (choice == 3) {
				object_free();
				game_control_data.running = false;
			}
		}
		else if (game_control_data.state == GAMEOVER) {

			int high_score[DIFFICULTY_COUNT] = { 0 };
			high_score_load(high_score);
			high_score[difficulty] = game_control_data.score > high_score[difficulty] ? game_control_data.score : high_score[difficulty];
			high_score_save(high_score);

			object_free();
			const GameplayVisualState state{
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				game_control_data.score,
				L"",
				(const Object*)player,
				(const List*)enemy_list,
				(const List*)bullet_list,
				difficulty,
				game_control_data.hp,
				starting_hp[difficulty]
			};

			const int choice = render_draw_wasted_page(&state, high_score, FPS);

			if (choice == 0) {
				object_init();
				game_control_start(&game_control_data, starting_hp[difficulty]);
			}
			else if (choice == 1) {
				game_control_to_menu(&game_control_data);
			}
			else if (choice == 2) {
				game_control_data.running = false;
			}
		}

		Sleep(1000 / FPS);
	}

	window_close();

	fprintf(stdout, "Exited.\n");

	return 0;
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
	Object* new_bullet = (Object*)malloc(sizeof(Object));
	if (!new_bullet) {
		fprintf(stderr, "malloc() failed.\n");
		exit(EXIT_FAILURE);
	}

	new_bullet->x = player->x + PLAYER_WIDTH / 2 - BULLET_WIDTH / 2;
	new_bullet->y = player->y;
	new_bullet->type = BULLET;

	list_append(bullet_list, new_bullet);
}

/**
 * @brief 处理敌机生成。
 */
void enemy_spawn() {
	Object* new_enemy = (Object*)malloc(sizeof(Object));
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
	for (Node* enemy_node = enemy_list->head->next; enemy_node; ) {
		/**
		 * 此处不能在 for 循环语句中写入 enemy_node = enemy_node->next，
		 * 因为在进入下一次循环之前，当前的 enemy_node 指针所指向的结点可能已经被释放，
		 * 若再次访问 enemy_node->next 会引发段错误。
		 * 应该额外用一个指针变量 next_enemy_node 提前记录后继节点的地址。
		 */
		Node* next_enemy_node = enemy_node->next;
		Object* enemy = (Object*)enemy_node->data;

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
	for (Node* bullet_node = bullet_list->head->next; bullet_node; ) {
		/**
		 * 创建指针变量 next_bullet_node 原因：
		 * 见函数 enemy_move() 中 next_enemy_node 定义处的注释。
		 */
		Node* next_bullet_node = bullet_node->next;
		Object* bullet = (Object*)bullet_node->data;

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
	for (Node* enemy_node = enemy_list->head->next; enemy_node; ) {
		/**
		 * 创建指针变量 next_enemy_node 原因：
		 * 见函数 enemy_move() 中 next_enemy_node 定义处的注释。
		 */
		Node* next_enemy_node = enemy_node->next;
		Object* enemy = (Object*)enemy_node->data;

		for (Node* bullet_node = bullet_list->head->next; bullet_node; ) {
			/**
			 * 创建指针变量 next_bullet_node 原因：
			 * 见函数 enemy_move() 中 next_enemy_node 定义处的注释。
			 */
			Node* next_bullet_node = bullet_node->next;
			Object* bullet = (Object*)bullet_node->data;

			if (object_collision(enemy, bullet)) {
				list_random_erase(enemy_list, enemy_node);
				list_random_erase(bullet_list, bullet_node);

				fprintf(stdout, "A bullet has been erased. (collision with enemy)\n");
				fprintf(stdout, "An enemy has been erased. (collision with bullet)\n");

				game_control_add_score(&game_control_data, POINTS_PER_HIT);

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
	for (Node* enemy_node = enemy_list->head->next; enemy_node; ) {
		/**
		 * 创建指针变量 next_enemy_node 原因：
		 * 见函数 enemy_move() 中 next_enemy_node 定义处的注释。
		 */
		Node* next_enemy_node = enemy_node->next;
		Object* enemy = (Object*)enemy_node->data;

		if (object_collision(enemy, player)) {
			list_random_erase(enemy_list, enemy_node);

			fprintf(stdout, "An enemy has been erased. (collision with player)\n");

			game_control_reduce_hp(&game_control_data, delta_hp[difficulty]);
		}

		enemy_node = next_enemy_node;
	}
}

/**
 * @brief 初始化游戏对象。
 */
void object_init() {
	player = (Object*)malloc(sizeof(Object));
	if (!player) {
		fprintf(stderr, "malloc() failed.\n");
		exit(EXIT_FAILURE);
	}

	player->x = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2;
	player->y = SCREEN_HEIGHT - PLAYER_HEIGHT - 100;
	player->type = PLAYER;

	enemy_list = list_init();
	bullet_list = list_init();

	last_bullet_spawn_time = last_enemy_spawn_time = clock();
}

/**
 * @brief 对当前帧，处理所有游戏对象的移动、添加、删除等操作。
 */
void object_update() {

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

/**
 * @brief 释放游戏对象的内存。
 */
void object_free() {
	list_free(enemy_list);
	list_free(bullet_list);
	enemy_list = bullet_list = NULL;

	if (player) {
		free(player);
	}
	player = NULL;
}
