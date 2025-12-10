#pragma once
#include <graphics.h>
#include <windows.h>
#include <string.h>

// 按钮的结构
typedef struct MenuItem
{
    char text[64];
    RECT rect;
    int hovered;
} MenuItem;

// 构造矩形
inline RECT MakeRect(int x, int y, int w, int h)
{
    RECT r; r.left = x; r.top = y; r.right = x + w; r.bottom = y + h;
    return r;
}

// 命中测试
inline int HitTest(const MenuItem* it, int x, int y)
{
    POINT pt; pt.x = x; pt.y = y;
    return PtInRect(&it->rect, pt) != 0;
}

// 绘制一个按钮
inline void DrawButton(const MenuItem* it)
{
    if (it->hovered) {                        // 悬停状态，高亮
        setfillcolor(RGB(80, 160, 255));
        setlinecolor(RGB(255, 255, 255));
        settextcolor(RGB(255, 255, 255));
    }
    else {
        setfillcolor(RGB(40, 80, 160));
        setlinecolor(RGB(200, 200, 200));
        settextcolor(RGB(230, 230, 230));
    }

    solidrectangle(it->rect.left, it->rect.top, it->rect.right, it->rect.bottom);
    rectangle(it->rect.left, it->rect.top, it->rect.right, it->rect.bottom);

    settextstyle(24, 0, "宋体");
    RECT tr = it->rect;
    drawtext(it->text, &tr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

// 主菜单（通用，接受窗口尺寸）
inline int ShowMainMenu(int winW, int winH)
{
    const int btnW = 240, btnH = 56, gap = 10;
    const int count = 3;
    const int centerX = (winW - btnW) / 2;
    const int baseY = winH / 2 - (btnH * count + gap * (count - 1)) / 2;

    MenuItem items[count];

    // 初始化按钮（使用安全的 strcpy_s）
    strcpy_s(items[0].text, sizeof(items[0].text), "Start Game");
    items[0].rect = MakeRect(centerX, baseY + 0 * (btnH + gap), btnW, btnH);
    items[0].hovered = 0;

    strcpy_s(items[1].text, sizeof(items[1].text), "Options");
    items[1].rect = MakeRect(centerX, baseY + 1 * (btnH + gap), btnW, btnH);
    items[1].hovered = 0;

    strcpy_s(items[2].text, sizeof(items[2].text), "Exit");
    items[2].rect = MakeRect(centerX, baseY + 2 * (btnH + gap), btnW, btnH);
    items[2].hovered = 0;

    IMAGE imgBg;
    const int hasBg = (loadimage(&imgBg, "bg.bmp") == 0);

    BeginBatchDraw();                                      // 开始批量绘制
    for (;;) {
        ExMessage msg;
        while (peekmessage(&msg, EM_MOUSE, true)) {
            if (msg.message == WM_MOUSEMOVE) {   // 检测移动
                int anyHover = 0;
                for (int i = 0; i < count; ++i) {
                    int h = HitTest(&items[i], msg.x, msg.y);
                    items[i].hovered = h;
                    if (h) anyHover = 1;
                }
                if (!anyHover) {
                    for (int i = 0; i < count; ++i)
                        items[i].hovered = 0;
                }
            }
            else if (msg.message == WM_LBUTTONDOWN) {      // 检测点击
                for (int i = 0; i < count; ++i) {
                    if (HitTest(&items[i], msg.x, msg.y)) {
                        EndBatchDraw();
                        return i;
                    }
                }
            }
        }

        // 绘制背景与标题
        if (hasBg) {
            putimage(0, 0, &imgBg);
        }
        else {
            setfillcolor(RGB(10, 20, 60));
            solidrectangle(0, 0, winW, winH);
            settextstyle(36, 0, "宋体");
            settextcolor(RGB(255, 255, 200));
            outtextxy(winW / 2 - 100, winH / 4 - 40, "飞机大战");
        }

        // 绘制按钮
        for (int i = 0; i < count; ++i) DrawButton(&items[i]);

      
        settextstyle(14, 0, "宋体");
        settextcolor(RGB(200, 200, 200));
        outtextxy(10, winH - 30, "鼠标悬停选择，左键点击确认");

        FlushBatchDraw();
        Sleep(16);
    }

    EndBatchDraw();
    return 2;
}


inline int MENU()
{
    const int W = 600;
    const int H = 800;

    initgraph(W, H);
    setbkmode(TRANSPARENT);

    int choice = ShowMainMenu(W, H);

    setfillcolor(RGB(0, 0, 0));
    solidrectangle(0, 0, W, H);
    settextstyle(28, 0, "宋体");
    settextcolor(RGB(255, 255, 255));
    if (choice == 0)
        outtextxy(80, H / 2 - 20, "你选择了：开始游戏");
    else if (choice == 1)
        outtextxy(80, H / 2 - 20, "你选择了：选项");
    else
        outtextxy(80, H / 2 - 20, "你选择了：退出");

    Sleep(1000);
    closegraph();
    return choice;
}
/*调用MENU函数会加载一个菜单*/

