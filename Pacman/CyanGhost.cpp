#include <cassert>
#include "World.h"

#include "CyanGhost.h"


namespace
{
const char IMAGE[] = "ghost_32_cyan.png";
}

CyanGhost::CyanGhost( const Vector2f& aPosition, Drawer* aDrawer, World* aWorld): Ghost(
	aPosition, aDrawer, aWorld, IMAGE)
{
}

void CyanGhost::GetNextTile( int anAvatarPositionX, int anAvatarPositionY )
{
	switch (myState)
	{
	case CHASE:
		break;

	case SCATTER:
		myWorld->GetPath(myCurrentTileX, myCurrentTileY, BOTTOM_RIGHT_X, BOTTOM_RIGHT_Y, &myPath);
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
