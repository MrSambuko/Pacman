#pragma once

#include "Ghost.h"


class RedGhost : public Ghost
{
public:
	RedGhost(const Vector2f& aPosition, Drawer* aDrawer, World* aWorld);

	void Update( float aTime, const Pacman* pacman ) override;
private:
	void GetNextTile( const Pacman* aPacman ) override;

	std::vector<PathmapTilePtr> GetPathToPlayer();

	size_t initialDotsNum;
};