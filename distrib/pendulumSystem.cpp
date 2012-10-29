
#include "pendulumSystem.h"

PendulumSystem::PendulumSystem(int numParticles):ParticleSystem(numParticles)
{
	m_numParticles = numParticles;
	
	// fill in code for initializing the state based on the number of particles
	for (int i = 0; i < m_numParticles; i++) {
		
		// for this system, we care about the position and the velocity
	
		//positions are at even indices, velocities are at odd indices
		m_vVecState.push_back(Vector3f(0, i, 0));
		m_vVecState.push_back(Vector3f(0, 0, 0));
		mass.push_back(1.0);
	}
}

// for a given state, evaluate f(X,t)
vector<Vector3f> PendulumSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;

	for (unsigned i = 0; i < m_numParticles; i++){	
		//velocity becomes the first element
		f.push_back(velocityOf(i, state));
		//next element is the sum of the forces 
		
	}

	return f;
}

// render the system (ie draw the particles)
void PendulumSystem::draw()
{
	for (int i = 0; i < m_numParticles; i++) {
		Vector3f pos = positionOf(i); //current state's position
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
	}
}

//read position of particle i in the current state
Vector3f PendulumSystem::positionOf(int i)
{
	return m_vVecState[2*i];
}

//read velocity of a particle i in the current state
Vector3f PendulumSystem::velocityOf(int i)
{
	return m_vVecState[2*i + 1];
}

//read position of a particle i at a given state
Vector3f PendulumSystem::positionOf(int i, vector<Vector3f> state)
{
	return state[2*i];
}

//read velocity of a particle i at a given state
Vector3f PendulumSystem::velocityOf(int i, vector<Vector3f> state)
{
	return state[2*i + 1];
}
