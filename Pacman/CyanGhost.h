#pragma once

#include "Ghost.h"


class CyanGhost : public Ghost
{
public:
	CyanGhost( const Vector2f& aPosition, Drawer* aDrawer, World* aWorld);

protected:
	void GetNextTile( int anAvatarPositionX, int anAvatarPositionY ) override;
};