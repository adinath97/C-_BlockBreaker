#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#pragma once

class Vector2 {

    public:
	Vector2() : x(0.0f), y(0.0f) { }
	Vector2(float x, float y): x(x), y(y) { }

	float x, y;

	Vector2 operator+(Vector2 const& a) {
		return Vector2(x + a.x, y + a.y);
	}

	Vector2& operator+=(Vector2 const& a) {
		x += a.x;
		y += a.y;

		return *this;
	}

	Vector2 operator*(float a) {
		return Vector2(x * a, y * a);
	}

};