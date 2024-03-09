#include <bits/stdc++.h>
#include <conio.h>
using namespace std;

const int N = 4;      // 行数
const int M = 30;     // 列数
const int FRESH = 60; // 帧数

char background[N + 1][M + 1];

void gotoxy(const int &x, const int &y)
{
    printf("\033[%d;%dH", x + 1, y + 1);
}

struct PointSprite
{
    int x, y;
    char shape;

    PointSprite(const int &_x, const int &_y, const char &_shape) : x(_x), y(_y), shape(_shape)
    {
        gotoxy(x, y);
        putchar(shape);
    }

    void move(int dx, int dy)
    {
        gotoxy(x, y);
        putchar(background[x][y]);
        x += dx;
        y += dy;

        if (y < 0)
        {
            y = 0;
        }
        else if (y >= M)
        {
            y = M - 1;
        }
        if (x < 0)
        {
            x = 0;
        }
        else if (x >= N)
        {
            x = N - 1;
        }
        gotoxy(x, y);
        putchar(shape);
    }

    bool touch(PointSprite other)
    {
        if (x == other.x && y == other.y)
        {
            return true;
        }
        return false;
    }
};

struct Dino : PointSprite
{
    Dino() : PointSprite(1, 3, '$') {}
};

struct Wall : PointSprite
{
    Wall(int _x) : PointSprite(_x, M - 1, '#') {}
};

int main()
{
    memset(background, ' ', sizeof(background));
    printf("\033[?25l\033[2J");
    srand(time(0));
    for (int i = 0; i < N; i++)
    {
        puts(background[i]);
    }
    Dino dino;
    deque<Wall> walls;
    int tick = 0, jian = FRESH;
    while (1)
    {
        _sleep(1000 / FRESH);
        if (_kbhit())
        {
            static unsigned char ch;
            ch = _getch();
            if (ch == 224)
            {
                ch = _getch();
                if (ch == 80)
                {
                    dino.move(1, 0);
                }
                else if (ch == 72)
                {
                    dino.move(-1, 0);
                }
            }
            else if (ch == 'q')
            {
                break;
            }
        }
        if (tick < jian)
        {
            tick++;
        }
        if (tick == jian)
        {
            static bool vis[N];
            memset(vis, 0, sizeof(bool) * N);
            for (int i = 1; i < N; i++)
            {
                vis[rand() % N] = 1;
            }
            for (int i = 0; i < N; i++)
            {
                if (vis[i])
                {
                    walls.push_back(Wall(i));
                }
            }
            tick = 0;
            if (jian > FRESH * 0.1 * N)
            {
                jian = jian * 0.99;
            }
        }
        for (auto &wall : walls)
        {
            wall.move(0, -1);
            if (wall.y == 0)
            {
                gotoxy(wall.x, wall.y);
                putchar(background[wall.x][wall.y]);
                walls.pop_front();
            }
            if (wall.touch(dino))
            {
                printf("You are over...");
                return 0;
            }
        }
        gotoxy(N, 0);
        printf("Score: %lu", clock() / FRESH);
    }

    return 0;
}
