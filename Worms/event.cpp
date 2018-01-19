#include "event.h"
#include "global.h"
#include "item.h"
#define _CRT_SECURE_NO_WARNINGS
void Render_Button(const GameButton& button, HDC hdcBmp, HDC hdcBuffer);

VOID Init(HWND hWnd, WPARAM wParam, LPARAM lParam) {

    pIpAddrTable = (MIB_IPADDRTABLE*)malloc(sizeof(MIB_IPADDRTABLE));
    FILE* fin = fopen("ip.txt", "r");
    fscanf(fin, "%s", ip_str);
    fclose(fin);

    if(GetIpAddrTable(pIpAddrTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER) {
        free(pIpAddrTable);
        pIpAddrTable = (MIB_IPADDRTABLE*)malloc(dwSize);
    }
    GetIpAddrTable(pIpAddrTable, &dwSize, 0);

    m_hwnd = hWnd;
    m_hTerrainBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
                               MAKEINTRESOURCE(IDB_TERR));
    // 加载背景位图
    m_BackBufferBmp[0] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
                                    MAKEINTRESOURCE(IDB_WINDOWS));
    m_BackBufferBmp[1] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
                                    MAKEINTRESOURCE(IDB_HEHE));
    srand((unsigned)time(0));
    m_hBackgroundBmp = m_BackBufferBmp[rand() % 2];
    // 加载Hero位图
    m_hHeroBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
                            MAKEINTRESOURCE(IDB_HERO));

    // 加载开始游戏按钮位图
    m_hGameStartButtonBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
                                       MAKEINTRESOURCE(IDB_START));

    m_hArrowBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
                             MAKEINTRESOURCE(IDB_ARROW));

    m_hSeaBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
                           MAKEINTRESOURCE(IDB_SEA));
    m_hExpBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
                           MAKEINTRESOURCE(IDB_EXP));
    m_hBoxBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
                           MAKEINTRESOURCE(IDB_BOX));
    m_hADDBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
                           MAKEINTRESOURCE(IDB_ADD));
    m_hBACKBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
                            MAKEINTRESOURCE(IDB_BACK));
    m_hGENBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
                           MAKEINTRESOURCE(IDB_GEN));
    m_hWORMSBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
                             MAKEINTRESOURCE(IDB_WORMS));
    m_hRETRYBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
                             MAKEINTRESOURCE(IDB_RETRY));
    m_hLAUNCHBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
                              MAKEINTRESOURCE(IDB_LAUNCH));
    m_hServerBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
                              MAKEINTRESOURCE(IDB_SERVER));
    m_hClientBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
                              MAKEINTRESOURCE(IDB_CLIENT));
    pWormpng[0] = Image::FromFile(L"res\\png\\output_68_54.png");
    pWormpng[1] = Image::FromFile(L"res\\png\\output_59_70.png");
    pWormpng[2] = Image::FromFile(L"res\\png\\output_57_63.png");
    pJumppng	= Image::FromFile(L"res\\png\\jump.png");
    pAimpng		= Image::FromFile(L"res\\png\\aim.png");
    pFootpng	= Image::FromFile(L"res\\png\\foot.png");
    pWeaponerpng[0] = Image::FromFile(L"res\\png\\bazookaer.png");
    pWeaponerpng[1] = Image::FromFile(L"res\\png\\grenadeer.png");
    pWeapon[0] = Image::FromFile(L"res\\png\\bazooka.png");
    pWeapon[1] = Image::FromFile(L"res\\png\\grenade.png");

    // 创建地形

    // 创建游戏开始按钮
    m_gameStartButton = CreateGameButton(570, 320, GAME_START_BUTTON_SIZE_X, GAME_START_BUTTON_SIZE_Y, m_hGameStartButtonBmp, 0);
    m_playAgainButton = CreateGameButton(200, 200, GAME_START_BUTTON_SIZE_X, GAME_START_BUTTON_SIZE_Y, m_hRETRYBmp, 0);
    m_backToMenuButton = CreateGameButton(200, 300, GAME_START_BUTTON_SIZE_X, GAME_START_BUTTON_SIZE_Y, m_hBACKBmp, 0);
    m_reGenerateButton = CreateGameButton(1000, 300, GAME_START_BUTTON_SIZE_X, GAME_START_BUTTON_SIZE_Y, m_hGENBmp, 0);
    m_launchButton = CreateGameButton(1000, 200, GAME_START_BUTTON_SIZE_X, GAME_START_BUTTON_SIZE_Y, m_hLAUNCHBmp, 0);

    m_wormNumButton = CreateGameButton(1000, 400, GAME_START_BUTTON_SIZE_X, GAME_START_BUTTON_SIZE_Y, m_hWORMSBmp, 0);
    for(int i = 0; i < 4; i++) {
        m_playerTypeButton[i] = CreateGameButton(100, 100 * (i + 1), GAME_START_BUTTON_SIZE_X, GAME_START_BUTTON_SIZE_Y, m_hADDBmp, 0);
    }
    m_serverbutton = CreateGameButton(570, 420, GAME_START_BUTTON_SIZE_X, GAME_START_BUTTON_SIZE_Y, m_hServerBmp, 0);
    m_clientbutton = CreateGameButton(570, 520, GAME_START_BUTTON_SIZE_X, GAME_START_BUTTON_SIZE_Y, m_hClientBmp, 0);
    //设定初始游戏状态
    gameStatus.status = 0;
    gameStatus.hBmp = m_hBackgroundBmp;
    gameStatus.maxWeapon = 2;
    gameStatus.curWeapon = 0;
    PlaySound(L"res\\wav\\menu.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

VOID Render_Server(HWND hWnd) {
    PAINTSTRUCT ps;
    HDC hdc;
    hdc = BeginPaint(hWnd, &ps);
    HDC hdcBuffer, hdcBmp;
    HBITMAP cptBmp;
    hdcBuffer = CreateCompatibleDC(hdc);
    hdcBmp = CreateCompatibleDC(hdc);
    cptBmp = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);
    SelectObject(hdcBuffer, cptBmp);
    SelectObject(hdcBmp, gameStatus.hBmp);
    BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBmp, 0, 0, SRCCOPY);


    Render_Button(m_reGenerateButton, hdcBmp, hdcBuffer);

    Render_Button(m_wormNumButton, hdcBmp, hdcBuffer);

    Render_Button(m_launchButton, hdcBmp, hdcBuffer);

    for(int i = 1; i < 4; i++) {
        Render_Button(m_playerTypeButton[i], hdcBmp, hdcBuffer);
    }

    {
        Bitmap bmp(WNDWIDTH, WNDHEIGHT);
        Graphics g_bmp(&bmp);
        HDC hdcplus = g_bmp.GetHDC();

        SelectObject(hdcplus, GetStockObject(NULL_PEN));
        HBRUSH terrianBrush = CreatePatternBrush(m_hTerrainBmp);
        SelectObject(hdcplus, terrianBrush);
        for(int i = 0; i < TERRAIN_WIDTH; i++) {
            vector<pii>& tmp = m_terrain.terrains[i];
            for(auto seg : tmp) {
                Rectangle(hdcplus, i, seg.first - 10, i + 2, seg.second);
            }
        }
        DeleteObject(terrianBrush);

        g_bmp.ReleaseHDC(hdcplus);

        Graphics graphics(hdcBuffer);
        const RectF t_window(300, 300, 640, 360);
        Pen pen(Color::Orange, 5);
        graphics.DrawRectangle(&pen, t_window);
        graphics.DrawImage(&bmp, t_window);
        Font font(L"微软雅黑", 20);
        SolidBrush brush(Color::Black);
        TCHAR str[100], name[3][20] = {L"酱油", L"人类", L"人工智能"};
        wsprintf(str, L"一共有%d个玩家\n每个玩家有%d条虫子\n玩家0是网络玩家\n玩家1是%ls\n玩家2是%ls\n玩家3是%ls\n",
                 gameStatus.playernum,
                 gameStatus.wormsperplayer,
                 name[m_playerTypeButton[1].status],
                 name[m_playerTypeButton[2].status],
                 name[m_playerTypeButton[3].status]);
        graphics.DrawString(str, _tcslen(str), &font, PointF(400, 50), &brush);
    }

    BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBuffer, 0, 0, SRCCOPY);
    DeleteObject(cptBmp);
    DeleteDC(hdcBuffer);
    DeleteDC(hdcBmp);
    EndPaint(hWnd, &ps);
}

VOID Render_Client(HWND hWnd) {
    PAINTSTRUCT ps;
    HDC hdc;
    hdc = BeginPaint(hWnd, &ps);
    HDC hdcBuffer, hdcBmp;
    HBITMAP cptBmp;
    hdcBuffer = CreateCompatibleDC(hdc);
    hdcBmp = CreateCompatibleDC(hdc);
    cptBmp = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);
    SelectObject(hdcBuffer, cptBmp);
    SelectObject(hdcBmp, gameStatus.hBmp);
    BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBmp, 0, 0, SRCCOPY);

    {
        Bitmap bmp(WNDWIDTH, WNDHEIGHT);
        Graphics g_bmp(&bmp);
        HDC hdcplus = g_bmp.GetHDC();

        SelectObject(hdcplus, GetStockObject(NULL_PEN));
        HBRUSH terrianBrush = CreatePatternBrush(m_hTerrainBmp);
        SelectObject(hdcplus, terrianBrush);
        for(int i = 0; i < TERRAIN_WIDTH; i++) {
            vector<pii>& tmp = m_terrain.terrains[i];
            for(auto seg : tmp) {
                Rectangle(hdcplus, i, seg.first - 10, i + 2, seg.second);
            }
        }
        DeleteObject(terrianBrush);

        g_bmp.ReleaseHDC(hdcplus);

        Graphics graphics(hdcBuffer);
        const RectF t_window(300, 300, 640, 360);
        Pen pen(Color::Orange, 5);
        graphics.DrawRectangle(&pen, t_window);
        graphics.DrawImage(&bmp, t_window);
        Font font(L"微软雅黑", 20);
        SolidBrush brush(Color::Black);
        TCHAR str[100], name[3][20] = {L"酱油", L"人类", L"人工智能"};
        wsprintf(str, L"一共有%d个玩家\n每个玩家有%d条虫子\n玩家0是本机客户端\n玩家1是%ls\n玩家2是%ls\n玩家3是%ls\n",
                 gameStatus.playernum,
                 gameStatus.wormsperplayer,
                 name[m_playerTypeButton[1].status],
                 name[m_playerTypeButton[2].status],
                 name[m_playerTypeButton[3].status]);
        graphics.DrawString(str, _tcslen(str), &font, PointF(400, 50), &brush);
    }

    BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBuffer, 0, 0, SRCCOPY);
    DeleteObject(cptBmp);
    DeleteDC(hdcBuffer);
    DeleteDC(hdcBmp);
    EndPaint(hWnd, &ps);
}

VOID Render_OnlineEnd(HWND hWnd) {
    PAINTSTRUCT ps;
    HDC hdc;
    hdc = BeginPaint(hWnd, &ps);
    HDC hdcBuffer, hdcBmp;
    HBITMAP cptBmp;
    hdcBuffer = CreateCompatibleDC(hdc);
    hdcBmp = CreateCompatibleDC(hdc);
    cptBmp = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);
    SelectObject(hdcBuffer, cptBmp);
    SelectObject(hdcBmp, gameStatus.hBmp);
    BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBmp, 0, 0, SRCCOPY);

    TCHAR res[50];
    if(gameStatus.winplayer == -1) {
        wcscpy_s(res, L"游戏平局");
    } else {
        wsprintf(res, L"玩家%d获得了胜利", gameStatus.winplayer);
    }
    {
        Font font(L"微软雅黑", 36);
        SolidBrush brush(Color::AliceBlue);
        Graphics graphics(hdcBuffer);
        graphics.DrawString(res, _tcslen(res), &font, PointF(500, 500), &brush);
    }

    BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBuffer, 0, 0, SRCCOPY);
    DeleteObject(cptBmp);
    DeleteDC(hdcBuffer);
    DeleteDC(hdcBmp);
    EndPaint(hWnd, &ps);
}

VOID Render(HWND hWnd) {
    //判断游戏状态
    switch(gameStatus.status) {
    case 0:
        //绘制开始界面
        Render_Start(hWnd);
        break;
    case 1:
        //绘制游戏界面
        Render_Game(hWnd);
        break;
    case 2:
        Render_Generate(hWnd);
        break;
    case 3:
        Render_End(hWnd);
        break;
    case 4:
        Render_Server(hWnd);
        break;
    case 5:
        Render_Client(hWnd);
        break;
    case 6:
        Render_OnlineEnd(hWnd);
        break;
    default:
        break;
    }
}

void Render_Button(const GameButton& button, HDC hdcBmp, HDC hdcBuffer) {
    SelectObject(hdcBmp, button.hBmp);
    TransparentBlt(hdcBuffer, button.pos.x, button.pos.y, button.size.cx, button.size.cy,
                   hdcBmp, 0, 0,
                   button.size.cx, button.size.cy, RGB(255, 255, 255));
}

VOID Render_Start(HWND hWnd) {
    PAINTSTRUCT ps;
    HDC hdc;
    // 开始绘制
    hdc = BeginPaint(hWnd, &ps);

    HDC	hdcBmp, hdcBuffer;
    HBITMAP	cptBmp;

    cptBmp = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);
    hdcBmp = CreateCompatibleDC(hdc);
    hdcBuffer = CreateCompatibleDC(hdc);


    // 绘制背景到缓存
    SelectObject(hdcBuffer, cptBmp);
    SelectObject(hdcBmp, gameStatus.hBmp);
    BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
           hdcBmp, 0, 0, SRCCOPY);

    // 绘制开始按钮
    Render_Button(m_gameStartButton, hdcBmp, hdcBuffer);
    Render_Button(m_serverbutton, hdcBmp, hdcBuffer);
    Render_Button(m_clientbutton, hdcBmp, hdcBuffer);

    {
        Graphics graphics(hdcBuffer);
        TCHAR szDist[100];
        unsigned ip = pIpAddrTable->table[1].dwAddr;
        unsigned char bytes[4];
        bytes[0] = ip & 0xFF;
        bytes[1] = (ip >> 8) & 0xFF;
        bytes[2] = (ip >> 16) & 0xFF;
        bytes[3] = (ip >> 24) & 0xFF;
        TCHAR buffer[100];
        for(int i = strlen(ip_str); i >= 0; i--) {
            buffer[i] = (TCHAR)ip_str[i];
        }
        wsprintf(szDist, L"你的ip是: %d.%d.%d.%d\n对方ip是: %s\n", bytes[0], bytes[1], bytes[2], bytes[3], buffer);
        graphics.DrawString(szDist, _tcslen(szDist), &Font(L"微软雅黑", 16), PointF(550, 200), &SolidBrush(Color::Black));
    }

    // 最后将所有的信息绘制到屏幕上
    BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBuffer, 0, 0, SRCCOPY);

    // 回收资源所占的内存
    DeleteObject(cptBmp);
    DeleteDC(hdcBuffer);
    DeleteDC(hdcBmp);

    // 结束绘制
    EndPaint(hWnd, &ps);
}

VOID Render_Game(HWND hWnd) {
    PAINTSTRUCT ps;
    HDC hdc;
    // 开始绘制
    hdc = BeginPaint(hWnd, &ps);

    HDC	hdcBmp, hdcBuffer;
    HBITMAP	cptBmp;

    cptBmp = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);
    hdcBmp = CreateCompatibleDC(hdc);
    hdcBuffer = CreateCompatibleDC(hdc);


    // 绘制背景到缓存
    SelectObject(hdcBuffer, cptBmp);
    SelectObject(hdcBmp, gameStatus.hBmp);
    BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
           hdcBmp, 0, 0, SRCCOPY);

    // 绘制地形
    SelectObject(hdcBuffer, GetStockObject(NULL_PEN));
    HBRUSH terrianBrush = CreatePatternBrush(m_hTerrainBmp);
    SelectObject(hdcBuffer, terrianBrush);
    for(int i = 0; i < TERRAIN_WIDTH; i++) {
        vector<pii>& tmp = m_terrain.terrains[i];
        for(auto seg : tmp) {
            Rectangle(hdcBuffer, i, seg.first - 10, i + 2, seg.second);
        }
    }
    DeleteObject(terrianBrush);

    {
        //绘制海洋
        SelectObject(hdcBmp, m_hSeaBmp);
        BitBlt(hdcBuffer, 0, gameStatus.sealevel, 1280, gameStatus.sealevel + 30, hdcBmp, 0, 0, SRCCOPY);
        HBRUSH seabrush = CreateSolidBrush(RGB(35, 94, 220));
        HBRUSH orig = (HBRUSH)SelectObject(hdcBuffer, seabrush);
        Rectangle(hdcBuffer, 0, gameStatus.sealevel + 30, 1280, WNDHEIGHT);
        SelectObject(hdcBuffer, orig);
        DeleteObject(seabrush);
    }

    // 绘制Hero到缓存
    {
        Graphics graphics(hdcBuffer);
        for(int i = 0; i < hero_num; i++) {
            Hero& tmp = m_hero[i];
            if(tmp.hp > 0) {
                //绘制hp

                TCHAR hpstr[20];
                wsprintf(hpstr, L"%d", tmp.hp);
                PointF center = CenterPoint(tmp.rect);
                center.Y -= 55;
                center.X -= 26;
                graphics.DrawString(hpstr, _tcslen(hpstr),  &Font(L"微软雅黑", 16), center, &SolidBrush(teamcolor[tmp.team]));
                if(i == curheroID) {
                    if(isShowWeapon()) {
                        continue;
                    }
                }

                center = CenterPoint(tmp.rect);
                Matrix mtx;
                REAL scale = 0.8f;
                mtx.Translate(center.X, center.Y);
                mtx.Scale(tmp.direction * scale, scale);
                mtx.Translate(-center.X, -center.Y);
                graphics.SetTransform(&mtx);
                if(m_terrain.isair(tmp.rect)) {
                    graphics.DrawImage(pJumppng, tmp.rect.GetLeft(), tmp.rect.GetTop());
                } else {
                    graphics.DrawImage(pWormpng[tmp.frametype], tmp.rect.GetLeft() - 20, tmp.rect.GetTop() - 10, tmp.curFrameIndex * tmp.FrameX, 0, tmp.FrameX, tmp.FrameY, UnitPixel);
                }
                graphics.ResetTransform();
            }
        }
    }
    {
        //绘制箱子
        SelectObject(hdcBmp, m_hBoxBmp);
        for(auto& box : boxes) {
            Point pos;
            box.rect.GetLocation(&pos);
            TransparentBlt(hdcBuffer, pos.X, pos.Y, 39, 30, hdcBmp, 0, 0, 39, 30, RGB(0, 0, 0));
        }
    }
    {
        Hero& cur = m_hero[curheroID];
        if(isShowWeapon()) {
            int length = 100;
            double rad = torad((double)gameStatus.WSangle);
            REAL vx = (REAL)(length * cos(rad) * cur.direction), vy = (REAL)(length * sin(rad));
            PointF center = CenterPoint(cur.rect);
            center.X += vx;
            center.Y += vy;
            Graphics graphics(hdcBuffer);
            REAL scale = 1.f;
            graphics.TranslateTransform(center.X, center.Y);
            graphics.ScaleTransform(scale * cur.direction, scale);
            graphics.RotateTransform((REAL)gameStatus.WSangle);
            graphics.DrawImage(pAimpng, -(pAimpng->GetWidth() / 2.f), -(pAimpng->GetHeight() / 2.f));
            graphics.ResetTransform();

            center = CenterPoint(cur.rect);
            scale = 0.75f;
            Matrix mtx;
            graphics.TranslateTransform(center.X, center.Y);
            graphics.ScaleTransform(cur.direction * scale, scale);
            graphics.DrawImage(pFootpng, -30, 10);
            graphics.TranslateTransform(0, 20);
            graphics.RotateTransform((REAL)gameStatus.WSangle);
            graphics.DrawImage(pWeaponerpng[gameStatus.curWeapon], -35, -50);
            graphics.ResetTransform();

            if(gameStatus.SPACEpressed == true) {
                REAL dx = vx * gameStatus.strength / 100;
                REAL dy = vy * gameStatus.strength / 100;
                LinearGradientBrush brush(PointF(center.X, center.Y), PointF(center.X + vx, center.Y + vy), Color::Green, Color::Red);
                Pen pen(&brush, 20);
                pen.SetEndCap(LineCapArrowAnchor);
                graphics.DrawLine(&pen, PointF(center.X, center.Y), PointF(center.X + dx, center.Y + dy));
            }
        }
    }

    if(gameStatus.launched == true && gameStatus.boomed == false) {
        switch (gameStatus.curWeapon) {
        case 0: {
            PointF center(CenterPoint(bazooka.rect));
            Graphics graphics(hdcBuffer);
            graphics.TranslateTransform(center.X, center.Y);
            double rad = atan2(bazooka.vel.y, bazooka.vel.x);
            graphics.RotateTransform((REAL)(rad * 180 / M_PI));
            graphics.DrawImage(pWeapon[0], -(REAL)(pWeapon[0]->GetWidth() / 2.f), -(REAL)(pWeapon[0]->GetHeight() / 2.f));
        }
        break;
        case 1:	{
            PointF center(CenterPoint(grenade.rect));
            Graphics graphics(hdcBuffer);
            graphics.TranslateTransform(center.X, center.Y);
            graphics.RotateTransform((REAL)(g_Time * 7 % 360));
            graphics.DrawImage(pWeapon[1], -(REAL)(pWeapon[1]->GetWidth() / 2.f), -(REAL)(pWeapon[1]->GetHeight() / 2.f));
            graphics.ResetTransform();
            TCHAR timestr[20];
            wsprintf(timestr, L"%d", 300 - (g_Time - grenade.time_flag));
            graphics.DrawString(timestr, _tcslen(timestr), &Font(L"微软雅黑", 16), center, &SolidBrush(teamcolor[m_hero[curheroID].team]));
        }
        break;
        default:
            break;
        }
    }

    {
        //绘制风向标
        Graphics graphics(hdcBuffer);
        graphics.TranslateTransform(100, 650);
        LinearGradientBrush brush(Point(0, 0), Point(100, 0), Color::Red, Color::Blue);
        Pen pen(&brush, 15);
        pen.SetEndCap(LineCapArrowAnchor);
        graphics.DrawLine(&pen, Point(0, 0), Point(100 * gameStatus.windvel / MAX_WIND, 0));
    }

    {
        //绘制当前玩家箭头
        Hero& cur = m_hero[curheroID];
        SelectObject(hdcBmp, m_hArrowBmp);
        TransparentBlt(hdcBuffer, cur.rect.GetLeft() - 10, cur.rect.GetTop() - 100, 40, 75, hdcBmp, (g_Time / 3) % 7 * 68, 0, 68, 114, RGB(0, 0, 0));
    }

    {
        //绘制爆炸效果
        if(gameStatus.boomtime && g_Time < gameStatus.boomtime + 16 * 3) {//为了防止一开局放一次动画
            int curFrame = (g_Time - gameStatus.boomtime) / 3;
            int x = curFrame % 4, y = curFrame / 4;
            SelectObject(hdcBmp, m_hExpBmp);
            PointF pos;
            if(m_boxpoint.X != (1 << 30)) {
                pos = m_boxpoint;
            } else {
                switch (gameStatus.curWeapon) {
                case 0:
                    pos = CenterPoint(bazooka.rect);
                    break;
                case 1:
                    pos = CenterPoint(grenade.rect);
                    break;
                default:
                    break;
                }
            }
            TransparentBlt(hdcBuffer, (int)pos.X - 50, (int)pos.Y - 50, 100, 100, hdcBmp, x * 64, y * 64, 64, 64, RGB(255, 255, 255));
        } else {
            m_boxpoint.X = 1 << 30;
        }
    }

    // 绘制分数
    TCHAR	szDist[100];
    TCHAR hehe[100];
    for(int i = strlen(Buffer); i >= 0; i--) {
        hehe[i] = (TCHAR)Buffer[i];
    }
    wsprintf(szDist, L"%d %d %s", g_Time, random_flag, hehe);
    SetTextColor(hdcBuffer, RGB(0, 0, 0));
    SetBkMode(hdcBuffer, TRANSPARENT);
    //wsprintf(szDist, _T("0 : 0"));
    TextOut(hdcBuffer, WNDWIDTH - 200, 15, szDist, _tcslen(szDist));

    // 最后将所有的信息绘制到屏幕上
    BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBuffer, 0, 0, SRCCOPY);

    // 回收资源所占的内存
    DeleteObject(cptBmp);
    DeleteDC(hdcBuffer);
    DeleteDC(hdcBmp);

    // 结束绘制
    EndPaint(hWnd, &ps);
}

VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    if(gameStatus.OnlineGame) {
        while(!msgqueue.empty()) {
            myMSG msg = msgqueue.front();
            if(msg.cTime == g_Time) {
                msgqueue.pop();
                switch (msg.type) {
                case 0:
                    ProcessKeyDown(msg.wParam);
                    break;
                case 1:
                    ProcessKeyUp(msg.wParam);
                    break;
                default:
                    break;
                }
            } else {
                break;
            }
        }
        if(g_Time >= g_Limit) {
            return;
        }
        if(g_Time + NET_ELAPSE > g_Limit) {
            Sleep(20);
        }
        if(g_Time == 1) {
            srand(g_seed);
        }
    }
    g_Time++;
    if(g_Time % NET_ELAPSE == 0) {
        random_flag = rand();
    }
    if(gameStatus.OnlineGame) {
        if(g_Time % NET_ELAPSE == 0) {
            char sbuf[100];
            if(isLocalMove()) {
                sprintf(sbuf, "h%d", g_Time);
            } else {
                sprintf(sbuf, "h%d", g_Time + NET_ELAPSE * NET_NUM);
            }
            sendstring(sbuf);
        }
    }
    if(!isHumanMove()) {
        ai.doSomedicision();
    }
    HeroUpdate();
    TerrianUpdate();
    GameStatusUpdate();
    InvalidateRect(hWnd, NULL, FALSE);
}

void GenerateGame() {
    hero_num = gameStatus.alivehero = gameStatus.wormsperplayer * gameStatus.playernum;
    for(int i = 0; i < gameStatus.alivehero; i++) {
        memset(&m_hero[i], 0, sizeof(Hero));
        m_hero[i].Init(randint(30, TERRAIN_WIDTH - 30), 0, 30, 30);
        int teamnumber = i % gameStatus.playernum;
        for(int j = 0, tmp = 0; j < 4; j++) {
            if(m_playerTypeButton[j].status != 0) {
                if(tmp == teamnumber) {
                    m_hero[i].team = j;
                    break;
                }
                tmp++;
            }
        }
    }
    curheroID = 0;
    m_terrain.BeizerGenerate();
    gameStatus.sealevel = WNDHEIGHT;
    gameStatus.boomtime = g_Time = 0;
    boxes.clear();
    memset(&bazooka, 0, sizeof(bazooka));
    memset(&grenade, 0, sizeof(grenade));
}

VOID HeroUpdate() {
    for(int i = 0; i < hero_num; i++) {
        Hero& tmp = m_hero[i];
        if(tmp.hp > 0) {
            if(m_terrain.isair(tmp.rect) || tmp.flytime) {
                if(i == curheroID) {
                    if(gameStatus.RETURNpressed == true) {
                        tmp.vel.x = tmp.direction * JUMP_VEL_X;
                        gameStatus.RETURNpressed = false;
                    }
                }
                tmp.vel.y += 3;
                POINT rvel;
                rvel.x = rvel.y = 1 << 30;
                int dist = abs(tmp.vel.x);
                int sign = sgn(tmp.vel.x);
                for(int vx = dist; vx >= 0; vx--) {
                    Rect next_rect = tmp.rect;
                    next_rect.Offset(vx * sign, 0);
                    if(!m_terrain.RectCollision(next_rect)) {
                        tmp.rect = next_rect;
                        rvel.x = vx * sign;
                        break;
                    }
                }
                dist = abs(tmp.vel.y);
                sign = sgn(tmp.vel.y);
                for(int vy = dist; vy >= 0; vy--) {
                    Rect next_rect = tmp.rect;
                    next_rect.Offset(0, vy * sign);
                    if(!m_terrain.RectCollision(next_rect)) {
                        tmp.rect = next_rect;
                        rvel.y = vy * sign;
                        break;
                    }
                }
                if(tmp.flytime) {
                    if(rvel.x != tmp.vel.x) {
                        tmp.vel.x *= -1;
                        tmp.vel.x += randint(-1, 1);
                    }
                    if(rvel.y != tmp.vel.y) {
                        tmp.vel.y *= -1;
                        tmp.vel.y += randint(-1, 1);
                    }
                    /*
                    if(rvel.x != tmp.vel.x && rvel.y == tmp.vel.y) {
                        tmp.vel.x *= -1;
                    } else if(rvel.y != tmp.vel.y && rvel.x == tmp.vel.x) {
                        tmp.vel.y *= -1;
                    } else if(rvel.x != tmp.vel.x && rvel.y != tmp.vel.y) {
                        Rect __tmp_rect = tmp.rect;
                        int height[TERRAIN_WIDTH], origx;
                        {
                            __tmp_rect.Offset(1, 0);
                            if(m_terrain.RectCollision(__tmp_rect)) {
                                origx = tmp.rect.GetRight();
                            } else {
                                origx = tmp.rect.GetLeft();
                            }
                        }
                        height[origx] = m_terrain.minHeight(tmp.rect) + tmp.rect.GetBottom();
                        int lb = origx - 1, rb = origx + 1;
                        const int M_LEN = 5;
                        __tmp_rect = tmp.rect;
                        for(;;) {
                            if(lb < 1 || lb < origx - M_LEN) {
                                break;
                            }
                            __tmp_rect.Offset(-1, 0);
                            int dh = m_terrain.minHeight(__tmp_rect);
                            if(dh == (1 << 30)) {
                                break;
                            }
                            __tmp_rect.Offset(0, dh);
                            if(m_terrain.isair(__tmp_rect)) {
                                break;
                            }
                            height[origx] = __tmp_rect.GetBottom();
                            lb--;
                        }
                        __tmp_rect = tmp.rect;
                        for(;;) {
                            if(rb > TERRAIN_WIDTH - 2 || rb > origx + M_LEN) {
                                break;
                            }
                            __tmp_rect.Offset(1, 0);
                            int dh = m_terrain.minHeight(__tmp_rect);
                            if(dh == (1 << 30)) {
                                break;
                            }
                            __tmp_rect.Offset(0, dh);
                            if(m_terrain.isair(__tmp_rect)) {
                                break;
                            }
                            height[origx] = __tmp_rect.GetBottom();
                            rb++;
                        }
                        double k;
                        if(lb + 1 == rb - 1) {
                            k = 0;
                        } else {
                            k = atan2(height[rb - 1] - height[lb + 1], (rb - 1) - (lb + 1));
                        }
                        double len = sqrt(SQR(tmp.vel.x) + SQR(tmp.vel.y));
                        double rad = atan2(tmp.vel.y, tmp.vel.x);
                        rad = 2 * k - rad;
                        tmp.vel.x = (int)floor(len * cos(rad) + 0.5);
                        tmp.vel.y = (int)floor(len * sin(rad) + 0.5);
                    }
                    */
                    if(g_Time > tmp.flytime + 10 || (tmp.vel.x == 0 && tmp.vel.y == 0)) {
                        tmp.flytime = 0;
                    }
                }
            } else {
                tmp.vel.x = tmp.vel.y = 0;
                if(i == curheroID) {
                    if(gameStatus.ADpressed == true) {
                        Rect next_rect = tmp.rect;
                        next_rect.Offset(tmp.direction * HERO_VEL_X, 0);
                        int height = m_terrain.minHeight(next_rect);
                        if(height != (1 << 30)) {
                            next_rect.Offset(0, height);
                            tmp.rect = next_rect;
                            if(height >= 1) {
                                if(tmp.frametype != 2) {
                                    tmp.SetFrame(2);
                                } else {
                                    tmp.curFrameIndex = (tmp.curFrameIndex + 1) % tmp.maxFrameSize;
                                }
                            } else if(height <= -1) {
                                if(tmp.frametype != 1) {
                                    tmp.SetFrame(1);
                                } else {
                                    tmp.curFrameIndex = (tmp.curFrameIndex + 1) % tmp.maxFrameSize;
                                }
                            } else {
                                if(tmp.frametype != 0) {
                                    tmp.SetFrame(0);
                                } else {
                                    tmp.curFrameIndex = (tmp.curFrameIndex + 1) % tmp.maxFrameSize;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
// TODO
//更新英雄位置、动作

VOID TerrianUpdate() {
    // TODO
    //更新地形
}

int getnextheroID(int cur) {
    int nxt = cur;
    do {
        nxt = (nxt + 1) % hero_num;
        if(m_hero[nxt].hp > 0) {
            return nxt;
        }
    } while(nxt != cur);
    return cur;
}

void zhuangB() {
    Graphics graphics(m_hwnd);
    Bitmap bmp(L"res\\png\\effect.png");
    graphics.DrawImage(&bmp, 0, 0, 1280, 720);

    Hero& cur = m_hero[curheroID];
    int length = 100;
    double rad = torad((double)gameStatus.WSangle);
    REAL vx = (REAL)(length * cos(rad) * cur.direction), vy = (REAL)(length * sin(rad));
    PointF center = CenterPoint(cur.rect);
    center.X += vx;
    center.Y += vy;
    REAL scale = 1.f;
    graphics.TranslateTransform(center.X, center.Y);
    graphics.ScaleTransform(scale * cur.direction, scale);
    graphics.RotateTransform((REAL)gameStatus.WSangle);
    graphics.DrawImage(pAimpng, -(pAimpng->GetWidth() / 2.f), -(pAimpng->GetHeight() / 2.f));
    graphics.ResetTransform();

    center = CenterPoint(cur.rect);
    scale = 0.75f;
    Matrix mtx;
    graphics.TranslateTransform(center.X, center.Y);
    graphics.ScaleTransform(cur.direction * scale, scale);
    graphics.DrawImage(pFootpng, -30, 10);
    graphics.TranslateTransform(0, 20);
    graphics.RotateTransform((REAL)gameStatus.WSangle);
    graphics.DrawImage(pWeaponerpng[gameStatus.curWeapon], -35, -50);
    graphics.ResetTransform();

    REAL dx = vx * gameStatus.strength / 100;
    REAL dy = vy * gameStatus.strength / 100;
    LinearGradientBrush brush(PointF(center.X, center.Y), PointF(center.X + vx, center.Y + vy), Color::Green, Color::Red);
    Pen pen(&brush, 20);
    pen.SetEndCap(LineCapArrowAnchor);
    graphics.DrawLine(&pen, PointF(center.X, center.Y), PointF(center.X + dx, center.Y + dy));

    mciSendString(L"stop res\\wav\\effect.wav", NULL, 0, NULL);
    mciSendString(L"play res\\wav\\effect.wav", NULL, 0, NULL);
    PlaySound(L"res\\wav\\effectbgm.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    Sleep(4000);
}

VOID GameStatusUpdate() {
    // TODO
    //更新游戏状态
    {
        int res = isGameEnd();
        if(res != -2) {
            gameStatus.winplayer = res;
            if(gameStatus.OnlineGame) {
                gameStatus.status = 6;
                char sbuf[100];
                sprintf(sbuf, "h%d", g_Time + 1000);
                sendstring(sbuf);
            } else {
                gameStatus.status = 3;
            }
            KillTimer(m_hwnd, TIMER_ID);
            PlaySound(L"res\\wav\\menu.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
            return;
        }
    }
    gameStatus.UpdateAngle();
    gameStatus.Updatestrength();
    {
        for(int i = 0; i < hero_num; i++) {
            Hero& tmp = m_hero[i];
            if(tmp.hp > 0) {
                if(OutOfBound(tmp.rect)) {
                    KillWorm(i);
                    mciSendString(L"stop res\\wav\\wade.wav", NULL, 0, NULL);
                    mciSendString(L"play res\\wav\\wade.wav", NULL, 0, NULL);
                    continue;
                }
            }
            for(int j = boxes.size() - 1; j >= 0; j--) {
                Box& box = boxes[j];
                if(OutOfBound(box.rect)) {
                    boxes.erase(boxes.begin() + j);
                } else {
                    if(box.rect.IntersectsWith(tmp.rect)) {
                        if(rand01() > 0.5) {
                            tmp.hp += randint(10, 30);
                        } else {
                            PointF center(CenterPoint(box.rect));
                            CauseDamage((int)center.X, (int)center.Y, 50, randint(10, 15));
                            m_boxpoint = CenterPoint(box.rect);
                            gameStatus.boomtime = g_Time;
                        }
                        boxes.erase(boxes.begin() + j);
                    }
                }
            }
        }
    }
    if(m_hero[curheroID].hp <= 0 || (gameStatus.launched == true && gameStatus.boomed == true)) {
        //进入下一个英雄
        PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC);
        bool andflag = true;
        for(int i = 0; i < hero_num; i++) {
            Hero& tmp = m_hero[i];
            if(tmp.hp > 0) {
                andflag &= tmp.vel.x == 0 && tmp.vel.y == 0 && m_terrain.isair(tmp.rect) == false;
            }
        }
        if(!(gameStatus.boomtime && g_Time < gameStatus.boomtime + 16 * 3)
                && andflag && gameStatus.alivehero > 0) {
            SwitchTime = g_Time;
            curheroID = getnextheroID(curheroID);
            gameStatus.launched = false;
            gameStatus.boomed = false;
            gameStatus.windvel = randint(-MAX_WIND, MAX_WIND);
            gameStatus.sealevel -= 10;
            gameStatus.ADpressed = false;
            if(!isHumanMove()) {
                ai.direction = rand() % 2 ? -1 : 1;
                ai.walk_timeleft = randint(100, 200);
                ai.computed_best = false;
                ai.space_pressed = false;
                gameStatus.curWeapon = 0;
            }
            if(rand01() > 0.7) {
                Box next_box;
                next_box.rect = Rect(randint(30, TERRAIN_WIDTH - 30), 0, 39, 30);
                boxes.push_back(next_box);
            }
        }
    }
    for(auto& box : boxes) {
        const int BOX_VEL = 5;
        for(int i = BOX_VEL; i >= 0; i--) {
            Rect next_rect = box.rect;
            next_rect.Offset(0, i);
            if(!m_terrain.RectCollision(next_rect)) {
                box.rect = next_rect;
                break;
            }
        }
    }
    if(gameStatus.launched == true && gameStatus.boomed == false) {
        const int ELAPSE_WIND = 10;
        switch (gameStatus.curWeapon) {
        case 0: {
            if(g_Time == bazooka.time_flag + ELAPSE_WIND) {
                mciSendString(L"stop res\\wav\\fly.wav", NULL, 0, NULL);
                mciSendString(L"play res\\wav\\fly.wav", NULL, 0, NULL);
                bazooka.vel.x += gameStatus.windvel;
            }
            bazooka.vel.y += 1;
            POINT rvel;
            rvel.x = rvel.y = 1 << 30;
            int use_flag = 0;
            int dist = abs(bazooka.vel.x);
            int sign = sgn(bazooka.vel.x);
            for(int vx = dist; vx >= 0; vx--) {
                Rect next_rect = bazooka.rect;
                next_rect.Offset(vx * sign, 0);
                if(!m_terrain.RectCollision(next_rect)) {
                    bazooka.rect = next_rect;
                    rvel.x = vx * sign;
                    use_flag |= 1;
                    break;
                }
            }
            dist = abs(bazooka.vel.y);
            sign = sgn(bazooka.vel.y);
            for(int vy = dist; vy >= 0; vy--) {
                Rect next_rect = bazooka.rect;
                next_rect.Offset(0, vy * sign);
                if(!m_terrain.RectCollision(next_rect)) {
                    bazooka.rect = next_rect;
                    rvel.y = vy * sign;
                    use_flag |= 2;
                    break;
                }
            }
            if(rvel.x != bazooka.vel.x
                    || rvel.y != bazooka.vel.y || OutOfBound(bazooka.rect)) {
                PointF center(CenterPoint(bazooka.rect));
                CauseDamage((int)center.X, (int)center.Y, 100, randint(15, 20));
                gameStatus.boomed = true;
                gameStatus.boomtime = g_Time;
            }
        }
        break;
        case 1: {
            if(grenade.time_flag == g_Time + ELAPSE_WIND) {
                grenade.vel.x += gameStatus.windvel;
            }
            bool boomflag = false;
            for(int i = 0; i < hero_num; i++) {
                if(i == curheroID) {
                    continue;
                }
                Hero& tmp = m_hero[i];
                if(tmp.hp > 0) {
                    if(tmp.rect.IntersectsWith(grenade.rect)) {
                        boomflag = true;
                    }
                }
            }
            if(OutOfBound(grenade.rect) || boomflag || grenade.time_flag + 300 < g_Time) {
                PointF center(CenterPoint(grenade.rect));
                CauseDamage((int)center.X, (int)center.Y, 100, randint(10, 15));
                gameStatus.boomed = true;
                gameStatus.boomtime = g_Time;
            }
            grenade.vel.y += 1;
            POINT rvel;
            rvel.x = rvel.y = 1 << 30;
            int dist = abs(grenade.vel.x);
            int sign = sgn(grenade.vel.x);
            for(int vx = dist; vx >= 0; vx--) {
                Rect next_rect = grenade.rect;
                next_rect.Offset(vx * sign, 0);
                if(!m_terrain.RectCollision(next_rect)) {
                    grenade.rect = next_rect;
                    rvel.x = vx * sign;
                    break;
                }
            }
            dist = abs(grenade.vel.y);
            sign = sgn(grenade.vel.y);
            for(int vy = dist; vy >= 0; vy--) {
                Rect next_rect = grenade.rect;
                next_rect.Offset(0, vy * sign);
                if(!m_terrain.RectCollision(next_rect)) {
                    grenade.rect = next_rect;
                    rvel.y = vy * sign;
                    break;
                }
            }
            if(rvel.x != grenade.vel.x) {
                grenade.vel.x *= -1;
                grenade.vel.x += randint(-1, 1);
            }
            if(rvel.y != grenade.vel.y) {
                grenade.vel.y *= -1;
                grenade.vel.x += randint(-1, 1);
            }
        }
        break;
        default:
            break;
        }
    }
}

BOOL ButtonClicked(POINT ptMouse, POINT buttonPos, SIZE buttonSize) {
    RECT rectButton;
    rectButton.left = buttonPos.x;
    rectButton.top = buttonPos.y;
    rectButton.right = buttonPos.x + buttonSize.cx;
    rectButton.bottom = buttonPos.y + buttonSize.cy;

    return PtInRect(&rectButton, ptMouse);
}

VOID ProcessKeyDown(WPARAM wParam) {
    if(!isHumanMove()) {
        return;
    }
    switch (wParam) {
    case 'W':
        gameStatus.WSstatus = -1;
        break;
    case 'S':
        gameStatus.WSstatus = 1;
        break;
    case 'A':
        gameStatus.ADpressed = true;
        m_hero[curheroID].direction = -1;
        break;
    case 'D':
        gameStatus.ADpressed = true;
        m_hero[curheroID].direction = 1;
        break;
    case VK_RETURN:
        if(!m_terrain.isair(m_hero[curheroID].rect)) {
            gameStatus.RETURNpressed = true;
            m_hero[curheroID].vel.y = -JUMP_VEL_Y;
            m_hero[curheroID].rect.Offset(0, -1);
        }
    case 'F':
        if(isShowWeapon()) {
            mciSendString(L"stop res\\wav\\putaway.wav", NULL, 0, NULL);
            mciSendString(L"play res\\wav\\putaway.wav", NULL, 0, NULL);
            gameStatus.curWeapon = (gameStatus.curWeapon + 1) % gameStatus.maxWeapon;
        }
        break;
    case VK_SPACE:
        SpaceDown();
        break;
    default:
        break;
    }
}

VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    // TODO
    if(!isLocalMove()) {
        return;
    }
    if(gameStatus.OnlineGame && g_Time <= SwitchTime + NET_ELAPSE * NET_NUM) {
        return;
    }
    ProcessKeyDown(wParam);
    if(gameStatus.OnlineGame) {
        char sbuf[100];
        sprintf(sbuf, "f%d %d", wParam, g_Time);
        sendstring(sbuf);
    }
}

void SpaceUp() {
    if(gameStatus.SPACEpressed == true && !gameStatus.launched) {
        gameStatus.SPACEpressed = false;
        launchWeapon();
        if(!gameStatus.OnlineGame && rand01() < 0.1) {
            zhuangB();
        }
    }
}

void ProcessKeyUp(WPARAM wParam) {
    if(!isHumanMove()) {
        return;
    }
    switch (wParam) {
    case 'W':
        gameStatus.WSstatus = 0;
        break;
    case 'S':
        gameStatus.WSstatus = 0;
        break;
    case 'A':
        gameStatus.ADpressed = false;
        break;
    case 'D':
        gameStatus.ADpressed = false;
        break;
    case VK_RETURN:
        gameStatus.RETURNpressed = false;
        break;
    case VK_SPACE:
        SpaceUp();
        break;
    default:
        break;
    }
}

VOID KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    // TODO
    if(!isLocalMove()) {
        return;
    }
    if(gameStatus.OnlineGame && g_Time <= SwitchTime + NET_ELAPSE * NET_NUM) {
        return;
    }
    ProcessKeyUp(wParam);
    if(gameStatus.OnlineGame) {
        char sbuf[100];
        sprintf(sbuf, "g%d %d", wParam, g_Time);
        sendstring(sbuf);
    }
}

void a_Launch() {
    gameStatus.status = 1;
    if(isLocalMove()) {
        g_Limit = NET_ELAPSE * NET_NUM;
    } else {
        g_Limit = 0;
    }
    SetTimer(m_hwnd, TIMER_ID, TIMER_ELAPSE, NULL);
    PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC);
    InvalidateRect(m_hwnd, NULL, FALSE);
}

void b_SetSeed() {
    srand(g_seed);
    m_terrain.seed = rand();
    GenerateGame();
    InvalidateRect(m_hwnd, NULL, FALSE);
}

void c_Gen() {
    m_terrain.seed = rand();
    GenerateGame();
    InvalidateRect(m_hwnd, NULL, FALSE);
}

void d_Worms() {
    gameStatus.wormsperplayer = (gameStatus.wormsperplayer + 1) % 4;
    GenerateGame();
    InvalidateRect(m_hwnd, NULL, FALSE);
}

void e_Player(int i) {
    if(m_playerTypeButton[i].status == 0) {
        gameStatus.playernum++;
    }
    m_playerTypeButton[i].status = (m_playerTypeButton[i].status + 1) % 3;
    if(m_playerTypeButton[i].status == 0) {
        gameStatus.playernum--;
    }
    GenerateGame();
    InvalidateRect(m_hwnd, NULL, FALSE);
}

DWORD WINAPI NetReceive(LPVOID pM) {
    //for debug
    if(gameStatus.isClient) {
        serv_sock = socket(AF_INET, SOCK_STREAM, 0);
        myAddr.sin_family = AF_INET;
        myAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
        myAddr.sin_port = htons(1234);
        bind(serv_sock, (LPSOCKADDR)&myAddr, sizeof(SOCKADDR));
        listen(serv_sock, 1);
        m_sock = accept(serv_sock, &fromAddr, &nSize);
    }
    int len, flag = false;
    for(;;) {
        if(flag == false) {
            len = recv(m_sock, Buffer, BUF_LEN, 0);
            while(Buffer[len - 1] != '\0') {
                len += recv(m_sock, Buffer + len, BUF_LEN, 0);
            }
        }
        if(len > 0) {
            switch (Buffer[0]) {
            case 'a':
                a_Launch();
                break;
            case 'b':
                sscanf(Buffer + 1, "%u", &g_seed);
                b_SetSeed();
                break;
            case 'c':
                c_Gen();
                break;
            case 'd':
                d_Worms();
                break;
            case 'e':
                int i;
                sscanf(Buffer + 1, "%d", &i);
                e_Player(i);
                break;
            case 'f': {
                WPARAM wParam;
                int cTime;
                sscanf(Buffer + 1, "%d%d", &wParam, &cTime);
                msgqueue.push(myMSG(0, wParam, cTime));
            }
            break;
            case 'g': {
                WPARAM wParam;
                int cTime;
                sscanf(Buffer + 1, "%d%d", &wParam, &cTime);
                msgqueue.push(myMSG(1, wParam, cTime));
            }
            break;
            case 'h': {
                int cTime;
                sscanf(Buffer + 1, "%d", &cTime);
                g_Limit = cTime;
            }
            break;
            default:
                break;
            }
            int cur = 0;
            while(Buffer[cur] != '\0') {
                cur++;
            }
            if(cur + 1 == len) {
                flag = false;
            } else {
                flag = true;
                for(int j = cur + 1; j <= len; j++) {
                    Buffer[j - cur - 1] = Buffer[j];
                }
                len -= cur + 1;
            }
        }
    }
    return 0;
}

void net_init() {
    if(gameStatus.isSever) {
        m_sock = socket(AF_INET, SOCK_STREAM, 0);
        itsAddr.sin_family = AF_INET;
        itsAddr.sin_addr.S_un.S_addr = inet_addr(ip_str);
        itsAddr.sin_port = htons(1234);
        connect(m_sock, (LPSOCKADDR)&itsAddr, sizeof(SOCKADDR));
    }
    CreateThread(NULL, 0, NetReceive, NULL, 0, NULL);
    gameStatus.OnlineGame = true;
    m_playerTypeButton[0].status = 1;
    gameStatus.playernum = 1;
}

void sendstring(const char* sbuf) {
    send(m_sock, sbuf, strlen(sbuf) + 1, 0);
}

VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    POINT ptMouse; // 鼠标点击的坐标
    ptMouse.x = LOWORD(lParam);
    ptMouse.y = HIWORD(lParam);

    switch (gameStatus.status) {
    case 0:
        if (ButtonClicked(ptMouse, m_gameStartButton.pos, m_gameStartButton.size)) {
            // 启动计时器
            // 停止计时器
            //KillTimer(hWnd, TIMER_ID);

            // 更改游戏状态
            gameStatus.status = 2;
            //srand(0);
            srand((unsigned)time(0));
            m_terrain.seed = rand();
            GenerateGame();
            InvalidateRect(hWnd, NULL, TRUE);//第三个参数设为TRUE，重画整个画布。
        }
        if(ButtonClicked(ptMouse, m_serverbutton.pos, m_serverbutton.size)) {
            gameStatus.status = 4;
            gameStatus.isSever = true;
            net_init();
            g_seed = (unsigned)time(0);
            {
                char sbuf[100];
                sprintf(sbuf, "b%u", g_seed);
                sendstring(sbuf);
            }
            b_SetSeed();
        }
        if(ButtonClicked(ptMouse, m_clientbutton.pos, m_clientbutton.size)) {
            gameStatus.status = 5;
            gameStatus.isClient = true;
            net_init();
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;
    case 2:
        if(ButtonClicked(ptMouse, m_launchButton.pos, m_launchButton.size)) {
            gameStatus.status = 1;
            SetTimer(m_hwnd, TIMER_ID, TIMER_ELAPSE, NULL);
            PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC);
            InvalidateRect(hWnd, NULL, FALSE);
        }
        if(ButtonClicked(ptMouse, m_reGenerateButton.pos, m_reGenerateButton.size)) {
            m_terrain.seed = rand();
            GenerateGame();
            InvalidateRect(hWnd, NULL, FALSE);
        }
        if(ButtonClicked(ptMouse, m_wormNumButton.pos, m_wormNumButton.size)) {
            gameStatus.wormsperplayer = (gameStatus.wormsperplayer + 1) % 4;
            GenerateGame();
            InvalidateRect(hWnd, NULL, FALSE);
        }
        for(int i = 0; i < 4; i++) {
            if(ButtonClicked(ptMouse, m_playerTypeButton[i].pos, m_playerTypeButton[i].size)) {
                if(m_playerTypeButton[i].status == 0) {
                    gameStatus.playernum++;
                }
                m_playerTypeButton[i].status = (m_playerTypeButton[i].status + 1) % 3;
                if(m_playerTypeButton[i].status == 0) {
                    gameStatus.playernum--;
                }
                GenerateGame();
                InvalidateRect(hWnd, NULL, FALSE);
            }
        }
        break;
    case 3:
        if(ButtonClicked(ptMouse, m_backToMenuButton.pos, m_backToMenuButton.size)) {
            gameStatus.status = 0;
            InvalidateRect(hWnd, NULL, FALSE);
        }
        if(ButtonClicked(ptMouse, m_playAgainButton.pos, m_playAgainButton.size)) {
            gameStatus.status = 2;
            m_terrain.seed = rand();
            GenerateGame();
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;
    case 4:
        if(ButtonClicked(ptMouse, m_launchButton.pos, m_launchButton.size)) {
            if(gameStatus.playernum >= 2 && gameStatus.wormsperplayer >= 1) {
                sendstring("a");
                a_Launch();
            }
        }
        if(ButtonClicked(ptMouse, m_reGenerateButton.pos, m_reGenerateButton.size)) {
            sendstring("c");
            c_Gen();
        }
        if(ButtonClicked(ptMouse, m_wormNumButton.pos, m_wormNumButton.size)) {
            sendstring("d");
            d_Worms();
        }
        for(int i = 1; i < 4; i++) {
            if(ButtonClicked(ptMouse, m_playerTypeButton[i].pos, m_playerTypeButton[i].size)) {
                {
                    char sbuf[100];
                    sprintf(sbuf, "e%d", i);
                    sendstring(sbuf);
                }
                e_Player(i);
            }
        }
        break;
    default:
        break;
    }
    //如果点击了游戏开始图标

}

POINT operator + (const POINT & a, const POINT & b) {
    POINT c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    return c;
}


PointF CenterPoint(const Rect & rect) {
    REAL x = (REAL)(rect.GetLeft() + rect.GetRight()) / 2;
    REAL y = (REAL)(rect.GetTop() + rect.GetBottom()) / 2;
    return PointF(x, y);
}

bool isShowWeapon() {
    const Hero& cur = m_hero[curheroID];
    if(gameStatus.launched == true) {
        return false;
    }
    if(gameStatus.ADpressed == false
            && m_terrain.isair(cur.rect) == false) {
        return true;
    }
    return false;
}

void CauseDamage(int x, int y, int r, int damage) {
    mciSendString(L"stop res\\wav\\boom.wav", NULL, 0, NULL);
    mciSendString(L"play res\\wav\\boom.wav", NULL, 0, NULL);
    m_terrain.Boom(Point(x, y), r);
    for(int i = 0; i < hero_num; i++) {
        //TODO
        Hero& tmp = m_hero[i];
        if(tmp.hp > 0) {
            PointF center(CenterPoint(tmp.rect));
            double dx = center.X - x;
            double dy = center.Y - y;
            double dist = sqrt(SQR(dx) + SQR(dy));
            if(dist <= r) {
                tmp.hp -= damage;
                if(tmp.hp <= 0) {
                    KillWorm(i);
                } else {
                    double rad = atan2(dy, dx);
                    const double FLY_VEL = 20;
                    tmp.vel.x = (int)(cos(rad) * FLY_VEL);
                    tmp.vel.y = (int)(sin(rad) * FLY_VEL);
                    tmp.flytime = g_Time;
                }
            }
        }
    }
}

void KillWorm(int x) {
    m_hero[x].hp = 0;
    gameStatus.alivehero--;
}

int isGameEnd() {//-1平局 -2尚未结束 否则返回胜利的队伍
    int res = -1;
    for(int i = 0; i < hero_num; i++) {
        Hero& tmp = m_hero[i];
        if(tmp.hp > 0) {
            if(res == -1) {
                res = tmp.team;
            } else if(res != tmp.team) {
                return -2;
            }
        }
    }
    return res;
}

bool OutOfBound(const Rect& rect) {
    return rect.GetRight() < 0
           || rect.GetLeft() > TERRAIN_WIDTH
           || rect.GetBottom() > gameStatus.sealevel;
}

VOID Render_End(HWND hWnd) {
    PAINTSTRUCT ps;
    HDC hdc;
    hdc = BeginPaint(hWnd, &ps);
    HDC hdcBuffer, hdcBmp;
    HBITMAP cptBmp;
    hdcBuffer = CreateCompatibleDC(hdc);
    hdcBmp = CreateCompatibleDC(hdc);
    cptBmp = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);
    SelectObject(hdcBuffer, cptBmp);
    SelectObject(hdcBmp, gameStatus.hBmp);
    BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBmp, 0, 0, SRCCOPY);


    Render_Button(m_backToMenuButton, hdcBmp, hdcBuffer);

    Render_Button(m_playAgainButton, hdcBmp, hdcBuffer);

    TCHAR res[50];
    if(gameStatus.winplayer == -1) {
        wcscpy_s(res, L"游戏平局");
    } else {
        wsprintf(res, L"玩家%d获得了胜利", gameStatus.winplayer);
    }
    {
        Font font(L"微软雅黑", 36);
        SolidBrush brush(Color::AliceBlue);
        Graphics graphics(hdcBuffer);
        graphics.DrawString(res, _tcslen(res), &font, PointF(500, 500), &brush);
    }

    BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBuffer, 0, 0, SRCCOPY);
    DeleteObject(cptBmp);
    DeleteDC(hdcBuffer);
    DeleteDC(hdcBmp);
    EndPaint(hWnd, &ps);
}

VOID Render_Generate(HWND hWnd) {
    PAINTSTRUCT ps;
    HDC hdc;
    hdc = BeginPaint(hWnd, &ps);
    HDC hdcBuffer, hdcBmp;
    HBITMAP cptBmp;
    hdcBuffer = CreateCompatibleDC(hdc);
    hdcBmp = CreateCompatibleDC(hdc);
    cptBmp = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);
    SelectObject(hdcBuffer, cptBmp);
    SelectObject(hdcBmp, gameStatus.hBmp);
    BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBmp, 0, 0, SRCCOPY);


    Render_Button(m_reGenerateButton, hdcBmp, hdcBuffer);

    Render_Button(m_wormNumButton, hdcBmp, hdcBuffer);

    Render_Button(m_launchButton, hdcBmp, hdcBuffer);

    for(int i = 0; i < 4; i++) {
        Render_Button(m_playerTypeButton[i], hdcBmp, hdcBuffer);
    }

    {
        Bitmap bmp(WNDWIDTH, WNDHEIGHT);
        Graphics g_bmp(&bmp);
        HDC hdcplus = g_bmp.GetHDC();

        SelectObject(hdcplus, GetStockObject(NULL_PEN));
        HBRUSH terrianBrush = CreatePatternBrush(m_hTerrainBmp);
        SelectObject(hdcplus, terrianBrush);
        for(int i = 0; i < TERRAIN_WIDTH; i++) {
            vector<pii>& tmp = m_terrain.terrains[i];
            for(auto seg : tmp) {
                Rectangle(hdcplus, i, seg.first - 10, i + 2, seg.second);
            }
        }
        DeleteObject(terrianBrush);

        g_bmp.ReleaseHDC(hdcplus);

        Graphics graphics(hdcBuffer);
        const RectF t_window(300, 300, 640, 360);
        Pen pen(Color::Orange, 5);
        graphics.DrawRectangle(&pen, t_window);
        graphics.DrawImage(&bmp, t_window);
        Font font(L"微软雅黑", 20);
        SolidBrush brush(Color::Black);
        TCHAR str[100], name[3][20] = {L"酱油", L"人类", L"人工智能"};
        wsprintf(str, L"一共有%d个玩家\n每个玩家有%d条虫子\n玩家0是%ls\n玩家1是%ls\n玩家2是%ls\n玩家3是%ls\n",
                 gameStatus.playernum,
                 gameStatus.wormsperplayer,
                 name[m_playerTypeButton[0].status],
                 name[m_playerTypeButton[1].status],
                 name[m_playerTypeButton[2].status],
                 name[m_playerTypeButton[3].status]);
        graphics.DrawString(str, _tcslen(str), &font, PointF(400, 50), &brush);
    }

    BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBuffer, 0, 0, SRCCOPY);
    DeleteObject(cptBmp);
    DeleteDC(hdcBuffer);
    DeleteDC(hdcBmp);
    EndPaint(hWnd, &ps);
}

bool isHumanMove() {
    return m_playerTypeButton[m_hero[curheroID].team].status == 1;
}


int AI_Simulator::statusMoveTo(const Rect& rect, int dir) {
    Hero& cur = m_hero[curheroID];
    Rect next_rect = rect;
    next_rect.Offset(dir * HERO_VEL_X, 0);
    int height = m_terrain.minHeight(next_rect);
    if(height != (1 << 30)) {
        next_rect.Offset(0, height);
        if(OutOfBound(next_rect)) {
            return 0;
        }
    } else {
        return 0;
    }
    if(!m_terrain.isair(next_rect)) {
        return 1;//平稳的走过去
    }
    for(;;) {
        next_rect.Offset(0, 1);
        if(OutOfBound(next_rect)) {
            return 0;//会摔死
        }
        if(!m_terrain.isair(next_rect)) {
            return 2;//能跳下去
        }
    }
}

bool AI_Simulator::canJumpTo(const Rect & __rect__, int dir) {
    POINT vel;
    vel.x = 0;
    vel.y = -JUMP_VEL_Y;
    Rect rect = __rect__;
    rect.Offset(0, -1);
    int cur_time = 0;
    bool pressed = true;
    for(;;) {
        cur_time++;
        if(pressed) {
            vel.x = dir * JUMP_VEL_X;
            pressed = false;
        }
        vel.y += 3;
        POINT rvel;
        rvel.x = rvel.y = 1 << 30;
        int dist = abs(vel.x);
        int sign = sgn(vel.x);
        for(int vx = dist; vx >= 0; vx--) {
            Rect next_rect = rect;
            next_rect.Offset(vx * sign, 0);
            if(!m_terrain.RectCollision(next_rect)) {
                rect = next_rect;
                rvel.x = vx * sign;
                break;
            }
        }
        dist = abs(vel.y);
        sign = sgn(vel.y);
        for(int vy = dist; vy >= 0; vy--) {
            Rect next_rect = rect;
            next_rect.Offset(0, vy * sign);
            if(!m_terrain.RectCollision(next_rect)) {
                rect = next_rect;
                rvel.y = vy * sign;
                break;
            }
        }
        if(OutOfBound(rect)) {
            return false;
        }
        if(!m_terrain.isair(rect)) {
            return rect.X != __rect__.X;
        }
    }
}

void launchWeapon() {
    Hero& cur = m_hero[curheroID];
    double rad = torad(gameStatus.WSangle);
    double length = (double)gameStatus.strength * 0.4;
    double vx = length * cos(rad) * cur.direction;
    double vy = length * sin(rad);
    gameStatus.launched = true;
    PointF center(CenterPoint(cur.rect));
    gameStatus.boomed = false;
    switch (gameStatus.curWeapon) {
    case 0: {
        mciSendString(L"stop res\\wav\\rpgfire.wav", NULL, 0, NULL);
        mciSendString(L"play res\\wav\\rpgfire.wav", NULL, 0, NULL);
        bazooka.rect = Rect(Point((int)center.X, (int)center.Y), Size(20, 20));
        bazooka.vel.x = (int)vx;
        bazooka.vel.y = (int)vy;
        bazooka.rect.Offset((int)vx, (int)vy);
        if(m_terrain.RectCollision(bazooka.rect)) {
            int height = m_terrain.minHeight(bazooka.rect);
            if(height == (1 << 30)) {
            } else {
                bazooka.rect.Offset(0, height);
            }
        }
        bazooka.time_flag = g_Time;
    }
    break;
    case 1: {
        mciSendString(L"stop res\\wav\\throw.wav", NULL, 0, NULL);
        mciSendString(L"play res\\wav\\throw.wav", NULL, 0, NULL);
        grenade.rect = Rect(Point((int)center.X, (int)center.Y), Size(30, 30));
        grenade.vel.x = (int)vx;
        grenade.vel.y = (int)vy;
        grenade.rect.Offset((int)vx, (int)vy);
        if(m_terrain.RectCollision(grenade.rect)) {
            int height = m_terrain.minHeight(grenade.rect);
            if(height == (1 << 30)) {
            } else {
                grenade.rect.Offset(0, height);
            }
        }
        grenade.time_flag = g_Time;
        break;
    }
    default:
        break;
    }
}

void SpaceDown() {
    if(gameStatus.SPACEpressed == false && !gameStatus.launched) {
        gameStatus.SPACEpressed = true;
        gameStatus.strength = 0;
        gameStatus.stren_dir = 1;
    }
}

void AI_Simulator::doSomedicision() {
    Hero& cur = m_hero[curheroID];
    walk_timeleft--;
    if(walk_timeleft > 0 && m_terrain.isair(cur.rect) == false) {
        int flag = statusMoveTo(cur.rect, direction);
        bool ok = canJumpTo(cur.rect, direction);
        vector<pair<int, double> >dice;
        //决策0:向方向移动一步
        //决策1：向方向跳一步
        //决策2：转向
        if(flag == 1)  {
            dice.push_back(make_pair(0, 0.8));
        } else if(flag == 2) {
            dice.push_back(make_pair(0, 0.1));
        }
        if(ok) {
            dice.push_back(make_pair(1, 0.03));
        }
        dice.push_back(make_pair(2, 0.01));
        double sumL = 0;
        for(auto& pr : dice) {
            sumL += pr.second;
        }
        for(auto& pr : dice) {
            pr.second /= sumL;
        }
        double t = rand01();
        int command = -1;
        for(auto& pr : dice) {
            if(pr.second > t) {
                command = pr.first;
                break;
            } else {
                t -= pr.second;
            }
        }
        if(command == 0) {
            cur.direction = direction;
            gameStatus.ADpressed = true;
        } else if(command == 1) {
            //assert(!m_terrain.isair(cur.rect)));
            cur.direction = direction;
            gameStatus.RETURNpressed = true;
            m_hero[curheroID].vel.x = 0;
            m_hero[curheroID].vel.y = -JUMP_VEL_Y;
            m_hero[curheroID].rect.Offset(0, -1);
        } else if(command == 2) {
            cur.direction = 0;
            gameStatus.ADpressed = false;
            direction *= -1;
        }
    } else {
        if(walk_timeleft == 0) {
            gameStatus.ADpressed = false;
            gameStatus.curWeapon = 0;
        }
        if(walk_timeleft < 0 && m_terrain.isair(cur.rect) == false && !gameStatus.launched) {
            if(!computed_best) {
                computed_best = true;
                best_score = double(-(1 << 30));
                for(int direction = -1; direction <= 1; direction += 2) {
                    for(int angle = -85; angle <= 85; angle += 5) {
                        for(int strength = 0; strength <= 100; strength += 2) {
                            PointF pos = physicsupdate(angle, direction, strength);
                            double score = getScore((int)pos.X, (int)pos.Y);
                            if(best_score < score) {
                                best_score = score;
                                best_angle = angle;
                                best_direction = direction;
                                best_strength = strength;
                            }
                        }
                    }
                }
            }
            if(cur.direction != best_direction) {
                cur.direction = best_direction;
                gameStatus.SPACEpressed = false;
            } else if(gameStatus.WSstatus || gameStatus.WSangle != best_angle) {
                if(gameStatus.WSstatus) {
                    if(gameStatus.WSangle == best_angle) {
                        gameStatus.WSstatus = 0;
                    } else {
                        gameStatus.WSstatus = gameStatus.WSangle > best_angle ? -1 : 1;
                    }
                } else {
                    gameStatus.WSstatus = gameStatus.WSangle > best_angle ? -1 : 1;
                }
            } else if(!space_pressed) {
                space_pressed = true;
                SpaceDown();
            } else if(gameStatus.strength == best_strength) {
                SpaceUp();
            }
        }
    }
}

double AI_Simulator::getScore(int x, int y) {
    int r = 100, damage = 20;
    double score = 0;
    for(int i = 0; i < hero_num; i++) {
        //TODO
        Hero& tmp = m_hero[i];
        if(tmp.hp > 0) {
            PointF center(CenterPoint(tmp.rect));
            double dx = center.X - x;
            double dy = center.Y - y;
            double dist = sqrt(SQR(dx) + SQR(dy));
            if(dist <= r) {
                if(tmp.team == m_hero[curheroID].team) {
                    score -= damage * 2 / 0.01;
                } else {
                    score += damage / (dist + 0.01);
                }
            }
        }
    }
    return score;
}

PointF AI_Simulator::physicsupdate(int angle, int direction, int strength) {
    const Hero& cur = m_hero[curheroID];
    double rad = torad(angle);
    double length = (double)strength * 0.4;
    double vx = length * cos(rad) * direction;
    double vy = length * sin(rad);
    PointF center(CenterPoint(cur.rect));
    Bazooka m_bazooka;
    m_bazooka.rect = Rect(Point((int)center.X, (int)center.Y), Size(20, 20));
    m_bazooka.vel.x = (int)vx;
    m_bazooka.vel.y = (int)vy;
    m_bazooka.rect.Offset((int)vx, (int)vy);
    if(m_terrain.RectCollision(m_bazooka.rect)) {
        int height = m_terrain.minHeight(m_bazooka.rect);
        if(height == (1 << 30)) {
        } else {
            m_bazooka.rect.Offset(0, height);
        }
    }
    int v_Time = 0;
    m_bazooka.time_flag = v_Time;
    for(;;) {
        v_Time++;
        const int ELAPSE_WIND = 10;
        if(v_Time == m_bazooka.time_flag + ELAPSE_WIND) {
            m_bazooka.vel.x += gameStatus.windvel;
        }
        m_bazooka.vel.y += 1;
        POINT rvel;
        rvel.x = rvel.y = 1 << 30;
        int use_flag = 0;
        int dist = abs(m_bazooka.vel.x);
        int sign = sgn(m_bazooka.vel.x);
        for(int vx = dist; vx >= 0; vx--) {
            Rect next_rect = m_bazooka.rect;
            next_rect.Offset(vx * sign, 0);
            if(!m_terrain.RectCollision(next_rect)) {
                m_bazooka.rect = next_rect;
                rvel.x = vx * sign;
                use_flag |= 1;
                break;
            }
        }
        dist = abs(m_bazooka.vel.y);
        sign = sgn(m_bazooka.vel.y);
        for(int vy = dist; vy >= 0; vy--) {
            Rect next_rect = m_bazooka.rect;
            next_rect.Offset(0, vy * sign);
            if(!m_terrain.RectCollision(next_rect)) {
                m_bazooka.rect = next_rect;
                rvel.y = vy * sign;
                use_flag |= 2;
                break;
            }
        }
        if(rvel.x != m_bazooka.vel.x
                || rvel.y != m_bazooka.vel.y || OutOfBound(m_bazooka.rect)) {
            PointF center(CenterPoint(m_bazooka.rect));
            return center;
        }
    }
}

bool isLocalMove() {
    if(gameStatus.OnlineGame) {
        Hero& cur = m_hero[curheroID];
        if(gameStatus.isClient) {
            return cur.team == 0;
        } else {
            return cur.team != 0;
        }
    } else {
        return true;
    }
}