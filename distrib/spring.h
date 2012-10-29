#ifndef SPRING_H
#define SPRING_H

#include <vecmath.h>
#include <vector>


class Spring
{
public:
	Spring(float k, float r, int particle_i, int particle_j);
	Vector3f getForce(Vector3f pos_i, Vector3f pos_j);
	int i;
	int j;
private:
	float k;		
	float r;
};

#endif
