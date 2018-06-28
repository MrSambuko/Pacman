#include "Avatar.h"
#include "Common.h"

Avatar::Avatar(const Vector2f& aPosition)
: MovableGameEntity(aPosition, "open_32.png")
{

}

void Avatar::Update(float aTime)
{
	const Vector2f destination(static_cast<float>(myNextTileX * TILE_SIZE), static_cast<float>(myNextTileY * TILE_SIZE));
	Vector2f direction = destination - myPosition;

	const float distanceToMove = aTime * 30.f;

	if (distanceToMove > direction.Length())
	{
		myPosition = destination;
		myCurrentTileX = myNextTileX;
		myCurrentTileY = myNextTileY;
	}
	else
	{
		direction.Normalize();
		myPosition += direction * distanceToMove;
	}
}
