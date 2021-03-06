#include <cassert>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Constants.h"

#include "Drawer.h"
#include <algorithm>

namespace
{
constexpr const SDL_Color fg = { 255,0,0,255 };
}

Drawer* Drawer::Create(SDL_Window* aWindow, SDL_Renderer* aRenderer, TTF_Font* aFont)
{
	Drawer* drawer = new Drawer(aWindow, aRenderer, aFont);

	if (!drawer->Init())
	{
		delete drawer;
		drawer = nullptr;
	}

	return drawer;
}

Drawer::~Drawer()
{
	for (auto&& renderItem : myRenderItems)
	{
		auto&& surface = renderItem.second.first;
		auto&& optimizedSurface = renderItem.second.second;
		SDL_DestroyTexture(optimizedSurface);
		SDL_FreeSurface(surface);
	}

	for (auto&& textLabels : myTextLabelItems)
	{
		auto&& surface = std::get<0>(textLabels.second);
		auto&& optimizedSurface = std::get<1>(textLabels.second);
		SDL_DestroyTexture(optimizedSurface);
		SDL_FreeSurface(surface);
	}
}

void Drawer::Draw( const std::string& anImage, int aCellX, int aCellY )
{
	const auto comparator = [&](std::pair<std::string, std::vector<std::pair<int, int>>>& item)
	{
		return item.first == anImage;
	};

	auto& it = std::find_if(myItemsToDraw.begin(), myItemsToDraw.end(), comparator);
	if (it != myItemsToDraw.end())
		it->second.push_back({aCellX, aCellY});
	else
		myItemsToDraw.push_back({anImage, {{aCellX, aCellY}}});
}

void Drawer::RegisterImage( const std::string& anImage)
{
	if (myRenderItems.find(anImage) != myRenderItems.end())
		return;

	SDL_Surface* surface = IMG_Load( anImage.c_str() ) ;

	if (surface == nullptr)
		return;

	SDL_Texture* optimizedSurface = SDL_CreateTextureFromSurface(myRenderer, surface);
		
	myRenderItems[anImage] = {surface, optimizedSurface};
}

void Drawer::RegisterTextLabel(const std::string& textLabel, int aX, int aY)
{
	if (myTextLabelItems.find(textLabel) != myTextLabelItems.end())
		return;

	SDL_Surface* surface = TTF_RenderText_Solid(myFont, textLabel.c_str(), fg);

	SDL_Texture* optimizedSurface = SDL_CreateTextureFromSurface(myRenderer, surface);

	SDL_Rect sizeRect;
	sizeRect.x = 0;
	sizeRect.y = 0;
	sizeRect.w = surface->w;
	sizeRect.h = surface->h;

	SDL_Rect posRect;
	posRect.x = aX;
	posRect.y = aY;
	posRect.w = sizeRect.w;
	posRect.h = sizeRect.h;

	myTextLabelItems[textLabel] = { surface, optimizedSurface, sizeRect, posRect };
}

void Drawer::DrawImages()
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
		sizeRect.h = surface->h;

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

void Drawer::DrawTexts()
{
	for (auto&& textLabel : myTextLablesToDraw)
	{
		const auto& textLabelData = myTextLabelItems[textLabel];
		const auto& optimizedSurface = std::get<1>(textLabelData);
		const auto& sizeRect = std::get<2>(textLabelData);
		const auto& posRect = std::get<3>(textLabelData);
		SDL_RenderCopy(myRenderer, optimizedSurface, &sizeRect, &posRect);
	}
	myTextLablesToDraw.clear();

	for (auto&& text : myTextsToDraw)
	{
		const auto& surface = std::get<0>(text);
		const auto& optimizedSurface = std::get<1>(text);
		const auto& sizeRect = std::get<2>(text);
		const auto& posRect = std::get<3>(text);
		SDL_RenderCopy(myRenderer, optimizedSurface, &sizeRect, &posRect);

		SDL_DestroyTexture(optimizedSurface);
		SDL_FreeSurface(surface);
	}
	myTextsToDraw.clear();
}

#ifdef _DEBUG
void Drawer::DrawLines()
{
	SDL_SetRenderDrawColor(myRenderer, 255, 255, 255, 255);
	for (auto&& pair : myLines)
		SDL_RenderDrawLine(myRenderer, pair.first.first, pair.first.second, pair.second.first, pair.second.second);
	myLines.clear();
}
#endif

void Drawer::Draw()
{
	DrawImages();
	DrawTexts();
#ifdef _DEBUG
	DrawLines();
#endif
}

Drawer::Drawer(SDL_Window* aWindow, SDL_Renderer* aRenderer, TTF_Font* aFont)
: myWindow(aWindow)
, myRenderer(aRenderer)
, myFont(aFont)
{
}


bool Drawer::Init() const
{
	// register text surfaces
	return myWindow != nullptr;	
}

void Drawer::DrawLabel(const std::string& label)
{
	myTextLablesToDraw.push_back(label);
}


void Drawer::DrawText(const std::string& aText, int aX, int aY)
{
	SDL_Surface* surface = TTF_RenderText_Solid(myFont, aText.c_str(), fg);
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

	auto&& text = std::make_tuple(surface, optimizedSurface, sizeRect, posRect);
	myTextsToDraw.emplace_back(text);

}

void Drawer::DrawMiddleScreenText( const std::string& aText )
{
	myItemsToDraw.clear();
	myTextsToDraw.clear();

	SDL_Surface* surface = TTF_RenderText_Solid(myFont, aText.c_str(), fg);
	SDL_Texture* optimizedSurface = SDL_CreateTextureFromSurface(myRenderer, surface);

	SDL_Rect sizeRect;
	sizeRect.x = 0;
	sizeRect.y = 0;
	sizeRect.w = surface->w;
	sizeRect.h = surface->h ;

	SDL_Rect posRect ;
	int sizeX = 0;
	int sizeY = 0;
	SDL_GetWindowSize(myWindow, &sizeX, &sizeY);
	posRect.x = sizeX / 2 - surface->w/2;
	posRect.y = sizeY / 2 - surface->h/2;
	posRect.w = sizeRect.w;
	posRect.h = sizeRect.h;

	auto&& text = std::make_tuple(surface, optimizedSurface, sizeRect, posRect);
	myTextsToDraw.emplace_back(text);
}