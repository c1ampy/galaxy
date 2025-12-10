//游戏控制头文件
#ifndef CONTROL_H
#define CONTROL_H

#define SCORE_PER_LEVEL 1000

//枚举游戏状态
typedef enum {
	STATE_MENU = 0,      //菜单状态
	STATE_PLAYING = 1,   //游戏进行
	STATE_PAUSED = 2,    //游戏暂停
	STATE_GAMEOVER = 3   //游戏结束
} GameState;

//游戏控制结构体
typedef struct {
	GameState state;     //当前游戏状态
	int score;           //当前得分
	int lives;           //剩余生命
	int level;           //当前关卡
	int isRunning;       //游戏是否在运行
} GameControl;

//函数声明

//初始化游戏控制
void initGameControl(GameControl* control);

//暂停游戏
void pauseGame(GameControl* control);

//继续游戏
void resumeGame(GameControl* control);

//游戏结束
void gameOver(GameControl* control);

//增加分数
void addScore(GameControl* control, int points);

//减少生命
void loseLife(GameControl* control);

//检查游戏是否应该结束
int shouldGameEnd(GameControl* control);

#endif /* CONTROL_H */