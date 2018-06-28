#include <cassert>
#include "Drawer.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Common.h"

Drawer* Drawer::Create(SDL_Window* aWindow, SDL_Renderer* aRenderer)
{
	Drawer* drawer = new Drawer(aWindow, aRenderer);

	if (!drawer->Init())
	{
		delete drawer;
		drawer = nullptr;
	}

	return drawer;
}

void Drawer::Draw( const std::string& anImage, int aCellX, int aCellY )
{
	if (myItemsToDraw.find(anImage) != myItemsToDraw.end())
		myItemsToDraw[anImage].push_back({aCellX, aCellY});
	else
		myItemsToDraw[anImage] = {{aCellX, aCellY}};
}

void Drawer::registerImage( const std::string& anImage)
{
	if (myRenderItems.find(anImage) != myRenderItems.end())
		return;

	SDL_Surface* surface = IMG_Load( anImage.c_str() ) ;

	if (surface == nullptr)
		return;

	SDL_Texture* optimizedSurface = SDL_CreateTextureFromSurface(myRenderer, surface);
		
	myRenderItems[anImage] = {surface, optimizedSurface};
}

void Drawer::DrawSurfaces()
{
	SDL_Rect sizeRect;
	SDL_Rect posRect;

	for (const auto& namePositions : myItemsToDraw)
	{
		const auto& surface = myRenderItems[namePositions.first].first;
		const auto& texture = myRenderItems[namePositions.first].second;

		sizeRect.x = 0;
		sizeRect.y = 0;
		sizeRect.w = surface->w;
		sizeRect.h = surface->h ;

		posRect.w = sizeRect.w;
		posRect.h = sizeRect.h;

		for (const auto& posPair : namePositions.second)
		{
			posRect.x = X_OFFSET + posPair.first;
			posRect.y = Y_OFFSET + posPair.second;
			
			SDL_RenderCopy(myRenderer, texture, &sizeRect, &posRect);
		}
	}

	myItemsToDraw.clear();
}

Drawer::Drawer(SDL_Window* aWindow, SDL_Renderer* aRenderer)
: myWindow(aWindow)
, myRenderer(aRenderer)
{
}


bool Drawer::Init() const
{
	return myWindow != nullptr;
}

void Drawer::DrawText(const char* aText, const char* aFontFile, int aX, int aY) const
{
	TTF_Font* font=TTF_OpenFont(aFontFile, 24);

	const SDL_Color fg={255,0,0,255};
	SDL_Surface* surface = TTF_RenderText_Solid(font, aText, fg);

	SDL_Texture* optimizedSurface = SDL_CreateTextureFromSurface(myRenderer, surface);

	SDL_Rect sizeRect;
	sizeRect.x = 0;
	sizeRect.y = 0;
	sizeRect.w = surface->w;
	sizeRect.h = surface->h ;

	SDL_Rect posRect ;
	posRect.x = aX;
	posRect.y = aY;
	posRect.w = sizeRect.w;
	posRect.h = sizeRect.h;

	SDL_RenderCopy(myRenderer, optimizedSurface, &sizeRect, &posRect);
	SDL_DestroyTexture(optimizedSurface);
	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
}
