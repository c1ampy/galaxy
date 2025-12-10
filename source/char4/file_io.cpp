#include "file_io.h"
#include <fstream>
#include <string>
#include <sstream>
#include <cstdio>

// 文件名常量
const std::string HIGH_SCORE_FILE = "high_score.dat";
const std::string CONFIG_FILE = "config.dat";
const std::string SAVE_FILE_PREFIX = "save_";
const std::string SAVE_FILE_SUFFIX = ".dat";

// 保存最高分
FileIOStatus file_io_save_high_score(uint32_t high_score) {
    std::ofstream file(HIGH_SCORE_FILE, std::ios::binary);
    if (!file.is_open()) {
        return FILE_IO_ERROR_OPEN;
    }

    file.write(reinterpret_cast<const char*>(&high_score), sizeof(uint32_t));
    file.close();

    return FILE_IO_SUCCESS;
}

// 读取最高分
FileIOStatus file_io_load_high_score(uint32_t* high_score) {
    if (high_score == nullptr) {
        return FILE_IO_ERROR_FORMAT;
    }

    std::ifstream file(HIGH_SCORE_FILE, std::ios::binary);
    if (!file.is_open()) {
        *high_score = 0; // 默认值
        return FILE_IO_SUCCESS;
    }

    file.read(reinterpret_cast<char*>(high_score), sizeof(uint32_t));
    file.close();

    if (file.gcount() != sizeof(uint32_t)) {
        *high_score = 0;
        return FILE_IO_ERROR_READ;
    }

    return FILE_IO_SUCCESS;
}

// 保存游戏配置
FileIOStatus file_io_save_config(const GameConfig* config) {
    if (config == nullptr) {
        return FILE_IO_ERROR_FORMAT;
    }

    std::ofstream file(CONFIG_FILE, std::ios::binary);
    if (!file.is_open()) {
        return FILE_IO_ERROR_OPEN;
    }

    file.write(reinterpret_cast<const char*>(config), sizeof(GameConfig));
    file.close();

    return FILE_IO_SUCCESS;
}

// 加载游戏配置
FileIOStatus file_io_load_config(GameConfig* config) {
    if (config == nullptr) {
        return FILE_IO_ERROR_FORMAT;
    }

    std::ifstream file(CONFIG_FILE, std::ios::binary);
    if (!file.is_open()) {
        file_io_get_default_config(config); // 使用默认配置
        return FILE_IO_SUCCESS;
    }

    file.read(reinterpret_cast<char*>(config), sizeof(GameConfig));
    file.close();

    if (file.gcount() != sizeof(GameConfig)) {
        file_io_get_default_config(config);
        return FILE_IO_ERROR_READ;
    }

    return FILE_IO_SUCCESS;
}

// 保存游戏存档
FileIOStatus file_io_save_game(const char* save_name, const GameSave* save_data) {
    if (save_name == nullptr || save_data == nullptr) {
        return FILE_IO_ERROR_FORMAT;
    }

    // 构建存档文件名
    std::stringstream ss;
    ss << SAVE_FILE_PREFIX << save_name << SAVE_FILE_SUFFIX;
    std::string filename = ss.str();

    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return FILE_IO_ERROR_OPEN;
    }

    file.write(reinterpret_cast<const char*>(save_data), sizeof(GameSave));
    file.close();

    return FILE_IO_SUCCESS;
}

// 加载游戏存档
FileIOStatus file_io_load_game(const char* save_name, GameSave* save_data) {
    if (save_name == nullptr || save_data == nullptr) {
        return FILE_IO_ERROR_FORMAT;
    }

    // 构建存档文件名
    std::stringstream ss;
    ss << SAVE_FILE_PREFIX << save_name << SAVE_FILE_SUFFIX;
    std::string filename = ss.str();

    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return FILE_IO_ERROR_OPEN;
    }

    file.read(reinterpret_cast<char*>(save_data), sizeof(GameSave));
    file.close();

    return (file.gcount() == sizeof(GameSave)) ? FILE_IO_SUCCESS : FILE_IO_ERROR_READ;
}

// 删除游戏存档
FileIOStatus file_io_delete_game(const char* save_name) {
    if (save_name == nullptr) {
        return FILE_IO_ERROR_FORMAT;
    }

    // 构建存档文件名
    std::stringstream ss;
    ss << SAVE_FILE_PREFIX << save_name << SAVE_FILE_SUFFIX;
    std::string filename = ss.str();

    // 尝试删除文件
    if (std::remove(filename.c_str()) != 0) {
        return FILE_IO_ERROR_OPEN; // 文件不存在或删除失败
    }

    return FILE_IO_SUCCESS;
}

// 获取默认游戏配置
void file_io_get_default_config(GameConfig* config) {
    if (config != nullptr) {
        config->difficulty = 1;      // 默认简单难度
        config->sound_volume = 80;   // 默认声音音量80%
        config->music_volume = 70;   // 默认音乐音量70%
        config->show_fps = 0;        // 默认不显示FPS
    }
}