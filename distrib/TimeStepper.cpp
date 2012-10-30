#include "TimeStepper.hpp"

#include<iostream>

void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{
	vector<Vector3f> cur_state = particleSystem->getState();
	vector<Vector3f> f_state = particleSystem->evalF(cur_state);
	vector<Vector3f> new_state;	
	
	for (unsigned i = 0; i < cur_state.size(); i++){	
		//X_1 = X_0 + h*f(X_0, t)	
		new_state.push_back(cur_state[i] + stepSize * f_state[i]);		
	}

	particleSystem->setState(new_state);
}

void Trapezoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
	vector<Vector3f> X = particleSystem->getState();
	vector<Vector3f> f_0 = particleSystem->evalF(X);
	vector<Vector3f> next_X;
	
	for (unsigned i = 0; i < X.size(); i++){
		next_X.push_back(X[i] + stepSize * f_0[i]);
	}

	vector<Vector3f> f_1 = particleSystem->evalF(next_X);
	vector<Vector3f> new_state (X);
	Vector3f new_particle;
	
	for (unsigned i = 0; i < X.size(); i++){
		new_state[i] = X[i] + stepSize / 2 * (f_0[i] + f_1[i]);
	}

	//cout<<"state print: "<<endl;
	//for (unsigned i = 0; i < new_state.size(); i++){
	//	new_state[i].print();
	//}

	particleSystem->setState(new_state);
}
