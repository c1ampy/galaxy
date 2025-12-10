//键盘输入控制头文件
#ifndef KEYBOARDINPUT_H
#define KEYBOARDINPUT_H

#include <windows.h>

//枚举,定义鼠标按键状态
typedef enum {
    MOUSE_LEFT = 0,//左键
    MOUSE_RIGHT = 1,//右键
}MouseState;

//键盘状态的结构体
typedef struct {
    //控制飞机上下左右运动
    int up;   //W键
    int down; //S键
    int left; //A键
    int right;//D键

    //控制游戏开始,退出,暂停
    int enter;//开始
    int esc;  //退出
    int space;//暂停

    //鼠标按键
    int mouseLeft;//鼠标左键
    int mouseRight;//鼠标右键

    //游戏窗口状态
    int playFocus;//游戏窗口是否获得焦点
}KeyboardState;

//函数声明

//初始化函数,形参是指向按键状态的指针
void zeroKeyboardState(KeyboardState* keyboard);

//实时更新函数,获取按键最新状态
void updateKeyboardState(KeyboardState* keyboard);

//判断焦点函数,检测游戏窗口是否获得焦点,返回值0则无焦点,1则有焦点
int checkPlayFocus(void);

//备用函数,对update函数的补充,检测单个按键的状态
//参数KeyCode的含义是windows虚拟键码,指定要检测的按键
int checkKey(int KeyCode);

//处理函数,将按键状态转换成具体功能
void handleKeyboardInput(KeyboardState* keyboard,int* playerX,int* playerY);

//退出函数,检测是否要退出,检测esc返回值为0则继续,1则退出
int checkExit(KeyboardState* keyboard);

#endif