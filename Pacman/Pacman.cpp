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

constexpr const float CYAN_START_TIME = 15.f;
constexpr const float ORANGE_START_TIME = 5.f;
constexpr const float PINK_START_TIME = 10.f;
constexpr const size_t CYAN_INDEX = 1;
constexpr const size_t ORANGE_INDEX = 2;
constexpr const size_t PINK_INDEX = 3;
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

Pacman::~Pacman()
{
	for (auto&& ghost : myGhosts)
		delete ghost;

	delete myAvatar;
	delete myWorld;
}

Pacman::Pacman(Drawer* aDrawer)
: myState(GAMEPLAY)
, myDrawer(aDrawer)
, myGhostSwitchStateTimer(0.f)
, myGhostSpawnTimer(.0f)
, myLives(3)
, myScore(0)
, myFps(0)
, myNextMovement(NO_DIRECTION)
, myDirection(NO_MOVE)
{
	myWorld = new World(myDrawer);
	myAvatar = new Avatar(START_PLAYER_POS, aDrawer);

	myGhosts[0] = new RedGhost(START_GHOST_POS, aDrawer, myWorld);
	//myGhosts[1] = new PinkGhost(START_GHOST_POS, aDrawer, myWorld);
	//myGhosts[2] = new OrangeGhost(START_GHOST_POS, aDrawer, myWorld);
	//myGhosts[3] = new CyanGhost(START_GHOST_POS, aDrawer, myWorld);

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

	if (myState == WIN_SCREEN || CheckEndGameCondition())
	{
		myDrawer->DrawMiddleScreenText("You win! Press any key to start over");
		myState = WIN_SCREEN;
		return true;
	}
	
	if (myState == LOSE_SCREEN || myLives <= 0)
	{
		myDrawer->DrawMiddleScreenText("Game Over! Press any key to start over");	
		myState = LOSE_SCREEN;
		return true;
	}

	CheckIntersections();

	UpdateAvatar(aTime);

	UpdateGhosts(aTime);

	SpawnGhosts(aTime);

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
		
		switch(myState)
		{
		case GAMEPLAY:
			for (const auto& directionVector : DIRECTION_TO_MOVE)
				if (myDirection & directionVector.first)
				{
					myNextMovement += directionVector.second;
					break;
				}
			break;

		case WIN_SCREEN:
			return false;
			break;

		case LOSE_SCREEN:
			Reset();
			return true;
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

void Pacman::SpawnGhosts(float aTime)
{
	myGhostSpawnTimer += aTime;

	if (myGhosts[CYAN_INDEX] == nullptr && myGhostSpawnTimer > CYAN_START_TIME)
		myGhosts[CYAN_INDEX] = new CyanGhost(START_GHOST_POS, myDrawer, myWorld);

	if (myGhosts[ORANGE_INDEX] == nullptr && myGhostSpawnTimer > ORANGE_START_TIME)
		myGhosts[ORANGE_INDEX] = new OrangeGhost(START_GHOST_POS, myDrawer, myWorld);

	if (myGhosts[PINK_INDEX] == nullptr && myGhostSpawnTimer > PINK_START_TIME)
		myGhosts[PINK_INDEX] = new PinkGhost(START_GHOST_POS, myDrawer, myWorld);
}

void Pacman::UpdateAvatar(float aTime) const
{
	MoveAvatar();
	myAvatar->Update(aTime);
}

void Pacman::UpdateGhosts(float aTime)
{
	const auto& x = myAvatar->GetCurrentTileX();
	const auto& y = myAvatar->GetCurrentTileY();
	int xDir = 0;
	int yDir = 0;
	myAvatar->GetDirection(&xDir, &yDir);
	for (auto& ghost : myGhosts)
		if (ghost != nullptr)
			ghost->Update(aTime, this);

	myGhostSwitchStateTimer -= aTime;
	if (myGhostSwitchStateTimer <= 0)
	{
		static int i = 0;
		const auto& newState = i++ % 2 ? SCATTER : CHASE;
		for (auto& ghost : myGhosts)
			if (ghost != nullptr)
				ghost->ChangeState(newState);
		myGhostSwitchStateTimer = 20.f;
	}
}

void Pacman::CheckIntersections()
{
	if (myWorld->HasIntersectedDot(myAvatar->GetPosition()))
		myScore += 10;

	if (myWorld->HasIntersectedBigDot(myAvatar->GetPosition()))
	{
		myScore += 20;
		myGhostSwitchStateTimer = 20.f;
		for (auto& ghost : myGhosts)
			if (ghost != nullptr)
				ghost->ChangeState(FRIGHTENED);
	}

	bool hitPlayer = false;
	for (auto& ghost : myGhosts)
	{
		if (ghost == nullptr)
			continue;

		const auto& state = ghost->GetState();
		
		if ((ghost->GetPosition() - myAvatar->GetPosition()).Length() < TILE_SIZE * 0.5f)
		{
			switch (state)
			{
			case CHASE:
			case SCATTER:
				hitPlayer = true;
				break;

			case FRIGHTENED:
				myScore += 50;
				ghost->ChangeState(DEAD);
				break;

			case DEAD:
				break;
			}
		}

		if (hitPlayer)
			break;
	}

	if (hitPlayer)
	{
		--myLives;
		Reset();
	}
}

void Pacman::Reset()
{
	myAvatar->Reset(START_PLAYER_POS);
	myGhosts[0]->Reset(START_GHOST_POS);
	delete myGhosts[CYAN_INDEX];  myGhosts[CYAN_INDEX] = nullptr;
	delete myGhosts[ORANGE_INDEX];myGhosts[ORANGE_INDEX] = nullptr;
	delete myGhosts[PINK_INDEX];  myGhosts[PINK_INDEX] = nullptr;
	myGhostSpawnTimer = .0f;
	myGhostSwitchStateTimer = .0f;
}

void Pacman::Draw() const
{
	if (myState == GAMEPLAY)
	{
		myWorld->Draw();
		myAvatar->Draw();
		for (const auto& ghost : myGhosts)
			if (ghost != nullptr)
				ghost->Draw();

		myDrawer->DrawLabel(SCORE_LABEL);
		myDrawer->DrawText(std::to_string(myScore), 110, 50);
	
		myDrawer->DrawLabel(LIVES_LABEL);
		myDrawer->DrawText(std::to_string(myLives), 110, 80);

		myDrawer->DrawLabel(FPS_LABEL);
		myDrawer->DrawText(std::to_string(myFps), 950, 50);
	}

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
