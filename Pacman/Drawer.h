#pragma once

#include <unordered_map>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;

class Drawer
{
public:
	static Drawer* Create(SDL_Window* aWindow, SDL_Renderer* aRenderer);

	void registerImage(const std::string& anImage, int aCellX = 0, int aCellY = 0);

	void Draw(const std::string& anImage, int aCellX = 0, int aCellY = 0);
	void DrawSurfaces();
	void DrawText(const char* aText, const char* aFontFile, int aX, int aY) const;

private:
	Drawer(SDL_Window* aWindow, SDL_Renderer* aRenderer);
	bool Init() const;
	
	SDL_Window* myWindow;
	SDL_Renderer* myRenderer;

	std::unordered_multimap<std::string, std::tuple<std::pair<int, int>, SDL_Surface*, SDL_Texture*>> mySurfaces;
	std::vector<std::pair<std::string, std::pair<int, int>>> myItemsToDraw;
};
