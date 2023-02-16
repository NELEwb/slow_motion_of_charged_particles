#include <freeglut.h>
#include "particle.hpp"
#include <cmath>
#include <thread>
#include <iostream>
#include <vector>

using namespace std;

/* Global defines */
#define ScreenWidthPx 1920 //Screen width specified in number of pixels
#define ScreenHeightPx 1080 //Screen height specified in number of pixels
#define NumberOfThreads 8 //Number of working threads. Specified ScreenWidthPx must be divisible by NumberOfThreads
#define DefNumberOfParticles 2 //Number of particles being simulated. (Note: increasing this number will not create particles on its own.)
#define DefMaxNumberOfParticles 10 //Maximum number of alowed particles

/* Constants */
const float pi = 3.14159265, eps0 = 8.85e-12;

/* Variables */
float e, color, E, Ex, Ey, ang_deg, vector_length, ang = 0.0f, ang_step, radius = 0.3f;

/* Array for electric field vertices */
float data_vectorLines[311100];

/* Array for electric potential in space */
float  data_potential[ScreenWidthPx*ScreenHeightPx*6];

/* Variables */
int n = 2, i, too_close, width_pix = ScreenWidthPx, height_pix = ScreenHeightPx, v = 0, steps_number = 360;

/* Instances of charged particles */
int NumberOfParticles = DefNumberOfParticles;
particle *p[DefNumberOfParticles];

/* This function calculates electrical potential and stores colors and coordinates in large data_potential array
It takes charged particle as argument passed by reference, and thread performing the calculation.
Based on thread ID and total number of worker thead, it determines region where it calculates potential.
*/
void calculate_potential(particle **p, int threadNumberArg)
{
	register float x, y, potential, incr_x, incr_y, lowerBorder_x, lowerBorder_y, border_x, border_y, r[10];
	incr_x = 1.0f / width_pix;
	incr_y = 1.0f / width_pix; 
	int m;
	#ifdef DebugConsole
	cout << endl << "Thread number: " << threadNumberArg <<" started" << endl;
	#endif
	/* Border for x axis (segmented by number of threads) */
	lowerBorder_x = (1.0f / NumberOfThreads)*(threadNumberArg);
	border_x = (1.0f/NumberOfThreads)*(threadNumberArg + 1);

	/* Border for y axis */
	lowerBorder_y = 0.0f;
	border_y = 1.0f / ((float)width_pix / height_pix);

	/* Initial index in data_potential array for potential */
	m = (((ScreenWidthPx*ScreenHeightPx * 6) / NumberOfThreads)*(threadNumberArg));
	int i;

	particle *LocalParticleCopies[DefMaxNumberOfParticles];

	/* Create local copies of particles */
	for (i = 0; i < NumberOfParticles; i++)
		LocalParticleCopies[i] = new particle (*p[i]);

	/* first 3 elements are "RGB" colors, then next 2 elements are x and y coordinates, and so on . . . */
	for (x = lowerBorder_x; x<border_x; x += incr_x)
	{
		for (y = lowerBorder_y; y <= border_y; y += incr_y)
		{
			potential = 0.0f;
			for (i = 0; i<n; i++)
			{
				r[i] = sqrt(pow((x - LocalParticleCopies[i]->getAxisParticlePosition('x')), 2) + pow((y - LocalParticleCopies[i]->getAxisParticlePosition('y')), 2));
				potential += LocalParticleCopies[i]->getParticleCharge() / (4 * pi*eps0*r[i]);
			}
			if (potential >= 0)
			{
				data_potential[m] = potential / 100.0f; m++; data_potential[m] = 0.0f; m++; data_potential[m] = 0.0f; m++;
			}
			else
			{
				data_potential[m] = 0.0f;
				m++;
				data_potential[m] = 0.0f;
				m++;
				data_potential[m] = -potential / 100.0f;
				m++;
			}
			data_potential[m] = x;
			m++;
			data_potential[m] = y;
			m++;
		}
	}

	/* Delete local copies of particles */
	for (i = 0; i < NumberOfParticles; i++)
	{
		delete LocalParticleCopies[i];
	}

	return;
}

/* This function calculates electric field and creates vertices for drawing vectors in data_vectorLines array */
void calculateVertices4Vector()
{
	register float x, y, r[10];
	for (x = -0.1f; x<1.1f; x += 30.0f / width_pix)
	{
		for (y = -0.1f; y <= 1.1f / ((float)width_pix / height_pix); y += 30.0f / height_pix)
		{
			Ex = 0; Ey = 0;	too_close = 0;
			for (i = 0; i<n; i++)
			{
				r[i] = sqrt(pow((x - p[i]->getAxisParticlePosition('x')), 2) + pow((y - p[i]->getAxisParticlePosition('y')), 2));
				if (r[i]<0.075)
					too_close++;
			}
			for (i = 0; i<n; i++)
			{
				if (too_close)
					break;
				E = p[i]->getParticleCharge() / (4 * pi*eps0*pow(r[i], 2));
				Ex += E*(x - p[i]->getAxisParticlePosition('x')) / r[i];
				Ey += E*(y - p[i]->getAxisParticlePosition('y')) / r[i];
				ang_deg = atan2(Ey, Ex) * 180 / pi;
			}
			vector_length = sqrt(pow(Ex, 2) + pow(Ey, 2)) / 10000.0f;
			/* vector with total of 8 vertices */
			data_vectorLines[v] = x; v++; data_vectorLines[v] = y; v++; //first 2 coordinates are the beginning of vector
			data_vectorLines[v] = x + vector_length*cos(ang_deg*pi / 180); v++; data_vectorLines[v] = y + vector_length*sin(ang_deg*pi / 180); v++; //the next 2 coordinates are the ending of vector
			/* arrow */
			data_vectorLines[v] = data_vectorLines[v - 2]; v++; data_vectorLines[v] = data_vectorLines[v - 2]; v++; //ending point
			data_vectorLines[v] = data_vectorLines[v - 2] + vector_length / 5.0f*cos((ang_deg + 165)*pi / 180); v++;
			data_vectorLines[v] = data_vectorLines[v - 2] + vector_length / 5.0f*sin((ang_deg + 165)*pi / 180); v++;
			data_vectorLines[v] = data_vectorLines[v - 4]; v++; data_vectorLines[v] = data_vectorLines[v - 4]; v++; //ending point again
			data_vectorLines[v] = data_vectorLines[v - 6] + vector_length / 5.0f*cos((ang_deg + 195)*pi / 180); v++;
			data_vectorLines[v] = data_vectorLines[v - 6] + vector_length / 5.0f*sin((ang_deg + 195)*pi / 180); v++;
		}
	}
	return;
}

void Draw() {

	glClear(GL_COLOR_BUFFER_BIT);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(3, GL_FLOAT, 5 * sizeof(float), &data_potential[0]);
	glVertexPointer(2, GL_FLOAT, 5 * sizeof(float), &data_potential[3]);
	glDrawArrays(GL_POINTS, 0, (ScreenWidthPx*ScreenHeightPx * 6) / 5); //draw potential
	glDisableClientState(GL_COLOR_ARRAY);
	glColor3f(0.0, 1.0, 0.0);
	glVertexPointer(2, GL_FLOAT, 0, data_vectorLines);
	glDrawArrays(GL_LINES, 0, v / 2); //draw vectors
	glDisableClientState(GL_VERTEX_ARRAY);
	glutSwapBuffers();

}

void Initialize() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, (1.0/((float)width_pix/(float)height_pix)), -1.0, 1.0);
}
void kb(unsigned char c, int temppp_none, int temp_none)
{
	if (c == 'f') //Enter full screen
		glutFullScreen();
	else
	{
		glutPositionWindow(50, 50); //Top left window position
		glutReshapeWindow(900, 562); //Exit full screen and set window size
	}
}

void Timer(int unused)
{
	/* Check keyboard input: keypress <<f>> on render window - Full screen, other input - exit full screen */
	glutKeyboardFunc(kb);

	/* Reset angle to zero when one revolution is completed */
	if (ang>2 * pi)
		ang -= 2 * pi;

	ang += ang_step; //Increment angle of negatively charged particle
	p[1]->setParticleAxisPosition(radius*cos(ang) + p[0]->getAxisParticlePosition('x'), 'x'); //set new position of x axis for negatively charged particle
	p[1]->setParticleAxisPosition(radius*sin(ang) + p[0]->getAxisParticlePosition('y'), 'y'); //set new position of y axis for negatively charged particle
	v = 0;

	vector<thread> t;
	/* Start threads for calculating electric potential */
	for (int i = 0; i < NumberOfThreads; i++) {
		thread th = thread([i]() { calculate_potential(p, i); });
		t.push_back(move(th));
	}

	/* Thread for calculating electric field */
	thread th_ef(calculateVertices4Vector);

	/* Join threads */
	for (auto& th : t)
	{
		th.join();
	}
	th_ef.join();

	glutPostRedisplay();
	glutTimerFunc(1, Timer, 0);
}

int main(int iArgc, char** cppArgv) {
	int temp;
	p[0] = new particle(pow(0.12, 10), 0.0f, 0.5f, 0.5f/((float)ScreenWidthPx/(float)ScreenHeightPx), 0.0f); //Initial charge, mass and position for positively charged particle 
	p[1] = new particle(-pow(0.12, 10), 0.0f, radius*cos(ang) + p[0]->getAxisParticlePosition('x'), radius*cos(ang) + p[0]->getAxisParticlePosition('y'), 0.0f); //Initial charge, mass and position for negatively charged particle
	ang_step = 2 * pi / steps_number; //Calculate angular increment based on global variable steps number
	calculate_potential(p, 0); //Initialize potential array for the first time
	calculateVertices4Vector(); //Initialize electrict field for the first time
	glutInit(&iArgc, cppArgv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(900, 562); //Initial window size
	glutInitWindowPosition(100, 100); //Initial top-left position of render window
	glutCreateWindow("very slow motion of charged particles"); //Render window title
	Initialize();
	glutDisplayFunc(Draw);
	Timer(1);
	glutMainLoop();
	delete p[0];
	delete p[1];
	cout << "Please enter any number to exit: ";
	cin >> temp;
	return 0;
}
