/** 
 * @file object.h
 * @brief 这份头文件声明了游戏音效的类型和一系列有关函数。
 * @author 胡健成
 * @date 2025-12-10
 * @version v1.0
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef SOUND_H
#define SOUND_H

typedef enum {
	SHOOT,
	EXPLODE,
	HIT,
	POWERUP,
	GAMEOVER,
	MENU
} SoundType;

/**
 * @brief 初始化音效系统。
 * 
 * 基于 Beep() 函数的简易音效无需初始化。
 */
void sound_init(void);

/**
 * @brief 基于 Beep() 函数，播放简易音效。
 * 
 * 射击：短高音
 * 爆炸：低长音
 * 击中：中音
 * 游戏结束：悲伤的音阶
 */
void sound_play(const SoundType sound);

/**
 * @brief 停止所有正在播放的音效。
 * 
 * 对于 .wav 格式的外部音效有作用。
 * 由于 Beep() 函数无法中途停止，故目前该函数无效。
 */
void sound_stop();

/**
 * @brief 清理音效资源。
 * 
 * 基于 Beep() 函数的简易音效无需清理资源。
 */
void sound_source_clear();

#endif /* SOUND_H */

#ifdef __cplusplus
}
#endif /* __cplusplus */
