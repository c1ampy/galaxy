#ifndef SCORE_H
#define SCORE_H

#include <stdint.h>

#define MAX_RANKINGS 10  // 排行榜最大记录数

// 排行榜条目结构
typedef struct {
    char name[20];      // 玩家名称
    uint32_t score;     // 分数
} RankingEntry;

// 分数系统初始化
void score_init(void);

// 获取当前分数
uint32_t score_get_current(void);

// 增加分数
void score_add(uint32_t points);

// 重置分数
void score_reset(void);

// 获取排行榜
void score_get_rankings(RankingEntry rankings[], uint8_t* count);

// 更新排行榜（添加新分数）
void score_update_ranking(const char* name, uint32_t score);

// 保存排行榜到文件
void score_save_rankings(void);

// 从文件加载排行榜
void score_load_rankings(void);

#endif // SCORE_H#pragma once
