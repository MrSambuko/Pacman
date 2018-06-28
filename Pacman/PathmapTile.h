#pragma once
#include <memory>

class PathmapTile
{
public:
	PathmapTile(int anX, int anY, bool aIsBlockingFlag);

	int myX;
	int myY;
	bool myIsBlockingFlag;
	bool myIsVisitedFlag;
};

using PathmapTilePtr = std::shared_ptr<PathmapTile>;