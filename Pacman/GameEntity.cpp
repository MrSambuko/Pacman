#include "GameEntity.h"
#include "Drawer.h"

GameEntity::GameEntity(const Vector2f& aPosition, Drawer* aDrawer, std::string&& anImage)
:myIdMarkedForDeleteFlag(false)
,myPosition(aPosition)
,myImage(anImage)
,myDrawer(aDrawer)
{
	aDrawer->registerImage(anImage, static_cast<int>(aPosition.myX), static_cast<int>(aPosition.myY));
}

void GameEntity::Draw() const
{
	myDrawer->Draw(myImage);
}


bool GameEntity::Intersect(GameEntity* aGameEntity)
{
	return false;	
}
