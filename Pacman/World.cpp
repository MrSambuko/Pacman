#include "World.h"
#include <fstream>
#include <string>

#include "PathmapTile.h"
#include "Dot.h"
#include "BigDot.h"
#include "Drawer.h"
#include "Common.h"
#include <set>
#include <algorithm>


void World::Init()
{
	InitPathmap();
	InitDots();
	InitBigDots();
}

bool World::InitPathmap()
{
	std::string line;
	std::ifstream myfile ("map.txt");
	if (myfile.is_open())
	{
		int lineIndex = 0;
		while (! myfile.eof() )
		{
			std::getline (myfile,line);
			for (unsigned int i = 0; i < line.length(); i++)
			{
				const auto&& tile = std::make_shared<PathmapTile>(i, lineIndex, (line[i] == 'x'));
				myPathmapTiles.emplace(tile);
			}

			lineIndex++;
		}
		myfile.close();
	}

	return true;
}

bool World::InitDots()
{
	std::string line;
	std::ifstream myfile ("map.txt");
	if (myfile.is_open())
	{
		int lineIndex = 0;
		while (! myfile.eof() )
		{
			std::getline (myfile,line);
			for (unsigned int i = 0; i < line.length(); i++)
			{
				if (line[i] == '.')
				{
					const auto&& ptr = std::make_shared<Dot>(Vector2f(static_cast<float>(i*TILE_SIZE), static_cast<float>(lineIndex*TILE_SIZE)));
					myDots.emplace(ptr);
				}
			}

			lineIndex++;
		}
		myfile.close();
	}

	return true;
}

bool World::InitBigDots()
{
	std::string line;
	std::ifstream myfile ("map.txt");
	if (myfile.is_open())
	{
		int lineIndex = 0;
		while (! myfile.eof() )
		{
			std::getline (myfile,line);
			for (unsigned int i = 0; i < line.length(); i++)
			{
				if (line[i] == 'o')
				{
					const auto&& ptr = std::make_shared<BigDot>(Vector2f(static_cast<float>(i*TILE_SIZE), 
																		 static_cast<float>(lineIndex*TILE_SIZE)));
					myBigDots.emplace(ptr);
				}
			}

			lineIndex++;
		}
		myfile.close();
	}

	return true;
}

void World::Draw(Drawer* aDrawer)
{
	aDrawer->Draw("playfield.png");

	for (const auto& dot : myDots)
	{
		dot->Draw(aDrawer);
	}

	for (const auto& dot : myBigDots)
	{
		dot->Draw(aDrawer);
	}
}

bool World::TileIsValid(int anX, int anY)
{
	const auto it = myPathmapTiles.find(std::make_shared<PathmapTile>(anX, anY, false));

	return it != myPathmapTiles.end();
}

bool World::HasIntersectedDot(const Vector2f& aPosition)
{
	for (const auto& dot : myDots)
	{
		if ((dot->GetPosition() - aPosition).Length() < 5.f)
		{
			myDots.erase(dot);
			return true;
		}
	}

	return false;
}

bool World::HasIntersectedBigDot(const Vector2f& aPosition)
{
	for (const auto& dot : myBigDots)
	{
		if ((dot->GetPosition() - aPosition).Length() < 5.f)
		{
			myBigDots.erase(dot);
			return true;
		}
	}

	return false;
}

bool World::HasIntersectedCherry(const Vector2f& aPosition)
{
	return true;
}

void World::GetPath(int aFromX, int aFromY, int aToX, int aToY, std::list<PathmapTilePtr>& aList)
{
	const auto fromTile = GetTile(aFromX, aFromY);
	const auto toTile = GetTile(aToX, aToY);
	
	for (const auto& tile : myPathmapTiles)
	{
		tile->myIsVisitedFlag = false;
	}

	Pathfind(fromTile, toTile, aList);
}

PathmapTilePtr World::GetTile(int aFromX, int aFromY)
{
	// as we don't care for flag search for flase & true same way
	const auto tile = makePathTilePtr(aFromX, aFromY, true);
	const auto end = myPathmapTiles.end();
	auto it = myPathmapTiles.find(tile);

	if (it == end)
	{
		tile->myIsBlockingFlag = false;
		it = myPathmapTiles.find(tile);
		if (it == end)
			return nullptr;
	}
	return *it;
}

bool World::ListDoesNotContain(PathmapTilePtr& aFromTile, std::list<PathmapTilePtr>& aList)
{
	for (const auto& tile : aList)
	{
		if (tile == aFromTile)
		{
			return false;
		}
	}

	return true;
}

bool SortFromGhostSpawn(const PathmapTilePtr& a, const PathmapTilePtr& b)
{
	const int& la = abs(a->myX - 13) + abs(a->myY - 13);
	const int& lb = abs(b->myX - 13) + abs(b->myY - 13);

    return la < lb;
}

bool World::Pathfind(PathmapTilePtr aFromTile, PathmapTilePtr aToTile, std::list<PathmapTilePtr>& aList)
{
	aFromTile->myIsVisitedFlag = true;

	if (aFromTile->myIsBlockingFlag)
		return false;

	if (aFromTile == aToTile)
		return true;

	std::set<PathmapTilePtr, bool(*)(const PathmapTilePtr&, const PathmapTilePtr&)> neighborList(&SortFromGhostSpawn);

	auto&& up = GetTile(aFromTile->myX, aFromTile->myY - 1);
	if (up && !up->myIsVisitedFlag && !up->myIsBlockingFlag && ListDoesNotContain(up, aList))
		neighborList.emplace(up);

	auto&& down = GetTile(aFromTile->myX, aFromTile->myY + 1);
	if (down && !down->myIsVisitedFlag && !down->myIsBlockingFlag && ListDoesNotContain(down, aList))
		neighborList.emplace(down);

	auto&& right = GetTile(aFromTile->myX + 1, aFromTile->myY);
	if (right && !right->myIsVisitedFlag && !right->myIsBlockingFlag && ListDoesNotContain(right, aList))
		neighborList.emplace(right);

	auto&& left = GetTile(aFromTile->myX - 1, aFromTile->myY);
	if (left && !left->myIsVisitedFlag && !left->myIsBlockingFlag && ListDoesNotContain(left, aList))
		neighborList.emplace(left);

	for (auto&& tile : neighborList)
	{
		aList.emplace_back(tile);

		if (Pathfind(tile, aToTile, aList))
			return true;

		aList.pop_back();
	}

	return false;
}