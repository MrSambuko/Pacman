#ifdef _DEBUG
#include "Drawer.h"
#endif
#include "Pacman.h"
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
	{
		int avatarPosX = 0;
		int avatarPosY = 0;
		aPacman->GetAvatarPosition(&avatarPosX, &avatarPosY);

		if (abs(myCurrentTileX - avatarPosX) + abs(myCurrentTileY - avatarPosY) < 8)
		{
			std::vector<PathmapTilePtr> pathToAvatar;
			myWorld->GetPath(BOTTOM_LEFT_X, BOTTOM_LEFT_Y, myCurrentTileX, myCurrentTileY, &pathToAvatar);
			if (pathToAvatar.size() < 2)
				break;
			myNextTileX = pathToAvatar[1]->myX;
			myNextTileY = pathToAvatar[1]->myY;
		}
		else
		{
			std::vector<PathmapTilePtr> pathToAvatar;
			myWorld->GetPath(avatarPosX, avatarPosY, myCurrentTileX, myCurrentTileY, &pathToAvatar);

			if (pathToAvatar.size() < 2)
				break;
			myNextTileX = pathToAvatar[1]->myX;
			myNextTileY = pathToAvatar[1]->myY;
		}
		break;
	}
		

	case SCATTER:
		myWorld->GetPath(myCurrentTileX, myCurrentTileY, BOTTOM_LEFT_X, BOTTOM_LEFT_Y, &myPath);
		break;

	case FRIGHTENED:
	case DEAD:
		break;
	}
}
