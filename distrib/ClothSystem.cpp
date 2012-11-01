#include "ClothSystem.h"
#include <iostream>

ClothSystem::ClothSystem(int num_x, int num_y):PendulumSystem()
{
	reset(num_x, num_y);
}

void ClothSystem::reset(int num_x, int num_y)
{
	num_cols = num_x;
	num_rows = num_y;
	m_numParticles = num_rows * num_cols;

	double length = 4.0;
	double scalefactor = length / num_x;
	double k = 150.0;
	double r = scalefactor*1.0;
	double m = 1.5;
	
	mass.clear();
	springs.clear();
	fixedpoints.clear();
	m_vVecState.clear();
	
	for (int i = 0; i < num_rows; i++){
		for (int j = 0; j < num_cols; j++){
			//cout<<"("<<j<<", 0, "<<i<<")"<<endl;
			m_vVecState.push_back(scalefactor * Vector3f(j, 0, i)); //position
			m_vVecState.push_back(scalefactor * Vector3f(0, 0, 0)); //velocity
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

	k = .7;
	r *= 1.414;
	//add shear springs
	for (int i = 1; i < num_rows; i++){
		for (int j = 1; j < num_cols; j++){
			Spring s1(k, r, indexOf(i - 1, j - 1), indexOf(i, j));
			Spring s2(k, r, indexOf(i - 1, j), indexOf(i, j - 1));
			springs.push_back(s1);
			springs.push_back(s2);
		}
	}	

	k *= .4;
	r = r / 1.414 * 2.0;
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

}

//draw the system
void ClothSystem::draw(){

	//drawSprings();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glBegin(GL_TRIANGLES);

	for (int j = 0; j < num_rows - 1; j++){
		for (int i = 0; i < num_cols - 1; i++){
			//cout<<i<<"  "<<i + 1<<"  "<<num_cols + i<<endl;
			Vector3f v1 = positionOf(num_cols*j + i); 
			Vector3f v2 = positionOf(num_cols*j + i + 1); 
			Vector3f v3 = positionOf(num_cols*(j + 1) + i); 
			Vector3f normal1 = Vector3f::cross(v1 - v2, v1 - v3);
			glNormal3fv(-1.0*normal1.normalized());
			glVertex3fv(v3);
			glVertex3fv(v2);
			glVertex3fv(v1);

			Vector3f v4 = positionOf(num_cols*(j + 1) + i + 1); 
			Vector3f v5 = positionOf(num_cols*j + i + 1); 
			Vector3f v6 = positionOf(num_cols*(j + 1) + i); 
			Vector3f normal2 = Vector3f::cross(v4 - v5, v4 - v6);
			glNormal3fv(normal2.normalized());
			glVertex3fv(v4);
			glVertex3fv(v5);
			glVertex3fv(v6);
	
			Vector3f v7 = positionOf(num_cols*j + i); 
			Vector3f v8 = positionOf(num_cols*j + i + 1); 
			Vector3f v9 = positionOf(num_cols*(j + 1) + i); 
			glNormal3fv(-1.0*normal1);
			glVertex3fv(v8);
			glVertex3fv(v9);
			glVertex3fv(v7);

			Vector3f v10 = positionOf(num_cols*(j + 1) + i + 1); 
			Vector3f v11 = positionOf(num_cols*j + i + 1); 
			Vector3f v12 = positionOf(num_cols*(j + 1) + i); 
			glNormal3fv(normal2);
			glVertex3fv(v10);
			glVertex3fv(v12);
			glVertex3fv(v11);

		}
	}
	glEnd();
	glPopAttrib();
}

//draw springs
void ClothSystem::drawSprings(){
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	for (unsigned i = 0; i < (num_rows*(num_cols - 1) + num_cols*(num_rows - 1)); i++)
	{	
		glPushMatrix();
		glLineWidth(3);
		glBegin(GL_LINES);
		glVertex3fv(positionOf(springs[i].i));
		glVertex3fv(positionOf(springs[i].j));
		glEnd();
		glPopMatrix();
	}
	glPopAttrib();
}

vector<Vector3f> ClothSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f = PendulumSystem::evalF(state);
	
	//deal with external forces
	for (unsigned i = 0; i < externalForces.size(); i++)
	{
		f[2*externalForces[i].particle_index + 1] += externalForces[i].e_force;
		//velocity in all other components not in the direction of the force are zero
		if (externalForces[i].e_force.x() == 0){
			f[2*externalForces[i].particle_index + 1] = f[2*externalForces[i].particle_index + 1] * Vector3f(0, 1, 1);
		}
		if (externalForces[i].e_force.y() == 0){
			f[2*externalForces[i].particle_index + 1] = f[2*externalForces[i].particle_index + 1] * Vector3f(1, 0, 1);
		}
		if (externalForces[i].e_force.z() == 0){
			f[2*externalForces[i].particle_index + 1] = f[2*externalForces[i].particle_index + 1] * Vector3f(1, 1, 0);
		}
	} 	
	
	//if no external forces, velocity of fp particles are zero
	if (externalForces.empty()){
		for (unsigned i = 0; i < fixedpoints.size(); i++){
			f[2*fixedpoints[i]] = Vector3f(0, 0, 0);
		}
	}	
	
	for (int i = 0; i < m_numParticles; i++){
		f[2*i + 1] = f[2*i + 1] / mass[i];
	}

	return f;
}

//cloth is initialized like
//0 --- 1 --- 2 --- 3
//4 --- 5 --- 6 --- 7...etc.
int ClothSystem::indexOf(int row, int col)
{
	return row*num_cols + col;
}

void ClothSystem::addExternalForce(ExternalForce e)
{
	externalForces.push_back(e);
}

void ClothSystem::clearExternalForces()
{
	externalForces.clear();
}
