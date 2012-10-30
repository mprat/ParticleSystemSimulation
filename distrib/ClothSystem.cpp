#include "ClothSystem.h"
#include <iostream>

ClothSystem::ClothSystem(int num_x, int num_y)
{
	//TODO: must be at least 8x8
	m_numParticles = num_x * num_y;

	double k = 5.0;
	double r = 1.0;
	double m = 2.0;

	for (int i = 0; i < num_x; i++){
		for (int j = 0; j < num_y; j++){
			m_vVecState.push_back(Vector3f(i, -j, 0)); //position
			m_vVecState.push_back(Vector3f(0, 0, 0)); //velocity
			mass.push_back(m);
		}
	}

	//cout<<m_vVecState.size()<<endl;
	//cout<<m_numParticles<<endl;
	//TODO: fill in springs
}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> ClothSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;
	
	for (unsigned i = 0; i < state.size(); i++) {f.push_back(Vector3f(0, 0, 0));}
	return f;
}

///TODO: render the system (ie draw the particles)
void ClothSystem::draw()
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

//TODO: given an index i, j return linear index of springs
int ClothSystem::indexOf(int i, int j)
{
	return 0;
}

void ClothSystem::statePrint(vector<Vector3f> s){
	cout<<"state print: "<<endl;
	for (unsigned i = 0; i < s.size(); i++){
		s[i].print();
	}
}

//read position of particle i in the current state
Vector3f ClothSystem::positionOf(int i)
{
	return m_vVecState[2*i];
}

//read velocity of a particle i in the current state
Vector3f ClothSystem::velocityOf(int i)
{
	return m_vVecState[2*i + 1];
}

//read position of a particle i at a given state
Vector3f ClothSystem::positionOf(int i, vector<Vector3f> state)
{
	return state[2*i];
}

//read velocity of a particle i at a given state
Vector3f ClothSystem::velocityOf(int i, vector<Vector3f> state)
{
	return state[2*i + 1];
}
