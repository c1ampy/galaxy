// 游戏控制
#include "control.h"
#include "sound.h"  // 需要播放音效
#include <stdio.h>


//初始化游戏控制状态
void initGameControl(GameControl* control) {
	control->state = STATE_MENU;  // 初始状态为菜单
	control->score = 0;           // 初始分数为 0
	control->lives = 3;           // 初始 3 条命（待定）
	control->level = 1;           // 第 1 关（关卡待定）
	control->isRunning = 1;       // 游戏正在运行
}


// 暂停游戏
// 只有在游戏进行中才能暂停
void pauseGame(GameControl* control) {
	if (control->state == STATE_PLAYING) {
		control->state = STATE_PAUSED;
		fprintf(stdout, "Game paused.\n");
		playSound(SOUND_MENU); // 播放暂停音效
	}
}


// 继续游戏
// 只有暂停状态才能继续
void resumeGame(GameControl* control) {
	if (control->state == STATE_PAUSED) {
		control->state = STATE_PLAYING;
		fprintf(stdout, "Game continuing.\n");
		playSound(SOUND_MENU); // 播放继续音效
	}
}


// 游戏结束
void gameOver(GameControl* control) {
	control->state = STATE_GAMEOVER;
	control->isRunning = 0; // 停止游戏循环
	
	fprintf(stdout, "Game over.\n");
	fprintf(stdout, "Final score: %d\n", control->score);
	fprintf(stdout, "Level: %d\n", control->level);
	
	playSound(SOUND_GAMEOVER); // 播放游戏结束音效
}


// 增加分数，并检查是否升级
// 每 SCORE_PER_LEVEL 分升一级
void addScore(GameControl* control,int points) {
	int oldScore = control->score;
	control->score += points;
	
	fprintf(stdout, "Score +%d\n", points);
	fprintf(stdout, "Total score now: %d\n", control->score);

	// 检查是否升级（每 SCORE_PER_LEVEL 分升一级）
	if (oldScore / SCORE_PER_LEVEL < control->score / SCORE_PER_LEVEL) {
		++control->level;
		fprintf(stdout, "Level up!\nLevel now: %d\n", control->level);
		playSound(SOUND_POWERUP);  // 播放升级音效
	}
}


// 减少生命值，检查是否游戏结束
void loseLife(GameControl* control) {
	if (control -> lives > 0) {
		control -> lives--;
		fprintf(stdout, "HP lost!\nHP now: %d\n", control->lives);
		playSound(SOUND_HIT);  // 播放被击中音效
		
		// 检查是否游戏结束
		if (control->lives <= 0) {
			gameOver(control);
		}
	}
}

// 检查游戏是否应该结束
// 返回值：1 = 应该结束，0 = 继续游戏
int shouldGameEnd(GameControl* control) {
	// 游戏结束条件：生命值为 0 或主动停止
	if (control->lives <= 0 || !control->isRunning) {
		return 1;
	}
	return 0;
}
