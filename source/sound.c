//音效控制
#include "sound.h"
#include <windows.h>
#include <stdio.h>


//作用:初始化音效系统
//Windows控制台使用Beep函数播放简单音效
void initSound(void)
{
    // Windows的Beep函数不需要初始化
    printf("[音效] 音效系统已初始化(使用Windows Beep)\n");
}


//播放指定类型的音效
//参数sound代表要播放的音效类型
/*音效设计:
//  射击:短高音
//  爆炸:低长音
//  击中:中音
    游戏结束:悲伤的音阶*/

void playSound(SoundType sound)
{
    switch(sound) {
        case SOUND_SHOOT:    //射击音效
            Beep(800,100);  //800Hz,100ms
            break;
            
        case SOUND_EXPLODE:  //爆炸音效
            Beep(200,300);  //200Hz,300ms
            break;
            
        case SOUND_HIT:      //击中音效
            Beep(500,150);  //500Hz,150ms
            break;
            
        case SOUND_POWERUP:  //道具音效
            Beep(600,100);
            Beep(800,100);
            break;
            
        case SOUND_GAMEOVER: //游戏结束音效
            Beep(400,300);
            Sleep(50);
            Beep(300,300);
            Sleep(50);
            Beep(200,400);
            break;
            
        case SOUND_MENU:     //菜单音效
            Beep(600,50);
            break;
            
        default:
            break;  //未知音效类型,不播放
    }
}


//该函数功能是停止所有正在播放的音效
//但前面使用的Beep函数无法中途停止,这个函数主要是预留接口
void stopAllSounds(void)
{
    //在替换成其他音效库时,发挥作用
    //Beep函数似乎会卡顿,后续可改
}


//作用:清理音效资源
void cleanupSound(void)
{
    printf("[音效] 音效系统资源已清理\n");
    //若使用Beep不需要清理
}