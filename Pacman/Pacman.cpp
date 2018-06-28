#include "Pacman.h"
#include "Drawer.h"
#include "SDL.h"
#include <iostream>
#include <sstream>
#include <string>

#include "Avatar.h"
#include "World.h"
#include "Ghost.h"

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
, myNextMovement(-1.f,0.f)
{
	myAvatar = new Avatar(Vector2f(13*22,22*22));
	myGhost = new Ghost(Vector2f(13*22,13*22));
	myWorld = new World();
}


bool Pacman::Init() const
{
	myWorld->Init();

	return true;
}

bool Pacman::Update(float aTime)
{
	if (!UpdateInput())
		return false;

	if (CheckEndGameCondition())
	{
		myDrawer->DrawText("You win!", "freefont-ttf\\sfd\\FreeMono.ttf", 20, 70);
		return true;
	}
	else if (myLives <= 0)
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

			myAvatar->SetPosition(Vector2f(13*22,22*22));
			myGhost->SetPosition(Vector2f(13*22,13*22));
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

bool Pacman::UpdateInput()
{
	const Uint8 *keystate = SDL_GetKeyboardState(nullptr);

	if (keystate[SDL_SCANCODE_UP])
		myNextMovement = Vector2f(0.f, -1.f);
	else if (keystate[SDL_SCANCODE_DOWN])
		myNextMovement = Vector2f(0.f, 1.f);
	else if (keystate[SDL_SCANCODE_RIGHT])
		myNextMovement = Vector2f(1.f, 0.f);
	else if (keystate[SDL_SCANCODE_LEFT])
		myNextMovement = Vector2f(-1.f, 0.f);

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

bool Pacman::Draw() const
{
	myWorld->Draw(myDrawer);
	myAvatar->Draw(myDrawer);
	myGhost->Draw(myDrawer);

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

	return true;
}
