#include "global.h"

GameButton CreateGameButton(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp, int status) {
    GameButton gameButton;
    gameButton.pos.x = posX;
    gameButton.pos.y = posY;
    gameButton.size.cx = sizeX;
    gameButton.size.cy = sizeY;
    gameButton.hBmp = hBmp;
    gameButton.status = status;
    return gameButton;
}

int Terrain::Random_ratio(int prev, double ratio) {
    int res;
    if(rand01() < ratio) {
        res = min(TERRAIN_MAX_HEIGHT, prev + randint(0, 5));
    } else {
        res = max(TERRAIN_MIN_HEIGHT, prev - randint(0, 5));
    }
    return res;
}

void Terrain::Generate() {
    srand(seed);
    for(int iCol = 0, prev = TERRAIN_START_HEIGHT; iCol < TERRAIN_WIDTH; iCol++) {

        auto& tmp = terrains[iCol];
        tmp.clear();

        double ratio;
        if(iCol * 3 < TERRAIN_WIDTH) {
            ratio = 0.7;
        } else if(iCol * 3 / 2 < TERRAIN_WIDTH) {
            ratio = 0.5;
        } else {
            ratio = 0.3;
        }

        int height = Random_ratio(prev, ratio);
        tmp.push_back(make_pair(WNDHEIGHT - height + 1, WNDHEIGHT));
        prev = height;
    }
}

void Terrain::BeizerGenerate() {
    srand(seed);
    for(int i = 0; i < WNDWIDTH; i++) {
        terrains[i].clear();
    }
    int lhei = 300, rhei = WNDHEIGHT - 100, gap = 50;
    double p[4];
    for(int i = 0; i < 4; i++) {
        p[i] = randint(lhei, rhei);
    }
    for(int i = 1; i + 1 < TERRAIN_WIDTH; i += gap) {
        for(int j = i; j < i + gap && j < TERRAIN_WIDTH; j++) {
            double t = (double)(j - i) / gap;
            double b = Beizer(p[0], p[1], p[2], p[3], t / 3);
            terrains[j].push_back(make_pair((int)b, WNDHEIGHT));
        }
        p[0] = Beizer(p[0], p[1], p[2], p[3], 1. / 3);
        p[1] = p[2];
        p[2] = p[3];
        p[3] = randint(lhei, rhei);
    }
    terrains[0].push_back(make_pair(-(1 << 30), (1 << 30)));
    terrains[TERRAIN_WIDTH - 1].push_back(make_pair(-(1 << 30), (1 << 30)));
}

bool Terrain::RectCollision(const Rect& rect) {
    int lcord = max(0, rect.GetLeft());
    int rcord = min(TERRAIN_WIDTH, rect.GetRight());
    pii seg = make_pair(rect.GetTop(), rect.GetBottom());
    for(int iCol = lcord; iCol < rcord; iCol++) {
        if(terrains[iCol].empty()) {
            continue;
        }
        for(auto e_seg : terrains[iCol]) {
            if(seg_collision(seg, e_seg)) {
                return true;
            }
        }
    }
    return false;
}

void Terrain::Boom(Point pos, int rad) {
    int lcord = max(1, pos.X - rad + 1);
    int rcord = min(TERRAIN_WIDTH - 1, pos.X + rad - 1);
    vector<pii> res;
    for(int xcord = lcord; xcord < rcord; xcord++) {
        int delx = abs(pos.X - xcord);
        int delh = (int)floor(sqrt(rad * rad - delx * delx) + 0.5);
        pii seg = make_pair(pos.Y - delh, pos.Y + delh);
        auto& tmp = terrains[xcord];
        for(int iVec = tmp.size() - 1; iVec >= 0; iVec--) {
            if(seg_collision(seg, tmp[iVec])) {
                seg_difference(seg, tmp[iVec], res);
                tmp.erase(tmp.begin() + iVec);
                for(int j = res.size() - 1; j >= 0; j--) {
                    tmp.insert(tmp.begin() + iVec, res[j]);
                }
            }
        }
    }
}

int Terrain::minHeight(const Rect& rect) {
    int res = (1 << 30);
    for(int i = DOWN_HEIGHT; i >= -CLIMB_HEIGHT; i--) {
        Rect next_rect = rect;
        next_rect.Offset(0, i);
        if(!RectCollision(next_rect)) {
            res = i;
            break;
        }
    }
    return res;
}

bool Terrain::isair(const Rect& rect) {
    Rect next_rect = rect;
    next_rect.Offset(0, 1);
    return RectCollision(next_rect) == false;
}

double rand01() {
    return rand() / (double)RAND_MAX;
}

int randint(int l, int r) {
    return l + (int)floor((r - l) * rand01() + 0.5);
}


bool point_on_seg(int x, int l, int r) {
    return l <= x && x <= r;
}

bool seg_collision(pii a, pii b) {
    return point_on_seg(a.first, b.first, b.second)
           || point_on_seg(a.second, b.first, b.second)
           || point_on_seg(b.first, a.first, a.second)
           || point_on_seg(b.second, a.first, a.second);
}

void seg_difference(pii seg, pii tmp, vector<pii>& res) {
    res.clear();
    if(seg.first <= tmp.first && tmp.second <= seg.second) {
        return;
    } else if(tmp.first < seg.first && seg.second < tmp.second) {
        res.push_back(make_pair(tmp.first, seg.first - 1));
        res.push_back(make_pair(seg.second + 1, tmp.second));
    } else if(tmp.first < seg.first) {
        res.push_back(make_pair(tmp.first, seg.first - 1));
    } else if(tmp.second > seg.second) {
        res.push_back(make_pair(seg.second + 1, tmp.second));
    }
}

int sgn(int x) {
    if(x == 0) {
        return 0;
    }
    return x > 0 ? 1 : -1;
}

double torad(double deg) {
    return deg / 180 * M_PI;
}

double Beizer(double p0, double p1, double p2, double p3, double t) {
    return p0 * CUB(1 - t) + 3 * p1 * t * SQR(1 - t)
           + 3 * p2 * SQR(t) * (1 - t) + p3 * CUB(t);
}

void GameStatus::UpdateAngle() {
    if(WSstatus) {
        int next_angle = WSangle + WSstatus * AIM_SPEED;
        next_angle = max(next_angle, -85);
        next_angle = min(next_angle, 85);
        WSangle = next_angle;
    }
}

void GameStatus::Updatestrength() {
    if(SPACEpressed == true) {
        strength += stren_dir * 2;
        if(strength > 100 || strength < 0) {
            stren_dir *= -1;
            strength += stren_dir;
        }
    }
}