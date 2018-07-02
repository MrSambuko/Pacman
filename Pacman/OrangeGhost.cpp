#include "OrangeGhost.h"

#include "World.h"


namespace
{
const char IMAGE[] = "ghost_32_orange.png";
}

OrangeGhost::OrangeGhost( const Vector2f& aPosition, Drawer* aDrawer, World* aWorld ): Ghost(
	aPosition, aDrawer, aWorld, IMAGE)
{
}

void OrangeGhost::GetNextTile( const Pacman* aPacman )
{
	switch (myState)
	{
	case CHASE:
		break;

	case SCATTER:
		myWorld->GetPath(myCurrentTileX, myCurrentTileY, BOTTOM_LEFT_X, BOTTOM_LEFT_Y, &myPath);
		break;

	case FRIGHTENED:
	case DEAD:
		break;
	}
}
