#pragma once

#include <list>
#include "MovableGameEntity.h"
#include "PathmapTile.h"

class World;

class Ghost : public MovableGameEntity
{
public:
	Ghost(const Vector2f& aPosition);

	void Update(float aTime, World* aWorld);

	bool myIsClaimableFlag;
	bool myIsDeadFlag;

	void SetImage(const char* anImage);

	void Die(World* aWorld);

	void Draw(Drawer* aDrawer);

protected:

	int myDesiredMovementX;
	int myDesiredMovementY;

	std::list<PathmapTilePtr> myPath;

};