#pragma once


#include <cmath> 
#include <functional>


class Vector2f
{
public:
	constexpr Vector2f()
		: myX(.0f)
		, myY(.0f) {}

	constexpr Vector2f(float anX, float anY) 
		: myX(anX)
		, myY(anY){}

	Vector2f operator-(const Vector2f& other) const 
	{
		return {myX - other.myX, myY - other.myY};
	}

	Vector2f operator+(const Vector2f& other) const 
	{
		return {myX + other.myX, myY + other.myY};
	}

	
	Vector2f operator*(const Vector2f& other) const 
	{
		return {myX*other.myX, myY*other.myY};
	}

	Vector2f& operator+=(const Vector2f& other) 
	{
		myX = myX + other.myX;
		myY = myY + other.myY;

		return *this;
	}

	Vector2f& operator-=(const Vector2f& other) 
	{
		myX = myX - other.myX;
		myY = myY - other.myY;

		return *this;
	}

	Vector2f& operator*=(const float aFloat) 
	{
		myX *= aFloat;
		myY *= aFloat;

		return *this;
	}

	Vector2f& operator/=(const float aFloat) 
	{
		myX /= aFloat;
		myY /= aFloat;

		return *this;
	}

	Vector2f operator*(const float aValue) const 
	{
		return {myX * aValue, myY * aValue};
	}

	bool operator == (const Vector2f& other) const
	{
		return (abs(myX - other.myX) < 0.000001f) && 
			   (abs(myY - other.myY) < 0.000001f);
	}

	bool operator != (const Vector2f& other) const
	{
		return !(*this == other);
	}

	float Length() const
	{
		return sqrt(myX*myX + myY*myY);
	}

	void Normalize()
	{
		const float length = Length();

		if (length > 0.f)
			*this /= length;
	}

	float myX;
	float myY;

	struct Hash
	{
		std::size_t operator() (const Vector2f& v) const
		{
			return std::hash<float>()(v.myX*1024.f + v.myY);
		}
	};
	struct Compare
	{
		size_t operator() (const Vector2f& a, const Vector2f& b) const
		{
			return (abs(a.myX - b.myX) < 0.000001f) && (abs(a.myY - b.myY) < 0.000001f);
		}
	};
};

