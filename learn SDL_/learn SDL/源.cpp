#include <stdio.h>
#include <SDL2\SDL.h>
#undef main
int main()
{
	int gamebuff = 1;
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window*window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,800,600,0);
	SDL_Renderer*renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	while (gamebuff);//保持窗口不关闭;但是测试以后读圈圈 ，不能自己退出

	SDL_Quit();
	return 0;
}