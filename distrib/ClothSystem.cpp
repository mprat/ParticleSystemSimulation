#include "ClothSystem.h"
#include <iostream>

ClothSystem::ClothSystem():PendulumSystem(){};

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

//get the normal based on the point around you
Vector3f ClothSystem::getNormal(int x, int y){
	Vector3f normal = Vector3f(0, 0, 0);
	Vector3f v1 = positionOf(indexOf(x, y));
	Vector3f v2 = Vector3f(0, 0, 0);
	Vector3f v3 = Vector3f(0, 0, 0);
	Vector3f v4 = Vector3f(0, 0, 0);
	Vector3f v5 = Vector3f(0, 0, 0);
	vector<Vector3f> normals;
	if (x - 1 > 0){
		v2 = v1 - positionOf(indexOf(x - 1, y));
	}
	if (y + 1 < num_rows){
		v3 = v1 - positionOf(indexOf(x, y + 1));
		if (v2 != Vector3f(0, 0, 0)){
			normals.push_back(Vector3f::cross(v2, v3));
		}	
	}
	if (x + 1 < num_cols){
		v4 = v1 - positionOf(indexOf(x + 1, y));
		if (v3 != Vector3f(0, 0, 0)){
			normals.push_back(Vector3f::cross(v3, v4));
		}
	}
	if (y - 1 > 0){
		v5 = v1 - positionOf(indexOf(x, y - 1));
		if (v4 != Vector3f(0, 0, 0)){
			normals.push_back(Vector3f::cross(v4, v5));
		}
		if (v2 != Vector3f(0, 0, 0)){
			normals.push_back(Vector3f::cross(v5, v2));
		}
	}
	for (unsigned i = 0; i < normals.size(); i++){
		normal += normals[i];
	}
	return -1.0*normal / normals.size();
}

//draw the system
void ClothSystem::draw(){

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
			//front triangles
			Vector3f v1 = positionOf(num_cols*j + i); 
			Vector3f v2 = positionOf(num_cols*j + i + 1); 
			Vector3f v3 = positionOf(num_cols*(j + 1) + i); 
			glNormal3fv(getNormal(i, j + 1));
			glVertex3fv(v3);
			glNormal3fv(getNormal(i + 1, j));	
			glVertex3fv(v2);
			glNormal3fv(getNormal(i, j));
			glVertex3fv(v1);

			v1 = positionOf(num_cols*(j + 1) + i + 1); 
			v2 = positionOf(num_cols*j + i + 1); 
			v3 = positionOf(num_cols*(j + 1) + i); 
			glNormal3fv(getNormal(i + 1, j + 1));
			glVertex3fv(v1);
			glNormal3fv(getNormal(i + 1, j));
			glVertex3fv(v2);
			glNormal3fv(getNormal(i, j + 1));
			glVertex3fv(v3);

			//back triangles	
			v1 = positionOf(num_cols*j + i); 
			v2 = positionOf(num_cols*j + i + 1); 
			v3 = positionOf(num_cols*(j + 1) + i); 
			glNormal3fv(getNormal(i + 1, j));
			glVertex3fv(v2);
			glNormal3fv(getNormal(i, j + 1));
			glVertex3fv(v3);
			glNormal3fv(getNormal(i, j));
			glVertex3fv(v1);

			v1 = positionOf(num_cols*(j + 1) + i + 1); 
			v2 = positionOf(num_cols*j + i + 1); 
			v3 = positionOf(num_cols*(j + 1) + i); 
			glNormal3fv(getNormal(i + 1, j + 1));
			glVertex3fv(v1);
			glNormal3fv(getNormal(i + 1, j));
			glVertex3fv(v3);
			glNormal3fv(getNormal(i, j + 1));
			glVertex3fv(v2);
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
