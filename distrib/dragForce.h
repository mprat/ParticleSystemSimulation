#ifndef DRAGFORCE_H
#define DRAGFORCE_H

#include <vecmath.h>
#include <vector>


class DragForce
{
public:
	DragForce(float k);
	Vector3f getForce(Vector3f velocity);
private:
	float d_k;		
};

#endif
