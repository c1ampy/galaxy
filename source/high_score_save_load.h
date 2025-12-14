/**
 * @file high_score_save_load.c
 * @brief 这份源文件实现了最高分保存、读取的函数。
 * @author 刘博闻
 * @date 2025-12-10
 * @version v1.0
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef HIGH_SCORE_SAVE_LOAD_H
#define HIGH_SCORE_SAVE_LOAD_H

#define DIFFICULTY_COUNT 3
#define HIGH_SCORE_FILE "high_score.dat"

	void high_score_save(const int high_score[DIFFICULTY_COUNT]);
	void high_score_load(int high_score[DIFFICULTY_COUNT]);

#endif /* HIGH_SCORE_SAVE_LOAD_H */

#ifdef __cplusplus
}
#endif /* __cplusplus */
