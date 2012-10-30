#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include <vecmath.h>
#include <vector>

#include <GL/glut.h>
#include "spring.h"
#include "particleSystem.h"

class ClothSystem: public ParticleSystem
{
///ADD MORE FUNCTION AND FIELDS HERE
public:
	ClothSystem(int num_x, int num_y);
	vector<Vector3f> evalF(vector<Vector3f> state);
	
	void draw();

	vector<double> mass;
	vector<Spring> springs;	

private:
	int indexOf(int i, int j);
	Vector3f positionOf(int i);
	Vector3f velocityOf(int i);
	Vector3f positionOf(int i, vector<Vector3f> state);
	Vector3f velocityOf(int i, vector<Vector3f> state);
	void statePrint(vector<Vector3f> s);
};


#endif
