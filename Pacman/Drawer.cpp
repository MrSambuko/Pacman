#include <cassert>
#include "Drawer.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

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
	myItemsToDraw.push_back({anImage, {aCellX, aCellY}});
}

void Drawer::registerImage( const std::string& anImage, int aCellX, int aCellY )
{
	auto it = mySurfaces.find(anImage);

	if (it == mySurfaces.end())
	{
		SDL_Surface* surface = IMG_Load( anImage.c_str() ) ;

		if (surface == nullptr)
			return;

		SDL_Texture* optimizedSurface = SDL_CreateTextureFromSurface(myRenderer, surface);
		
		mySurfaces.insert({anImage, {{aCellX, aCellY}, surface, optimizedSurface}});
	}
	else
	{
		mySurfaces.insert({anImage, {{aCellX, aCellY}, std::get<1>(it->second), std::get<2>(it->second)}});
	}
}

void Drawer::DrawSurfaces()
{
	SDL_Rect sizeRect;
	SDL_Rect posRect ;

	for (const auto& namePos : myItemsToDraw )
	{
		assert(mySurfaces.find(namePos.first) != mySurfaces.end() && "Not registered");
		auto surfaces = mySurfaces.equal_range(namePos.first);
		const auto& xDiff = namePos.second.first;
		const auto& yDiff = namePos.second.second;

		for (auto& surfacesPair = surfaces.first; surfacesPair != surfaces.second; ++surfacesPair)
		{
			sizeRect.x = 0;
			sizeRect.y = 0;
			const auto& surface = std::get<1>(surfacesPair->second);
			sizeRect.w = surface->w;
			sizeRect.h = surface->h ;

			const auto& xy = std::get<0>(surfacesPair->second);
			posRect.x = xy.first + xDiff;
			posRect.y = xy.second + yDiff;
			posRect.w = sizeRect.w;
			posRect.h = sizeRect.h;
			SDL_RenderCopy(myRenderer, std::get<2>(surfacesPair->second), &sizeRect, &posRect);
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
