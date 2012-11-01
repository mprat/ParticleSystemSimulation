#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include <vecmath.h>
#include <vector>

#include <GL/glut.h>
#include "spring.h"
#include "pendulumSystem.h"
#include "externalforce.h"

class ClothSystem: public PendulumSystem
{
///ADD MORE FUNCTION AND FIELDS HERE
public:
	ClothSystem(int num_x, int num_y);
	vector<Vector3f> evalF(vector<Vector3f> state);
	void draw();
	void addExternalForce(ExternalForce e);
	void clearExternalForces();
	void reset(int num_x, int num_y);
	void drawSprings();
private:
	int indexOf(int i, int j);
	int num_cols;
	int num_rows;
	vector<ExternalForce> externalForces;
	Vector3f getNormal(int x, int y);
};


#endif
