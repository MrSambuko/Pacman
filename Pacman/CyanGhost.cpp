#include <cassert>
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
			if (myWorld->TileIsValid(redGhostPosX+vectToAvatarX, redGhostPosY+vectToAvatarY))
			{	
				myWorld->GetPath(myCurrentTileX, myCurrentTileY, redGhostPosX+vectToAvatarX, redGhostPosY+vectToAvatarY, &path);
			}
			else
			{
				myWorld->GetPath(myCurrentTileX, myCurrentTileY, avatarPosX, avatarPoxY, &path);
			}

			const size_t& index = path.size() - 1;
			myNextTileX = path[index]->myX;
			myNextTileY = path[index]->myY;

			break;
		}
		

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
