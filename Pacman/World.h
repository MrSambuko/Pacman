#pragma once


#include <list>
#include "Vector2f.h"
#include "Dot.h"
#include "BigDot.h"
#include "Cherry.h"
#include "PathmapTile.h"

class Drawer;

class World
{
public:
	void Init();

	void Draw(Drawer* aDrawer);
	bool TileIsValid(int anX, int anY);

	bool HasIntersectedDot(const Vector2f& aPosition);
	bool HasIntersectedBigDot(const Vector2f& aPosition);
	bool HasIntersectedCherry(const Vector2f& aPosition);

	void Update();

	void GetPath(int aFromX, int aFromY, int aToX, int aToY, std::list<PathmapTilePtr>& aList);

private:

	PathmapTilePtr GetTile(int aFromX, int aFromY);
	bool Pathfind(PathmapTilePtr aFromTile, PathmapTilePtr aToTile, std::list<PathmapTilePtr>& aList);
	bool ListDoesNotContain(PathmapTilePtr aFromTile, std::list<PathmapTilePtr>& aList);


	bool InitPathmap();
	bool InitDots();
	bool InitBigDots();

	std::list<PathmapTilePtr> myPathmapTiles;
	std::list<DotPtr> myDots;
	std::list<BigDotPtr> myBigDots;
	std::list<CherryPtr> myCherry; //?? unused

};