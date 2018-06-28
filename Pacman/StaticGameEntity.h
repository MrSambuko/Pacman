#pragma once


#include "GameEntity.h"
#include "Vector2f.h"

class StaticGameEntity : public GameEntity
{
public:
	StaticGameEntity(Vector2f aPosition, Drawer* aDrawer, std::string&& anImage);
};
