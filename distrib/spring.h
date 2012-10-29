#ifndef SPRING_H
#define SPRING_H

#include <vecmath.h>
#include <vector>


class Spring
{
public:
	Spring(double k_s, double r_s, int particle_i, int particle_j);
	Vector3f getForce(Vector3f pos_i, Vector3f pos_j);
	int i;
	int j;
private:
	double k;		
	double r;
};

#endif
