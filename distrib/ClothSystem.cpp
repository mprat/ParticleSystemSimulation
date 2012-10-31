#include "ClothSystem.h"
#include <iostream>

ClothSystem::ClothSystem(int num_x, int num_y):PendulumSystem()
{
	num_cols = num_x;
	num_rows = num_y;
	//TODO: must be at least 8x8
	m_numParticles = num_rows * num_cols;

	double k = 5.0;
	double r = 1.0;
	double m = 2.0;

	for (int i = 0; i < num_rows; i++){
		for (int j = 0; j < num_cols; j++){
			//cout<<"("<<j<<", 0, "<<i<<")"<<endl;
			m_vVecState.push_back(Vector3f(j, 0, i)); //position
			m_vVecState.push_back(Vector3f(0, 0, 0)); //velocity
			mass.push_back(m);
		}
	}
	
	//add horizontal springs
	for (int i = 0; i < num_rows; i++){
		for (int j = 1; j < num_cols; j++){
			Spring s(k, r, indexOf(i, j), indexOf(i, j - 1));
			springs.push_back(s);
			//Spring s2(k, r, indexOf(j, i), indexOf(j - 1, i));
			//springs.push_back(s2);
		}
	}
	
	//add vertical springs
	for (int i = 1; i < num_rows; i++){
		for (int j = 0; j < num_cols; j++){
			Spring s(k, r, indexOf(i, j), indexOf(i - 1, j));
			springs.push_back(s);
		}
	}
	
	cout<<"numparticles = "<<m_numParticles<<endl;
	cout<<"numsprings = "<<springs.size()<<endl;
}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> ClothSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;
	
	for (unsigned i = 0; i < state.size(); i++) {f.push_back(Vector3f(0, 0, 0));}
	return f;
}

//cloth is initialized like
//0 --- 1 --- 2 --- 3
//4 --- 5 --- 6 --- 7...etc.
int ClothSystem::indexOf(int row, int col)
{
	return row*num_cols + col;
}
