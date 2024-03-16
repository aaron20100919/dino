#include <bits/stdc++.h>
#include <conio.h>
using namespace std;

const int N = 4;      // 行数
const int M = 40;     // 列数
const int FRESH = 75; // 帧数

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

	void set(int _x, int _y)
	{
		gotoxy(x, y);
		putchar(background[x][y]);
		x = _x;
		y = _y;

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
	Wall(int _x = 0) : PointSprite(_x, M - 1, '#') {}
};

struct Coin : PointSprite
{
	Coin(int _x = 0) : PointSprite(_x, M - 1, 'o') {}
};

struct Recover : PointSprite
{
	Recover(int _x = 0) : PointSprite(_x, M - 1, '+') {}
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
	Coin coin(rand() % N);
	Recover recover(rand() % N);
	long long tick = 0, jian = FRESH, score = 0, life = 3, clocks = clock();
	while (1)
	{
		_sleep(1000 / FRESH);
		clocks = clock();
#define clock() clocks
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
			if (wall.touch(dino))
			{
				wall.move(0, -1);
				dino.set(dino.x, dino.y);
			}
			else
			{
				wall.move(0, -1);
			}
			if (wall.y == 0)
			{
				gotoxy(wall.x, wall.y);
				putchar(background[wall.x][wall.y]);
				walls.pop_front();
			}
			if (wall.touch(dino))
			{
				life--;
				if (life <= 0)
				{
					gotoxy(N, 0);
					printf("Life: %lld Score: %lld\n", life, clock() * FRESH / 5000 + score);
					printf("You are over...");
					return 0;
				}
				dino.set(dino.x, dino.y);
			}
		}

		coin.move(0, -1 * (clock() % 2));
		if (coin.touch(dino))
		{
			score += 100;
			coin.set(rand() % N, M - 1);
			dino.set(dino.x, dino.y);
		}
		else if (coin.y == 0)
		{
			coin.set(rand() % N, M - 1);
			score -= 100;
		}

		recover.move((1 - rand() % 3) * (rand() % 30 == 0), -1 * (clock() % 30 == 0));
		if (recover.touch(dino))
		{
			life++;
			recover.set(rand() % N, M - 1);
			dino.set(dino.x, dino.y);
		}
		else if (recover.y == 0)
		{
			recover.set(rand() % N, M - 1);
		}

		gotoxy(N, 0);
		printf("Life: %lld Score: %lld", life, clock() * FRESH / 5000 + score);
	}

	return 0;
}
