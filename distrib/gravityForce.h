#ifndef GRAVITYFORCE_H
#define GRAVITYFORCE_H

#include <vecmath.h>
#include <vector>


class GravityForce
{
public:
	GravityForce(double mass);
	Vector3f getForce();
private:
	double g_mass;		
	Vector3f g_force;
};

#endif
