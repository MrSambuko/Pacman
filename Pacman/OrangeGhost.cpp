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
			if (pathToAvatar.size() == 0)
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

#ifdef _DEBUG
			{
				int x = pathToAvatar[0]->myX * TILE_SIZE + X_OFFSET;
				int y = pathToAvatar[0]->myY * TILE_SIZE + Y_OFFSET;
				myDrawer->DrawLine(myPosition.myX + X_OFFSET, myPosition.myY + Y_OFFSET, x, y);
			}
#endif
		}
		break;
	}
		

	case SCATTER:
		myWorld->GetPath(myCurrentTileX, myCurrentTileY, BOTTOM_LEFT_X, BOTTOM_LEFT_Y, &myPath);
#ifdef _DEBUG
		{
			int x = BOTTOM_LEFT_X * TILE_SIZE + X_OFFSET;
			int y = BOTTOM_LEFT_Y * TILE_SIZE + Y_OFFSET;
			myDrawer->DrawLine(myPosition.myX + X_OFFSET, myPosition.myY + Y_OFFSET, x, y);
		}
#endif
		break;

	case FRIGHTENED:
	case DEAD:
		break;
	}
}
