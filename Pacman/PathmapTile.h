#pragma once
#include <memory>
#include <tuple>
#include "Constants.h"

class PathmapTile
{
public:
	PathmapTile(int anX, int anY, bool aIsBlockingFlag) 
		: myX(anX)
		, myY(anY)
		, myIsBlockingFlag(aIsBlockingFlag)
	{}

	int myX;
	int myY;
	bool myIsBlockingFlag;

	struct Hash
	{
		std::size_t operator() (const std::shared_ptr<PathmapTile>& p) const 
		{
			return std::hash<int>()(p->myX*MAX_TILE_NUM+p->myY);
		}
	};
	struct Compare
	{
		size_t operator() (const std::shared_ptr<PathmapTile>& a, const std::shared_ptr<PathmapTile>& b) const 
		{
			return a->myX == b->myX && a->myY == b->myY && a->myIsBlockingFlag == b->myIsBlockingFlag;
		}
	};
};
using PathmapTilePtr = std::shared_ptr<PathmapTile>;

struct PathTilePairComparator
{
	size_t operator() (const std::pair<PathmapTilePtr, PathmapTilePtr>& p1, const std::pair<PathmapTilePtr, PathmapTilePtr>& p2) const
	{
		return p1.first == p2.first && p1.second == p2.second;
	}
};
struct PathTilePairHash
{
	size_t operator() (const std::pair<PathmapTilePtr, PathmapTilePtr>& p) const
	{
		return std::hash<void*>()(p.first.get());
	}
};

static PathmapTilePtr makePathTilePtr(int anX, int anY, bool aIsBlockingFlag)
{
	return std::make_shared<PathmapTile>(anX, anY, aIsBlockingFlag);
}