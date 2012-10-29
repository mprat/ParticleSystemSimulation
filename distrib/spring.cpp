#include "spring.h"
#include<iostream>

using namespace std;

Spring::Spring(double k_s, double r_s, int particle_i, int particle_j)
{
	k = k_s;
	r = r_s;
	i = particle_i;
	j = particle_j;	
}

Vector3f Spring::getForce(Vector3f pos_i, Vector3f pos_j)
{
	Vector3f d = pos_i - pos_j;
	//cout<<"posi = "<<endl;
	//pos_i.print();
	//cout<<"posj = "<<endl;
	//pos_j.print();
	//cout<<"k = "<<k<<" dabs - r = "<<d.abs() - r<<endl;
	//d.normalized().print();
	//(-k * (d.abs() - r) * d.normalized()).print();
	return -k * (d.abs() - r) * d.normalized();
}
