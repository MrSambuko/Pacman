#include <cassert>
#include <algorithm>
#include <queue>
#include <fstream>
#include <string>

#include "PathmapTile.h"
#include "Dot.h"
#include "BigDot.h"
#include "Drawer.h"
#include "Constants.h"
#include "Common.h"

#include "World.h"

namespace
{
constexpr const char FIELD[] = "playfield.png";
constexpr const char MAP_FILENAME[] = "map.txt";


inline size_t MakeIndex(int tileX, int tileY)
{
	return tileX*MAX_TILE_NUM + tileY;
}
}



void World::Init()
{
	myDrawer->RegisterImage(FIELD);
	InitMap();

	BuildTileGraph();
#ifndef _DEBUG
	BuildPaths();
#endif
}

void World::InitMap()
{
	std::string line;
	std::ifstream myfile(MAP_FILENAME);
	if (myfile.is_open())
	{
		int lineIndex = 0;
		size_t maxChars = 0;
		while (!myfile.eof())
		{
			std::getline(myfile, line);
			if (maxChars < line.length())
				maxChars = line.length();

			for (size_t i = 0; i < line.length(); ++i)
			{
				const auto&& tile = std::make_shared<PathmapTile>(i, lineIndex, (line[i] == 'x'));
				myPathmapTiles.emplace(tile);

				if (line[i] == '.')
				{
					const auto&& ptr = std::make_shared<Dot>(Vector2f(static_cast<float>(i*TILE_SIZE),
																	  static_cast<float>(lineIndex*TILE_SIZE)),
															 myDrawer);
					myDots.emplace(ptr);
				}
				else if (line[i] == 'o')
				{
					const auto&& ptr = std::make_shared<BigDot>(Vector2f(static_cast<float>(i*TILE_SIZE),
																		 static_cast<float>(lineIndex*TILE_SIZE)),
																myDrawer);
					myBigDots.emplace(ptr);
				}
			}
			++lineIndex;
		}
	}
}


void World::BuildTileGraph()
{
	for (const auto& tile : myPathmapTiles)
	{
		if (!tile->myIsBlockingFlag)
		{
			myGraph[tile] = std::unordered_set<PathmapTilePtr>();

			auto&& left = GetTile(tile->myX - 1, tile->myY);
			if (left != nullptr && !left->myIsBlockingFlag)
				myGraph[tile].emplace(std::move(left));

			auto&& up = GetTile(tile->myX, tile->myY - 1);
			if (up != nullptr && !up->myIsBlockingFlag)
				myGraph[tile].emplace(std::move(up));

			auto&& right = GetTile(tile->myX + 1, tile->myY);
			if (right != nullptr && !right->myIsBlockingFlag)
				myGraph[tile].emplace(std::move(right));

			auto&& down = GetTile(tile->myX, tile->myY + 1);
			if (down != nullptr && !down->myIsBlockingFlag)
				myGraph[tile].emplace(std::move(down));
		}
	}
}
#ifndef _DEBUG 
void World::BuildPaths()
{
	// build paths from each tile to each tile in graph
	for (auto tile1 = myGraph.begin(); tile1 != std::prev(myGraph.end()); ++tile1)
	{
		const auto& x = MakeIndex(tile1->first->myX, tile1->first->myY);
		for (auto tile2 = std::next(tile1); tile2 != myGraph.end(); ++tile2)
		{
			const auto& y = MakeIndex(tile2->first->myX, tile2->first->myY);
			
			if (!myPaths[x][y].empty())
				continue;
			Pathfind(tile1->first, tile2->first, &myPaths[x][y]);
			myPaths[y][x] = myPaths[x][y];
			std::reverse(myPaths[y][x].begin(), myPaths[y][x].end());	
		}
	}
}
#endif

void World::Draw() const
{
	myDrawer->Draw(FIELD, -X_OFFSET, -Y_OFFSET);
	for (const auto& dot : myDots)
		dot->Draw();

	for (const auto& dot : myBigDots)
		dot->Draw();
}

bool World::TileIsValid(int anX, int anY)
{
	const auto& it = myPathmapTiles.find(std::make_shared<PathmapTile>(anX, anY, false));

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


void World::GetPath(int aFromX, int aFromY, int aToX, int aToY, std::vector<PathmapTilePtr>* aList)
{
	auto&& fromTile = GetTile(aFromX, aFromY);
	auto&& toTile = GetTile(aToX, aToY);
#ifndef _DEBUG
	const auto& x = MakeIndex(fromTile->myX, fromTile->myY);
	const auto& y = MakeIndex(toTile->myX, toTile->myY);
	
	*aList = myPaths[x][y];
#else
	Pathfind(fromTile, toTile, aList);
#endif
}

PathmapTilePtr World::GetRandomNearbyTile(int currentTileX, int currentTileY, int prevTileX, int prevTileY)
{
	const auto& tile = GetTile(currentTileX, currentTileY);
	const auto& prevTile = GetTile(prevTileX, prevTileY);
	auto nearbyTiles = myGraph[tile];
	nearbyTiles.erase(prevTile);
	const auto& size = nearbyTiles.size();
	if (size == 0)
		return prevTile;
	if (size == 1)
		return *nearbyTiles.begin();

	const size_t targetIndex = getRandomInt(0, size-1);
	auto it = nearbyTiles.begin();
	for (size_t index = 0; index < targetIndex; ++index)
		++it;
	return *it;
}

PathmapTilePtr World::GetTile(int aFromX, int aFromY) const
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

bool World::Pathfind(const PathmapTilePtr& aFromTile, const PathmapTilePtr& aToTile, std::vector<PathmapTilePtr>* aList)
{
	// check if path is already cached
	const int& x = MakeIndex(aFromTile->myX, aFromTile->myY);
	const int& y = MakeIndex(aToTile->myX, aToTile->myY);

	if (!myPaths[x][y].empty())
	{
		*aList = myPaths[x][y];
		return true;
	}
	
	std::queue<PathmapTilePtr> queue;
	const auto& start = myGraph.find(aFromTile);
	const auto& end = myGraph.find(aToTile);
	assert(start != myGraph.end() && ("Can't find tile " + std::to_string(aFromTile->myX) + "." + std::to_string(aFromTile->myY)).c_str());
	assert(end != myGraph.end() && ("Can't find tile " + std::to_string(aToTile->myX) + "." + std::to_string(aToTile->myY)).c_str());

	queue.push(aFromTile);
	std::unordered_map<PathmapTilePtr, PathmapTilePtr> visited;
	visited[aFromTile] = nullptr;

	while (!queue.empty())
	{
		const auto current = queue.front();
		queue.pop();

		if (current==aToTile)
			break;

		const auto& nextTiles = myGraph[current];
		for (const auto& next : nextTiles)
		{
			if (visited.find(next) == visited.end())
			{
				queue.push(next);
				visited[next] = current;
			}
		}
	}
	
	auto current = aToTile;
	while (current != aFromTile)
	{
		aList->push_back(current);
		current = visited[current];
	}
	aList->push_back(aFromTile);

	myPaths[x][y] = myPaths[y][x] = *aList;
	return false;
}