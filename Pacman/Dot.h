#pragma once

#include <memory>

#include "StaticGameEntity.h"
#include "Vector2f.h"


class Dot : public StaticGameEntity
{
public:
	Dot(Vector2f aPosition, Drawer* aDrawer);
};
using DotPtr = std::shared_ptr<Dot>;