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
			myWorld->GetPath(myCurrentTileX, myCurrentTileY, BOTTOM_LEFT_X, BOTTOM_LEFT_Y, &pathToAvatar);
			if (pathToAvatar.empty())
				break;
			const size_t& index = pathToAvatar.size() - 1;

			myNextTileX = pathToAvatar[index]->myX;
			myNextTileY = pathToAvatar[index]->myY;
		}
		else
		{
			std::vector<PathmapTilePtr> pathToAvatar;
			myWorld->GetPath(myCurrentTileX, myCurrentTileY, avatarPosX, avatarPosY, &pathToAvatar);
			const size_t& index = pathToAvatar.size() - 1;

			myNextTileX = pathToAvatar[index]->myX;
			myNextTileY = pathToAvatar[index]->myY;
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
