#include "Bullet.h"

#define PI 3.14159265359

Bullet::Bullet()
{
	extent = vec2(0.02f);
	alive = true;
	velocity = 7.5f;;
	timer = 0.0f;
	name = "Bullet";
}


Bullet::~Bullet()
{
}

void Bullet::draw()
{
	if (alive)
	{
		Gizmos::add2DCircle(vec2(transform[3].x, transform[3].y), extent.x, 10, vec4(1,0,0,1), &transform);
	}
}

void Bullet::update(float deltaTime)
{
	if (alive)
	{
		timer += 1.5f * deltaTime;
		transform = translate(transform, direction * (deltaTime * velocity));
		appearOtherSide(this);

		if (timer > 1.0f)
		{
			alive = false;
		}
	}
}
