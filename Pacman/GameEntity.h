#pragma once


#include "Vector2f.h"
#include <string>

class Drawer;

class GameEntity
{
public:
	GameEntity(const Vector2f& aPosition, Drawer* aDrawer, std::string&& anImage);
	virtual ~GameEntity() = default;

	virtual void Draw() const;

	Vector2f GetPosition() const { return myPosition; }
	void SetPosition(const Vector2f& aPosition){ myPosition = aPosition; }

	bool Intersect(GameEntity* aGameEntity);

	void MarkForDelete() { myIdMarkedForDeleteFlag = true; }
	bool IsMarkedForDelete() const { return myIdMarkedForDeleteFlag; }

protected:
	bool myIdMarkedForDeleteFlag;
	Vector2f myPosition;
	std::string myImage;
	Drawer* myDrawer = nullptr;
};
