#include <graphics.h>
#include <windows.h>                
#include <string.h>                  

// The struct of
typedef struct MenuItem
{
    char text[64];
    RECT rect;
    int hovered;
} MenuItem;

//构造矩形
static RECT MakeRect(int x, int y, int w, int h)
{
    RECT r; r.left = x; r.top = y; r.right = x + w; r.bottom = y + h;
    return r;
}

// 命中测试
static int HitTest(const MenuItem* it, int x, int y)
{
    POINT pt; pt.x = x; pt.y = y;
    return PtInRect(&it->rect, pt) != 0;
}

// 绘制一个按钮
static void DrawButton(const MenuItem* it)
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


    settextstyle(24, 0, "宋体");
    RECT tr = it->rect;
    drawtext(it->text, &tr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

//主菜单
int ShowMainMenu(int winW, int winH)
{
    const int btnW = 240, btnH = 56, gap = 10;
    const int count = 3;
    const int centerX = (winW - btnW) / 2;
    const int baseY = winH / 2 - (btnH * count + gap * (count - 1)) / 2;

    MenuItem items[count];

    // 初始化按钮
    strcpy_s(items[0].text, "Start Game");
    items[0].rect = MakeRect(centerX, baseY + 0 * (btnH + gap), btnW, btnH);
    items[0].hovered = 0;

    strcpy_s(items[1].text, "Options");
    items[1].rect = MakeRect(centerX, baseY + 1 * (btnH + gap), btnW, btnH);
    items[1].hovered = 0;

    strcpy_s(items[2].text, "Exit");
    items[2].rect = MakeRect(centerX, baseY + 2 * (btnH + gap), btnW, btnH);
    items[2].hovered = 0;

    IMAGE imgBg;
    const int hasBg = (loadimage(&imgBg, "bg.bmp") == 0);

    BeginBatchDraw();                                      //开始批量绘制
    //处理鼠标的消息
    for (;;) {
        ExMessage msg;
        while (peekmessage(&msg, EM_MOUSE, true)) {
            if (msg.message == WM_MOUSEMOVE) {   //检测移动          
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
            else if (msg.message == WM_LBUTTONDOWN) {      //检测点击
                for (int i = 0; i < count; ++i) {
                    if (HitTest(&items[i], msg.x, msg.y)) {
                        EndBatchDraw();
                        return i;
                    }
                }
            }
        }


        
            setfillcolor(RGB(10, 20, 60));
            solidrectangle(0, 0, winW, winH);
            settextstyle(36, 0, "宋体");
            settextcolor(RGB(255, 255, 200));
            outtextxy(winW / 2 - 180, winH / 4 - 40, "飞机大战");
        }

        for (int i = 0; i < count; ++i) DrawButton(&items[i]);

      

        FlushBatchDraw();
        Sleep(16);
    }
}


int main()
{
    const int W = 480, H = 640;
    initgraph(W, H);
    setbkmode(TRANSPARENT);
    // 调用主菜单
    int choice = ShowMainMenu(W, H);

    setfillcolor(RGB(0, 0, 0));
    solidrectangle(0, 0, W, H);
    settextstyle(28, 0, "宋体");
    settextcolor(RGB(255, 255, 255));
    if (choice == 0)
        outtextxy(80, H / 2 - 20, "你选择了：Start Game");
    else if (choice == 1)
        outtextxy(80, H / 2 - 20, "你选择了：Options");
    else
        outtextxy(80, H / 2 - 20, "你选择了：Exit");

    Sleep(1000);
    closegraph();
    return 0;
}