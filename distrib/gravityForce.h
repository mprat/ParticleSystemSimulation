#ifndef GRAVITYFORCE_H
#define GRAVITYFORCE_H

#include <vecmath.h>
#include <vector>


class GravityForce
{
public:
	GravityForce();
	Vector3f getForce(double mass);
private:
	Vector3f g_force;
};

#endif
