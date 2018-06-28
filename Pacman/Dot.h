#pragma once

#include <memory>

#include "StaticGameEntity.h"
#include "Vector2f.h"


class Dot : public StaticGameEntity
{
public:
	Dot(Vector2f aPosition);
};
using DotPtr = std::shared_ptr<Dot>;