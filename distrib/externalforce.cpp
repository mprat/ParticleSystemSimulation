#include "externalforce.h"

using namespace std;

ExternalForce::ExternalForce(int p_index, Vector3f force)
{
	e_force = force;	
	particle_index = p_index;
}
