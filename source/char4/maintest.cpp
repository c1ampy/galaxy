#include "score.h"
#include "file_io.h"
#include "item_system.h"
#include <iostream>
#include <string>

int main() {
    std::cout << "=== 飞机大战数据管理系统测试 ===\n" << std::endl;

    // 1. 初始化所有系统
    std::cout << "1. 初始化系统...\n";
    score_init();
    item_system_init();
    std::cout << "系统初始化完成！\n" << std::endl;

    // 2. 测试分数系统
    std::cout << "2. 测试分数系统：\n";
    std::cout << "初始分数: " << score_get_current() << std::endl;

    score_add(100);
    std::cout << "增加100分后: " << score_get_current() << std::endl;

    score_add(250);
    std::cout << "再增加250分后: " << score_get_current() << std::endl;

    score_add(500);
    std::cout << "再增加500分后: " << score_get_current() << std::endl;

    score_reset();
    std::cout << "重置后分数: " << score_get_current() << std::endl;

    // 3. 测试排行榜功能
    std::cout << "\n3. 测试排行榜功能：\n";

    // 添加测试分数
    score_update_ranking("Player1", 1500);
    score_update_ranking("Player2", 2500);
    score_update_ranking("Player3", 1000);
    score_update_ranking("Player4", 3000);
    score_update_ranking("Player5", 2000);

    // 获取并打印排行榜
    RankingEntry rankings[MAX_RANKINGS];
    uint8_t count = 0;
    score_get_rankings(rankings, &count);

    std::cout << "排行榜 (" << static_cast<int>(count) << " 条记录):\n";
    for (uint8_t i = 0; i < count; i++) {
        std::cout << i + 1 << ". " << rankings[i].name << " - " << rankings[i].score << std::endl;
    }

    // 4. 测试道具系统
    std::cout << "\n4. 测试道具系统：\n";

    // 初始化道具数量
    item_system_add_item(ITEM_LIFE, 3);
    item_system_add_item(ITEM_BOMB, 2);

    std::cout << "道具数量：\n";
    std::cout << "生命道具: " << static_cast<int>(item_system_get_item_count(ITEM_LIFE)) << std::endl;
    std::cout << "炸弹道具: " << static_cast<int>(item_system_get_item_count(ITEM_BOMB)) << std::endl;

    // 使用道具
    std::cout << "\n使用道具测试：\n";
    if (item_system_use_item(ITEM_LIFE)) {
        std::cout << "成功使用生命道具！" << std::endl;
    }
    else {
        std::cout << "无法使用生命道具！" << std::endl;
    }

    std::cout << "剩余生命道具: " << static_cast<int>(item_system_get_item_count(ITEM_LIFE)) << std::endl;

    // 测试道具效果
    std::cout << "\n道具效果测试：\n";
    item_system_add_item(ITEM_SHIELD, 1);
    if (item_system_use_item(ITEM_SHIELD)) {
        std::cout << "成功使用护盾道具！" << std::endl;
        std::cout << "护盾效果剩余时间: " << item_system_get_effect_remaining_time(ITEM_SHIELD) << " 帧" << std::endl;
    }

    // 5. 测试文件IO系统
    std::cout << "\n5. 测试文件IO系统：\n";

    // 测试保存最高分
    uint32_t high_score = 5000;
    FileIOStatus status = file_io_save_high_score(high_score);
    std::cout << "保存最高分状态: " << (status == FILE_IO_SUCCESS ? "成功" : "失败") << std::endl;

    // 测试读取最高分
    uint32_t loaded_high_score = 0;
    status = file_io_load_high_score(&loaded_high_score);
    std::cout << "读取最高分状态: " << (status == FILE_IO_SUCCESS ? "成功" : "失败") << std::endl;
    std::cout << "读取到的最高分: " << loaded_high_score << std::endl;

    // 测试游戏配置
    GameConfig config;
    file_io_get_default_config(&config);
    std::cout << "默认配置：难度=" << static_cast<int>(config.difficulty)
        << ", 音量=" << static_cast<int>(config.sound_volume) << "%" << std::endl;

    // 6. 测试保存游戏配置
    config.difficulty = 2;
    config.sound_volume = 90;
    status = file_io_save_config(&config);
    std::cout << "保存配置状态: " << (status == FILE_IO_SUCCESS ? "成功" : "失败") << std::endl;

    // 7. 测试读取游戏配置
    GameConfig loaded_config;
    status = file_io_load_config(&loaded_config);
    std::cout << "读取配置状态: " << (status == FILE_IO_SUCCESS ? "成功" : "失败") << std::endl;
    std::cout << "读取到的配置：难度=" << static_cast<int>(loaded_config.difficulty)
        << ", 音量=" << static_cast<int>(loaded_config.sound_volume) << "%" << std::endl;

    // 8. 重置所有系统
    std::cout << "\n6. 重置所有系统...\n";
    score_reset();
    item_system_reset();
    std::cout << "系统重置完成！\n" << std::endl;

    std::cout << "=== 测试完成！所有系统运行正常！===\n" << std::endl;

    return 0;
}