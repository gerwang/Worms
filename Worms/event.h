#ifndef EVENT_H
#define EVENT_H

#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <Windows.h>
#include <gdiplus.h>
using namespace Gdiplus;

// ������̺���
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


// ��ʼ��
VOID Init(HWND hWnd, WPARAM wParam, LPARAM lParam);

//˫�������
VOID Render(HWND hWnd);
VOID Render_Start(HWND hWnd);//������Ϸ��ʼ����
VOID Render_Game(HWND hWnd);//������Ϸ����
VOID Render_Generate(HWND hWnd);
VOID Render_End(HWND hWnd);


//��ʱ���¼�
VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);

//hero����
VOID HeroUpdate();

//���θ���
VOID TerrianUpdate();

//��Ϸ״̬����
VOID GameStatusUpdate();

//�ж��Ƿ�����ť
BOOL ButtonClicked(POINT, RECT);

//���̰����¼�����
VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

//�����ɿ��¼�����
VOID KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

//��������¼�
VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

//����ṹ���һЩ����
POINT operator + (const POINT& a, const POINT& b);

PointF CenterPoint(const Rect& rect);

bool isShowWeapon();

//��һ��Բ�ڵ���������˺����ı����
void CauseDamage(int x, int y, int r, int daamge);

int getnextheroID(int cur);

void KillWorm(int x);

//�ж���Ϸ�Ƿ����
int isGameEnd();

void GenerateGame();

//�ж������Ƿ����
bool OutOfBound(const Rect& rect);

bool isHumanMove();

//����ģʽ�У��ж��Ƿ�ñ�������
bool isLocalMove();

//AI��ص���
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

//���غʹ���TCP���ݵĺ���
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
