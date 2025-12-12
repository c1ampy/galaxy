/** 
 * @file object.h
 * @brief 这份源文件包含了游戏状态控制有关的一系列函数的实现。
 * @author 胡健成
 * @date 2025-12-10
 * @version v1.0
 */

#include "control.h"
#include <stdio.h>
#include <stdbool.h>

void game_init(GameControlData *game_control_data) {
	game_control_data->state = MENU;
	game_control_data->running = true;
}

void game_start(GameControlData *game_control_data, const int starting_hp) {
	if (game_control_data->state == PLAYING) {
		fprintf(stderr, "Attempting to start the game when the game is running.\n");
	}

	game_control_data->hp = starting_hp;
	game_control_data->score = 0;
	game_control_data->state = PLAYING;
	fprintf(stdout, "Game started.\n");
}

void game_pause(GameControlData *game_control_data) {
	if (game_control_data->state != PLAYING) {
		fprintf(stderr, "Attempting to pause the game when the game is not running.\n");
		return;
	}

	game_control_data->state = PAUSED;
	fprintf(stdout, "Game paused.\n");
}

void game_resume(GameControlData *game_control_data) {
	if (game_control_data->state != PAUSED) {
		fprintf(stderr, "Attempting to resume the game when the game hasn't been paused.\n");
		return;
	}

	game_control_data->state = PLAYING;
	fprintf(stdout, "Game resumed.\n");
}

void game_over(GameControlData *game_control_data) {
	game_control_data->state = GAMEOVER;
	fprintf(stdout, "Game over.\n");
	fprintf(stdout, "Final score: %d\n", game_control_data->score);
}

void add_score(GameControlData *game_control_data, const int points) {
	game_control_data->score += points;
	fprintf(stdout, "Score +%d\n", points);
	fprintf(stdout, "Total score now: %d\n", game_control_data->score);
}

/**
 * @brief 函数包含对生命值的扣除和对是否应该结束游戏的判断。
 */
void reduce_hp(GameControlData *game_control_data, const int delta) {
	if (game_control_data->hp <= 0) {
		fprintf(stderr, "Attempting to reduce hp when hp is not positive.\n");
		return;
	}

	game_control_data->hp -= delta;
	fprintf(stdout, "HP reduce by!\nCurrent HP: %d\n", game_control_data->hp);

	if (game_control_data->hp <= 0) {
		game_over(game_control_data);
	}
}
