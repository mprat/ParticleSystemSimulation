#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include <vecmath.h>
#include <vector>

#include <GL/glut.h>
#include "spring.h"
#include "pendulumSystem.h"

class ClothSystem: public PendulumSystem
{
///ADD MORE FUNCTION AND FIELDS HERE
public:
	ClothSystem(int num_x, int num_y);
	vector<Vector3f> evalF(vector<Vector3f> state);
	
private:
	int indexOf(int i, int j);
};


#endif
