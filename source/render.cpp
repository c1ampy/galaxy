/**
 * @file render.cpp
 * @brief 这份源文件实现了渲染飞机大战游戏画面的有关函数。
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
#include "render.h"

// 在一个编译单元里定义纹理实例，其他文件只会使用 extern 声明。
RenderTextures g_render_textures = { 0 };

/**
 * @brief 创建 EasyX 窗口
 */
void window_create(const int width, const int height, const wchar_t *title) {
	initgraph(width, height);
	setbkmode(TRANSPARENT);

	if (title != NULL && title[0] != L'\0') {
		SetWindowTextW(GetHWnd(), title);
	}
}

/**
 * @brief 关闭 EasyX 窗口
 */
void window_close() {
	closegraph();
}

int render_load_menu_texture(const wchar_t *menu_background_path) {
	// 如果没有菜单背景图片就用纯色的窗口
	return load_internal_texture(&g_render_textures.menu_background,
		&g_render_textures.menu_background_ok,
		menu_background_path);
}

int render_load_gameplay_textures(
	const wchar_t *game_background_path,
	const wchar_t *player_path,
	const wchar_t *enemy_path,
	const wchar_t *bullet_path) {
	
	// 游戏内所有贴图的批量加载
	int ok1 = load_internal_texture(&g_render_textures.game_background, &g_render_textures.game_background_ok, game_background_path);
	int ok2 = load_internal_texture(&g_render_textures.player, &g_render_textures.player_ok, player_path);
	int ok3 = load_internal_texture(&g_render_textures.enemy, &g_render_textures.enemy_ok, enemy_path);
	int ok4 = load_internal_texture(&g_render_textures.bullet, &g_render_textures.bullet_ok, bullet_path);

	return ok1 & ok2 & ok3 & ok4;
}

/**
 * @brief 构造一个矩形。
 */
static inline RECT menu_make_rect(const int x, const int y, const int w, const int h) {
	return RECT { x, y, x + w, y + h };
}

static inline int menu_hit_test(const Button *item, const int x, const int y) {
	POINT pt = { x, y };
	return PtInRect(&item->rect, pt);
}

static inline void menu_copy_label(wchar_t *dst, size_t cap, const wchar_t *src) {
	if (dst == NULL || cap == 0) {
		return;
	}

#if defined(_MSC_VER)
	wcsncpy_s(dst, cap, (src != NULL) ? src : L"", _TRUNCATE);
#else
	wcsncpy(dst, (src != NULL) ? src : L"", cap - 1);
	dst[cap - 1] = L'\0';
#endif
}

static inline void menu_draw_button(const Button *button) {
	// 根据 hovered 状态渲染不同的按钮样式。
	if (button->hovered) {
		setfillcolor(RGB(80, 160, 255));
		setlinecolor(RGB(255, 255, 255));
		settextcolor(RGB(255, 255, 255));
	}
	else {
		setfillcolor(RGB(40, 80, 160));
		setlinecolor(RGB(200, 200, 200));
		settextcolor(RGB(230, 230, 230));
	}

	solidrectangle(button->rect.left, button->rect.top, button->rect.right, button->rect.bottom);
	rectangle(button->rect.left, button->rect.top, button->rect.right, button->rect.bottom);

	settextstyle(24, 0, L"宋体");
	RECT textRect = button->rect;
	drawtext(button->text, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

static void menu_render_frame(const Button *buttons, const size_t button_count, const int width, const int height, const int high_score) {
	wchar_t high_score_text[64];

	BeginBatchDraw();

	// 如果贴图没加载成功，则使用纯色背景。
	if (g_render_textures.menu_background_ok) {
		putimage(0, 0, &g_render_textures.menu_background);
	}
	else {
		setfillcolor(RGB(10, 20, 60));
		solidrectangle(0, 0, width, height);
	}

	settextstyle(36, 0, L"宋体");
	settextcolor(RGB(255, 255, 200));
	outtextxy(width / 2 - 110, height / 4 - 40, L"飞机大战");

	settextstyle(20, 0, L"宋体");
	settextcolor(RGB(200, 200, 200));
	_snwprintf_s(high_score_text, sizeof(high_score_text) / sizeof(high_score_text[0]), L"最高分：%d", high_score);
	outtextxy(width / 2 - textwidth(high_score_text) / 2, height / 4 + 10, high_score);

	for (size_t i = 0; i < button_count; ++i) {
		menu_draw_button(&buttons[i]);
	}

	settextstyle(16, 0, L"宋体");
	settextcolor(RGB(180, 180, 180));
	outtextxy(10, height - 36, L"鼠标悬停选择，左键点击确认");

	FlushBatchDraw();
}

/**
 * @brief 渲染主菜单的主要接口。
 * @returns 返回被按下的按钮的 id：0 = 开始游戏，1 = 选项，2 = 退出
 */
int render_draw_main_menu(const int width, const int height, const int high_score, const int fps) {
	const wchar_t* labels[] = { L"开始游戏", L"选项", L"退出" };
	const size_t button_count = sizeof(labels) / sizeof(labels[0]);
	const int button_width = 240;
	const int button_height = 56;
	const int spacing = 12;
	const int center_x = (width - button_width) / 2;
	const int center_y = height / 2 - (int)((button_height * button_count + spacing * (button_count - 1)) / 2);

	Button buttons[3] = { 0 };

	for (size_t i = 0; i < button_count; ++i) {
		menu_copy_label(buttons[i].text, sizeof(buttons[i].text) / sizeof(buttons[i].text[0]), labels[i]);
		buttons[i].rect = menu_make_rect(center_x, center_y + (int)i * (button_height + spacing), button_width, button_height);
		buttons[i].hovered = 0;
	}

	while (1) {
		ExMessage msg;
		while (peekmessage(&msg, EM_MOUSE, TRUE)) {
			if (msg.message == WM_MOUSEMOVE) {
				for (size_t i = 0; i < button_count; ++i) {
					buttons[i].hovered = menu_hit_test(&buttons[i], msg.x, msg.y);
				}
			}
			else if (msg.message == WM_LBUTTONDOWN) {
				for (size_t i = 0; i < button_count; ++i) {
					if (menu_hit_test(&buttons[i], msg.x, msg.y)) {
						return (int)i;
					}
				}
			}
		}

		menu_render_frame(buttons, button_count, width, height, high_score);
		Sleep(1000 / fps);
	}

	return (int)(button_count - 1);
}

/**
 * @brief 渲染游戏画面的主要接口。
 */
void render_draw_current_frame(const GameplayVisualState *state) {
	if (state == NULL) {
		return;
	}

	BeginBatchDraw();

	if (g_render_textures.game_background_ok) {
		putimage(0, 0, &g_render_textures.game_background);
	}
	else {
		setfillcolor(RGB(5, 15, 40));
		solidrectangle(0, 0, state->width, state->height);
	}

	if (g_render_textures.enemy_ok && state->enemy_list) {
		for (Node *enemy_node = state->enemy_list->head->next; enemy_node; enemy_node = enemy_node->next) {
			const Object *enemy = (Object *)enemy_node->data;
			putimage(enemy->x, enemy->y, &g_render_textures.enemy);
		}
	}

	if (g_render_textures.bullet_ok && state->bullet_list) {
		for (Node *bullet_node = state->bullet_list->head->next; bullet_node; bullet_node = bullet_node->next) {
			const Object *bullet = (Object *)bullet_node->data;
			putimage(bullet->x, bullet->y, &g_render_textures.bullet);
		}
	}

	if (g_render_textures.player_ok && state->player) {
		putimage(state->player->x, state->player->y, &g_render_textures.player);
	}

	settextstyle(22, 0, L"宋体");
	settextcolor(RGB(255, 255, 255));

	wchar_t score_text[64];
	_snwprintf_s(score_text, sizeof(score_text) / sizeof(score_text[0]), L"SCORE %d", state->score);
	outtextxy(state->width / 2 - textwidth(score_text) / 2, state->height - 32, score_text);

	if (state->player_dead) {
		const wchar_t* death_reason = (state->death_reason != NULL) ? state->death_reason : L"";
		const wchar_t* prompt = L"按任意键返回标题界面...";

		setfillcolor(RGB(60, 60, 60));
		solidrectangle(0, 0, state->width, state->height);

		settextstyle(72, 0, L"Impact");
		settextcolor(RGB(220, 220, 220));
		outtextxy(state->width / 2 - textwidth(L"WASTED") / 2, state->height / 2 - 120, L"WASTED");

		settextstyle(28, 0, L"宋体");
		outtextxy(state->width / 2 - textwidth(death_reason) / 2, state->height / 2 - 40, death_reason);

		settextstyle(20, 0, L"宋体");
		outtextxy(state->width / 2 - textwidth(prompt) / 2, state->height / 2 + 60, prompt);
	}

	FlushBatchDraw();
}

const wchar_t * resolve_asset_path(const wchar_t *relative_path) {
	static wchar_t resolved[MAX_PATH * 4];
	wchar_t module_path[MAX_PATH];
	wchar_t base[MAX_PATH];
	wchar_t temp[MAX_PATH * 4];
	wchar_t *last_slash;

	const wchar_t* parents[] = { L"", L"..\\", L"..\\..\\", L"..\\..\\..\\" };

	if (relative_path == NULL || relative_path[0] == L'\0') {
		resolved[0] = L'\0';
		return resolved;
	}

	if ((relative_path[0] == L'\\' && relative_path[1] == L'\\') ||
		(relative_path[1] == L':')) {
		wcsncpy_s(resolved, relative_path, sizeof(resolved) / sizeof(resolved[0]));
		resolved[(sizeof(resolved) / sizeof(resolved[0])) - 1] = L'\0';
		return resolved;
	}

	GetModuleFileNameW(NULL, module_path, MAX_PATH);
	wcscpy_s(base, module_path);

	last_slash = wcsrchr(base, L'\\');
	if (last_slash != NULL) {
		*(last_slash + 1) = L'\0';
	}

	for (size_t i = 0; i < sizeof(parents) / sizeof(parents[0]); ++i) {
		_snwprintf_s(temp, sizeof(temp) / sizeof(temp[0]), L"%ls%ls%ls", base, parents[i], relative_path);

		if (_waccess(temp, 0) == 0) {
			wcsncpy_s(resolved, temp, sizeof(resolved) / sizeof(resolved[0]));
			resolved[(sizeof(resolved) / sizeof(resolved[0])) - 1] = L'\0';
			return resolved;
		}
	}

	_snwprintf_s(resolved, sizeof(resolved) / sizeof(resolved[0]), L"%ls%ls", base, relative_path);
	resolved[(sizeof(resolved) / sizeof(resolved[0])) - 1] = L'\0';
	return resolved;
}

static inline int load_internal_texture(IMAGE *img, int *flag, const wchar_t *path) {
	const wchar_t* resolved = resolve_asset_path(path);
	int ok = (resolved[0] != L'\0' && loadimage(img, resolved) == 0);

	if (flag != NULL) {
		*flag = ok;
	}

	if (!ok) {
		wchar_t buffer[512];
		_snwprintf_s(buffer, sizeof(buffer) / sizeof(buffer[0]),
			L"加载图片失败：%ls\n尝试路径：%ls",
			(path != NULL) ? path : L"(null)", resolved);

		OutputDebugStringW(buffer);
		MessageBoxW(GetHWnd(), buffer, L"资源加载失败", MB_ICONERROR | MB_OK);
	}

	return ok;
}
