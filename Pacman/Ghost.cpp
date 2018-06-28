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

Ghost::Ghost(const Vector2f& aPosition, Drawer* aDrawer)
: MovableGameEntity(aPosition, aDrawer, GHOST)
{
	myIsClaimableFlag = false;
	myIsDeadFlag = false;

	myDesiredMovementX = 0;
	myDesiredMovementY = -1;

	aDrawer->registerImage(GHOST_DEAD);
	aDrawer->registerImage(GHOST_VULNERABLE);
}

void Ghost::Die(World* aWorld)
{
	myPath.clear();
	aWorld->GetPath(myCurrentTileX, myCurrentTileY, 13, 13, myPath);
}

void Ghost::Update(float aTime, World* aWorld)
{
	float speed = 30.f;
	const int nextTileX = GetCurrentTileX() + myDesiredMovementX;
	const int nextTileY = GetCurrentTileY() + myDesiredMovementY;

	if (myIsDeadFlag)
		speed = 120.f;

	if (IsAtDestination())
	{
		if (!myPath.empty())
		{
			const PathmapTilePtr nextTile = myPath.front();
			myPath.pop_front();
			SetNextTile(nextTile->myX, nextTile->myY);
		}
		else if (aWorld->TileIsValid(nextTileX, nextTileY))
		{
			SetNextTile(nextTileX, nextTileY);
		}
		else
		{
			if (myDesiredMovementX == 1)
			{
				myDesiredMovementX = 0;
				myDesiredMovementY = 1;
			} else if (myDesiredMovementY == 1)
			{
				myDesiredMovementX = -1;
				myDesiredMovementY = 0;			
			} else if (myDesiredMovementX == -1)
			{
				myDesiredMovementX = 0;
				myDesiredMovementY = -1;
			} else
			{
				myDesiredMovementX = 1;
				myDesiredMovementY = 0;
			}

			myIsDeadFlag = false;
		}
	}

	const Vector2f destination(static_cast<float>(myNextTileX * TILE_SIZE), static_cast<float>(myNextTileY * TILE_SIZE));
	Vector2f direction = destination - myPosition;

	const float distanceToMove = aTime * speed;

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

void Ghost::SetImage(const char* anImage)
{
	myImage = anImage;
}

void Ghost::Draw() const
{
	if (myIsDeadFlag)
		myDrawer->Draw(GHOST_DEAD, static_cast<int>(myPosition.myX), static_cast<int>(myPosition.myY));
	else if (myIsClaimableFlag)
		myDrawer->Draw(GHOST_VULNERABLE, static_cast<int>(myPosition.myX) + 220, static_cast<int>(myPosition.myY));
	else
		myDrawer->Draw(myImage, static_cast<int>(myPosition.myX), static_cast<int>(myPosition.myY));
}
