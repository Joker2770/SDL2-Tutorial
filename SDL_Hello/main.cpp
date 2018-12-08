#include<SDL.h>

int main( int argc, char* args[] )
{

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window*window=SDL_CreateWindow("bmp photo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
	SDL_Renderer*renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderClear(renderer);
	SDL_Surface*surface=SDL_LoadBMP("testprinter.bmp");
	SDL_Rect box={0, 0, surface->w, surface->h};
	SDL_Texture*texture=SDL_CreateTextureFromSurface(renderer, surface);
	SDL_RenderCopy(renderer, texture, &box, &box);
	SDL_RenderPresent(renderer);
	SDL_Delay(2000);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}