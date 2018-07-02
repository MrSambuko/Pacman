#pragma once


#include "Ghost.h"


class OrangeGhost : public Ghost
{
public:
	OrangeGhost( const Vector2f& aPosition, Drawer* aDrawer, World* aWorld );

protected:
	void GetNextTile( const Pacman* aPacman ) override;
};