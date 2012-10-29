#include "gravityForce.h"

using namespace std;

GravityForce::GravityForce(double mass)
{
	g_mass = mass;	
	g_force = Vector3f(0, -1, 0);
}

Vector3f GravityForce::getForce()
{
	return g_mass*g_force;
}
