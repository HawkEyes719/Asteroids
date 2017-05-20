#include "Collision.h"
#include "Gizmos.h"

bool Collision::circleCircle(vec2 a_center1, float a_radius1, vec2 a_center2, float a_radius2)
{
	return false;
}

bool Collision::AABB(vec2 a_min1, vec2 a_max1, vec2 a_min2, vec2 a_max2)
{
	if (a_max1.x < a_min2.x || a_max2.x < a_min1.x ||
		a_max1.y < a_min2.y || a_max2.y < a_min1.y)
	{
		//cout << "noOverlap" << endl;
		return false;
	}

	if (a_max1.x >= a_min2.x && a_max2.x >= a_min1.x &&
		a_max1.y >= a_min2.y && a_max2.y >= a_min1.y)
	{
		//cout << "collided" << endl;
		return true;
	}

	return false;
}

bool Collision::LineLine(vec2 a_P1, vec2 a_P2, vec2 a_P3, vec2 a_P4, vec2& a_rCrossPoint)
{
	float Ta = (a_P4.x - a_P3.x) * (a_P1.y - a_P3.y) - (a_P4.y - a_P3.y) * (a_P1.x - a_P3.x) / (a_P4.y - a_P3.y) * (a_P2.x - a_P1.x) - (a_P4.x - a_P3.x) * (a_P2.y - a_P1.y);
	float Tb = (a_P2.x - a_P1.x) * (a_P1.y - a_P3.y) - (a_P2.y - a_P1.y) * (a_P1.x - a_P3.x) / (a_P4.y - a_P3.y) * (a_P2.x - a_P1.x) - (a_P4.x - a_P3.x) * (a_P2.y - a_P1.y);

	vec2 La = a_P1 + Ta * (a_P2 - a_P1);
	vec2 Lb = a_P3 + Tb * (a_P4 - a_P3);

	//if (La == Lb)
	{
		if (a_P1.x + Ta * (a_P2.x - a_P1.x) == a_P3.x + Tb * (a_P4.x - a_P3.x))
		{
			//cout << "collided" << endl;
			return true;
		}

		if (a_P1.y + Ta * (a_P2.y - a_P1.y) == a_P3.y + Tb * (a_P4.y - a_P3.y))
		{
			//cout << "collided" << endl;
			return true;
		}

		//cout << "collided" << endl;
		//return true;
	}
	
	return false;
}
