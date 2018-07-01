#include "GameEntity.h"
#include "Drawer.h"
#include "Constants.h"

GameEntity::GameEntity(const Vector2f& aPosition, Drawer* aDrawer, std::string&& anImage)
:myIdMarkedForDeleteFlag(false)
,myPosition(aPosition)
,myImage(anImage)
,myDrawer(aDrawer)
{
	aDrawer->RegisterImage(anImage);
}

void GameEntity::Draw() const
{
	myDrawer->Draw(myImage, static_cast<int>(myPosition.myX), static_cast<int>(myPosition.myY));
}


bool GameEntity::Intersect(GameEntity* aGameEntity)
{
	return false;	
}
