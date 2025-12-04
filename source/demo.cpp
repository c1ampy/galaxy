#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <Windows.h>
#include <graphics.h>
#include <stdbool.h>
#pragma comment(lib, "Winmm.lib")

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHTH 800
#define PLANE_SIZE 50
#define ENEMY_SPEED 20
#define ENEMY_NUM 10
int enemyPlanelen = 0;

typedef struct condition//通用的记录对象的结构体
{
    int x;
    int y;
    int radius;
    bool exist;
}cod;
cod enemyplane[ENEMY_NUM];
int startTime =0;

bool collision(int x1, int y1, int x2, int y2, int r1, int r2) // 碰撞判定，这里想用以图片中心为圆心建立原型代替矩形，
{
    double d = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)); // 计算圆心距离
    if (d < (r1 + r2))
    {
        return true; // 撞上了返回true
    }
    else
    {
        return false; // 没撞上返回false
    }
} // 飞机(x1,y1)，半径r1；敌机(x2,y2)，半径r2
// 子弹与飞机碰撞，飞机与飞机碰撞应该都能用
// 建立结构体，对每个对象（飞机、敌机、子弹）存储横纵坐标，实际大小（半径），状态（有无）

void initEnemyplane() // 生成敌机
{
    
    int endTime = time(NULL);// 获取当前时间
    double deltaTime = difftime(endTime, startTime);//计算两次释放敌机的时间差
    if (deltaTime >= ENEMY_SPEED)//判断时间差够不够大，这个时间差可以随难度变化
    {
        if(enemyPlanelen < ENEMY_NUM)//判断飞机数量，屏幕上飞机不能过多
        {
            int x =(rand()% (SCREEN_WIDTH - 2* PLANE_SIZE) + PLANE_SIZE);//确保出现在画幅内
            int y = -PLANE_SIZE;//确保纵向上飞机从画幅外掉落
            enemyplane[enemyPlanelen].x = x;
            enemyplane[enemyPlanelen].y = y;
            enemyplane[enemyPlanelen].exist = true;//更新状态
            enemyPlanelen++;//敌机数量++
        }
        startTime = endTime;//生成后将初始时间更新成最近的一次生成敌机时间
    }
}

int main()
{

    return 0;
}
