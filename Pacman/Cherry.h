#pragma once

#include "StaticGameEntity.h"
#include "Vector2f.h"
#include <memory>

class Cherry : public StaticGameEntity
{
public:
	Cherry(Vector2f aPosition);
};
using CherryPtr = std::shared_ptr<Cherry>;