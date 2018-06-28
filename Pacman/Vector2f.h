#pragma once


#include <cmath> 

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
};