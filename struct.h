#pragma once
#include <cmath>
#include <numbers>



struct vector3
{
	float x, y, z;

	constexpr const vector3& operator+(const vector3& v) const
	{
		return vector3(x + v.x, y + v.y, z + v.z);
	}

	constexpr const vector3& operator-(const vector3& v) const
	{
		return vector3(x - v.x, y - v.y, z - v.z);
	}

};

struct Angles {
	float yaw;
	float pitch;
};

