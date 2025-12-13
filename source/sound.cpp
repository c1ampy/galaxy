//音效控制
#include "sound.h"
#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>

//链接windows多媒体库
#pragma comment(lib, "winmm.lib")

static const char* systemSounds[SOUND_COUNT] = {
    "SystemExclamation",   // 射击音效
    "SystemHand",          // 爆炸音效
    "SystemAsterisk",      // 击中音效
    "SystemQuestion",      // 道具音效
    "SystemExit",          // 游戏结束
    "SystemStart"          // 菜单音效
};

//初始化音效系统
static int initSound1 = 0;
int initSound(void)
{
    printf("[音效] 正在初始化音效系统...\n");
    //waveOutGetNumDevs返回可用的音频数目
    int deviceCount = waveOutGetNumDevs();
    //判断是否有音频可用
    if(deviceCount == 0){
        printf("[警告] 未检测到音频输出设备\n");
        printf("       可能是电脑没有声卡，或声卡驱动未安装\n");
        printf("       游戏将继续运行，但是没有声音\n");
        //标记为未初始化
        initSound1 = 0;
        //返回0表示初始化失败
        return 0;
    }
    //初始化成功
    // 标记为已初始化
    initSound1 = 1;

    printf("[音效] 系统音效初始化成功！\n");
    printf("       • 使用Windows内置音效（无需外部文件）\n");
    printf("       • 异步播放，游戏不会卡顿\n");
    printf("       • 检测到 %d 个音频设备\n", deviceCount);
    //返回1表示初始化成功
    return 1;
}

void playSound(SoundType sound)
{
    //检查是否已初始化
    if(!initSound1){
        return;
    }
    //检查音效是否有效
    if(sound<0 || sound>=SOUND_COUNT){
        printf("[音效错误] 无效的音效类型:%d\n",sound);
        printf("          有效的范围:0到%d\n",SOUND_COUNT - 1);
        return;
    }
    
    PlaySound(systemSounds[sound],
              NULL,
        //播放的标志
              SND_ASYNC | SND_ALIAS | SND_NODEFAULT);
}

void stopAllSounds(void)
{
    //检查是否初始化
    if(initSound1){
        PlaySound(
            NULL,//停止播放
            NULL,//不用
            0
        );
    }
}


//作用:清理音效资源
void cleanupSound(void)
{
    //停止播放音效
    stopAllSounds();
    //初始化
    initSound1 = 0;

    printf("[音效] 音效系统资源已清理\n");
    
}