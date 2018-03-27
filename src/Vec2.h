#pragma once


namespace math
{
	class Vec2
	{
	public:
		float x, y;
		Vec2(float x, float y);

		Vec2& operator+=(const Vec2& right);
		Vec2& operator-=(const Vec2& right);
		Vec2& operator*=(const Vec2& right);

		friend Vec2 operator+(const Vec2& left, const Vec2& right);
		friend Vec2 operator-(const Vec2& left, const Vec2& right);
		friend float operator*(const Vec2& left, const Vec2& right);
		friend Vec2 operator*(float left, const Vec2& right);
		friend Vec2 operator*(const Vec2& left, float right);
	};
}


