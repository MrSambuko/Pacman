#include "Pacman.h"
#include "Drawer.h"
#include "SDL.h"
#include "SDL_events.h"
#include <string>

#include "Avatar.h"
#include "World.h"
#include "RedGhost.h"

#include <unordered_map>
#include "PinkGhost.h"
#include "OrangeGhost.h"
#include "CyanGhost.h"

namespace {
std::unordered_map<SDL_Keycode, int> KEY_TO_DIRECTION =
{
{ SDLK_w, UP },
{ SDLK_UP, UP },
{ SDLK_d, RIGHT },
{ SDLK_RIGHT, RIGHT },
{ SDLK_s, DOWN },
{ SDLK_DOWN, DOWN },
{ SDLK_a, LEFT },
{ SDLK_LEFT, LEFT },
};

std::unordered_map<int, Vector2f> DIRECTION_TO_MOVE =
{
{ NO_MOVE, NO_DIRECTION },
{ UP,	 UP_DIRECTION},
{ RIGHT, RIGHT_DIRECTION},
{ DOWN,	 DOWN_DIRECTION },
{ LEFT,	 LEFT_DIRECTION }
};

constexpr const char SCORE_LABEL[] = "Score: ";
constexpr const char LIVES_LABEL[] = "Lives: ";
constexpr const char FPS_LABEL[] = "FPS: ";
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
, myNextMovement(NO_DIRECTION)
, myDirection(NO_MOVE)
{
	myWorld = new World(myDrawer);
	myAvatar = new Avatar(START_PLAYER_POS, aDrawer);

	myGhosts[0] = new RedGhost(START_GHOST_POS, aDrawer, myWorld);
	myGhosts[1] = new PinkGhost(START_GHOST_POS, aDrawer, myWorld);
	myGhosts[2] = new OrangeGhost(START_GHOST_POS, aDrawer, myWorld);
	myGhosts[3] = new CyanGhost(START_GHOST_POS, aDrawer, myWorld);

	myDrawer->RegisterTextLabel(SCORE_LABEL, 20, 50);
	myDrawer->RegisterTextLabel(LIVES_LABEL, 20, 80);
	myDrawer->RegisterTextLabel(FPS_LABEL, 880, 50);
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
		myDrawer->DrawText("You win!", 20, 70);
		return true;
	}
	
	if (myLives <= 0)
	{
		myDrawer->DrawText("You lose!", 20, 70);	
		return false;
	}

	MoveAvatar();
	myAvatar->Update(aTime);
	const auto& x = myAvatar->GetCurrentTileX();
	const auto& y = myAvatar->GetCurrentTileY();
	int xDir = 0;
	int yDir = 0;
	myAvatar->GetDirection(&xDir, &yDir);
	for (auto& ghost : myGhosts)
		ghost->Update(aTime, this);

	if (myWorld->HasIntersectedDot(myAvatar->GetPosition()))
		myScore += 10;

	myGhostGhostCounter -= aTime;

	if (myWorld->HasIntersectedBigDot(myAvatar->GetPosition()))
	{
		myScore += 20;
		myGhostGhostCounter = 20.f;
		for (auto& ghost : myGhosts)
			ghost->ChangeState(FRIGHTENED);
	}

	/*
	if (myGhostGhostCounter <= 0)
	{
		static int i = 0;
		const auto& newState = i++ % 2 ? SCATTER : CHASE;
		for (auto& ghost : myGhosts)
			ghost->ChangeState(newState);
		myGhostGhostCounter = 20.f;
	}
	*/

	for (auto& ghost : myGhosts)
	{
		const auto& state = ghost->GetState();
		
		if ((ghost->GetPosition() - myAvatar->GetPosition()).Length() < TILE_SIZE * 0.5f)
		{
			switch (state)
			{
			case CHASE:
			case SCATTER:
				--myLives;
				Reset();
				break;

			case FRIGHTENED:
				myScore += 50;
				ghost->ChangeState(DEAD);
				break;

			case DEAD:
				break;
			}
		}

	}

	if (aTime > 0)
		myFps = static_cast<int>((1 / aTime));

	return true;
}

bool Pacman::updateInput(const SDL_Event* event)
{
	const Uint8 *keystate = SDL_GetKeyboardState(nullptr);
	
	myNextMovement = NO_DIRECTION;
	myDirection = NO_MOVE;

	if (event->type == SDL_KEYDOWN)
	{
		const auto& it = KEY_TO_DIRECTION.find(event->key.keysym.sym);

		const auto& direction = it == KEY_TO_DIRECTION.end() ? NO_MOVE : it->second;
		
		/*if (event->type == SDL_KEYDOWN)
			myDirection |= direction;
		else
			*/myDirection ^= direction;
		
		for (const auto& directionVector : DIRECTION_TO_MOVE)
			if (myDirection & directionVector.first)
			{
				myNextMovement += directionVector.second;
				break;
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
	return myWorld->DotsLeft() == 0;
}

void Pacman::Reset() const
{
	myAvatar->Reset(START_PLAYER_POS);
	for (auto& ghost : myGhosts)
		ghost->Reset(START_GHOST_POS);
}

void Pacman::Draw() const
{
	myWorld->Draw();
	myAvatar->Draw();
	for (const auto& ghost : myGhosts)
		ghost->Draw();

	myDrawer->DrawLabel(SCORE_LABEL);
	myDrawer->DrawText(std::to_string(myScore), 110, 50);
	
	myDrawer->DrawLabel(LIVES_LABEL);
	myDrawer->DrawText(std::to_string(myLives), 110, 80);

	myDrawer->DrawLabel(FPS_LABEL);
	myDrawer->DrawText(std::to_string(myFps), 950, 50);

	myDrawer->Draw();
}

void Pacman::GetAvatarPosition( int* __restrict aPosX, int*__restrict  aPosY ) const
{
	*aPosX = myAvatar->GetCurrentTileX();
	*aPosY = myAvatar->GetCurrentTileY();
}

void Pacman::GetAvatarDirection( int* __restrict aDirX, int* __restrict aDirY ) const
{
	myAvatar->GetDirection(aDirX, aDirY);
}

void Pacman::GetRedGhostPos( int* __restrict aPosX, int* __restrict aPosY ) const
{
	*aPosX = myGhosts[0]->GetCurrentTileX();
	*aPosY = myGhosts[0]->GetCurrentTileY();
}
