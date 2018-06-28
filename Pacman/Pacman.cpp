#include "Pacman.h"
#include "Drawer.h"
#include "SDL.h"
#include "SDL_events.h"
#include <iostream>
#include <sstream>
#include <string>

#include "Avatar.h"
#include "World.h"
#include "Ghost.h"

#include <unordered_map>

namespace {
std::unordered_map<SDL_Keycode, int> KEY_TO_DIRECTION =
{
{ SDL_SCANCODE_W, UP },
{ SDL_SCANCODE_UP, UP },
{ SDL_SCANCODE_D, RIGHT },
{ SDL_SCANCODE_RIGHT, RIGHT },
{ SDL_SCANCODE_S, DOWN },
{ SDL_SCANCODE_DOWN, DOWN },
{ SDL_SCANCODE_A, LEFT },
{ SDL_SCANCODE_LEFT, LEFT },
};

std::unordered_map<int, Vector2f> DIRECTION_TO_MOVE =
{
{ NO_MOVE, NO_MOVEMENT },
{ UP,	 UP_MOVEMENT},
{ RIGHT, RIGHT_MOVEMENT},
{ DOWN,	 DOWN_MOVEMENT },
{ LEFT,	 LEFT_MOVEMENT }
};
}

Pacman* Pacman::Create(Drawer* aDrawer)
{
	Pacman* pacman = new Pacman(aDrawer);

	if (!pacman->Init())
	{
		delete pacman;
		pacman = nullptr;
	}

	return pacman;
}

Pacman::Pacman(Drawer* aDrawer)
: myDrawer(aDrawer)
, myGhostGhostCounter(0.f)
, myLives(3)
, myScore(0)
, myFps(0)
, myNextMovement(NO_MOVEMENT)
{
	myAvatar = new Avatar(START_PLAYER_POS, aDrawer);
	myGhost = new Ghost(START_GHOST_POS, aDrawer);
	myWorld = new World(myDrawer);
}


bool Pacman::Init() const
{
	myWorld->Init();

	return true;
}

bool Pacman::Update(const SDL_Event* event, float aTime)
{
	if (!updateInput(event))
		return false;

	if (CheckEndGameCondition())
	{
		myDrawer->DrawText("You win!", "freefont-ttf\\sfd\\FreeMono.ttf", 20, 70);
		return true;
	}
	
	if (myLives <= 0)
	{
		myDrawer->DrawText("You lose!", "freefont-ttf\\sfd\\FreeMono.ttf", 20, 70);	
		return true;
	}

	MoveAvatar();
	myAvatar->Update(aTime);
	myGhost->Update(aTime, myWorld);

	if (myWorld->HasIntersectedDot(myAvatar->GetPosition()))
		myScore += 10;

	myGhostGhostCounter -= aTime;

	if (myWorld->HasIntersectedBigDot(myAvatar->GetPosition()))
	{
		myScore += 20;
		myGhostGhostCounter = 20.f;
		myGhost->myIsClaimableFlag = true;
	}

	if (myGhostGhostCounter <= 0)
	{
		myGhost->myIsClaimableFlag = false;
	}

	if ((myGhost->GetPosition() - myAvatar->GetPosition()).Length() < 10.f)
	{
		if (myGhostGhostCounter <= 0.f)
		{
			myLives--;

			myAvatar->SetPosition({13*22, 22*22});
			myGhost->SetPosition({13*22, 13*22});
		}
		else if (myGhost->myIsClaimableFlag && !myGhost->myIsDeadFlag)
		{
			myScore += 50;
			myGhost->myIsDeadFlag = true;
			myGhost->Die(myWorld);
		}
	}
	
	if (aTime > 0)
		myFps = static_cast<int>((1 / aTime));

	return true;
}

bool Pacman::updateInput(const SDL_Event* event)
{
	const Uint8 *keystate = SDL_GetKeyboardState(nullptr);

	if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP)
	{
		const auto& it = KEY_TO_DIRECTION.find(event->key.keysym.sym);

		const auto& direction = it == KEY_TO_DIRECTION.end() ? NO_MOVE : it->second;
		const auto& directionToMove = DIRECTION_TO_MOVE[direction];;
		if (event->type == SDL_KEYDOWN)
		{	
			myNextMovement += directionToMove;
		}
		else if (event->type == SDL_KEYUP)
		{
			myNextMovement -= directionToMove;
		}
	}


	return !keystate[SDL_SCANCODE_ESCAPE];
}

void Pacman::MoveAvatar() const
{
	const int nextTileX = myAvatar->GetCurrentTileX() + static_cast<int>(myNextMovement.myX);
	const int nextTileY = myAvatar->GetCurrentTileY() + static_cast<int>(myNextMovement.myY);

	if (myAvatar->IsAtDestination())
	{
		if (myWorld->TileIsValid(nextTileX, nextTileY))
		{
			myAvatar->SetNextTile(nextTileX, nextTileY);
		}
	}
}

bool Pacman::CheckEndGameCondition() const
{
	return false;
}

void Pacman::Draw() const
{
	myWorld->Draw();
	myAvatar->Draw();
	myGhost->Draw();

	myDrawer->DrawSurfaces();

	std::stringstream scoreStream;
	scoreStream << myScore;
	std::string scoreString(scoreStream.str());
	myDrawer->DrawText("Score", "freefont-ttf\\sfd\\FreeMono.ttf", 20, 50);
	myDrawer->DrawText(scoreString.c_str(), "freefont-ttf\\sfd\\FreeMono.ttf", 90, 50);

	
	std::stringstream liveStream;
	liveStream << myLives;
	std::string livesString(liveStream.str());

	myDrawer->DrawText("Lives", "freefont-ttf\\sfd\\FreeMono.ttf", 20, 80);
	myDrawer->DrawText(livesString.c_str(), "freefont-ttf\\sfd\\FreeMono.ttf", 90, 80);

	myDrawer->DrawText("FPS", "freefont-ttf\\sfd\\FreeMono.ttf", 880, 50);

	std::stringstream fpsStream;
	fpsStream << myFps;
	std::string fpsString(fpsStream.str());
	myDrawer->DrawText(fpsString.c_str(), "freefont-ttf\\sfd\\FreeMono.ttf", 930, 50);
}
