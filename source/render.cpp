/**
 * @file render.cpp
 * @brief 这份源文件实现了渲染飞机大战游戏画面的有关函数。
 * @author 李牧远
 * @date 2025-12-13
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
 * @brief 获取数字难度对应的文字。
 */ 
static const wchar_t* difficulty_to_text(const int difficulty) {
	switch (difficulty) {
	case 0:  return L"简单";
	case 1:  return L"普通";
	case 2:  return L"困难";
	default: return L"未知";
	}
}

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

static void menu_render_frame(const Button* buttons, const size_t button_count, const int width, const int height, const int high_score[3], const int difficulty) {
	wchar_t line_easy[64];
	wchar_t line_normal[64];
	wchar_t line_hard[64];

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
	const wchar_t *title = L"飞机大战";
	outtextxy(width / 2 - textwidth(title) / 2, height / 4 - 40, title);

	// 将「当前难度 + 三行最高分」移动到底部并居中显示
	settextstyle(18, 0, L"宋体");
	settextcolor(RGB(200, 200, 160));
	wchar_t diff_buf[64];
	_snwprintf_s(diff_buf, _countof(diff_buf), L"当前难度：%ls", difficulty_to_text(difficulty));

	settextstyle(20, 0, L"宋体");
	settextcolor(RGB(200, 200, 200));
	_snwprintf_s(line_easy, _countof(line_easy), L"简单模式最高分：%d", high_score[0]);
	_snwprintf_s(line_normal, _countof(line_normal), L"普通模式最高分：%d", high_score[1]);
	_snwprintf_s(line_hard, _countof(line_hard), L"困难模式最高分：%d", high_score[2]);

	// 计算底部起始 y，四行逐行向上排列并居中（水平以文本宽度居中）
	const int footer_lines = 4;
	const int footer_spacing = 28; // 行间距，可根据视觉调整
	const int bottom_margin = 24; // 离窗口底部的外边距
	const int start_y = height - bottom_margin - footer_spacing * (footer_lines - 1);

	// 先绘制难度（使用 18 号字体），再绘制三行分数（20 号字体）
	settextstyle(18, 0, L"宋体");
	settextcolor(RGB(200, 200, 160));
	outtextxy(width / 2 - textwidth(diff_buf) / 2, start_y, diff_buf);

	settextstyle(20, 0, L"宋体");
	settextcolor(RGB(200, 200, 200));
	outtextxy(width / 2 - textwidth(line_easy) / 2, start_y + footer_spacing * 1, line_easy);
	outtextxy(width / 2 - textwidth(line_normal) / 2, start_y + footer_spacing * 2, line_normal);
	outtextxy(width / 2 - textwidth(line_hard) / 2, start_y + footer_spacing * 3, line_hard);

	for (size_t i = 0; i < button_count; ++i) {
		menu_draw_button(&buttons[i]);
	}

	settextstyle(16, 0, L"宋体");
	settextcolor(RGB(180, 180, 180));
	outtextxy(10, 36, L"鼠标悬停选择，左键点击确认");

	FlushBatchDraw();
}

/**
 * @brief 渲染主菜单的主要接口。
 * @returns 返回被按下的按钮的 id：0 = 开始游戏，1 = 选项，2 = 退出
 */
int render_draw_main_menu(const int width, const int height, const int high_score[3], const int difficulty, const int fps) {
	const wchar_t* labels[] = { L"开始游戏", L"选择难度", L"退出" };
	const size_t button_count = _countof(labels);
	const int button_width = 240;
	const int button_height = 56;
	const int spacing = 12;
	const int center_x = (width - button_width) / 2;
	const int center_y = height / 2 - (int)((button_height * button_count + spacing * (button_count - 1)) / 2);

	Button buttons[3] = { 0 };
	for (size_t i = 0; i < button_count; ++i) {
		menu_copy_label(buttons[i].text, _countof(buttons[i].text), labels[i]);
		buttons[i].rect = menu_make_rect(center_x, center_y + (int)i * (button_height + spacing), button_width, button_height);
		buttons[i].hovered = 0;
	}

	while (true) {
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

		menu_render_frame(buttons, button_count, width, height, high_score, difficulty);
		Sleep(1000 / fps);
	}
}

/**
 * @brief 渲染难度选择界面。
 * @return 返回选择的难度。
 */
int render_draw_difficulty_menu(const int width, const int height, const int difficulty, const int fps) {
	const wchar_t *labels[] = { L"简单", L"普通", L"困难" };
	const size_t button_count = _countof(labels);
	const int button_width = 220;
	const int button_height = 56;
	const int spacing = 12;
	const int center_x = (width - button_width) / 2;
	const int center_y = height / 2 - (int)((button_height * button_count + spacing * (button_count - 1)) / 2);

	Button buttons[3] = { 0 };
	for (size_t i = 0; i < button_count; ++i) {
		menu_copy_label(buttons[i].text, _countof(buttons[i].text), labels[i]);
		buttons[i].rect = menu_make_rect(center_x, center_y + (int)i * (button_height + spacing), button_width, button_height);
		buttons[i].hovered = 0;
	}

	while (true) {
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

		BeginBatchDraw();
		if (g_render_textures.menu_background_ok) {
			putimage(0, 0, &g_render_textures.menu_background);
		}
		else {
			setfillcolor(RGB(10, 20, 60));
			solidrectangle(0, 0, width, height);
		}

		settextstyle(36, 0, L"宋体");
		settextcolor(RGB(255, 255, 200));
		const wchar_t* title = L"选择难度";
		outtextxy(width / 2 - textwidth(title) / 2, height / 4 - 40, title);

		settextstyle(18, 0, L"宋体");
		wchar_t current_buf[64];
		_snwprintf_s(current_buf, _countof(current_buf), L"当前：%ls", difficulty_to_text(difficulty));
		outtextxy(width / 2 - textwidth(current_buf) / 2, height / 4 - 8, current_buf);

		for (size_t i = 0; i < button_count; ++i) {
			menu_draw_button(&buttons[i]);
		}

		FlushBatchDraw();
		Sleep(1000 / fps);
	}

	return difficulty;
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
	_snwprintf_s(score_text, _countof(score_text), L"SCORE %d", state->score);
	outtextxy(state->width / 2 - textwidth(score_text) / 2, state->height - 32, score_text);

	settextstyle(18, 0, L"宋体");
	wchar_t diff_text[64];
	_snwprintf_s(diff_text, _countof(diff_text), L"难度：%ls", difficulty_to_text(state->difficulty));
	outtextxy(12, 12, diff_text);

	wchar_t hp_text[64];
	_snwprintf_s(hp_text, _countof(hp_text), L"HP：%d / %d", state->hp, state->starting_hp > 0 ? state->starting_hp : 1);
	outtextxy(12, 36, hp_text);

	FlushBatchDraw();
}

/**
 * @brief 渲染暂停界面。
 * @return 0 = 返回游戏，1 = 重新开始游戏，2 = 返回主菜单，3 = 退出游戏。
 */
int render_draw_pause_menu(const int width, const int height, const int fps) {
	const wchar_t* labels[] = { L"返回游戏", L"重新开始游戏", L"返回主菜单", L"退出游戏" };
	const size_t button_count = _countof(labels);
	const int button_width = 320;
	const int button_height = 56;
	const int spacing = 12;
	const int center_x = (width - button_width) / 2;
	const int center_y = height / 2 - (int)((button_height * button_count + spacing * (button_count - 1)) / 2);

	Button buttons[4] = { 0 };
	for (size_t i = 0; i < button_count; ++i) {
		menu_copy_label(buttons[i].text, _countof(buttons[i].text), labels[i]);
		buttons[i].rect = menu_make_rect(center_x, center_y + (int)i * (button_height + spacing), button_width, button_height);
		buttons[i].hovered = 0;
	}

	while (true) {
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
			else if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE) {
				return 0;
			}
		}

		BeginBatchDraw();

		settextstyle(48, 0, L"宋体");
		settextcolor(RGB(255, 255, 255));
		const wchar_t* title = L"游戏已暂停";
		outtextxy(width / 2 - textwidth(title) / 2, height / 4 - 60, title);

		for (size_t i = 0; i < button_count; ++i) {
			menu_draw_button(&buttons[i]);
		}

		FlushBatchDraw();
		Sleep(1000 / fps);
	}

	return 0;
}

/**
 * @brief 渲染 WASTED 页面。
 * @return 0 = 重新开始，1 = 返回主菜单，2 = 退出游戏。
 */
int render_draw_wasted_page(const GameplayVisualState *state, const int high_score[3], const int fps) {
	if (state == NULL) {
		return 2;
	}

	const wchar_t *labels[] = { L"重新开始游戏", L"返回主菜单", L"退出游戏" };
	const size_t button_count = _countof(labels);
	const int button_width = 320;
	const int button_height = 56;
	const int spacing = 12;
	const int center_x = (state->width - button_width) / 2;
	const int center_y = state->height / 2 + 40;

	Button buttons[3] = { 0 };
	for (size_t i = 0; i < button_count; ++i) {
		menu_copy_label(buttons[i].text, _countof(buttons[i].text), labels[i]);
		buttons[i].rect = menu_make_rect(center_x, center_y + (int)i * (button_height + spacing), button_width, button_height);
		buttons[i].hovered = 0;
	}

	while (true) {
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

		BeginBatchDraw();
		setfillcolor(RGB(60, 60, 60));
		solidrectangle(0, 0, state->width, state->height);

		
		wchar_t score_buf[128];
		_snwprintf_s(score_buf, _countof(score_buf), L"本次分数：%d", state->score);
		settextstyle(24, 0, L"宋体");
		settextcolor(RGB(220, 220, 220));
		outtextxy(state->width / 2 - textwidth(score_buf) / 2, 12, score_buf);

		wchar_t high_buf[128];
		_snwprintf_s(high_buf, _countof(high_buf), L"最高分（%ls）：%d", difficulty_to_text(state->difficulty), high_score[state->difficulty]);
		settextstyle(18, 0, L"宋体");
		settextcolor(RGB(200, 200, 200));
		outtextxy(state->width / 2 - textwidth(high_buf) / 2, 12 + 30, high_buf);

		// 主标题 WASTED 保持在中间偏上显示
		settextstyle(72, 0, L"Impact");
		settextcolor(RGB(220, 220, 220));
		outtextxy(state->width / 2 - textwidth(L"WASTED") / 2, state->height / 2 - 160, L"WASTED");

		settextstyle(28, 0, L"宋体");
		const wchar_t* reason = state->death_reason != NULL ? state->death_reason : L"";
		outtextxy(state->width / 2 - textwidth(reason) / 2, state->height / 2 - 60, reason);

		settextstyle(24, 0, L"宋体");
		
		for (size_t i = 0; i < button_count; ++i) {
			menu_draw_button(&buttons[i]);
		}

		FlushBatchDraw();
		Sleep(1000 / fps);
	}

	return 1;
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
