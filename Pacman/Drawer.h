#pragma once

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;

class Drawer
{
public:
	static Drawer* Create(SDL_Window* aWindow, SDL_Renderer* aRenderer);

	void Draw(const char* anImage, int aCellX = 0, int aCellY = 0) const;
	void DrawText(const char* aText, const char* aFontFile, int aX, int aY) const;

private:
	Drawer(SDL_Window* aWindow, SDL_Renderer* aRenderer);
	bool Init() const;
	
	SDL_Window* myWindow;
	SDL_Renderer* myRenderer;
};
