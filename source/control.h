/**
 * @file object.h
 * @brief 这份头文件声明了有关游戏状态控制的一系列对象以及函数。
 * @author 胡健成
 * @date 2025-12-10
 * @version v1.0
 */

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef CONTROL_H
#define CONTROL_H

	/**
	 * @brief 当前的游戏状态，包含菜单、游戏中、设置、暂停、结束五种。
	 */
	typedef enum {
		MENU,
		SETTINGS,
		PLAYING,
		PAUSED,
		GAMEOVER
	} GameState;

	/**
	 * @brief 有关游戏状态控制的一系列参数。
	 */
	typedef struct {
		GameState state;
		int score;
		int hp;
		bool running; // 指程序是否运行，而非实际游戏是否运行。
	} GameControlData;

	// 有关游戏状态控制的一系列函数。

	void game_control_to_menu(GameControlData* game_control_data);
	void game_control_to_settings(GameControlData* game_control_data);
	void game_control_start(GameControlData* game_control_data, const int starting_hp);
	void game_control_pause(GameControlData* game_control_data);
	void game_control_resume(GameControlData* game_control_data);
	void game_control_end(GameControlData* game_control_data);
	void game_control_add_score(GameControlData* game_control_data, const int points);
	void game_control_reduce_hp(GameControlData* game_control_data, const int delta);

#endif /* CONTROL_H */

#ifdef __cplusplus
}
#endif /* __cplusplus */
