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

#include <stdio.h>
#include <iostream>

int main(int argc, char *argv[])
{
	SDL_Window *window = nullptr;

	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);

	bool isRunning = true;
	SDL_Event ev;

	std::string text = "";
	SDL_StartTextInput();

	while (isRunning)
	{
		while (SDL_PollEvent(&ev) != 0)
		{
			if (ev.type == SDL_QUIT)
				isRunning = false;
			//Text event
			else if (ev.type == SDL_TEXTINPUT || ev.type == SDL_KEYDOWN)
			{
				system("cls");
				if ((ev.type == SDL_KEYDOWN) && (ev.key.keysym.sym == SDLK_BACKSPACE) && (text.length() > 0))
					text = text.substr(0, text.length() - 1);
				else if(ev.type == SDL_TEXTINPUT)
					text += ev.text.text;
				printf("%s", text);
			}
		}
		SDL_UpdateWindowSurface(window);
	}

	SDL_StopTextInput();

	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_Quit();

	return 0;
}