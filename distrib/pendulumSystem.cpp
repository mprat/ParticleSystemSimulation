#include <iostream>
#include "pendulumSystem.h"
#include "gravityForce.h"
#include "dragForce.h"

PendulumSystem::PendulumSystem():ParticleSystem(1){};

PendulumSystem::PendulumSystem(int numParticles):ParticleSystem(numParticles)
{
	reset(numParticles);
	//cout<<"fplen = "<<fixedpoints.size()<<endl;
}

void PendulumSystem::reset(int numParticles){
	m_numParticles = numParticles;
	double k = 5.0;
	double r = 0.5;

	m_vVecState.clear();
	mass.clear();
	springs.clear();

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
	fixedpoints.clear();
	fixedpoints.push_back(0);
	
	d.setK(0.8);

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
		f[2*i + 1] += g.getForce(mass[i]);
		f[2*i + 1] += d.getForce(velocityOf(i, state));
	}

	//add in spring forces by iterating through list of all springs
	for (unsigned i = 0; i < springs.size(); i++){
		f[2*(springs[i].i) + 1] += springs[i].getForce(positionOf(springs[i].i, state), positionOf(springs[i].j, state));
		f[2*(springs[i].j) + 1] += springs[i].getForce(positionOf(springs[i].j, state), positionOf(springs[i].i, state));
	}
	//f[3].print();
	
	for (int i = 0; i < m_numParticles; i++){
		f[2*i + 1] = f[2*i + 1] / mass[i];
	}

	//set fixedpoints by setting forces = 0
	//still add drag to the fixed points
	for (unsigned i = 0; i < fixedpoints.size(); i++){
		f[2*fixedpoints[i] + 1] = Vector3f(0, 0, 0);
		f[2*fixedpoints[i] + 1] += d.getForce(velocityOf(i, state));
	}
	return f;
}

//make all the forces for all fixed points zero
void PendulumSystem::setFixedPointForces(vector<Vector3f> forces){
	for (unsigned i = 0; i < fixedpoints.size(); i++){
		forces[2*fixedpoints[i] + 1] = Vector3f(0, 0, 0);
	}
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

	drawSprings();
}

//draw springs
void PendulumSystem::drawSprings()
{
	for (unsigned i = 0; i < springs.size(); i++)
	{	
		glPushMatrix();
		glBegin(GL_LINES);
		glVertex3fv(positionOf(springs[i].i));
		glVertex3fv(positionOf(springs[i].j));
		glEnd();
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
