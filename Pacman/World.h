#pragma once


#include <list>
#include "Vector2f.h"
#include "Dot.h"
#include "BigDot.h"
#include "Cherry.h"
#include "PathmapTile.h"
#include <unordered_map>
#include <unordered_set>

class Drawer;

class World
{
public:
	World(Drawer* drawer) : myDrawer(drawer) {}
	void Init();

	void Draw() const;
	bool TileIsValid(int anX, int anY);

	bool HasIntersectedDot(const Vector2f& aPosition);
	bool HasIntersectedBigDot(const Vector2f& aPosition);
	bool HasIntersectedCherry(const Vector2f& aPosition);

	size_t DotsLeft() const { return myDots.size() + myBigDots.size(); }
	
	void GetPath(int aFromX, int aFromY, int aToX, int aToY, std::unordered_set<PathmapTilePtr>& aList);

private:

	PathmapTilePtr GetTile(int aFromX, int aFromY);
	bool Pathfind(PathmapTilePtr aFromTile, PathmapTilePtr aToTile, std::unordered_set<PathmapTilePtr>& aList);
	bool ListDoesNotContain(PathmapTilePtr& aFromTile, std::list<PathmapTilePtr>& aList);


	bool InitPathmap();
	bool InitDots();
	bool InitBigDots();

	Drawer* myDrawer = nullptr;

	std::unordered_set<PathmapTilePtr, PathmapTile::Hash, PathmapTile::Compare> myPathmapTiles;
	std::unordered_set<DotPtr> myDots;
	std::unordered_set<BigDotPtr> myBigDots;
	std::unordered_set<CherryPtr> myCherry; //?? unused

};