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
#include <stdio.h>
#include "render.h"
 // 在一个编译单元里定义纹理实例，其他文件只会使用 extern 声明。
RenderTextures g_render_textures = { 0 };

static const wchar_t* difficulty_to_text(int difficulty)
{
    switch (difficulty)
    {
    case DIFFICULTY_EASY:   return L"简单";
    case DIFFICULTY_NORMAL: return L"普通";
    case DIFFICULTY_HARD:   return L"困难";
    default:                return L"未知";
    }
}

static int safe_high_score(const int high_scores[3], int difficulty)
{
    if (high_scores == NULL || difficulty < DIFFICULTY_EASY || difficulty > DIFFICULTY_HARD)
    {
        return 0;
    }
    return high_scores[difficulty];
}
/**
 * @brief 构造一个矩形。
 */
static RECT menu_make_rect(int x, int y, int w, int h)
{
    RECT r = { x, y, x + w, y + h };
    return r;
}

static int menu_hit_test(const Button* item, int x, int y)
{
    POINT pt = { x, y };
    return PtInRect(&item->rect, pt);
}

static void menu_copy_label(wchar_t* dst, size_t cap, const wchar_t* src)
{
    if (dst == NULL || cap == 0)
    {
        return;
    }

#if defined(_MSC_VER)
    wcsncpy_s(dst, cap, src != NULL ? src : L"", _TRUNCATE);
#else
    wcsncpy(dst, src != NULL ? src : L"", cap - 1);
    dst[cap - 1] = L'\0';
#endif
}

static void menu_draw_button(const Button* button)
{// 根据 hovered 状态渲染不同的按钮样式
    if (button->hovered)
    {
        setfillcolor(RGB(80, 160, 255));
        setlinecolor(RGB(255, 255, 255));
        settextcolor(RGB(255, 255, 255));
    }
    else
    {
        setfillcolor(RGB(40, 80, 160));
        setlinecolor(RGB(200, 200, 200));
        settextcolor(RGB(230, 230, 230));
    }

    solidrectangle(button->rect.left, button->rect.top, button->rect.right, button->rect.bottom);
    rectangle(button->rect.left, button->rect.top, button->rect.right, button->rect.bottom);

    settextstyle(24, 0, L"宋体");
    RECT text_rect = button->rect;
    drawtext(button->text, &text_rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

static void menu_render_frame(
    const Button* buttons,
    size_t button_count,
    int width,
    int height,
    const int high_scores[3],
    int current_difficulty)
{
    wchar_t line_easy[64];
    wchar_t line_normal[64];
    wchar_t line_hard[64];

    BeginBatchDraw();

    // 如果贴图没加载成功，则使用纯色背景。
    if (g_render_textures.menu_background_ok)
    {
        putimage(0, 0, &g_render_textures.menu_background);
    }
    else
    {
        setfillcolor(RGB(10, 20, 60));
        solidrectangle(0, 0, width, height);
    }

    settextstyle(36, 0, L"宋体");
    settextcolor(RGB(255, 255, 200));
    const wchar_t* title = L"飞机大战";
    outtextxy(width / 2 - textwidth(title) / 2, height / 4 - 40, title);

    settextstyle(18, 0, L"宋体");
    settextcolor(RGB(200, 200, 160));
    wchar_t diff_buf[64];
    _snwprintf_s(diff_buf, _countof(diff_buf), L"当前难度：%ls", difficulty_to_text(current_difficulty));
    outtextxy(width / 2 - textwidth(diff_buf) / 2, height / 4 - 8, diff_buf);

    settextstyle(20, 0, L"宋体");
    settextcolor(RGB(200, 200, 200));
    _snwprintf_s(line_easy, _countof(line_easy), L"简单模式：%d", safe_high_score(high_scores, DIFFICULTY_EASY));
    _snwprintf_s(line_normal, _countof(line_normal), L"普通模式：%d", safe_high_score(high_scores, DIFFICULTY_NORMAL));
    _snwprintf_s(line_hard, _countof(line_hard), L"困难模式：%d", safe_high_score(high_scores, DIFFICULTY_HARD));

    const int base_y = height / 4 + 16;
    const int spacing = 28;
    outtextxy(width / 2 - textwidth(line_easy) / 2, base_y, line_easy);
    outtextxy(width / 2 - textwidth(line_normal) / 2, base_y + spacing, line_normal);
    outtextxy(width / 2 - textwidth(line_hard) / 2, base_y + spacing * 2, line_hard);

    for (size_t i = 0; i < button_count; ++i)
    {
        menu_draw_button(&buttons[i]);
    }

    settextstyle(16, 0, L"宋体");
    settextcolor(RGB(180, 180, 180));
    outtextxy(10, height - 36, L"鼠标悬停选择，左键点击确认");

    FlushBatchDraw();
}

static void draw_overlay_background(int width, int height, BYTE alpha)
{
    setfillcolor(RGB(0, 0, 0));
    setalpha(alpha);
    solidrectangle(0, 0, width, height);
    setalpha(255);
}

void window_create(int width, int height, const wchar_t* title)
{
    initgraph(width, height);
    setbkmode(TRANSPARENT);

    if (title != NULL && title[0] != L'\0')
    {
        SetWindowTextW(GetHWnd(), title);
    }
}

void window_close(void)
{
    closegraph();
}

int render_load_menu_texture(const wchar_t* menu_background_path)
{// 如果没有菜单背景图片就用纯色的窗口
    return loadimage(&g_render_textures.menu_background, resolve_asset_path(menu_background_path)) == 0
        ? (g_render_textures.menu_background_ok = 1)
        : (g_render_textures.menu_background_ok = 0);
}

static int load_texture(IMAGE* image, int* flag, const wchar_t* path)
{
    const wchar_t* resolved = resolve_asset_path(path);
    int ok = (resolved[0] != L'\0' && loadimage(image, resolved) == 0);
    if (flag != NULL)
    {
        *flag = ok;
    }

    if (!ok)
    {
        wchar_t buffer[512];
        _snwprintf_s(buffer, _countof(buffer), L"加载图片失败：%ls\n尝试路径：%ls",
            (path != NULL) ? path : L"(null)", resolved);
        OutputDebugStringW(buffer);
        MessageBoxW(GetHWnd(), buffer, L"资源加载失败", MB_ICONERROR | MB_OK);
    }
    return ok;
}

int render_load_gameplay_textures(
    const wchar_t* game_background_path,
    const wchar_t* player_path,
    const wchar_t* enemy_path,
    const wchar_t* bullet_path)
{	// 游戏内所有贴图的批量加载
    int ok1 = load_texture(&g_render_textures.game_background, &g_render_textures.game_background_ok, game_background_path);
    int ok2 = load_texture(&g_render_textures.player, &g_render_textures.player_ok, player_path);
    int ok3 = load_texture(&g_render_textures.enemy, &g_render_textures.enemy_ok, enemy_path);
    int ok4 = load_texture(&g_render_textures.bullet, &g_render_textures.bullet_ok, bullet_path);
    return ok1 & ok2 & ok3 & ok4;
}

/**
 * @brief 渲染主菜单的主要接口。
 * @returns 返回被按下的按钮的 id：0 = 开始游戏，1 = 选项，2 = 退出
 */
int render_draw_main_menu(
    int width,
    int height,
    const int high_scores[3],
    int current_difficulty,
    int fps)
{
    const wchar_t* labels[] = { L"开始游戏", L"选择难度", L"退出" };
    const size_t button_count = _countof(labels);
    const int button_width = 240;
    const int button_height = 56;
    const int spacing = 12;
    const int center_x = (width - button_width) / 2;
    const int center_y = height / 2 - (int)((button_height * button_count + spacing * (button_count - 1)) / 2);

    Button buttons[3] = { 0 };
    for (size_t i = 0; i < button_count; ++i)
    {
        menu_copy_label(buttons[i].text, _countof(buttons[i].text), labels[i]);
        buttons[i].rect = menu_make_rect(center_x, center_y + (int)i * (button_height + spacing), button_width, button_height);
        buttons[i].hovered = 0;
    }

    for (;;)
    {
        ExMessage msg;
        while (peekmessage(&msg, EM_MOUSE, TRUE))
        {
            if (msg.message == WM_MOUSEMOVE)
            {
                for (size_t i = 0; i < button_count; ++i)
                {
                    buttons[i].hovered = menu_hit_test(&buttons[i], msg.x, msg.y);
                }
            }
            else if (msg.message == WM_LBUTTONDOWN)
            {
                for (size_t i = 0; i < button_count; ++i)
                {
                    if (menu_hit_test(&buttons[i], msg.x, msg.y))
                    {
                        return (int)i;
                    }
                }
            }
        }

        menu_render_frame(buttons, button_count, width, height, high_scores, current_difficulty);
        Sleep(1000 / fps);
    }
}
/**
* @brief渲染难度界面
* 012分别是简单普通困难
*/
int render_draw_difficulty_menu(int width, int height, int current_difficulty, int fps)
{
    const wchar_t* labels[] = { L"简单", L"普通", L"困难" };
    const size_t button_count = _countof(labels);
    const int button_width = 220;
    const int button_height = 56;
    const int spacing = 12;
    const int center_x = (width - button_width) / 2;
    const int center_y = height / 2 - (int)((button_height * button_count + spacing * (button_count - 1)) / 2);

    Button buttons[3] = { 0 };
    for (size_t i = 0; i < button_count; ++i)
    {
        menu_copy_label(buttons[i].text, _countof(buttons[i].text), labels[i]);
        buttons[i].rect = menu_make_rect(center_x, center_y + (int)i * (button_height + spacing), button_width, button_height);
        buttons[i].hovered = 0;
    }

    for (;;)
    {
        ExMessage msg;
        while (peekmessage(&msg, EM_MOUSE, TRUE))
        {
            if (msg.message == WM_MOUSEMOVE)
            {
                for (size_t i = 0; i < button_count; ++i)
                {
                    buttons[i].hovered = menu_hit_test(&buttons[i], msg.x, msg.y);
                }
            }
            else if (msg.message == WM_LBUTTONDOWN)
            {
                for (size_t i = 0; i < button_count; ++i)
                {
                    if (menu_hit_test(&buttons[i], msg.x, msg.y))
                    {
                        return (int)i;
                    }
                }
            }
        }

        BeginBatchDraw();
        if (g_render_textures.menu_background_ok)
        {
            putimage(0, 0, &g_render_textures.menu_background);
        }
        else
        {
            setfillcolor(RGB(10, 20, 60));
            solidrectangle(0, 0, width, height);
        }

        settextstyle(36, 0, L"宋体");
        settextcolor(RGB(255, 255, 200));
        const wchar_t* title = L"选择难度";
        outtextxy(width / 2 - textwidth(title) / 2, height / 4 - 40, title);

        settextstyle(18, 0, L"宋体");
        wchar_t current_buf[64];
        _snwprintf_s(current_buf, _countof(current_buf), L"当前：%ls", difficulty_to_text(current_difficulty));
        outtextxy(width / 2 - textwidth(current_buf) / 2, height / 4 - 8, current_buf);

        for (size_t i = 0; i < button_count; ++i)
        {
            menu_draw_button(&buttons[i]);
        }

        FlushBatchDraw();
        Sleep(1000 / fps);
    }

    return current_difficulty;
}

/**
* @brief 渲染暂停菜单 0，1，2，3分别对应这返回游戏，重开，菜单，退出游戏
 */

int render_draw_pause_menu(int width, int height, const GameplayVisualState* state, int fps)
{
    const wchar_t* labels[] = { L"返回游戏", L"重新开始游戏", L"返回主菜单", L"退出游戏" };
    const size_t button_count = _countof(labels);
    const int button_width = 320;
    const int button_height = 56;
    const int spacing = 12;
    const int center_x = (width - button_width) / 2;
    const int center_y = height / 2 - (int)((button_height * button_count + spacing * (button_count - 1)) / 2);

    Button buttons[4] = { 0 };
    for (size_t i = 0; i < button_count; ++i)
    {
        menu_copy_label(buttons[i].text, _countof(buttons[i].text), labels[i]);
        buttons[i].rect = menu_make_rect(center_x, center_y + (int)i * (button_height + spacing), button_width, button_height);
        buttons[i].hovered = 0;
    }

    for (;;)
    {
        ExMessage msg;
        while (peekmessage(&msg, EM_MOUSE, TRUE))
        {
            if (msg.message == WM_MOUSEMOVE)
            {
                for (size_t i = 0; i < button_count; ++i)
                {
                    buttons[i].hovered = menu_hit_test(&buttons[i], msg.x, msg.y);
                }
            }
            else if (msg.message == WM_LBUTTONDOWN)
            {
                for (size_t i = 0; i < button_count; ++i)
                {
                    if (menu_hit_test(&buttons[i], msg.x, msg.y))
                    {
                        return (int)i;
                    }
                }
            }
            else if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE)
            {
                return 0;
            }
        }

        if (state != NULL)
        {
            render_draw_current_frame(state);
        }

        BeginBatchDraw();
        draw_overlay_background(width, height, 150);

        settextstyle(48, 0, L"宋体");
        settextcolor(RGB(255, 255, 255));
        const wchar_t* title = L"游戏已暂停";
        outtextxy(width / 2 - textwidth(title) / 2, height / 4 - 60, title);

        for (size_t i = 0; i < button_count; ++i)
        {
            menu_draw_button(&buttons[i]);
        }

        FlushBatchDraw();
        Sleep(1000 / fps);
    }

    return 0;
}

/**
* @brief 死亡结算画面 0，1，2分别对应重开，主菜单，退出游戏
*/
int render_draw_wasted_page(const GameplayVisualState* state, const int high_scores[3], int fps)
{
    if (state == NULL)
    {
        return 2;
    }

    const wchar_t* labels[] = { L"重新开始游戏", L"返回主菜单", L"退出游戏" };
    const size_t button_count = _countof(labels);
    const int button_width = 320;
    const int button_height = 56;
    const int spacing = 12;
    const int center_x = (state->width - button_width) / 2;
    const int center_y = state->height / 2 + 40;

    Button buttons[3] = { 0 };
    for (size_t i = 0; i < button_count; ++i)
    {
        menu_copy_label(buttons[i].text, _countof(buttons[i].text), labels[i]);
        buttons[i].rect = menu_make_rect(center_x, center_y + (int)i * (button_height + spacing), button_width, button_height);
        buttons[i].hovered = 0;
    }

    for (;;)
    {
        ExMessage msg;
        while (peekmessage(&msg, EM_MOUSE, TRUE))
        {
            if (msg.message == WM_MOUSEMOVE)
            {
                for (size_t i = 0; i < button_count; ++i)
                {
                    buttons[i].hovered = menu_hit_test(&buttons[i], msg.x, msg.y);
                }
            }
            else if (msg.message == WM_LBUTTONDOWN)
            {
                for (size_t i = 0; i < button_count; ++i)
                {
                    if (menu_hit_test(&buttons[i], msg.x, msg.y))
                    {
                        return (int)i;
                    }
                }
            }
            else if (msg.message == WM_KEYDOWN)
            {
                return 1; // 任意键返回主菜单
            }
        }

        BeginBatchDraw();
        setfillcolor(RGB(60, 60, 60));
        solidrectangle(0, 0, state->width, state->height);

        settextstyle(72, 0, L"Impact");
        settextcolor(RGB(220, 220, 220));
        outtextxy(state->width / 2 - textwidth(L"WASTED") / 2, state->height / 2 - 160, L"WASTED);

            settextstyle(28, 0, L"宋体");
        const wchar_t* reason = state->death_reason != NULL ? state->death_reason : L"";
        outtextxy(state->width / 2 - textwidth(reason) / 2, state->height / 2 - 60, reason);

        settextstyle(24, 0, L"宋体");
        wchar_t score_buf[128];
        _snwprintf_s(score_buf, _countof(score_buf), L"本次分数：%d", state->score);
        outtextxy(state->width / 2 - textwidth(score_buf) / 2, state->height / 2 - 10, score_buf);

        wchar_t high_buf[128];
        int diff = (state->difficulty >= DIFFICULTY_EASY && state->difficulty <= DIFFICULTY_HARD)
            ? state->difficulty
            : DIFFICULTY_NORMAL;
        _snwprintf_s(high_buf, _countof(high_buf), L"最高分（%ls）：%d",
            difficulty_to_text(diff), safe_high_score(high_scores, diff));
        outtextxy(state->width / 2 - textwidth(high_buf) / 2, state->height / 2 + 30, high_buf);

        for (size_t i = 0; i < button_count; ++i)
        {
            menu_draw_button(&buttons[i]);
        }

        settextstyle(20, 0, L"宋体");
        const wchar_t* prompt = L"按任意键返回主菜单...";
        outtextxy(state->width / 2 - textwidth(prompt) / 2, state->height - 60, prompt);

        FlushBatchDraw();
        Sleep(1000 / fps);
    }

    return 1;
}

/**
 * @brief 渲染游戏画面的主要接口。
 */

void render_draw_current_frame(const GameplayVisualState* state)
{
    if (state == NULL)
    {
        return;
    }

    BeginBatchDraw();

    if (g_render_textures.game_background_ok)
    {
        putimage(0, 0, &g_render_textures.game_background);
    }
    else
    {
        setfillcolor(RGB(5, 15, 40));
        solidrectangle(0, 0, state->width, state->height);
    }

    if (g_render_textures.player_ok && state->player)
    {
        putimage((int)state->player->x, (int)state->player->y, &g_render_textures.player);
    }

    if (g_render_textures.enemy_ok && state->enemy_list)
    {
        for (Node* enemy_node = state->enemy_list->head->next; enemy_node; enemy_node = enemy_node->next)
        {
            const Object* enemy = (const Object*)enemy_node->data;
            putimage((int)enemy->x, (int)enemy->y, &g_render_textures.enemy);
        }
    }

    if (g_render_textures.bullet_ok && state->bullet_list)
    {
        for (Node* bullet_node = state->bullet_list->head->next; bullet_node; bullet_node = bullet_node->next)
        {
            const Object* bullet = (const Object*)bullet_node->data;
            putimage((int)bullet->x, (int)bullet->y, &g_render_textures.bullet);
        }
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
    _snwprintf_s(hp_text, _countof(hp_text), L"HP：%d / %d", state->hp, state->max_hp > 0 ? state->max_hp : 1);
    outtextxy(12, 36, hp_text);

    FlushBatchDraw();
}

const wchar_t* resolve_asset_path(const wchar_t* relative_path)
{
    static wchar_t resolved[MAX_PATH * 4];
    wchar_t module_path[MAX_PATH];
    wchar_t base[MAX_PATH];
    wchar_t temp[MAX_PATH * 4];
    wchar_t* last_slash = NULL;

    const wchar_t* parents[] = { L"", L"..\\", L"..\\..\\", L"..\\..\\..\\" };

    if (relative_path == NULL || relative_path[0] == L'\0')
    {
        resolved[0] = L'\0';
        return resolved;
    }

    if ((relative_path[0] == L'\\' && relative_path[1] == L'\\') ||
        (relative_path[1] == L':'))
    {
        wcsncpy_s(resolved, _countof(resolved), relative_path, _TRUNCATE);
        return resolved;
    }

    GetModuleFileNameW(NULL, module_path, MAX_PATH);
    wcscpy_s(base, _countof(base), module_path);
    last_slash = wcsrchr(base, L'\\');
    if (last_slash != NULL)
    {
        *(last_slash + 1) = L'\0';
    }

    for (size_t i = 0; i < _countof(parents); ++i)
    {
        _snwprintf_s(temp, _countof(temp), L"%ls%ls%ls", base, parents[i], relative_path);
        if (_waccess(temp, 0) == 0)
        {
            wcsncpy_s(resolved, _countof(resolved), temp, _TRUNCATE);
            return resolved;
        }
    }

    _snwprintf_s(resolved, _countof(resolved), L"%ls%ls", base, relative_path);
    return resolved;
}