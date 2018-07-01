#include "World.h"
#include <algorithm>
#include <fstream>
#include <string>

#include "PathmapTile.h"
#include "Dot.h"
#include "BigDot.h"
#include "Drawer.h"
#include "Common.h"
#include <set>
#include <algorithm>
#include <queue>

namespace
{
	constexpr const char FIELD[] = "playfield.png";
	constexpr const char MAP_FILENAME[] = "map.txt";
}
void World::Init()
{
	myDrawer->RegisterImage(FIELD);
	InitPathmap();
	InitDots();
	InitBigDots();
}

bool World::InitPathmap()
{
	std::string line;
	std::ifstream myfile (MAP_FILENAME);
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
	std::ifstream myfile (MAP_FILENAME);
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
					const auto&& ptr = std::make_shared<Dot>(Vector2f(static_cast<float>(i*TILE_SIZE), static_cast<float>(lineIndex*TILE_SIZE)),
															 myDrawer);
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
	std::ifstream myfile (MAP_FILENAME);
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
																		 static_cast<float>(lineIndex*TILE_SIZE)),
																myDrawer);
					myBigDots.emplace(ptr);
				}
			}

			lineIndex++;
		}
		myfile.close();
	}

	return true;
}

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

void World::GetPath(int aFromX, int aFromY, int aToX, int aToY, std::vector<PathmapTilePtr>& aList)
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

bool World::Pathfind(PathmapTilePtr aFromTile, PathmapTilePtr aToTile, std::vector<PathmapTilePtr>& aList)
{
	std::queue<PathmapTilePtr> queue;

	std::unordered_map<PathmapTilePtr, std::unordered_set<PathmapTilePtr>, PathmapTile::Hash, PathmapTile::Compare> graph;

	for (const auto& tile : myPathmapTiles)
	{
		if (!tile->myIsBlockingFlag || tile == aFromTile || tile == aToTile)
		{
			graph[tile] = std::unordered_set<PathmapTilePtr>();

			auto&& left = GetTile(tile->myX-1, tile->myY);
			if (left != nullptr && !left->myIsBlockingFlag)
				graph[tile].emplace(std::move(left));

			auto&& up = GetTile(tile->myX, tile->myY-1);
			if (up != nullptr && !up->myIsBlockingFlag)
				graph[tile].emplace(std::move(up));
			
			auto&& right = GetTile(tile->myX+1, tile->myY);
			if (right != nullptr && !right->myIsBlockingFlag)
				graph[tile].emplace(std::move(right));

			auto&& down = GetTile(tile->myX, tile->myY+1);
			if (down != nullptr && !down->myIsBlockingFlag)
				graph[tile].emplace(std::move(down));
		}
	}

	const auto& start = graph.find(aFromTile);
	const auto& end = graph.find(aToTile);

	queue.push(aFromTile);
	std::unordered_map<PathmapTilePtr, PathmapTilePtr> visited;
	visited[aFromTile] = nullptr;

	while (!queue.empty())
	{
		const auto current = queue.front();
		queue.pop();

		if (current==aToTile)
			break;

		const auto& nextTiles = graph[current];
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
	aList.push_back(aToTile);
	while (current != aFromTile)
	{
		current = visited[current];
		aList.push_back(current);
	}
	
	return false;
}