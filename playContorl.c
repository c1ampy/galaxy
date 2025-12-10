//游戏控制
#include "control.h"
#include "sound.h"  //需要播放音效
#include <stdio.h>


//初始化游戏控制状态
void initGameControl(GameControl* control)
{
    control->state = STATE_MENU;  //初始状态为菜单
    control->score = 0;           //初始分数为0
    control->lives = 3;           //初始3(待定)条命
    control->level = 1;           //第1关(关卡待定)
    control->isRunning = 1;       //游戏正在运行
}


//暂停游戏
//只有在游戏进行中才能暂停
void pauseGame(GameControl* control)
{
    if (control->state == STATE_PLAYING) {
        control->state = STATE_PAUSED;
        printf("[游戏] 游戏已暂停\n");
        playSound(SOUND_MENU);  //播放暂停音效
    }
}


//继续游戏
//只有暂停状态才能继续
void resumeGame(GameControl* control)
{
    if (control->state == STATE_PAUSED) {
        control->state = STATE_PLAYING;
        printf("[游戏] 游戏继续\n");
        playSound(SOUND_MENU);  //播放继续音效
    }
}


//游戏结束
void gameOver(GameControl* control)
{
    control->state = STATE_GAMEOVER;
    control->isRunning = 0;  //停止游戏循环
    
    printf("[游戏] 游戏结束！\n");
    printf("[游戏] 最终得分：%d\n",control->score);
    printf("[游戏] 达到关卡：%d\n",control->level);
    
    playSound(SOUND_GAMEOVER);  //播放游戏结束音效
}


//增加分数,并检查是否升级
//每1000分升一级
void addScore(GameControl* control,int points)
{
    int oldScore = control->score;
    control->score += points;
    
    printf("[游戏] 得分 +%d！当前总分：%d\n",points,control->score);
    
    //检查是否升级(每1000分升一级)
    if (oldScore / 1000 < control->score / 1000) {
        control->level++;
        printf("[游戏] 恭喜升级！当前等级：%d\n",control->level);
        playSound(SOUND_POWERUP);  //播放升级音效
    }
}


//减少生命值，检查是否游戏结束
void loseLife(GameControl* control)
{
    if (control -> lives > 0) {
        control -> lives--;
        printf("[游戏] 失去一条生命！剩余生命：%d\n", control -> lives);
        playSound(SOUND_HIT);  //播放被击中音效
        
        //检查是否游戏结束
        if (control->lives <= 0) {
            gameOver(control);
        }
    }
}

//检查游戏是否应该结束
//返回值:1=应该结束,0=继续游戏
int shouldGameEnd(GameControl* control)
{
    //游戏结束条件:生命值为0或主动停止
    if (control->lives <= 0 || !control->isRunning) {
        return 1;
    }
    return 0;
}