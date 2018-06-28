#include "Common.h"
#include "MovableGameEntity.h"

MovableGameEntity::MovableGameEntity(const Vector2f& aPosition, Drawer* aDrawer, std::string&& aImage)
: GameEntity(aPosition, aDrawer, std::move(aImage))
{
	myCurrentTileX = myNextTileX = static_cast<int>(myPosition.myX / TILE_SIZE);
	myCurrentTileY = myNextTileY = static_cast<int>(myPosition.myY / TILE_SIZE);
}

bool MovableGameEntity::IsAtDestination() const
{
	return myCurrentTileX == myNextTileX && myCurrentTileY == myNextTileY;
}

void MovableGameEntity::Reset( const Vector2f& toPosition )
{
	myCurrentTileX = myNextTileX = static_cast<int>(toPosition.myX / TILE_SIZE);
	myCurrentTileY = myNextTileY = static_cast<int>(toPosition.myY / TILE_SIZE);
}

void MovableGameEntity::SetNextTile(int anX, int anY)
{
	myNextTileX = anX;
	myNextTileY = anY;
}
