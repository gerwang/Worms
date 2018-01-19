#ifndef GLOBAL_H
#define GLOBAL_H

#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <gdiplus.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <math.h>
#include "resource.h"
#include <utility>
#include <vector>
#include <ctime>
#include <cwchar>
#include <mmsystem.h>
#include <iphlpapi.h>
#include <string>
#include <queue>
using namespace Gdiplus;
using namespace std;


// 定义宏变量，WNDWIDTH、WNDHEIGHT为窗口的宽和高
#define TIMER_ID             1		//计时器ID
#define NET_ELAPSE			 10
#define NET_NUM				 10
#define TIMER_ELAPSE         20		//计时器间隔(ms)
#define WNDWIDTH             1280	//窗口的宽
#define WNDHEIGHT            720	//窗口的高
#define TITLEBARHEIGHT       32		//标题栏高

// 定义资源的尺寸
#define HERO_SIZE_X          30
#define HERO_SIZE_Y          30
#define HERO_MAX_FRAME_NUM   3
#define GAME_START_BUTTON_SIZE_X   147
#define GAME_START_BUTTON_SIZE_Y   60

//定义游戏常量
#define MAX_TERRIAN_NUM      1		//最大地块数量
#define GRAVITY				 10		//重力
#define HERO_VEL_X			 2		//英雄水平速度
#define HERO_VEL_Y			 4		//英雄竖直速度

#define TERRAIN_WIDTH WNDWIDTH
#define TERRAIN_MAX_HEIGHT 400
#define TERRAIN_MIN_HEIGHT 50
#define TERRAIN_START_HEIGHT 100
#define TERRAINBUSH_R 1

#define CLIMB_HEIGHT 20
#define DOWN_HEIGHT 20

#define JUMP_VEL_Y 24
#define JUMP_VEL_X 8

#define MAX_WIND 15

#define SQR(x) ((x)*(x))
#define CUB(x) ((x)*(x)*(x))

#define AIM_SPEED 5

#define BUF_LEN 20000

typedef pair<int, int> pii;
const double M_PI = acos(-1.0);

const Color teamcolor[4] = {Color(255, 0, 0, 255), Color(255, 0, 255, 0), Color(255, 255, 0, 0), Color(255, 0, 0, 0)};

// 英雄结构体，英雄位图、位置、大小*/
typedef struct {
    POINT   vel;
    Rect	rect;
    int     curFrameIndex;
    int     maxFrameSize;
    int		FrameX;
    int		FrameY;
    int		direction;
    int		frametype;
    int		hp;
    int		flytime;
    int		team;
    void SetFrame(int x) {
        frametype = x;
        curFrameIndex = 0;
        if(x == 0) {
            maxFrameSize = 18;
            FrameX = 68;
            FrameY = 54;
        } else if(x == 1) {
            maxFrameSize = 20;
            FrameX = 59;
            FrameY = 70;
        } else if(x == 2) {
            maxFrameSize = 17;
            FrameX = 57;
            FrameY = 63;
        }
    }
    void Init(int left, int up, int right, int bottom) {
        rect = Rect(left, up, right, bottom);
        SetFrame(0);
        hp = 100;
        flytime = 0;
    }
} Hero;

/*地形结构体，地形方块位置、大小*/

/*按钮结构体*/
typedef struct {
    HBITMAP	hBmp;	//按钮图片
    POINT   pos;	//按钮位置
    SIZE	size;	//按钮尺寸
    int	status;	//按钮状态
}  GameButton;

/*游戏状态结构体*/
typedef struct {
    HBITMAP	hBmp;	//背景图片
    int	status;	//游戏状态
    bool ADpressed;
    bool RETURNpressed;
    int WSstatus;
    int WSangle;
    bool SPACEpressed;
    bool launched;
    int curWeapon, maxWeapon;
    void UpdateAngle();
    int strength, stren_dir;
    void Updatestrength();
    bool boomed;
    int alivehero;
    int sealevel;
    int windvel;
    int boomtime;
    int wormsperplayer;
    int playernum;
    int winplayer;
    bool OnlineGame;
    bool isSever;
    bool isClient;
}  GameStatus;


typedef struct {
    vector<pii> terrains[TERRAIN_WIDTH];
    unsigned seed;
    int Random_ratio(int prev, double ratio);
    void Generate();
    bool RectCollision(const Rect&);
    void Boom(Point pos, int rad);
    void BeizerGenerate();
    int minHeight(const Rect& rect);
    bool isair(const Rect& rect);
} Terrain;

struct Box {
    Rect rect;
};

struct Bazooka {
    Rect rect;
    POINT vel;
    int time_flag;
};

struct Grenade {
    Rect rect;
    POINT vel;
    int time_flag;
};

// 全局函数

/*************************************************
Fucntion : 创建英雄人物
Parameter:
posX、posY表示矩形左上角的坐标
sizeX、sizeY表示矩形的size
hBmp表示位图句柄
curFrameIndex当前帧
maxFrameSize最大帧率
*************************************************/

/*************************************************
Fucntion : 创建游戏状态
Parameter:
posX、posY表示矩形左上角的坐标
sizeX、sizeY表示矩形的size
hBmp表示位图句柄
*************************************************/
GameButton CreateGameButton(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp, int status);

/*************************************************
Fucntion : 创建单个地形
Parameter:
posX、posY表示矩形左上角的坐标
sizeX、sizeY表示矩形的size
*************************************************/

double rand01();
int randint(int l, int r);
bool point_on_seg(int x, int l, int r);
bool seg_collision(pii a, pii b);
void seg_difference(pii seg, pii tmp, vector<pii>& res);
int sgn(int x);
double torad(double deg);
double Beizer(double _p0, double _p1, double _p2, double _p3, double t);

struct myMSG {
    int type;
    WPARAM wParam;
    int cTime;
    myMSG(int _type, WPARAM _wParam, int _cTime):
        type(_type),
        wParam(_wParam),
        cTime(_cTime) {

    }
};

#endif // !GLOBAL_H
