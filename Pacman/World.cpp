#include "World.h"
#include <fstream>
#include <string>

#include "PathmapTile.h"
#include "Dot.h"
#include "BigDot.h"
#include "Drawer.h"
#include "Common.h"
#include <set>


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
				myPathmapTiles.emplace_back(tile);
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
					myDots.emplace_back(ptr);
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
					myBigDots.emplace_back(ptr);
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
	for (const auto& tile : myPathmapTiles )
	{
		if (anX == tile->myX && anY == tile->myY && !tile->myIsBlockingFlag)
			return true;
	}

	return false;
}

bool World::HasIntersectedDot(const Vector2f& aPosition)
{
	for (const auto& dot : myDots)
	{
		if ((dot->GetPosition() - aPosition).Length() < 5.f)
		{
			myDots.remove(dot);
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
			myBigDots.remove(dot);
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
	PathmapTilePtr fromTile = GetTile(aFromX, aFromY);
	PathmapTilePtr toTile = GetTile(aToX, aToY);
	
	for (const auto& tile : myPathmapTiles)
	{
		tile->myIsVisitedFlag = false;
	}

	Pathfind(fromTile, toTile, aList);
}

PathmapTilePtr World::GetTile(int aFromX, int aFromY)
{
	for (const auto& tile : myPathmapTiles )
	{
		if (tile->myX == aFromX && tile->myY == aFromY)
		{
			return tile;
		}
	}

	return nullptr;
}

bool World::ListDoesNotContain(PathmapTilePtr aFromTile, std::list<PathmapTilePtr>& aList)
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

	const auto&& up = GetTile(aFromTile->myX, aFromTile->myY - 1);
	if (up && !up->myIsVisitedFlag && !up->myIsBlockingFlag && ListDoesNotContain(up, aList))
		neighborList.emplace(up);

	const auto&& down = GetTile(aFromTile->myX, aFromTile->myY + 1);
	if (down && !down->myIsVisitedFlag && !down->myIsBlockingFlag && ListDoesNotContain(down, aList))
		neighborList.emplace(down);

	const auto&& right = GetTile(aFromTile->myX + 1, aFromTile->myY);
	if (right && !right->myIsVisitedFlag && !right->myIsBlockingFlag && ListDoesNotContain(right, aList))
		neighborList.emplace(right);

	const auto&& left = GetTile(aFromTile->myX - 1, aFromTile->myY);
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