#ifdef _WIN32
//Windows
extern "C"
{
#include "SDL.h"
#include "SDL_image.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C" {
#endif

#include <SDL.h>
#include <SDL_image.h>

#ifdef __cplusplus
};
#endif
#endif

#include <iostream>

SDL_Texture *LoadTexture(std::string filePath, SDL_Renderer *renderTarget)
{
	SDL_Texture *texture = nullptr;
	SDL_Surface *surface = IMG_Load(filePath.c_str());
	if (surface == NULL)
		std::cout << "Error!" << std::endl;
	else
	{
		texture = SDL_CreateTextureFromSurface(renderTarget, surface);
		if (texture == NULL)
			std::cout << "Error!" << std::endl;
	}
	SDL_FreeSurface(surface);

	return texture;
}

int main(int argc, char *argv[])
{
	const int FPS = 32;
	float frameTime = 0.0f;
	int prevTime = 0;
	int currentTime = 0;
	float deltaTime = 0.0f;
	float moveSpeed = 1000.0f;
	const Uint8 *keyState;

	SDL_Window *window = nullptr;
	SDL_Texture *currentImage = nullptr;
	SDL_Renderer *renderTarget = nullptr;
	SDL_Rect playerRect;
	SDL_Rect playerPosition;

	playerPosition.x = playerPosition.y = 0;
	playerPosition.w = playerPosition.h = 64;

	int textureWidth, textureHeight;
	int frameWidth, frameHeight;

	SDL_Init(SDL_INIT_VIDEO);

	if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)) & (IMG_INIT_JPG | IMG_INIT_PNG))
		std::cout << "Error: " << IMG_GetError() << std::endl;

	window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
	renderTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	currentImage = LoadTexture(argv[1], renderTarget);

	SDL_QueryTexture(currentImage, NULL, NULL, &textureWidth, &textureHeight);

	frameWidth = textureWidth / 4;
	frameHeight = textureHeight;

	playerRect.x = playerRect.y = 0;
	playerRect.w = frameWidth;
	playerRect.h = frameHeight;

	SDL_SetRenderDrawColor(renderTarget, 0x10, 0X5F, 0x7F, 0X10);

	bool isRunning = true;
	SDL_Event ev;
	while (isRunning)
	{
		prevTime = currentTime;
		currentTime = SDL_GetTicks();
		deltaTime = (currentTime - prevTime) / 1000.0f;
		while (SDL_PollEvent(&ev) != 0)
		{
			if (ev.type == SDL_QUIT)
				isRunning = false;
			else if (ev.type == SDL_KEYDOWN)
			{
				switch (ev.key.keysym.sym)
				{
				case SDLK_RIGHT:
					playerPosition.x += (moveSpeed * deltaTime);
					break;
				case SDLK_LEFT:
					playerPosition.x -= (moveSpeed * deltaTime);
					break;
				case SDLK_UP:
					playerPosition.y -= (moveSpeed * deltaTime);
					break;
				case SDLK_DOWN:
					playerPosition.y += (moveSpeed * deltaTime);
					break;
				default:
					break;
				}
			}
		}
		keyState = SDL_GetKeyboardState(NULL);
		if (keyState[SDL_SCANCODE_D])
			playerPosition.x += moveSpeed * deltaTime;
		else if(keyState[SDL_SCANCODE_A])
			playerPosition.x -= moveSpeed * deltaTime;
		else if (keyState[SDL_SCANCODE_S])
			playerPosition.y += moveSpeed * deltaTime;
		else if (keyState[SDL_SCANCODE_W])
			playerPosition.y -= moveSpeed * deltaTime;

		frameTime += deltaTime;

		if (frameTime >= 0.25f)
		{
			frameTime = 0;
			playerRect.x += frameWidth;
			if (playerRect.x >= textureWidth)
			{
				playerRect.y += frameHeight;
				if (playerRect.y >= textureHeight)
					playerRect.y = 0;
				playerRect.x = 0;
			}
		}

		SDL_RenderClear(renderTarget);
		SDL_RenderCopy(renderTarget, currentImage, &playerRect, &playerPosition);
		SDL_RenderPresent(renderTarget);
	}

	SDL_DestroyWindow(window);
	SDL_DestroyTexture(currentImage);
	SDL_DestroyRenderer(renderTarget);
	window = nullptr;
	currentImage = nullptr;
	renderTarget = nullptr;
	SDL_Quit();

	return 0;
}