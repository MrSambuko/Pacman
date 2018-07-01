#pragma once

#include <unordered_map>
#include "SDL_ttf.h"
#include "SDL_rect.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;

namespace
{
using SDLRenderItems = std::unordered_map<std::string, std::pair<SDL_Surface*, SDL_Texture*>>;
using SDLItemsToDraw = std::unordered_map<std::string, std::vector<std::pair<int, int>>>;
using SDLTextInfo = std::tuple<SDL_Surface*, SDL_Texture*, SDL_Rect, SDL_Rect>;
using SDLTextLables = std::unordered_map<std::string, SDLTextInfo>;
}


class Drawer
{
public:
	static Drawer* Create(SDL_Window* aWindow, SDL_Renderer* aRenderer, TTF_Font* aFont);
	~Drawer();

	void RegisterImage(const std::string& anImage);
	void RegisterTextLabel(const std::string& textLabel, int aX, int aY);

	void Draw(const std::string& anImage, int aCellX = 0, int aCellY = 0);
	void Draw();
	void DrawLabel(const std::string& label);
	void DrawText(const std::string& aText, int aX, int aY);

private:
	Drawer(SDL_Window* aWindow, SDL_Renderer* aRenderer, TTF_Font* aFont);
	bool Init() const;

	void DrawImages();
	void DrawTexts();
	
	SDL_Window* myWindow;
	SDL_Renderer* myRenderer;
	TTF_Font* myFont;

	SDLRenderItems myRenderItems;
	SDLItemsToDraw myItemsToDraw;

	SDLTextLables myTextLabelItems;
	std::vector<std::string> myTextLablesToDraw;
	std::vector<SDLTextInfo> myTextsToDraw;
};
