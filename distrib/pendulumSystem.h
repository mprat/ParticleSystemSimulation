#ifndef PENDULUMSYSTEM_H
#define PENDULUMSYSTEM_H

#include <vecmath.h>
#include <vector>
#include <GL/glut.h>

#include "particleSystem.h"
#include "spring.h"

class PendulumSystem: public ParticleSystem
{
public:
	PendulumSystem(int numParticles);
	
	vector<Vector3f> evalF(vector<Vector3f> state);
	
	void draw();

	vector<double> mass;
	
	vector<Spring> springs;	
private:
	Vector3f positionOf(int i);
	Vector3f velocityOf(int i);
	Vector3f positionOf(int i, vector<Vector3f> state);
	Vector3f velocityOf(int i, vector<Vector3f> state);
	void statePrint(vector<Vector3f> s);
	
	double k;
	double rest_length;	
};

#endif
