#pragma once

#include <list>
#include "MovableGameEntity.h"
#include "PathmapTile.h"
#include <unordered_set>

class World;
class Pacman;

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
	GhostState GetState() const { return myState; }
	virtual void Update( float aTime, const Pacman* aPacman );

	void Draw() const override;

	void Reset( const Vector2f& toPosition ) override;

	bool myIsClaimableFlag;
	bool myIsDeadFlag;
protected:
	void GoHome();
	virtual void GetNextTile( const Pacman* aPacman ) = 0;

	World* myWorld;
	int myDesiredMovementX;
	int myDesiredMovementY;

	float mySpeed;

	GhostState myState;
	std::vector<PathmapTilePtr> myPath;

private:
	int myPreviousTileX;
	int myPreviousTileY;
};