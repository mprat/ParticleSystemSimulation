#include <iostream>
#include "pendulumSystem.h"
#include "gravityForce.h"
#include "dragForce.h"

PendulumSystem::PendulumSystem(int numParticles):ParticleSystem(numParticles)
{
	m_numParticles = numParticles;
	double k = 5.0;
	double r = 1.0;

	// fill in code for initializing the state based on the number of particles
	for (int i = 0; i < m_numParticles; i++) {
		
		// for this system, we care about the position and the velocity
	
		//positions are at even indices, velocities are at odd indices
		m_vVecState.push_back(Vector3f(-i, 0, 0));
		m_vVecState.push_back(Vector3f(0, 0, 0));
		mass.push_back(2.0);
		if (i > 0)
		{
			Spring s (k, r, i - 1, i);	
			springs.push_back(s);
		}
	}
}

// for a given state, evaluate f(X,t)
// state is alternating position-velocity
vector<Vector3f> PendulumSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f(state);

	for (int i = 0; i < m_numParticles; i++){	
		//velocity becomes the first element
		f[2*i] = velocityOf(i, state);
		f[2*i + 1] = Vector3f(0, 0, 0);
		//next element is the sum of the forces 
		if (i > 0){
			GravityForce g(mass[i]);
			f[2*i + 1] += g.getForce();
			DragForce d(2.0);
			f[2*i + 1] += d.getForce(velocityOf(i, state));
		}
	}

	//add in spring forces by iterating through list of all springs
	for (unsigned i = 0; i < springs.size(); i++){
		f[2*(springs[i].j) + 1] += springs[i].getForce(positionOf(springs[i].j, state), positionOf(springs[i].i, state));
	}
	//f[3].print();
	
	for (int i = 0; i < m_numParticles; i++){
		f[2*i + 1] = f[2*i + 1] / mass[i];
	}
	
	//statePrint(f);
	return f;
}

void PendulumSystem::statePrint(vector<Vector3f> s){
	cout<<"state print: "<<endl;
	for (unsigned i = 0; i < s.size(); i++){
		s[i].print();
	}
}

// render the system (ie draw the particles)
void PendulumSystem::draw()
{
	for (int i = 0; i < m_numParticles; i++) {
		Vector3f pos = positionOf(i); //current state's position
		//pos.print();
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
