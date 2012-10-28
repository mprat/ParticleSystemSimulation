#include "TimeStepper.hpp"

#include<iostream>

void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{
	//get current state (X_0)
	//evalF on current state f(X_0, t)
	//multitply by h
	//set current state to X_0 + hf(X_0, h)

	vector<Vector3f> cur_state = particleSystem->getState();
	vector<Vector3f> f_state = particleSystem->evalF(cur_state);
	vector<Vector3f> new_state;	
	Vector3f cur_particle;
	Vector3f new_particle;
	Vector3f f_particle;
	
	for (int i = 0; i < cur_state.size(); i++){	
		cur_particle = cur_state[i];
		f_particle = f_state[i];
	
		//X_1 = X_0 + h*f(X_0, t)	
		new_particle = cur_particle + stepSize * f_particle;		
//
//		cout<<"cur_particle:";
//		cur_particle.print();
//		cout<<"f_particle:";
//		f_particle.print();
//		cout<<"new_particle:";
//		new_particle.print();
//
		new_state.push_back(new_particle);
	}
	
	particleSystem->setState(new_state);
}

///TODO: implement Trapzoidal rule here
void Trapezoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
}
