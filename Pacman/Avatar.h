#pragma once

#include "MovableGameEntity.h"
#include "Vector2f.h"

class Avatar : public MovableGameEntity
{
public:
	Avatar(const Vector2f& aPosition, Drawer* aDrawer);

	void Update(float aTime);
	void Draw() const override;

private:
	Vector2f myDirection;
	bool myIsClosed;
};