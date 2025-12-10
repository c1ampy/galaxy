#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdint.h>

// 游戏配置结构
typedef struct {
    uint8_t difficulty;      // 难度级别（1-简单, 2-中等, 3-困难）
    uint8_t sound_volume;    // 声音音量（0-100）
    uint8_t music_volume;    // 音乐音量（0-100）
    uint8_t show_fps;        // 是否显示FPS（0-不显示, 1-显示）
} GameConfig;

// 游戏存档结构
typedef struct {
    uint32_t level;          // 当前关卡
    uint32_t score;          // 当前分数
    uint32_t lives;          // 剩余生命数
    uint32_t power;          // 武器威力
    uint32_t coins;          // 金币数量
    uint8_t  items[10];      // 道具数量
} GameSave;

// 文件操作结果枚举
typedef enum {
    FILE_IO_SUCCESS = 0,
    FILE_IO_ERROR_OPEN,
    FILE_IO_ERROR_READ,
    FILE_IO_ERROR_WRITE,
    FILE_IO_ERROR_FORMAT
} FileIOStatus;

// 保存最高分
FileIOStatus file_io_save_high_score(uint32_t high_score);

// 读取最高分
FileIOStatus file_io_load_high_score(uint32_t* high_score);

// 保存游戏配置
FileIOStatus file_io_save_config(const GameConfig* config);

// 加载游戏配置
FileIOStatus file_io_load_config(GameConfig* config);

// 保存游戏存档
FileIOStatus file_io_save_game(const char* save_name, const GameSave* save_data);

// 加载游戏存档
FileIOStatus file_io_load_game(const char* save_name, GameSave* save_data);

// 删除游戏存档
FileIOStatus file_io_delete_game(const char* save_name);

// 获取默认游戏配置
void file_io_get_default_config(GameConfig* config);

#endif // FILE_IO_Hgma once
