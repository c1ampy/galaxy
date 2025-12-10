#include "item_system.h"
#include <cstring>

// 道具信息数组
static const ItemInfo item_info_list[ITEM_MAX_TYPES] = {
    {ITEM_NONE, 0, 0, 0, 0},                                    // 无效道具
    {ITEM_LIFE, 1, 0, 0, 5},                                    // 生命恢复：+1生命，无持续时间，无冷却，最大堆叠5个
    {ITEM_WEAPON_UPGRADE, 1, 0, 180, 3},                        // 武器升级：+1级，无持续时间，3秒冷却，最大堆叠3个
    {ITEM_SHIELD, 100, 600, 300, 1},                            // 护盾：100点护盾，10秒持续时间，5秒冷却，最大堆叠1个
    {ITEM_BOMB, 1, 0, 120, 5},                                  // 炸弹：1个炸弹，无持续时间，2秒冷却，最大堆叠5个
    {ITEM_COIN_DOUBLE, 2, 900, 1800, 1},                        // 金币翻倍：2倍金币，15秒持续时间，30秒冷却，最大堆叠1个
    {ITEM_SPEED_UP, 50, 600, 300, 1},                           // 速度提升：50%速度，10秒持续时间，5秒冷却，最大堆叠1个
    {ITEM_INVINCIBLE, 1, 300, 1800, 1},                         // 无敌状态：无敌，5秒持续时间，30秒冷却，最大堆叠1个
    {ITEM_MULTI_SHOT, 2, 900, 1800, 1},                         // 多重射击：2发子弹，15秒持续时间，30秒冷却，最大堆叠1个
    {ITEM_HOMING_MISSILE, 1, 600, 1200, 1}                      // 追踪导弹：1发追踪导弹，10秒持续时间，20秒冷却，最大堆叠1个
};

// 静态全局变量
static uint8_t item_counts[ITEM_MAX_TYPES] = { 0 };             // 道具数量
static ItemEffect active_effects[ITEM_MAX_TYPES] = {  };        // 激活的道具效果
static uint8_t active_effect_count = 0;                       // 激活的效果数量

// 道具系统初始化
void item_system_init(void) {
    std::memset(item_counts, 0, sizeof(item_counts));
    std::memset(active_effects, 0, sizeof(active_effects));
    active_effect_count = 0;
}

// 获取道具信息
const ItemInfo* item_system_get_item_info(ItemType type) {
    if (type >= ITEM_NONE && type < ITEM_MAX_TYPES) {
        return &item_info_list[type];
    }
    return nullptr;
}

// 获取道具数量
uint8_t item_system_get_item_count(ItemType type) {
    if (type >= ITEM_NONE && type < ITEM_MAX_TYPES) {
        return item_counts[type];
    }
    return 0;
}

// 添加道具
void item_system_add_item(ItemType type, uint8_t count) {
    if (type > ITEM_NONE && type < ITEM_MAX_TYPES) {
        const ItemInfo* info = &item_info_list[type];
        uint8_t new_count = item_counts[type] + count;
        // 限制最大堆叠数量
        if (new_count > info->max_stack) {
            new_count = info->max_stack;
        }
        item_counts[type] = new_count;
    }
}

// 检查道具是否可以使用
static uint8_t can_use_item(ItemType type) {
    const ItemInfo* info = &item_info_list[type];
    if (item_counts[type] == 0) {
        return 0; // 道具数量不足
    }

    // 检查冷却时间
    for (uint8_t i = 0; i < active_effect_count; i++) {
        if (active_effects[i].type == type) {
            if (active_effects[i].remaining_cooldown > 0) {
                return 0; // 冷却时间未结束
            }
            break;
        }
    }

    return 1;
}

// 添加效果到激活列表
static void add_effect(ItemType type, uint32_t duration) {
    // 检查是否已存在该效果
    for (uint8_t i = 0; i < active_effect_count; i++) {
        if (active_effects[i].type == type) {
            // 更新效果持续时间
            active_effects[i].remaining_time = duration;
            active_effects[i].stack_count = 1;
            return;
        }
    }

    // 添加新效果
    if (active_effect_count < ITEM_MAX_TYPES) {
        active_effects[active_effect_count].type = type;
        active_effects[active_effect_count].remaining_time = duration;
        active_effects[active_effect_count].remaining_cooldown = item_info_list[type].cooldown;
        active_effects[active_effect_count].stack_count = 1;
        active_effect_count++;
    }
}

// 使用道具
uint8_t item_system_use_item(ItemType type) {
    if (!can_use_item(type)) {
        return 0; // 无法使用道具
    }

    const ItemInfo* info = &item_info_list[type];

    // 减少道具数量
    item_counts[type]--;

    // 激活效果（如果有持续时间）
    if (info->duration > 0) {
        add_effect(type, info->duration);
    }

    return 1; // 道具使用成功
}

// 更新道具效果
void item_system_update(void) {
    // 更新激活效果
    for (uint8_t i = 0; i < active_effect_count; i++) {
        // 更新持续时间
        if (active_effects[i].remaining_time > 0) {
            active_effects[i].remaining_time--;
        }

        // 更新冷却时间
        if (active_effects[i].remaining_cooldown > 0) {
            active_effects[i].remaining_cooldown--;
        }

        // 移除已过期的效果
        if (active_effects[i].remaining_time == 0 && active_effects[i].remaining_cooldown == 0) {
            // 从数组中移除
            for (uint8_t j = i; j < active_effect_count - 1; j++) {
                active_effects[j] = active_effects[j + 1];
            }
            active_effect_count--;
            i--; // 调整索引
        }
    }
}

// 检查道具效果是否激活
uint8_t item_system_is_effect_active(ItemType type) {
    for (uint8_t i = 0; i < active_effect_count; i++) {
        if (active_effects[i].type == type && active_effects[i].remaining_time > 0) {
            return 1;
        }
    }
    return 0;
}

// 获取道具效果剩余时间
uint32_t item_system_get_effect_remaining_time(ItemType type) {
    for (uint8_t i = 0; i < active_effect_count; i++) {
        if (active_effects[i].type == type && active_effects[i].remaining_time > 0) {
            return active_effects[i].remaining_time;
        }
    }
    return 0;
}

// 重置道具系统
void item_system_reset(void) {
    std::memset(item_counts, 0, sizeof(item_counts));
    std::memset(active_effects, 0, sizeof(active_effects));
    active_effect_count = 0;
}

// 获取所有激活的道具效果
void item_system_get_active_effects(ItemEffect effects[], uint8_t* count) {
    if (effects == nullptr || count == nullptr) {
        return;
    }

    // 复制激活的效果到输出数组
    uint8_t valid_effects = 0;
    for (uint8_t i = 0; i < active_effect_count; i++) {
        if (active_effects[i].remaining_time > 0) {
            effects[valid_effects] = active_effects[i];
            valid_effects++;
        }
    }

    *count = valid_effects;
}