
#include "simpleSystem.h"

using namespace std;

SimpleSystem::SimpleSystem()
{
	m_numParticles = 1;
	m_vVecState.clear();
	m_vVecState.push_back(Vector3f(1, 0, 0));		
}

// for a given state, evaluate f(X,t)
vector<Vector3f> SimpleSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;
	Vector3f next_deriv; 

	for (unsigned i = 0; i < state.size(); i++){
		next_deriv = Vector3f(-1.0*state[i].y(), state[i].x(), 0);
		f.push_back(next_deriv);
	}

	return f;
}

// render the system (ie draw the particles)
void SimpleSystem::draw()
{
	for (unsigned i = 0; i < m_vVecState.size(); i++){
		Vector3f pos = m_vVecState[i];
	  glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
	}
}
