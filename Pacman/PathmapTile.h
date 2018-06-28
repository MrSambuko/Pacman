#pragma once
#include <memory>
#include <tuple>
#include "Common.h"

class PathmapTile
{
public:
	PathmapTile(int anX, int anY, bool aIsBlockingFlag) 
		: myX(anX)
		, myY(anY)
		, myIsBlockingFlag(aIsBlockingFlag)
		, myIsVisitedFlag(false)
	{}

	int myX;
	int myY;
	bool myIsBlockingFlag;
	bool myIsVisitedFlag;

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

static PathmapTilePtr makePathTilePtr(int anX, int anY, bool aIsBlockingFlag)
{
	return std::make_shared<PathmapTile>(anX, anY, aIsBlockingFlag);
}