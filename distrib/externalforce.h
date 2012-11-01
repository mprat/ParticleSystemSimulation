#ifndef EXTERNALFORCE_H
#define EXTERNALFORCE_H

#include <vecmath.h>
#include <vector>


class ExternalForce
{
public:
	ExternalForce(int p_index, Vector3f force = Vector3f(0, 0, 0));
	Vector3f e_force;		
	int particle_index;
};

#endif
