#pragma once

#include <array>
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
	
	void GetPath(int aFromX, int aFromY, int aToX, int aToY, std::vector<PathmapTilePtr>* aList);
	PathmapTilePtr GetRandomNearbyTile(int currentTileX, int currentTileY, int prevTileX, int prevTileY);

private:
	PathmapTilePtr GetTile(int aFromX, int aFromY) const;
	bool Pathfind(const PathmapTilePtr& aFromTile, const PathmapTilePtr& aToTile, std::vector<PathmapTilePtr>* aList);

	void InitMap();

	void BuildTileGraph();
	void BuildPaths();

	Drawer* myDrawer = nullptr;

	std::unordered_set<PathmapTilePtr, PathmapTile::Hash, PathmapTile::Compare> myPathmapTiles;
	std::unordered_map<PathmapTilePtr, std::unordered_set<PathmapTilePtr>, PathmapTile::Hash, PathmapTile::Compare> myGraph;
	std::array< std::array< std::vector<PathmapTilePtr>, MAX_TILE_NUM*MAX_TILE_NUM>, MAX_TILE_NUM*MAX_TILE_NUM > myPaths;

	std::unordered_set<DotPtr> myDots;
	std::unordered_set<BigDotPtr> myBigDots;
	std::unordered_set<CherryPtr> myCherry; //?? unused

};