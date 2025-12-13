/**
 * @file render.h
 * @brief 这份头文件声明了渲染飞机大战游戏画面的有关函数。
 */

#pragma once
#include <windows.h>
#include <graphics.h>
#include <stddef.h>
#include <wchar.h>
#include <io.h>
#include "list.h"
#include "object.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef RENDER_H
#define RENDER_H

    enum RenderDifficulty
    {
        DIFFICULTY_EASY = 0,
        DIFFICULTY_NORMAL = 1,
        DIFFICULTY_HARD = 2
    };

    typedef struct Button
    {
        wchar_t text[64];
        RECT    rect;
        int     hovered;
    } Button;

    typedef struct GameplayVisualState
    {
        int width;
        int height;
        int score;
        int player_dead;
        const wchar_t* death_reason;
        const Object* player;
        const List* enemy_list;
        const List* bullet_list;

        int difficulty; // DIFFICULTY_*
        int hp;
        int max_hp;
    } GameplayVisualState;

    typedef struct RenderTextures
    {
        IMAGE menu_background;
        IMAGE game_background;
        IMAGE player;
        IMAGE enemy;
        IMAGE bullet;

        int menu_background_ok;
        int game_background_ok;
        int player_ok;
        int enemy_ok;
        int bullet_ok;
    } RenderTextures;

    extern RenderTextures g_render_textures;

    void window_create(int width, int height, const wchar_t* title);
    void window_close(void);

    int render_load_menu_texture(const wchar_t* menu_background_path);
    int render_load_gameplay_textures(
        const wchar_t* game_background_path,
        const wchar_t* player_path,
        const wchar_t* enemy_path,
        const wchar_t* bullet_path);

    /**
     * @brief 渲染主菜单。
     * @param high_scores 大小为 3 的数组，对应三个难度的最高分。
     * @param current_difficulty 当前选中难度，供 UI 显示。
     * @return 0=开始游戏，1=选择难度，2=退出。
     */
    int render_draw_main_menu(
        int width,
        int height,
        const int high_scores[3],
        int current_difficulty,
        int fps);

    /**
     * @brief 难度选择界面。
     * @return 返回 0/1/2（对应 DIFFICULTY_*），取消则返回 current_difficulty。
     */
    int render_draw_difficulty_menu(
        int width,
        int height,
        int current_difficulty,
        int fps);

    /**
     * @brief 暂停菜单。
     * @return 0=返回游戏，1=重新开始，2=返回主菜单，3=退出游戏。
     */
    int render_draw_pause_menu(
        int width,
        int height,
        const GameplayVisualState* state,
        int fps);

    /**
     * @brief WASTED 页面。
     * @return 0=重新开始，1=返回主菜单，2=退出游戏。
     */
    int render_draw_wasted_page(
        const GameplayVisualState* state,
        const int high_scores[3],
        int fps);

    /**
     * @brief 渲染正常游戏画面（不含 WASTED/Pause）。
     */
    void render_draw_current_frame(const GameplayVisualState* state);

    const wchar_t* resolve_asset_path(const wchar_t* relative_path);

#endif /* RENDER_H */

#ifdef __cplusplus
}
#endif