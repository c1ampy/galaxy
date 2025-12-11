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

typedef struct PlayerVisual {
	float x;
	float y;
} PlayerVisual;

typedef struct EnemyVisual {
	float x;
	float y;
} EnemyVisual;

typedef struct BulletVisual {
	float x;
	float y;
	int from_player_or_enemy; // 子弹来自玩家还是敌人：玩家 = 1，敌人 = 0
} BulletVisual;

typedef struct GameplayVisualState {
	int width;
	int height;
	int score; // 在底部显示当前分数。
	int player_dead; // 如果玩家死了，在屏幕上显示 WASTED 并且显示死亡原因。
	const wchar_t *death_reason;
	PlayerVisual player;
	const EnemyVisual *enemies;
	size_t enemy_cnt;
	const BulletVisual *bullets;
	size_t bullet_cnt;
} GameplayVisualState;

/**
 * @brief 用来存放图片，如果加载图片没成功，也不会崩溃。
 */
typedef struct RenderTextures {
	IMAGE menu_background;
	IMAGE game_background;
	IMAGE player;
	IMAGE enemy;
	IMAGE player_bullet;
	IMAGE enemy_bullet;
	int   menu_background_ok;
	int   game_background_ok;
	int   player_ok;
	int   enemy_ok;
	int   player_bullet_ok;
	int   enemy_bullet_ok;
} RenderTextures;

extern RenderTextures g_renderTextures;

// 分别负责创建窗口，关闭窗口
void render_init(const int width, const int height, const wchar_t *title);
void render_shutdown();

// 加载资源
int render_load_menu_texture(const wchar_t *menu_background_path);
int render_load_gameplay_textures(
	const wchar_t *menu_background_path,
	const wchar_t *player_path,
	const wchar_t *enemy_path,
	const wchar_t *player_bullet_path,
	const wchar_t *enemy_bullet_path);

// 绘制菜单有关函数
static inline RECT menu_make_rect(const int x, const int y, const int w, const int h);
static inline int menu_hit_test(const Button *item, const int x, const int y);
static inline void menu_copy_label(wchar_t *dst, size_t cap, const wchar_t *src);
static inline void menu_draw_button(const Button *button);
static void menu_render_frame(const Button *buttons, const size_t button_count, const int width, const int height, const int high_score);

// 渲染菜单和游戏画面的接口

/**
 * @brief 渲染主菜单的主要接口。
 * @returns 返回被按下的按钮的 id：0 = 开始游戏，1 = 选项，2 = 退出
 */
int render_draw_main_menu(const int width, const int height, const int high_score, const int fps);

/**
 * @brief 渲染游戏画面的主要接口。
 */
void render_draw_current_frame(const GameplayVisualState *state);

// 处理纹理路径有关函数
const wchar_t * resolve_asset_path(const wchar_t *relative_path);
static inline int load_internal_texture(IMAGE *img, int *flag, const wchar_t *path);

#endif /* RENDER_H */

#ifdef __cplusplus
}
#endif /* __cplusplus */
