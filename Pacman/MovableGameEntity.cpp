#include "Common.h"
#include "MovableGameEntity.h"

MovableGameEntity::MovableGameEntity(const Vector2f& aPosition, const char* anImage)
: GameEntity(aPosition, anImage)
{
	myCurrentTileX = myNextTileX = static_cast<int>(myPosition.myX / TILE_SIZE);
	myCurrentTileY = myNextTileY = static_cast<int>(myPosition.myY / TILE_SIZE);
}

bool MovableGameEntity::IsAtDestination() const
{
	return myCurrentTileX == myNextTileX && myCurrentTileY == myNextTileY;
}

void MovableGameEntity::SetNextTile(int anX, int anY)
{
	myNextTileX = anX;
	myNextTileY = anY;
}
