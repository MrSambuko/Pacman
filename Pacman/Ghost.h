#pragma once

#include <list>
#include "MovableGameEntity.h"
#include "PathmapTile.h"
#include <unordered_set>

class World;

class Ghost : public MovableGameEntity
{
public:
	Ghost(const Vector2f& aPosition, Drawer* aDrawer);

	void Update(float aTime, World* aWorld);

	bool myIsClaimableFlag;
	bool myIsDeadFlag;

	void SetImage(const char* anImage);

	void Die(World* aWorld);

	void Draw() const override;

	void Reset( const Vector2f& toPosition ) override;
protected:

	int myDesiredMovementX;
	int myDesiredMovementY;

	std::vector<PathmapTilePtr> myPath;

};