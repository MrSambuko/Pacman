#include "GameEntity.h"
#include "Drawer.h"

GameEntity::GameEntity(const Vector2f& aPosition, const char* anImage)
:myIdMarkedForDeleteFlag(false)
,myPosition(aPosition)
,myImage(anImage)
{
}

bool GameEntity::Intersect(GameEntity* aGameEntity)
{
	return false;	
}

void GameEntity::Draw(Drawer* aDrawer)
{
	aDrawer->Draw(myImage, static_cast<int>(myPosition.myX) + 220, static_cast<int>(myPosition.myY) + 60);
}
