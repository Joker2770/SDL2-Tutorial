#ifdef _WIN32
//Windows
extern "C"
{
#include "SDL.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C"{
#endif

#include <SDL.h>

#ifdef __cplusplus
};
#endif
#endif

#include <iostream>

int main(int argc, char *argv[])
{
	SDL_Window *window = nullptr;
	SDL_Surface *windowSurface = nullptr;
	SDL_Surface *image1 = nullptr;
	SDL_Surface *image2 = nullptr;
	SDL_Surface *image3 = nullptr;
	SDL_Surface *currentImage = nullptr;

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

	//if (SDL_NumJoysticks() < 0)
		SDL_Log("%d number of controllers are connected", 0);
		//std::cout << "Error!" << std::endl;
	//else
		SDL_Joystick *joystick = SDL_JoystickOpen(0);

		std::cout << "Controller name: " << SDL_JoystickName(joystick) << std::endl;
		std::cout << "Num Axes: " << SDL_JoystickNumAxes(joystick) << std::endl;
		std::cout << "Num Buttons: " << SDL_JoystickNumButtons(joystick) << std::endl;

	window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
	windowSurface = SDL_GetWindowSurface(window);

	image1 = SDL_LoadBMP("test1.bmp");
	image2 = SDL_LoadBMP("test2.bmp");
	image3 = SDL_LoadBMP("test3.bmp");
	currentImage = image1;

	bool isRunning = true;
	SDL_Event ev;
	while (isRunning)
	{
		while (SDL_PollEvent(&ev) != 0)
		{
			//Get QUIT event, KEYBOARD event or any other event
			if (ev.type == SDL_QUIT)
			{
				isRunning = false;
			}
			else if (ev.type == SDL_KEYDOWN)
			{
				switch (ev.key.keysym.sym)
				{
				case SDLK_1:
					currentImage = image1;
					break;
				case SDLK_2:
					currentImage = image2;
					break;
				case SDLK_3:
					currentImage = image3;
					break;
				default:
					break;
				}
			}
		}
		SDL_BlitSurface(currentImage, NULL, windowSurface, NULL);
		SDL_UpdateWindowSurface(window);
	}
	SDL_FreeSurface(image1);
	SDL_FreeSurface(image2);
	SDL_FreeSurface(image3);
	currentImage = image1 = image2 = image3 = nullptr;

	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_Quit();

	return 0;
}