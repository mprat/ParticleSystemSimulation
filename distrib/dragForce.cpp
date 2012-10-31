#include "dragForce.h"

using namespace std;

DragForce::DragForce(double k)
{
	d_k = k;	
}

void DragForce::setK(double k)
{
	d_k = k;
}

Vector3f DragForce::getForce(Vector3f velocity)
{
	return -d_k*velocity;
}
