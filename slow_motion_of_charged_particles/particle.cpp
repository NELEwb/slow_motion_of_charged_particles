#include "particle.hpp"
#include <iostream>
using namespace std;

/* Constructor of a particle */
particle::particle(float chargeArg, float massArg, float positionxArg, float positionyArg, float positionzArg)
{
	#ifdef DebugConsole
	cout << "\nNew particle is being created...";
	#endif
	this->charge = chargeArg;
	this->mass = massArg;
	this->position[0] = positionxArg;
	this->position[1] = positionyArg;
	this->position[2] = positionzArg;
}

/* Copy constructor of a particle */
particle::particle(particle &p)
{
	int i;
	#ifdef DebugConsole 
	cout << "\nA particle is being copied..."; 
	#endif
	this->charge = p.charge;
	this->mass = p.mass;
	for (i = 0; i < 3; i++)
		this->position[i] = p.position[i];
}
/* Assigment operator */
particle & particle::operator=(const particle &object)
{
	/* check for "self assignment" and do nothing in that case */
	if (this == &object) return *this;
	else {
		this->charge = object.charge;
		this->mass = object.mass;
		this->position[0] = object.position[0];
		this->position[1] = object.position[1];
		this->position[2] = object.position[2];
		}
		return *this; // return this particle
}

/* Destructor of a particle */
particle::~particle()
{
	#ifdef DebugConsole
	cout << "\nA particle is being destroyed...";
	#endif
}

/* Get particle charge */
float particle::getParticleCharge()
{
	return this->charge;
}

/* Set particle position by providing values for all thre axes */
int particle::setParticlePosition(float x, float y, float z)
{
	int return_value = 1;
	this->position[0] = x;
	this->position[1] = y;
	this->position[2] = z;
	return_value = 0;
	return return_value;
}

/* Set particle single axis position, first argument is axis value and second one specifies the axis (e.g. 'x') */
int particle::setParticleAxisPosition(float positionArg, char AxisArg)
{
	int return_value = 1;
	if (AxisArg == 'x')
		this->position[0] = positionArg;
	else if (AxisArg == 'y')
		this->position[1] = positionArg;
	else if (AxisArg == 'z')
		this->position[2] = positionArg;
	return_value = 0;
	return return_value;
}

/* Get a particle position and store in array float *positionArg */
int particle::getParticlePosition(float *positionArg)
{
	int i, return_value = 1;
	for (i = 0; i < 3; i++)
		positionArg[i] = this->position[i];
	return_value = 0;
	return return_value;
}

/* Get a specific axis coordinate, request axis by passing character (e.g. 'x') */
float particle::getAxisParticlePosition(char AxisArg)
{
	float return_value = 0.0;
	if (AxisArg == 'x')
		return_value = this->position[0];
	else if (AxisArg == 'y')
		return_value = this->position[1];
	else if (AxisArg == 'z')
		return_value = this->position[2];
	return return_value;
}