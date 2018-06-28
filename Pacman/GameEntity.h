#pragma once


#include "Vector2f.h"

class Drawer;

class GameEntity
{
public:
	GameEntity(const Vector2f& aPosition, const char* anImage);
	virtual ~GameEntity() = default;

	Vector2f GetPosition() const { return myPosition; }
	void SetPosition(const Vector2f& aPosition){ myPosition = aPosition; }

	bool Intersect(GameEntity* aGameEntity);
	virtual void Draw(Drawer* aDrawer);

	void MarkForDelete() { myIdMarkedForDeleteFlag = true; }
	bool IsMarkedForDelete() const { return myIdMarkedForDeleteFlag; }

protected:

	bool myIdMarkedForDeleteFlag;
	Vector2f myPosition;
	const char* myImage;
};
