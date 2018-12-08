#include <SDL.h>
#include <iostream>

int main(int argc, char *argv[])
{
	SDL_Window *window = NULL;
	SDL_Surface *windowSurface = NULL;
	SDL_Surface *imageSurface = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		std::cout << "Video Initialization Error: " << SDL_GetError() << std::endl;
	else
	{
		window = SDL_CreateWindow("SDL_CodingModeEasy Series", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
		if (window == NULL)
			std::cout << "Window Creation Error: " << SDL_GetError() << std::endl;
		else
		{
			//Window Created
			windowSurface = SDL_GetWindowSurface(window);
			imageSurface = SDL_LoadBMP("./testprinter.bmp");

			if(imageSurface == NULL)
				std::cout << "Image Load Error: " << SDL_GetError() << std::endl;
			else
			{
				SDL_BlitSurface(imageSurface, NULL, windowSurface, NULL);
				SDL_UpdateWindowSurface(window);
				SDL_Delay(6000);
			}
		}
	}

	SDL_FreeSurface(imageSurface);
	imageSurface = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
	windowSurface = NULL;
	SDL_Quit();

	return 0;
}
