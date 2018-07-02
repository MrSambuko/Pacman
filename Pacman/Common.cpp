#include <random>

#include "Common.h"


namespace
{
std::random_device d;
std::default_random_engine e(d());
}
int getRandomInt(int min, int max)
{
	const std::uniform_int_distribution<> d(min, max);
	return d(e);
}
