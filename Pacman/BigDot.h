#pragma once


#include "StaticGameEntity.h"
#include "Vector2f.h"
#include <memory>

class BigDot : public StaticGameEntity
{
public:
	BigDot(Vector2f aPosition, Drawer* aDrawer);
};
using BigDotPtr = std::shared_ptr<BigDot>;