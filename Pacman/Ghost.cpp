#include "Ghost.h"
#include "World.h"
#include "PathmapTile.h"
#include "Drawer.h"

Ghost::Ghost(const Vector2f& aPosition)
: MovableGameEntity(aPosition, "ghost_32.png")
{
	myIsClaimableFlag = false;
	myIsDeadFlag = false;

	myDesiredMovementX = 0;
	myDesiredMovementY = -1;
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
			PathmapTile* nextTile = myPath.front();
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

	constexpr const int tileSize = 22;
	const Vector2f destination(static_cast<float>(myNextTileX * tileSize), static_cast<float>(myNextTileY * tileSize));
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

void Ghost::Draw(Drawer* aDrawer)
{
	if (myIsDeadFlag)
		aDrawer->Draw("Ghost_Dead_32.png", static_cast<int>(myPosition.myX) + 220, static_cast<int>(myPosition.myY) + 60);
	else if (myIsClaimableFlag)
		aDrawer->Draw("Ghost_Vulnerable_32.png", static_cast<int>(myPosition.myX) + 220, static_cast<int>(myPosition.myY) + 60);
	else
		aDrawer->Draw(myImage, static_cast<int>(myPosition.myX) + 220, static_cast<int>(myPosition.myY) + 60);
}
