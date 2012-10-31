#ifndef PENDULUMSYSTEM_H
#define PENDULUMSYSTEM_H

#include <vecmath.h>
#include <vector>
#include <GL/glut.h>

#include "particleSystem.h"
#include "spring.h"
#include "gravityForce.h"
#include "dragForce.h"

class PendulumSystem: public ParticleSystem
{
public:
	PendulumSystem();
	PendulumSystem(int numParticles);
		
	vector<Vector3f> evalF(vector<Vector3f> state);
	
	void draw();
	void drawSprings();

	vector<double> mass;
	vector<Spring> springs;
	vector<int> fixedpoints;	
private:
	void statePrint(vector<Vector3f> s);
	void setFixedPointForces(vector<Vector3f> forces);
	
	double k;
	double rest_length;	
protected:
	GravityForce g;
	DragForce d;
	Vector3f positionOf(int i);
	Vector3f velocityOf(int i);
	Vector3f positionOf(int i, vector<Vector3f> state);
	Vector3f velocityOf(int i, vector<Vector3f> state);

};

#endif
