#ifndef EVENT_H
#define EVENT_H

#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <Windows.h>
#include <gdiplus.h>
using namespace Gdiplus;

// 窗体过程函数
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


// 初始化
VOID Init(HWND hWnd, WPARAM wParam, LPARAM lParam);

//双缓冲绘制
VOID Render(HWND hWnd);
VOID Render_Start(HWND hWnd);//绘制游戏开始界面
VOID Render_Game(HWND hWnd);//绘制游戏界面
VOID Render_Generate(HWND hWnd);
VOID Render_End(HWND hWnd);


//定时器事件
VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);

//hero更新
VOID HeroUpdate();

//地形更新
VOID TerrianUpdate();

//游戏状态更新
VOID GameStatusUpdate();

//判断是否点击按钮
BOOL ButtonClicked(POINT, RECT);

//键盘按下事件处理
VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

//键盘松开事件处理
VOID KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

//左鼠标点击事件
VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

//补充结构体的一些方法
POINT operator + (const POINT& a, const POINT& b);

PointF CenterPoint(const Rect& rect);

bool isShowWeapon();

//对一个圆内的物体造成伤害并改变地形
void CauseDamage(int x, int y, int r, int daamge);

int getnextheroID(int cur);

void KillWorm(int x);

//判断游戏是否结束
int isGameEnd();

void GenerateGame();

//判断人物是否出界
bool OutOfBound(const Rect& rect);

bool isHumanMove();

//多人模式中，判断是否该本机操作
bool isLocalMove();

//AI相关的类
struct AI_Simulator {
    int direction;
    int walk_timeleft;
    bool space_pressed;
    bool computed_best;
    double best_score;
    int best_angle, best_direction, best_strength;
    int statusMoveTo(const Rect&, int dir);
    bool canJumpTo(const Rect&, int dir);
    void doSomedicision();
    PointF physicsupdate(int angle, int direction, int strength);
    double getScore(int x, int y);
};

void SpaceDown();
void SpaceUp();
void launchWeapon();

VOID Render_Server(HWND hWnd);
VOID Render_Client(HWND hWnd);
VOID Render_OnlineEnd(HWND hWnd);

//加载和传输TCP数据的函数
void net_init();
void sendstring(const char* sbuf);
void ProcessKeyDown(WPARAM wParam);
void ProcessKeyUp(WPARAM wParam);
void a_Launch();
void b_SetSeed();
void c_Gen();
void d_Worms();
void e_Player(int i);

void zhuangB();
#endif // !EVENT_H
