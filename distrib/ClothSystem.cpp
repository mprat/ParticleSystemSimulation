#include "ClothSystem.h"
#include <iostream>

ClothSystem::ClothSystem(int num_x, int num_y):PendulumSystem()
{
	num_cols = num_x;
	num_rows = num_y;
	//TODO: must be at least 8x8
	m_numParticles = num_rows * num_cols;

	double k = 4.0;
	double r = 1.0;
	double m = 3.0;

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

	//add shear springs
	for (int i = 1; i < num_rows; i++){
		for (int j = 1; j < num_cols; j++){
			Spring s1(k, r, indexOf(i - 1, j - 1), indexOf(i, j));
			Spring s2(k, r, indexOf(i - 1, j), indexOf(i, j - 1));
			springs.push_back(s1);
			springs.push_back(s2);
		}
	}	

	//add horizontal flex springs
	for (int i = 0; i < num_rows; i++){
		for (int j = 0; j < num_cols - 2; j++){
			Spring s(k, r, indexOf(i, j), indexOf(i, j + 2));
			springs.push_back(s);
		}
	} 
	//add vertical flex springs
	for (int i = 0; i < num_rows - 2; i++){
		for (int j = 0; j < num_cols; j++){
			Spring s(k, r, indexOf(i, j), indexOf(i + 2, j));
			springs.push_back(s);
		}
	}
	
	//add fixed points
	fixedpoints.push_back(indexOf(0, 0));
	fixedpoints.push_back(indexOf(0, num_cols - 1));

	d.setK(0.8);	
	
	cout<<"numparticles = "<<m_numParticles<<endl;
	cout<<"numsprings = "<<springs.size()<<endl;
}
//
//// for a given state, evaluate f(X,t)
//vector<Vector3f> ClothSystem::evalF(vector<Vector3f> state)
//{
//	vector<Vector3f> f(state);
//
//	//add gravity and drag and velocity
//	for (int i = 0; i < m_numParticles; i++)
//	{
//		f[2*i] = velocityOf(i, state);
//		f[2*i + 1] = Vector3f(0, 0, 0);
//		f[2*i + 1] += g.getForce(mass[i]);
//		f[2*i + 1] += d.getForce(velocityOf(i, state));
//	}
//	for (unsigned i = 0; i < springs.size();  i++)
//	{
//		f[2*(springs[i].i) + 1] += springs[i].getForce(positionOf(springs[i].i, state), positionOf(springs[i].j, state));
//		f[2*(springs[i].j) + 1] += springs[i].getForce(positionOf(springs[i].j, state), positionOf(springs[i].i, state));
//	}
//
//	//normalize by mass
//	for (int i = 0; i < m_numParticles; i++){
//		f[2*i + 1] = f[2*i + 1] / mass[i];
//	}
//	
//	//set fixed points
//	for (unsigned i = 0; i < fixedpoints.size(); i++){
//		f[2*fixedpoints[i] + 1] = Vector3f(0, 0, 0);
//	}
//	
//	return f;
//}

//cloth is initialized like
//0 --- 1 --- 2 --- 3
//4 --- 5 --- 6 --- 7...etc.
int ClothSystem::indexOf(int row, int col)
{
	return row*num_cols + col;
}
