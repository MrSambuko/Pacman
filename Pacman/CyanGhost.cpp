#include <cassert>

#ifdef _DEBUG
#include "Drawer.h"
#endif
#include "World.h"
#include "Pacman.h"
#include "CyanGhost.h"


namespace
{
const char IMAGE[] = "ghost_32_cyan.png";
}

CyanGhost::CyanGhost( const Vector2f& aPosition, Drawer* aDrawer, World* aWorld): Ghost(
	aPosition, aDrawer, aWorld, IMAGE)
{
}

void CyanGhost::GetNextTile( const Pacman* aPacman )
{
	switch (myState)
	{
	case CHASE:
		{
			int avatarPosX = 0;
			int avatarPoxY = 0;
			int redGhostPosX = 0;
			int redGhostPosY = 0;
			aPacman->GetAvatarPosition(&avatarPosX, &avatarPoxY);
			aPacman->GetRedGhostPos(&redGhostPosX, &redGhostPosY);
			int vectToAvatarX = avatarPosX - redGhostPosX;
			int vectToAvatarY = avatarPoxY - redGhostPosY;
			
			vectToAvatarX *= 2;
			vectToAvatarY *= 2;

			std::vector<PathmapTilePtr> path;
			int x;
			int y;
			if (myWorld->TileIsValid(redGhostPosX+vectToAvatarX, redGhostPosY+vectToAvatarY))
			{	
				x = redGhostPosX + vectToAvatarX;
				y = redGhostPosY + vectToAvatarY;
			}
			else
			{
				x = avatarPosX;
				y = avatarPoxY;
			}

			myWorld->GetPath(myCurrentTileX, myCurrentTileY, x, y, &path);
#ifdef _DEBUG
			x = x * TILE_SIZE + X_OFFSET;
			y = y * TILE_SIZE + Y_OFFSET;
			myDrawer->DrawLine(myPosition.myX+X_OFFSET, myPosition.myY+Y_OFFSET, x, y);
#endif

			const size_t& index = path.size() - 1;
			myNextTileX = path[index]->myX;
			myNextTileY = path[index]->myY;

			break;
		}
		

	case SCATTER:
		myWorld->GetPath(myCurrentTileX, myCurrentTileY, BOTTOM_RIGHT_X, BOTTOM_RIGHT_Y, &myPath);
#ifdef _DEBUG
		{
			int x = BOTTOM_RIGHT_X * TILE_SIZE + X_OFFSET;
			int y = BOTTOM_RIGHT_Y * TILE_SIZE + Y_OFFSET;
			myDrawer->DrawLine(myPosition.myX + X_OFFSET, myPosition.myY + Y_OFFSET, x, y);
		}
#endif
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
