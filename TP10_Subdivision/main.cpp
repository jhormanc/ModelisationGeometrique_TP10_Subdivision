#pragma once
/*
*	Tableau des points permettant de gérer les points de controles
* On sélectionne le point en se déplaçant avec + et -, ...
* On sélectionne ensuite si on veut faire monter, descendre amener vers la gauche ou la droite le point.
*   d : translation à droite
*   q : à gauche
*   z : en haut
*   s : en bas
*
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include "struct.h"
#include <vector>

/* au cas ou M_PI ne soit defini */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define ESC 27

float tx = 0.0f;
float ty = 0.0f;

// Tableau des points de contrôles en global ...
point3 TabPC[4] = { point3(-2., -2., 0.), point3(-1., 1., 0.), point3(1., 1., 0.), point3(2., -2., 0.) };
point3 TabPC2[4] = { point3(0., 0., 0.), point3(2., 2, 0.), point3(2., 2., 0.), point3(1., -1., 0.) };

// Ordre de la courbre  : Ordre
// Degré de la courbe = Ordre - 1
int Ordre = 4;

// Point de controle selectionné
int numPoint = 0;

void drawChaikin(const std::vector<point3> &tab, const int ordre, const point3 &color)
{
	if (ordre == 0)
	{
		if (tab.size() > 0)
		{
			glColor3f(color.x, color.y, color.z);
			glBegin(GL_LINE_STRIP);
			point3 p;
			for (int i = 0; i < tab.size(); i++)
			{
				p = tab[i];
				glVertex3f(p.x, p.y, p.z);
			}
			p = tab[0];
			glVertex3f(p.x, p.y, p.z);
			glEnd();
		}
	}
	else
	{
		std::vector<point3> new_tab;

		for (int i = 0; i < tab.size(); i++)
		{
			point3 p1(tab[i]);

			point3 p2 = (i < tab.size() - 1 ? tab[i + 1] : tab[0]);

			point3 q(p1 * 0.75 + p2 * 0.25);
			point3 r(p1 * 0.25 + p2 * 0.75);

			new_tab.push_back(q);
			new_tab.push_back(r);
		}

		drawChaikin(new_tab, ordre - 1, color);
	}
}

/* initialisation d'OpenGL*/
static void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

/* Dessin */
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	std::vector<point3> tab = { point3(-10., -10., 0.), point3(-6.,6, 0.), point3(6., 10., 0.), point3(10., 5., 0.), point3(7., -6., 0.), point3(-2., -12., 0.) };
	drawChaikin(tab, 0, point3(0, 255, 0));
	drawChaikin(tab, 4, point3(255, 0, 0));

	glEnd();
	glFlush();
}

/* Au cas ou la fenetre est modifiee ou deplacee */
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	  glOrtho(-10, 10, -10, 10, -10, 10);
	glOrtho(-3, 3, -3, 3, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '+':
		if (numPoint < Ordre - 1)
			numPoint = numPoint + 1;
		else
			numPoint = 0;
		tx = 0;
		ty = 0;
		break;
	case '-':
		if (numPoint > 0)
			numPoint = numPoint - 1;
		else
			numPoint = Ordre - 1;
		tx = 0;
		ty = 0;
		break;

	case 'd':
		tx = 0.1;
		ty = 0;
		break;
	case 'q':
		tx = -0.1;
		ty = 0;
		break;
	case 'z':
		ty = 0.1;
		tx = 0;
		break;
	case 's':
		ty = -0.1;
		tx = 0;
		break;
	case ESC:
		exit(0);
		break;
	default:
		tx = 0;
		ty = 0;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInitWindowSize(400, 400);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("Courbe de Bézier");
	init();
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}