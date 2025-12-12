/** 
 * @file object.h
 * @brief 这份源文件基于 windos.h 的 Beep() 函数实现了音效播放函数。
 * @author 胡健成
 * @date 2025-12-10
 * @version v1.0
 */

#include "sound.h"
#include <windows.h>
#include <stdio.h>

/**
 * @brief 初始化音效系统。
 * 
 * 基于 Beep() 函数的简易音效无需初始化。
 */
void sound_init() {

}

/**
 * @brief 基于 Beep() 函数，播放简易音效。
 * 
 * 射击：短高音
 * 爆炸：低长音
 * 击中：中音
 * 游戏结束：悲伤的音阶
 */
void sound_play(const SoundType sound) {

	switch(sound) {
	case SHOOT:
		Beep(800, 100);
		break;
		
	case EXPLODE:
		Beep(200, 300);
		break;
		
	case HIT:
		Beep(500, 150);
		break;
		
	case POWERUP:
		Beep(600, 100);
		Beep(800, 100);
		break;
		
	case GAMEOVER:
		Beep(400, 300);
		Sleep(50);
		Beep(300, 300);
		Sleep(50);
		Beep(200, 400);
		break;
		
	case MENU:
		Beep(600, 50);
		break;
		
	default:
		break;
	}
}

/**
 * @brief 停止所有正在播放的音效。
 * 
 * 对于 .wav 格式的外部音效有作用。
 * 由于 Beep() 函数无法中途停止，故目前该函数无效。
 */
void sound_stop() {

}

/**
 * @brief 清理音效资源。
 * 
 * 基于 Beep() 函数的简易音效无需清理资源。
 */
void sound_source_clear() {

}
