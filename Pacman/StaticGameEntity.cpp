#include "StaticGameEntity.h"

StaticGameEntity::StaticGameEntity(Vector2f aPosition, Drawer* aDrawer, std::string&& anImage)
: GameEntity(aPosition, aDrawer, std::move(anImage))
{
}