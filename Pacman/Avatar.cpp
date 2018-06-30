#include <unordered_map>

#include "Avatar.h"
#include "Drawer.h"
#include "Common.h"

namespace
{
constexpr const char DEAFULT[] = "open_32.png";

constexpr const char OPEN_LEFT[] = "open_left_32.png";
constexpr const char OPEN_UP[]   = "open_up_32.png";
constexpr const char OPEN_RIGHT[]= "open_right_32.png";
constexpr const char OPEN_DOWN[] = "open_down_32.png";

constexpr const char CLOSED_LEFT[] = "closed_left_32.png";
constexpr const char CLOSED_UP[]   = "closed_up_32.png";
constexpr const char CLOSED_RIGHT[]= "closed_right_32.png";
constexpr const char CLOSED_DOWN[] = "closed_down_32.png";

std::unordered_map<Vector2f, std::pair<const char*, const char*>, Vector2f::Hash, Vector2f::Compare> DIRECITON_TO_PIC = 
{
	{NO_DIRECTION,   {DEAFULT,		DEAFULT}},
	{LEFT_DIRECTION, {CLOSED_LEFT,	OPEN_LEFT}},
	{UP_DIRECTION,	 {CLOSED_UP,	OPEN_UP}},
	{RIGHT_DIRECTION,{CLOSED_RIGHT, OPEN_RIGHT}},
	{DOWN_DIRECTION, {CLOSED_DOWN,	OPEN_DOWN}},
};
}

Avatar::Avatar(const Vector2f& aPosition, Drawer* aDrawer)
: MovableGameEntity(aPosition, aDrawer, DEAFULT)
, myIsOpened(true)
, myDirection(NO_DIRECTION)
{
	aDrawer->registerImage(OPEN_LEFT);
	aDrawer->registerImage(OPEN_UP);
	aDrawer->registerImage(OPEN_RIGHT);
	aDrawer->registerImage(OPEN_DOWN);

	aDrawer->registerImage(CLOSED_LEFT);
	aDrawer->registerImage(CLOSED_UP);
	aDrawer->registerImage(CLOSED_RIGHT);
	aDrawer->registerImage(CLOSED_DOWN);
}

void Avatar::Update(float aTime)
{
	const Vector2f destination(static_cast<float>(myNextTileX * TILE_SIZE), static_cast<float>(myNextTileY * TILE_SIZE));
	Vector2f& direction = destination - myPosition;
	Vector2f newDirection = direction;
	newDirection.Normalize();
	if (newDirection != NO_DIRECTION)
		myDirection = std::move(newDirection);

	const float& distanceToMove = aTime * PLAYER_SPEED;

	const auto& distance = direction.Length();
	if (distance < EPSILON)
		return;

	if (distanceToMove > distance)
	{
		myPosition = destination;
		myCurrentTileX = myNextTileX;
		myCurrentTileY = myNextTileY;
		myIsOpened = !myIsOpened;
	}
	else
	{
		myPosition += myDirection * distanceToMove;
	}

}

void Avatar::Draw() const
{
	const auto& strings = DIRECITON_TO_PIC[myDirection];
	const auto& image = myIsOpened ? strings.first : strings.second;
	myDrawer->Draw(image, static_cast<int>(myPosition.myX), static_cast<int>(myPosition.myY));
}

