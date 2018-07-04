#include "Ghost.h"
#include "World.h"
#include "Pacman.h"
#include "PathmapTile.h"
#include "Drawer.h"


namespace
{
	const char GHOST_DEAD[] = "Ghost_Dead_32.png";
	const char GHOST_VULNERABLE[] = "Ghost_Vulnerable_32.png";
}

Ghost::Ghost(const Vector2f& aPosition, Drawer* aDrawer, World* aWorld, std::string&& anImage)
: MovableGameEntity(aPosition, aDrawer, std::move(anImage))
, myWorld(aWorld)
, mySpeed(GHOST_SPEED)
, myState(CHASE)
, myPreviousTileX(0)
, myPreviousTileY(0)
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
	if (myState == DEAD)
		mySpeed = DEAD_GHOST_SPEED;
	else
		mySpeed = GHOST_SPEED;

	myPath.clear();
}

void Ghost::Update( float aTime, const Pacman* aPacman )
{
	const auto popNext = [&]()
	{
		const auto& nextTile = myPath.back();
		myPath.pop_back();
		SetNextTile(nextTile->myX, nextTile->myY);
	};

	if (IsAtDestination())
	{
		if (myState == FRIGHTENED)
		{
			const auto& tile = myWorld->GetRandomNearbyTile(myCurrentTileX, myCurrentTileY, myPreviousTileX, myPreviousTileY);
			myNextTileX = tile->myX;
			myNextTileY = tile->myY;
		}
		else if (myState == DEAD && myPath.empty())
		{
			if (myCurrentTileX != START_GHOST_TILE_X && myCurrentTileY != START_GHOST_TILE_Y)
			{
				GoHome();
				popNext();
			}
			else
			{
				ChangeState(CHASE);
			}
		}
		else
		{
			if (myPath.empty())
			{
				GetNextTile(aPacman);
			}
			else
			{
				popNext();
			}
		}
	}	


	const Vector2f destination(static_cast<float>(myNextTileX * TILE_SIZE), static_cast<float>(myNextTileY * TILE_SIZE));
	Vector2f direction = destination - myPosition;

	const float distanceToMove = aTime * mySpeed;

	if (distanceToMove > direction.Length())
	{
		myPosition = destination;
		myPreviousTileX = myCurrentTileX;
		myPreviousTileY = myCurrentTileY;
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
	switch (myState)
	{
	case FRIGHTENED:
		myDrawer->Draw(GHOST_VULNERABLE, static_cast<int>(myPosition.myX), static_cast<int>(myPosition.myY));
		break;
	case DEAD:
		myDrawer->Draw(GHOST_DEAD, static_cast<int>(myPosition.myX), static_cast<int>(myPosition.myY));
		break;
	default:
		myDrawer->Draw(myImage, static_cast<int>(myPosition.myX), static_cast<int>(myPosition.myY));
	}
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
