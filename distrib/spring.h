#ifndef SPRING_H
#define SPRING_H

#include <vecmath.h>
#include <vector>


class Spring
{
public:
	Spring(float k, float r, Vector3f* x_i, Vector3f* x_j);
	Vector3f getForce();
private:
	float k;		
	float r;
	Vector3f* x_i;
	Vector3f* x_j;
};

#endif
