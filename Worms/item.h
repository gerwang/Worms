#ifndef ITEM_H
#define ITEM_H

#include "global.h"


//// 全局变量

// 声明位图句柄
HBITMAP m_BackBufferBmp[2];
HBITMAP m_hBackgroundBmp;
HBITMAP m_hHeroBmp;
HBITMAP m_hGameStartButtonBmp;
HBITMAP m_hTerrainBmp;
HBITMAP m_hArrowBmp;
HBITMAP m_hSeaBmp;
HBITMAP m_hExpBmp;
HBITMAP m_hBoxBmp;
HBITMAP m_hServerBmp, m_hClientBmp;

HBITMAP m_hADDBmp, m_hBACKBmp, m_hGENBmp, m_hLAUNCHBmp, m_hRETRYBmp, m_hWORMSBmp;

HWND m_hwnd;

Image* pWormpng[3];
Image* pJumppng;
Image* pAimpng;
Image* pFootpng;
Image* pWeaponerpng[2];
Image* pWeapon[2];
int g_Time;
int g_Limit;
int SwitchTime;

Bazooka bazooka;
Grenade grenade;
PointF m_boxpoint;

vector<Box>boxes;

// 声明英雄、建筑、地形、按钮
Hero          m_hero[16];
int hero_num, curheroID;
Terrain       m_terrain;
GameButton    m_gameStartButton;

GameButton	m_playAgainButton;
GameButton  m_backToMenuButton;

GameButton	m_reGenerateButton;
GameButton	m_launchButton;

GameButton m_wormNumButton;
GameButton m_playerTypeButton[4];

GameButton m_serverbutton, m_clientbutton;

AI_Simulator ai;

// 声明游戏状态
GameStatus gameStatus;

MIB_IPADDRTABLE* pIpAddrTable;
DWORD dwSize;
char ip_str[100];
unsigned g_seed;
sockaddr_in myAddr, itsAddr;
SOCKET m_sock, serv_sock;
SOCKADDR fromAddr;
int nSize = sizeof(SOCKADDR);
char Buffer[BUF_LEN];
queue<myMSG>msgqueue;
int random_flag;

#endif