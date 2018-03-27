#include "Vec2.h"

namespace math
{
	Vec2::Vec2(float x, float y) : x(x), y(y) {}

	Vec2& Vec2::operator+=(const Vec2& right)
	{
		x += right.x;
		y += right.y;
		return *this;
	};

	Vec2& Vec2::operator-=(const Vec2& right)
	{
		x -= right.x;
		y -= right.y;
		return *this;
	};

	Vec2& Vec2::operator*=(const Vec2& right)
	{
		x *= right.x;
		y *= right.y;
		return *this;
	};

	Vec2 operator+(const Vec2& left, const Vec2& right)
	{
		return Vec2(left.x + right.x, left.y + right.y);
	};

	Vec2 operator-(const Vec2& left, const Vec2& right)
	{
		return Vec2(left.x - right.x, left.y - right.y);
	};

	float operator*(const Vec2& left, const Vec2& right)
	{
		return left.x * right.x + left.y * right.y;
	};

	Vec2 operator*(float left, const Vec2& right)
	{
		return Vec2(left * right.x, left * right.y);
	};

	Vec2 operator*(const Vec2& left, float right)
	{
		return Vec2(left.x * right, left.y * right);
	};
}


