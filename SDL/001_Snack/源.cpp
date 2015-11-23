#include <SDL2\SDL.h>
#include <stdio.h>
SDL_Window * mainWindow = NULL;
SDL_Renderer *renderer = NULL;
SDL_Point point[10000] = { { 10, 100}, {20, 100} };
int point_count = 2;

enum {UP, DOWN, LEFT, RIGHT} head_direction, tail_direction;

int main(void)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	mainWindow = SDL_CreateWindow("Snack", 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		200, 200, 
		0);
	if (NULL == mainWindow) {
		printf("Failed to create window. exiting...\n");
		return 0;
	}
	renderer = SDL_CreateRenderer(mainWindow, -1, 0);
	if (NULL == renderer) {
		printf("Failed to create renderer. exiting...\n");
		return 0;
	}
	int alive = 1;
	int count = 0;
	while (alive) {
		Uint32 start = SDL_GetTicks();
		if (point[point_count - 1].x == point[point_count - 2].x) {
			if (point[point_count - 1].y > point[point_count - 2].y) {
				head_direction = DOWN;
			}
			else if (point[point_count - 1].y < point[point_count - 2].y) {
				head_direction = UP;
			}
			else {
				alive = 0;
			}
		
		}
		else if (point[point_count - 1].y == point[point_count - 2].y) {
			if (point[point_count - 1].x > point[point_count - 2].x) {
				head_direction = RIGHT;
			}
			else if (point[point_count - 1].y < point[point_count - 2].y) {
				head_direction = LEFT;
			}
			else {
				printf("%d\n", __LINE__);
				alive = 0;
			}
		}
		else {
			printf("%d\n", __LINE__);
			alive = 0;
		}
		if (point[0].x == point[1].x) {
			if (point[0].y > point[1].y) {
				tail_direction = UP;
			}
			else if (point[0].y < point[1].y) {
				tail_direction = DOWN;
			}
			else {
				printf("%d\n", __LINE__);
				alive = 0;
			}

		}
		else if (point[0].y == point[1].y) {
			if (point[0].x > point[1].x) {
				tail_direction = LEFT;
			}
			else if (point[0].x < point[1].x) {
				tail_direction = RIGHT;
			}
			else {
				printf("%d\n", __LINE__);
				alive = 0;
			}
		}
		else {
			alive = 0;
		}
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch(event.type ) {
			case SDL_QUIT:
				alive = 0;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_DOWN:
					if (head_direction == LEFT || head_direction == RIGHT) {
						point[count] = point[count - 1];
						point[count].y++;
						count++;
					}
					break;
				case SDLK_UP:
					if (head_direction == LEFT || head_direction == RIGHT) {
						point[count] = point[count-1];
						point[count].y--;
						count++;
					}
					break;
				case SDLK_RIGHT:
				case SDLK_LEFT:
					break;
				default:
					break;
				}
			}
		}
		switch (head_direction)
		{
		case RIGHT:
			point[point_count - 1].x++;
			break;
		case LEFT:
			point[point_count - 1].x--;
			break;
		case DOWN:
			point[point_count - 1].y++;
			break;
		case UP:
			point[point_count - 1].y--;
			break;
		default:
			break;
		}
		switch (tail_direction)
		{
		case RIGHT:
			point[0].x++;
			break;
		case LEFT:
			point[0].x--;
			break;
		case DOWN:
			point[0].y++;
			break;
		case UP:
			point[0].y--;
			break;
		default:
			break;
		}
		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0x00);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0xf0, 0xa0, 0x00, 0x00);
		SDL_RenderDrawLines(renderer, point, point_count);
		SDL_RenderPresent(renderer);
		if (SDL_GetTicks() - start < 50) {
			SDL_Delay(50 - (SDL_GetTicks() - start));
		}
	}
	SDL_Quit();
	return 0;
}