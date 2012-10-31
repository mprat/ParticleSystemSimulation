#include "gravityForce.h"

using namespace std;

GravityForce::GravityForce()
{
	g_force = Vector3f(0, -1, 0);
}

Vector3f GravityForce::getForce(double mass)
{
	return mass*g_force;
}
