#pragma once

/* Global defines */
//#define DebugConsole

class particle
{
	private:
		float charge, mass; //particle's electric charge and mass
		float position[3]; //particle 3D position (x, y, z)

	public:
		particle(float chargeArg, float massArg, float positionxArg, float positionyArg, float positionzArg);	//Constructor
		particle(particle &ob); //Copy constructor
		particle & operator=(const particle &L); //Assigment operator
		~particle(); //Destructor
		float getParticleCharge(); //Get particle charge
		int setParticlePosition(float x, float y, float z); //Set particle position
		int setParticleAxisPosition(float positionArg, char AxisArg); //Set particle single axis position
		int getParticlePosition(float *positionArg); //Get a particle position
		float getAxisParticlePosition(char AxisArg); //Get a specific axis coordinate
};