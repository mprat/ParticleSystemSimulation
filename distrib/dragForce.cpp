#include "dragForce.h"

using namespace std;

DragForce::DragForce(float k)
{
	d_k = k;	
}

Vector3f DragForce::getForce(Vector3f velocity)
{
	return -1.0*d_k*velocity;
}
