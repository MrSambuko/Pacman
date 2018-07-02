#pragma once

#include "Ghost.h"


class PinkGhost : public Ghost
{
public:
	PinkGhost( const Vector2f& aPosition, Drawer* aDrawer, World* aWorld );

protected:
	void GetNextTile( const Pacman* aPacman ) override;
};
