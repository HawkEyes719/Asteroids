#pragma once

#include <glm/glm.hpp>
#include <iostream>

using namespace glm;
using namespace std;

class Collision
{
public:

	static bool circleCircle(vec2 a_center1, float a_radius1, vec2 a_center2, float a_radius2);	static bool AABB(vec2 a_min1, vec2 a_max1, vec2 a_min2, vec2 a_max2);
	static bool LineLine(vec2 a_P1, vec2 a_P2, vec2 a_P3, vec2 a_P4, vec2& a_rCrossPoint);
};

