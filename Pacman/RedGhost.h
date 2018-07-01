#pragma once

#include "Ghost.h"


class RedGhost : public Ghost
{
public:
	RedGhost(const Vector2f& aPosition, Drawer* aDrawer, World* aWorld);

	void Update(float aTime, int anAvatarTileX, int anAvatarTileY) override;
private:
	void GetNextTile(int* aNextTileX, int* aNextTileY, int anAvatarPositionX, int anAvatarPositionY) override;

	std::vector<PathmapTilePtr> GetPathToPlayer();

	size_t initialDotsNum;
};