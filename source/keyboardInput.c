//键盘输入处理
#include "keyboardinput.h"
#include <windows.h>


//初始化键盘状态,所有按键状态清零,焦点状态为1 
void zeroKeyboardState(KeyboardState* keyboard)
{
	//方向控制键上下左右
	keyboard->up = 0;
	keyboard->down = 0;
	keyboard->left = 0;
	keyboard->right = 0;
	
	//功能键:分别是开始游戏,退出游戏,暂停游戏
	keyboard->enter = 0;
	keyboard->esc = 0;
	keyboard->space = 0;
	
	//鼠标按键
	keyboard->mouseLeft = 0;
	keyboard->mouseRight = 0;
	
	//窗口焦点状态(初始假设有焦点)
	keyboard->playFocus = 1;
}


//检测游戏窗口是否获得焦点
//返回值:1=有焦点，0=无焦点
int checkPlayFocus(void)
{
	//HWND是窗口句柄的数据类型,本质是整数标识符
	HWND gameWindow = GetConsoleWindow();      //调用GetConsoleWindow获取函数游戏窗口句柄
	HWND activeWindow = GetForegroundWindow(); //调用GetForegroundWindow获取当前正在操作的活动窗口
	
	if (gameWindow == NULL) {
		return 0;  //无法获取窗口句柄
	}
	
	return (activeWindow == gameWindow);  //比较是否相同
}


//检测单个按键状态(备用）
//参数:KeyCode代表Windows虚拟键码
//返回值:1=按下,0=未按下
 int checkKey(int KeyCode)
{
	return (GetAsyncKeyState(KeyCode) & 0x8000) ? 1 : 0;
}


//更新所有按键状态(核心函数)
//先检查窗口焦点,有焦点才检测按键
void updateKeyboardState(KeyboardState* keyboard)
{
	//更新窗口焦点状态
	keyboard->playFocus = checkPlayFocus();
	
	//只有窗口有焦点时才检测具体按键
	if (keyboard->playFocus) {
		//方向键检测(WASD)
		keyboard->up = (GetAsyncKeyState('W') & 0x8000);
		keyboard->down = (GetAsyncKeyState('S') & 0x8000);
		keyboard->left = (GetAsyncKeyState('A') & 0x8000) ;
		keyboard->right = (GetAsyncKeyState('D') & 0x8000); 
		
		//功能键检测
		keyboard->enter = GetAsyncKeyState(VK_RETURN) & 0x8000;   //回车键
		keyboard->esc = GetAsyncKeyState(VK_ESCAPE) & 0x8000;     //ESC键
		keyboard->space = GetAsyncKeyState(VK_SPACE) & 0x8000;    //空格键
		
		//鼠标按键检测
		keyboard->mouseLeft = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
		keyboard->mouseRight = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
		
	} else {
		//窗口没有焦点,清空所有按键状态
		keyboard->up = keyboard->down = keyboard->left = keyboard->right = 0;
		keyboard->enter = keyboard->esc = keyboard->space = 0;
		keyboard->mouseLeft = keyboard->mouseRight = 0;
	}
}


//将按键状态转换成飞机移动
//注意:只处理移动,不处理射击
void handleKeyboardInput(KeyboardState* keyboard, int* playerX, int* playerY)
{
	//只有窗口有焦点才处理移动
	if (!keyboard->playFocus) {
		return;
	}
	
	//屏幕边界(应该从游戏配置传入,这里用默认值)
	const int MIN_X = 0, MAX_X = 79;
	const int MIN_Y = 0, MAX_Y = 23;
	
	//处理水平移动
	if (keyboard->left && *playerX > MIN_X) {
		(*playerX)--;
	}
	if (keyboard->right && *playerX < MAX_X) {
		(*playerX)++;
	}
	
	//处理垂直移动
	if (keyboard->up && *playerY > MIN_Y) {
		(*playerY)--;
	}
	if (keyboard->down && *playerY < MAX_Y) {
		(*playerY)++;
	}
}


//检测是否要退出游戏
//返回值:0=继续,1=退出
int checkExit(KeyboardState* keyboard)
{
	return keyboard->esc;  //ESC键退出
}