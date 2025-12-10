#ifndef ITEM_SYSTEM_H
#define ITEM_SYSTEM_H

#include <stdint.h>

// 道具类型枚举
typedef enum {
    ITEM_NONE = 0,
    ITEM_LIFE = 1,          // 生命恢复
    ITEM_WEAPON_UPGRADE = 2, // 武器升级
    ITEM_SHIELD = 3,        // 护盾
    ITEM_BOMB = 4,          // 炸弹
    ITEM_COIN_DOUBLE = 5,   // 金币翻倍
    ITEM_SPEED_UP = 6,      // 速度提升
    ITEM_INVINCIBLE = 7,    // 无敌状态
    ITEM_MULTI_SHOT = 8,    // 多重射击
    ITEM_HOMING_MISSILE = 9,// 追踪导弹
    ITEM_MAX_TYPES          // 道具类型总数
} ItemType;

// 道具属性结构
typedef struct {
    ItemType type;          // 道具类型
    uint32_t effect_value;  // 效果值
    uint32_t duration;      // 持续时间（帧数）
    uint32_t cooldown;      // 冷却时间（帧数）
    uint8_t  max_stack;     // 最大堆叠数量
} ItemInfo;

// 道具效果状态
typedef struct {
    ItemType type;          // 道具类型
    uint32_t remaining_time; // 剩余持续时间
    uint32_t remaining_cooldown; // 剩余冷却时间
    uint8_t  stack_count;   // 当前堆叠数量
} ItemEffect;

// 道具系统初始化
void item_system_init(void);

// 获取道具信息
const ItemInfo* item_system_get_item_info(ItemType type);

// 获取道具数量
uint8_t item_system_get_item_count(ItemType type);

// 添加道具
void item_system_add_item(ItemType type, uint8_t count);

// 使用道具
uint8_t item_system_use_item(ItemType type);

// 更新道具效果
void item_system_update(void);

// 检查道具效果是否激活
uint8_t item_system_is_effect_active(ItemType type);

// 获取道具效果剩余时间
uint32_t item_system_get_effect_remaining_time(ItemType type);

// 重置道具系统
void item_system_reset(void);

// 获取所有激活的道具效果
void item_system_get_active_effects(ItemEffect effects[], uint8_t* count);

#endif // ITEM_SYSTEM_H#pragma once
