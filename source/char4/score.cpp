#define _CRT_SECURE_NO_WARNINGS
#include "score.h"
#include <cstring>
#include <fstream>
#include <algorithm>

// 静态全局变量
static uint32_t current_score = 0;
static RankingEntry rankings[MAX_RANKINGS];
static uint8_t ranking_count = 0;

// 分数系统初始化
void score_init(void) {
    current_score = 0;
    ranking_count = 0;
    std::memset(rankings, 0, sizeof(rankings));
    score_load_rankings(); // 从文件加载排行榜
}

// 获取当前分数
uint32_t score_get_current(void) {
    return current_score;
}

// 增加分数
void score_add(uint32_t points) {
    current_score += points;
}

// 重置分数
void score_reset(void) {
    current_score = 0;
}

// 获取排行榜
void score_get_rankings(RankingEntry rankings[], uint8_t* count) {
    if (rankings != nullptr && count != nullptr) {
        std::memcpy(rankings, ::rankings, sizeof(RankingEntry) * ranking_count);
        *count = ranking_count;
    }
}

// 比较函数，用于排行榜排序
static bool compare_rankings(const RankingEntry& entry1, const RankingEntry& entry2) {
    // 按分数降序排列
    return entry1.score > entry2.score;
}

// 更新排行榜（添加新分数）
void score_update_ranking(const char* name, uint32_t score) {
    // 检查是否可以添加到排行榜
    if (ranking_count < MAX_RANKINGS || score > rankings[MAX_RANKINGS - 1].score) {
        // 添加新条目
        if (ranking_count < MAX_RANKINGS) {
            ranking_count++;
        }

        // 复制名称和分数
        std::strncpy(rankings[ranking_count - 1].name, name, sizeof(rankings[0].name) - 1);
        rankings[ranking_count - 1].name[sizeof(rankings[0].name) - 1] = '\0';
        rankings[ranking_count - 1].score = score;

        // 排序排行榜（使用C++的sort函数）
        std::sort(rankings, rankings + ranking_count, compare_rankings);

        // 保存到文件
        score_save_rankings();
    }
}

// 保存排行榜到文件
void score_save_rankings(void) {
    std::ofstream file("rankings.dat", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(&ranking_count), sizeof(uint8_t));
        file.write(reinterpret_cast<const char*>(rankings), sizeof(RankingEntry) * ranking_count);
        file.close();
    }
}

// 从文件加载排行榜
void score_load_rankings(void) {
    std::ifstream file("rankings.dat", std::ios::binary);
    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(&ranking_count), sizeof(uint8_t));
        if (ranking_count > MAX_RANKINGS) {
            ranking_count = MAX_RANKINGS;
        }
        file.read(reinterpret_cast<char*>(rankings), sizeof(RankingEntry) * ranking_count);
        file.close();
    }
}