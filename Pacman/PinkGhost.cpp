#include <cassert>

#ifdef _DEBUG
#include "Drawer.h"
#endif
#include "Pacman.h"
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

void PinkGhost::GetNextTile( const Pacman* aPacman )
{
	switch (myState)
	{
	case CHASE:
	{
		int avatarPositionX = 0;
		int avatarPositionY = 0;
		int avatarDirectionX = 0;
		int avatarDirectionY = 0;
		aPacman->GetAvatarDirection(&avatarDirectionX, &avatarDirectionY);
		aPacman->GetAvatarPosition(&avatarPositionX, &avatarPositionY);
		if (avatarDirectionX == 0 && avatarDirectionY == 0)
			break;
		// get 4 tiles ahead of avatar
		const bool modifyX = avatarDirectionX != 0;
		
		// try to find tile that is valid. If tile is not valid move closer to avatar
		for (int i = 4; i >= 0; --i)
		{
			const auto& x = modifyX ? avatarDirectionX * i : avatarDirectionX;
			const auto& y = modifyX ? avatarDirectionY : avatarDirectionY * i;
			if (myWorld->TileIsValid(avatarPositionX+x, avatarPositionY+y))
			{
				avatarDirectionX = x;
				avatarDirectionY = y;
				break;
			}
		}

		std::vector<PathmapTilePtr> pathToAvatar;
		myWorld->GetPath(myCurrentTileX, myCurrentTileY, avatarPositionX+avatarDirectionX, avatarPositionY+avatarDirectionY, &pathToAvatar);
		if (pathToAvatar.size() == 0)
			break;

		const size_t& index = pathToAvatar.size() - 1;

		myNextTileX = pathToAvatar[index]->myX;
		myNextTileY = pathToAvatar[index]->myY;

#ifdef _DEBUG
		{
			int x = myNextTileX * TILE_SIZE + X_OFFSET;
			int y = myNextTileY * TILE_SIZE + Y_OFFSET;
			myDrawer->DrawLine(myPosition.myX + X_OFFSET, myPosition.myY + Y_OFFSET, x, y);
		}
#endif
		break;
	}

	case SCATTER:
		myWorld->GetPath(myCurrentTileX, myCurrentTileY, TOP_RIGHT_X, TOP_RIGHT_Y, &myPath);
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
