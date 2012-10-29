#include "spring.h"

using namespace std;

Spring::Spring(float k, float r, Vector3f* x_i, Vector3f* x_j)
{
	k = k;
	r = r;
	x_i = x_i;
	x_j = x_j;	
}

Vector3f Spring::getForce()
{
	Vector3f d = &x_i - &x_j;
	return -1.0 * k * (d.abs() - r) * d / d.abs();
}
