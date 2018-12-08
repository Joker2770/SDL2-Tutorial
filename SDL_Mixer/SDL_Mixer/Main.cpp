#ifdef _WIN32
//Windows
extern "C"
{
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_Mixer.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C" {
#endif

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

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
		//SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0x10, 0x5F, 0x7F));
		texture = SDL_CreateTextureFromSurface(renderTarget, surface);
		SDL_SetTextureColorMod(texture, 0, 255, 0);
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

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	if (TTF_Init() < 0)
		std::cout << "Error: " << TTF_GetError() << std::endl;

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

	TTF_Font *font = TTF_OpenFont("Freestyle.woff.ttf", 20);
	SDL_Color color = { 0, 255, 0, 255 };
	SDL_Surface *textsurface = TTF_RenderText_Solid(font, "Hello World!", color);
	SDL_Texture *text = SDL_CreateTextureFromSurface(renderTarget, textsurface);

	SDL_Rect textRect;
	textRect.x = textRect.y = 0;

	SDL_QueryTexture(text, NULL, NULL, &textRect.w, &textRect.h);

	SDL_FreeSurface(textsurface);
	textsurface = nullptr;

	SDL_SetRenderDrawColor(renderTarget, 0x10, 0X5F, 0x7F, 0X10);

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "Error: " << Mix_GetError() << std::endl;
	}

	Mix_Music *bgm = Mix_LoadMUS("ab.mp3");
	Mix_Chunk *soundEffect = Mix_LoadWAV("201710160test.wav");

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
				case SDLK_p:
					if (!Mix_PlayingMusic())
						Mix_PlayMusic(bgm, -1);
					else if(Mix_PausedMusic())
						Mix_ResumeMusic();
					else
						Mix_PauseMusic();
					break;
				case SDLK_s:
					Mix_HaltMusic();
					break;
				case SDLK_1:
					Mix_PlayChannel(-1, soundEffect, 2); //play 3 times
					break;
				default:
					break;
				}
			}
		}
		keyState = SDL_GetKeyboardState(NULL);
		if (keyState[SDL_SCANCODE_D])
			playerPosition.x += moveSpeed * deltaTime;
		else if (keyState[SDL_SCANCODE_A])
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
		SDL_RenderCopy(renderTarget, currentImage, NULL, NULL);
		//SDL_RenderCopy(renderTarget, text, NULL, &textRect);
		SDL_RenderPresent(renderTarget);
	}

	SDL_DestroyWindow(window);
	SDL_DestroyTexture(currentImage);
	SDL_DestroyTexture(text);
	SDL_DestroyRenderer(renderTarget);
	text = nullptr;
	window = nullptr;
	currentImage = nullptr;
	renderTarget = nullptr;

	Mix_FreeChunk(soundEffect);
	Mix_FreeMusic(bgm);

	soundEffect = nullptr;
	bgm = nullptr;

	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();

	return 0;
}