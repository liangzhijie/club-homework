#include <cstdio>
#include <SDL2\SDL.h>
#include <cstdlib>
#include <iostream>
#include <ctime>
//#include<SDL2\SDL_image.h>

#undef main

int run = 1;
SDL_Point pos[40000];
int _count_ = 2;
int edited = 0;
enum direct { UP, DOWN, LEFT, RIGHT } head, tail;
SDL_Point wind_size = {200, 200 };
SDL_Point snack;
SDL_Window* window;
const int step = 5;
int eatcandy;
SDL_Point walls[] = { { 0, 0 },{ 0, wind_size.y },{ wind_size.x, wind_size.y, },{ wind_size.x, 0 },{ 0, 0 } };

using namespace std;

void judge_direction();
bool isPointOnLine(SDL_Point p, SDL_Point l[], int lc);
int isQuit();
void tailMove();
int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	srand(time(0));
	window = SDL_CreateWindow("abc", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, wind_size.x, wind_size.y, 0);
	SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	pos[0] = { 30, 10 };
	snack = { rand() % (wind_size.x / step * 9 / 10) *step, rand() % (wind_size.y / step * 9 / 10) * step };
	pos[1] = pos[0];
	pos[1].x -= 30;
	eatcandy = 0;
	while (run) {
		if (isPointOnLine(snack, pos, _count_)) {
			eatcandy += 5;
			//(cout << "EC: " << eatcandy << endl).flush();
			while (isPointOnLine(snack, pos, _count_) || isPointOnLine(snack, walls, 5))//保证食物不在蛇身上或者边界
				snack = { rand() % (wind_size.x / step * 9 / 10) *step, rand() % (wind_size.y / step * 9 / 10) * step };

		}

		Uint32 tick = SDL_GetTicks();
		judge_direction();
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_QUIT:
				run = !isQuit();
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_LEFT:
				case SDLK_RIGHT:
					if (head == UP || head == DOWN && !edited) {
						for (int i = _count_; i > 0; i--) {
							pos[i] = pos[i - 1];
						}
						_count_++;
						pos[0] = pos[1];
						if (event.key.keysym.sym == SDLK_LEFT) {
							pos[0].x -= step;
						}
						else {
							pos[0].x += step;
						}
						edited = 1;
						tailMove();
					}
					break;
				case SDLK_UP:
				case SDLK_DOWN:
					if (head == LEFT || head == RIGHT && !edited) {
						for (int i = _count_; i > 0; i--) {
							pos[i] = pos[i - 1];
						}
						_count_++;
						pos[0] = pos[1];
						if (event.key.keysym.sym == SDLK_UP) {
							pos[0].y -= step;
						}
						else {
							pos[0].y += step;
						}
						edited = 1;
						tailMove();
					}
					break;
				default:
					break;
				}
			default:
				break;
			}
		}

		if (!edited) {
			switch (head) {
			case LEFT:
				pos[0].x -= step;
				break;
			case RIGHT:
				pos[0].x += step;
				break;
			case UP:
				pos[0].y -= step;
				break;
			case DOWN:
				pos[0].y += step;
				break;
			}
			tailMove();
		}

		if (_count_ > 3) {
			//cout << "C: " << _count_ << endl;
			for (int i = 0; i < _count_; i++) {
				//cout << "I: " << i << endl;
				for (int j = 0; j < i - 1; j++) {
					//cout << "J1: " << j << " : " << (
					run = run && !isPointOnLine(pos[i], pos + j, 2)
						//) << endl
						;
				}
				for (int j = i + 1; j < _count_ - 1; j++) {
					//cout << "J1: " << j << " : " << (
					run = run && !isPointOnLine(pos[i], pos + j, 2)
						//) << endl
						;
				}

			}
		}
		run = run && !isPointOnLine(pos[0], walls, 5);
		if (eatcandy > 0) {
			eatcandy--;
			switch (tail) {
			case LEFT:
				pos[_count_ - 1].x += step;
				break;
			case RIGHT:
				pos[_count_ - 1].x -= step;
				break;
			case UP:
				pos[_count_ - 1].y += step;
				break;
			case DOWN:
				pos[_count_ - 1].y -= step;
				break;
			}
			//cout << "longer" << endl;
		}
		SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
		SDL_RenderClear(render);
		SDL_SetRenderDrawColor(render, 0xff, 0xff, 0xff, 0);
		/*cerr << "Drawing";
		for (int i = _count_-1; i > 0; i--) {
		cerr << pos[i].x << "," << pos[i].y << " ";
		SDL_RenderDrawLine(render, pos[i].x, pos[i].y, pos[i - 1].x, pos[i - 1].y);
		}
		cerr << endl;*/
		SDL_RenderDrawLines(render, pos, _count_);
		SDL_SetRenderDrawColor(render, 0xff, 0, 0, 0);
		SDL_RenderDrawPoint(render, snack.x, snack.y);
		SDL_Rect rct = { snack.x - 2, snack.y - 2, 5, 5 };
		SDL_RenderDrawRect(render, &rct);
		SDL_RenderPresent(render);
		if (SDL_GetTicks() - tick < 50) {
			SDL_Delay(50 - (SDL_GetTicks() - tick));
		}
		edited = 0;
#ifdef _DEBUG
		if (run == 0) {
			run = 1;
			pos[0] = { 30, 10 };
			_count_ = 2;
			pos[1] = pos[0];
			pos[1].x -= 30;
			eatcandy = 0;
		}

#endif

	}
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
	//system("pause");
	exit(EXIT_SUCCESS);
	return 0;
}

int isQuit() {
	SDL_MessageBoxData data;
	SDL_MessageBoxButtonData buttons[] = { { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "&Yes" },{ 0, 0, "Do&n't Exit!" } };
	data.buttons = buttons;
	data.colorScheme = NULL;
	data.flags = 0;
	data.message = "Are you sure you are going to exit?";
	data.numbuttons = 2;
	data.title = "Snacks";
	data.window = window;
	int button;
	if (SDL_ShowMessageBox(&data, &button) >= 0)
		return button;
	else
		return 0;
}

void judge_direction() {
	if (pos[0].x == pos[1].x) {
		if (pos[0].y > pos[1].y) {
			head = DOWN;
		}
		else {
			head = UP;
		}
	}
	else if (pos[0].y == pos[1].y) {
		if (pos[0].x > pos[1].x) {
			head = RIGHT;
		}
		else {
			head = LEFT;
		}
	}
	if (pos[_count_ - 1].x == pos[_count_ - 2].x) {
		if (pos[_count_ - 1].y > pos[_count_ - 2].y) {
			tail = UP;
		}
		else {
			tail = DOWN;
		}
	}
	else if (pos[_count_ - 1].y == pos[_count_ - 2].y) {
		if (pos[_count_ - 1].x > pos[_count_ - 2].x) {
			tail = LEFT;
		}
		else {
			tail = RIGHT;
		}
	}
	/*else {

	exit(EXIT_FAILURE);
	}*/
}
#define dis(a, b) (sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)))
bool isPointOnLine(SDL_Point p, SDL_Point l[], int lc)
{
	for (int i = 0; i < lc - 1; i++) {
		if (dis(p, l[i]) + dis(p, l[i + 1]) - dis(l[i], l[i + 1]) < 0.001) {
			return 1;
		}
	}
	return 0;
}
void tailMove() {
	if ((pos[_count_ - 1].x - pos[_count_ - 2].x)*(pos[_count_ - 1].x - pos[_count_ - 2].x) + (pos[_count_ - 1].y - pos[_count_ - 2].y)*(pos[_count_ - 1].y - pos[_count_ - 2].y) <= 5 * 5 && _count_ > 2) {
		_count_--;
	}
	else {
		switch (tail) {
		case LEFT:
			pos[_count_ - 1].x -= 5;
			break;
		case RIGHT:
			pos[_count_ - 1].x += 5;
			break;
		case UP:
			pos[_count_ - 1].y -= 5;
			break;
		case DOWN:
			pos[_count_ - 1].y += 5;
			break;
		}
	}
}