//音效控制头文件
#ifndef SOUND_H
#define SOUND_H

//枚举六种主要音效类型（待定）
typedef enum {
    SOUND_SHOOT = 0,      //射击音效
    SOUND_EXPLODE = 1,    //爆炸音效
    SOUND_HIT = 2,        //击中音效
    SOUND_POWERUP = 3,    //道具音效
    SOUND_GAMEOVER = 4,   //游戏结束音效
    SOUND_MENU = 5,        //菜单音效
    SOUND_COUNT           //音效的数目
} SoundType;

//函数声明

//初始化音效系统
int initSound(void);

//播放指定音效
void playSound(SoundType sound);

//停止所有音效
void stopAllSounds(void);

//清理音效资源
void cleanupSound(void);

#endif