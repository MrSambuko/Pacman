#include <cassert>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "Pacman.h"
#include "Drawer.h"

namespace
{
constexpr const char FONT[] = "freefont-ttf\\sfd\\FreeMono.ttf";
}

int main(int argc, char **argv)
{
	/* initialize SDL */
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		assert(0 && "Failed to initialize video!");
		exit(-1);
	}
	
	SDL_Window* window = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_OPENGL);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if(!window)
	{
		assert(0 && "Failed to create window!");
		exit(-1);
	}

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	if (TTF_Init() == -1)
	{
		assert(0 && "Failed to create ttf!");
		exit(-1);
	}

	const auto font = TTF_OpenFont(FONT, 24);
	Drawer* drawer = Drawer::Create(window, renderer, font);
	Pacman* pacman = Pacman::Create(drawer);

	float lastFrame = static_cast<float>(SDL_GetTicks()) * 0.001f;
	SDL_Event event;
	while (SDL_PollEvent(&event) >= 0)
	{
		const float currentFrame = static_cast<float>(SDL_GetTicks()) * 0.001f;
		const float elapsedTime = currentFrame - lastFrame;

		if (!pacman->Update(&event, elapsedTime))
			break;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		pacman->Draw();
		
		lastFrame = currentFrame;		

		SDL_RenderPresent(renderer);
		SDL_Delay(10);
	}

	delete pacman;
	delete drawer;

	TTF_CloseFont(font);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit( );

	return 0;
}

