#include <iostream>

#include "Ghost.h"
#include "World.h"
#include "PathmapTile.h"
#include "Drawer.h"


namespace
{
	const char GHOST[] = "ghost_32.png";
	const char GHOST_DEAD[] = "Ghost_Dead_32.png";
	const char GHOST_VULNERABLE[] = "Ghost_Vulnerable_32.png";
}

Ghost::Ghost(const Vector2f& aPosition, Drawer* aDrawer, World* aWorld, std::string&& anImage)
: MovableGameEntity(aPosition, aDrawer, std::move(anImage))
, myWorld(aWorld)
, mySpeed(GHOST_SPEED)
, myState(CHASE)
{
	myIsClaimableFlag = false;
	myIsDeadFlag = false;

	myDesiredMovementX = 0;
	myDesiredMovementY = -1;

	aDrawer->RegisterImage(GHOST_DEAD);
	aDrawer->RegisterImage(GHOST_VULNERABLE);
}

void Ghost::ChangeState(GhostState aNewState)
{
	myState = aNewState;
	myPath.clear();
}

void Ghost::Update(float aTime, int anAvatarTileX, int anAvatarTileY)
{
	int nextTileX = 0;
	int nextTileY = 0;
	
	if (myPath.empty())
		GetNextTile(&myNextTileX, &myNextTileY, anAvatarTileX, anAvatarTileY);

	if (IsAtDestination())
	{
		if (!myPath.empty())
		{
			const auto& nextTile = myPath.back();
			myPath.pop_back();
			SetNextTile(nextTile->myX, nextTile->myY);
		}
		else
		{
			SetNextTile(nextTileX, nextTileY);
		}
	}

	if (myState == FRIGHTENED)
	{
		const auto& tile = myWorld->getRandomNearbyTile(myNextTileX, myNextTileY, myCurrentTileX, myCurrentTileY);
		myNextTileX = tile->myX;
		myNextTileY = tile->myY;
	}

	const Vector2f destination(static_cast<float>(myNextTileX * TILE_SIZE), static_cast<float>(myNextTileY * TILE_SIZE));
	Vector2f direction = destination - myPosition;

	const float distanceToMove = aTime * mySpeed;

	if (distanceToMove > direction.Length())
	{
		myPosition = destination;
		myCurrentTileX = myNextTileX;
		myCurrentTileY = myNextTileY;
	}
	else
	{
		direction.Normalize();
		myPosition += direction * distanceToMove;
	}
}


void Ghost::Draw() const
{
	if (myIsDeadFlag)
		myDrawer->Draw(GHOST_DEAD, static_cast<int>(myPosition.myX), static_cast<int>(myPosition.myY));
	else if (myIsClaimableFlag)
		myDrawer->Draw(GHOST_VULNERABLE, static_cast<int>(myPosition.myX), static_cast<int>(myPosition.myY));
	else
		myDrawer->Draw(myImage, static_cast<int>(myPosition.myX), static_cast<int>(myPosition.myY));
}

void Ghost::Reset( const Vector2f& toPosition )
{
	MovableGameEntity::Reset(toPosition);
	myIsDeadFlag = false;
	myDesiredMovementX = 0;
	myDesiredMovementY = -1;
}

void Ghost::GoHome()
{
	myWorld->GetPath(myCurrentTileX, myCurrentTileY, START_GHOST_TILE_X, START_GHOST_TILE_Y, &myPath);
}
