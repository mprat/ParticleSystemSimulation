#include "spring.h"

using namespace std;

Spring::Spring(float k, float r, int particle_i, int particle_j)
{
	k = k;
	r = r;
	i = i;
	j = j;	
}

Vector3f Spring::getForce(Vector3f pos_i, Vector3f pos_j)
{
	Vector3f d = pos_i - pos_j;
	return -1.0 * k * (d.abs() - r) * d / d.abs();
}
