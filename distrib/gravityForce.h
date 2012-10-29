#ifndef GRAVITYFORCE_H
#define GRAVITYFORCE_H

#include <vecmath.h>
#include <vector>


class GravityForce
{
public:
	GravityForce(float mass);
	Vector3f getForce();
private:
	float g_mass;		
	Vector3f g_force;
};

#endif
