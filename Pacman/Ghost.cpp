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
	aWorld->GetPath(myCurrentTileX, myCurrentTileY, START_GHOST_TILE_X, START_GHOST_TILE_Y, myPath);
	myNextTileX = myCurrentTileX;
	myNextTileY = myCurrentTileY;
}

void Ghost::Update(float aTime, World* aWorld)
{
	const float& speed = myIsDeadFlag ? DEAD_GHOST_SPEED : GHOST_SPEED;
	const int nextTileX = GetCurrentTileX() + myDesiredMovementX;
	const int nextTileY = GetCurrentTileY() + myDesiredMovementY;

	static auto goingLeft = [&] () { return myDesiredMovementX == -1 && myDesiredMovementY ==  0; };
	static auto goingUp   = [&] () { return myDesiredMovementX ==  0 && myDesiredMovementY == -1; };
	static auto goingRight= [&] () { return myDesiredMovementX ==  1 && myDesiredMovementY ==  0; };
	static auto goingDown = [&] () { return myDesiredMovementX ==  0 && myDesiredMovementY ==  1; };
	static auto goLeft = [&] () { myDesiredMovementX = -1; myDesiredMovementY =  0; };
	static auto goUp   = [&] () { myDesiredMovementX =  0; myDesiredMovementY = -1; };
	static auto goRight= [&] () { myDesiredMovementX =  1; myDesiredMovementY =  0; };
	static auto goDown = [&] () { myDesiredMovementX =  0; myDesiredMovementY =  1; };
	static auto canGoLeft = [&] () { return aWorld->TileIsValid(myCurrentTileX-1, myCurrentTileY+0); };
	static auto canGoUp   = [&] () { return aWorld->TileIsValid(myCurrentTileX+0, myCurrentTileY-1); };
	static auto canGoRight= [&] () { return aWorld->TileIsValid(myCurrentTileX+1, myCurrentTileY+0); };
	static auto canGoDown = [&] () { return aWorld->TileIsValid(myCurrentTileX+0, myCurrentTileY+1); };


	if (IsAtDestination())
	{
		if (!myPath.empty())
		{
			const auto& nextTile = myPath.back();
			std::cout << "going next to " << nextTile->myX << " " << nextTile->myY << "\n";
			myPath.pop_back();
			SetNextTile(nextTile->myX, nextTile->myY);
		}
		else if (aWorld->TileIsValid(nextTileX, nextTileY))
		{
			SetNextTile(nextTileX, nextTileY);
		}
		else
		{
			if (goingLeft())
			{
				if (canGoUp())
					goUp();
				else if (canGoDown())
					goDown();
				else
					goRight();
			}
			else if (goingUp())
			{
				if (canGoRight())
					goRight();
				else if (canGoLeft())
					goLeft();
				else
					goDown();
			}
			else if (goingRight())
			{
				if (canGoDown())
					goDown();
				else if (canGoUp())
					goUp();
				else
					goLeft();
			}
			else // going down
			{
				if (canGoLeft())
					goLeft();
				else if (canGoRight())
					goRight();
				else
					goUp();
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
