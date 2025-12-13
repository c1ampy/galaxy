/**
 * @file render.h
 * @brief 这份头文件声明了渲染飞机大战游戏画面的有关函数。
 * @author 李牧远
 * @date 2025-12-11
 * @version v1.0
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
#endif /* __cplusplus */

#ifndef RENDER_H
#define RENDER_H

typedef struct Button {
	wchar_t text[64];
	RECT rect;
	int hovered;
} Button;

typedef struct GameplayVisualState {
	int width;
	int height;
	int score;
	const wchar_t *death_reason;
	const Object *player;
	const List *enemy_list;
	const List *bullet_list;
	int difficulty;
	int hp;
	int starting_hp;
} GameplayVisualState;

/**
 * @brief 用来存放图片，如果加载图片没成功，也不会崩溃。
 */
typedef struct RenderTextures {
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

/**
 * @brief 获取数字难度对应的文字。
 */ 
static const wchar_t* difficulty_to_text(const int difficulty);

// 分别负责创建窗口，关闭窗口
void window_create(const int width, const int height, const wchar_t *title);
void window_close();

// 加载资源
int render_load_menu_texture(const wchar_t *menu_background_path);
int render_load_gameplay_textures(
	const wchar_t *game_background_path,
	const wchar_t *player_path,
	const wchar_t *enemy_path,
	const wchar_t *bullet_path);

// 绘制菜单有关函数
static inline RECT menu_make_rect(const int x, const int y, const int w, const int h);
static inline int menu_hit_test(const Button *item, const int x, const int y);
static inline void menu_copy_label(wchar_t *dst, size_t cap, const wchar_t *src);
static inline void menu_draw_button(const Button *button);
static void menu_render_frame(const Button* buttons, const size_t button_count, const int width, const int height, const int high_scores[3], const int difficulty);

// 渲染菜单和游戏画面的接口

/**
 * @brief 渲染主菜单。
 * @return 0 = 开始游戏，1 = 选择难度，2 = 退出。
 */
int render_draw_main_menu(const int width, const int height, const int high_scores[3], const int difficulty, const int fps);

/**
 * @brief 渲染游戏画面的主要接口。
 */
void render_draw_current_frame(const GameplayVisualState *state);

/**
 * @brief WASTED 页面。
 * @return 0 = 重新开始，1 = 返回主菜单，2 = 退出游戏。
 */
int render_draw_wasted_page(const GameplayVisualState *state, const int high_scores[3], const int fps);

// 处理纹理路径有关函数
const wchar_t * resolve_asset_path(const wchar_t *relative_path);
static inline int load_internal_texture(IMAGE *img, int *flag, const wchar_t *path);

#endif /* RENDER_H */

#ifdef __cplusplus
}
#endif /* __cplusplus */
