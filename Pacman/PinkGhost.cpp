#include <cassert>

#include "World.h"
#include "PinkGhost.h"


namespace
{
const char IMAGE[] = "ghost_32_pink.png";
}

PinkGhost::PinkGhost( const Vector2f& aPosition, Drawer* aDrawer, World* aWorld ): Ghost(
	aPosition, aDrawer, aWorld, IMAGE)
{
}

void PinkGhost::GetNextTile( int anAvatarPositionX, int anAvatarPositionY )
{
	switch (myState)
	{
	case CHASE:
		break;

	case SCATTER:
		myWorld->GetPath(myCurrentTileX, myCurrentTileY, TOP_LEFT_X, TOP_LEFT_Y, &myPath);
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
