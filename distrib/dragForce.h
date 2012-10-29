#ifndef DRAGFORCE_H
#define DRAGFORCE_H

#include <vecmath.h>
#include <vector>


class DragForce
{
public:
	DragForce(double k);
	Vector3f getForce(Vector3f velocity);
private:
	double d_k;		
};

#endif
