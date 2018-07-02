#include <cassert>

#include "Constants.h"
#include "RedGhost.h"
#include "World.h"

namespace
{
const char IMAGE[] = "ghost_32_red.png";
}

RedGhost::RedGhost(const Vector2f & aPosition, Drawer * aDrawer, World * aWorld) 
: Ghost(aPosition, aDrawer, aWorld, IMAGE) 
, initialDotsNum(aWorld->DotsLeft())
{
}


void RedGhost::Update(float aTime, int anAvatarTileX, int anAvatarTileY)
{
	if (myState == FRIGHTENED)
		mySpeed = DEAD_GHOST_SPEED;

	const auto& numDots = myWorld->DotsLeft();

	if (numDots < initialDotsNum * 0.33f)
	{
		mySpeed *= 1.05f;
	}
	else if (numDots < initialDotsNum * 0.66f)
	{
		mySpeed *= 1.05f;
	}

	Ghost::Update(aTime, anAvatarTileX, anAvatarTileY);
}




void RedGhost::GetNextTile( int anAvatarPositionX, int anAvatarPositionY )
{
	switch (myState)
	{
	case CHASE:
	{
		// each step choose player's tile as target
		std::vector<PathmapTilePtr> pathToAvatar;
		myWorld->GetPath(myCurrentTileX, myCurrentTileY, anAvatarPositionX, anAvatarPositionY, &pathToAvatar);
		const size_t& index = pathToAvatar.size() - 1;
		myNextTileX = pathToAvatar[index]->myX;
		myNextTileY = pathToAvatar[index]->myY;
		break;
	}

	case SCATTER:
		if (myWorld->DotsLeft() < initialDotsNum * 0.25f)
		{
			std::vector<PathmapTilePtr> pathToAvatar;
			myWorld->GetPath(anAvatarPositionX, anAvatarPositionY, myCurrentTileX, myCurrentTileY, &pathToAvatar);
			myNextTileX = pathToAvatar[0]->myX;
			myNextTileY = pathToAvatar[0]->myY;
		}
		else
		{
			myWorld->GetPath(myCurrentTileX, myCurrentTileY, TOP_LEFT_X, TOP_LEFT_Y, &myPath);
		}
		break;

	case FRIGHTENED:
		break;

	case DEAD:
		break;

	default:
		assert(false && "Unknown ghost state");
		break;
	}
}
