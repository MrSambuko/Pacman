#pragma once

#include <list>
#include "MovableGameEntity.h"
#include "PathmapTile.h"
#include <unordered_set>

class World;

enum GhostState
{
	CHASE,
	SCATTER,
	FRIGHTENED,
	DEAD,
};



class Ghost : public MovableGameEntity
{
public:
	Ghost(const Vector2f& aPosition, Drawer* aDrawer, World* aWorld, std::string&& anImage);

	void ChangeState(GhostState aNewState);
	virtual void Update(float aTime, int anAvatarTileX, int anAvatarTileY);

	void Draw() const override;

	void Reset( const Vector2f& toPosition ) override;

	bool myIsClaimableFlag;
	bool myIsDeadFlag;
protected:
	void GoHome();
	virtual void GetNextTile(int* aNextTileX, int* aNextTileY, int anAvatarPositionX, int anAvatarPositionY) = 0;

	World* myWorld;
	int myDesiredMovementX;
	int myDesiredMovementY;

	float mySpeed;

	GhostState myState;
	std::vector<PathmapTilePtr> myPath;

};