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

// Fonction Factorielle
float fact(int n)
{
	if (n <= 0)
		return 1;
	return n * fact(n - 1);
}

float Bernstein(int i, int n, float t)
{
	return (fact(n) / (fact(i) * fact(n - i))) * powf(t, i) * powf(1 - t, n - i);
}

void drawHermite(point3 p0, point3 p1, point3 v0, point3 v1)
{
	glBegin(GL_LINE_STRIP);
	for (float u = 0.f; u <= 1.f; u += 0.01f)
	{
		const float u3 = u * u * u;
		const float u2 = u * u;
		const float f1 = 2.f * u3 - 3.f * u2 + 1.f;
		const float f2 = -2.f * u3 + 3.f * u2;
		const float f3 = u3 - 2.f * u2 + u;
		const float f4 = u3 - u2;
		point3 p((p0 * f1) + (p1 * f2) + (v0 * f3) + (v1 * f4));
		glVertex3f(p.x, p.y, p.z);
	}
	glEnd();
}

void drawBezier(point3 * TabPC, unsigned int Ordre)
{
	glColor3f(0.f, 1.f, 0.f);
	glBegin(GL_LINE_STRIP);
	for (float t = 0.f; t <= 1.f; t += 0.01f)
	{
		point3 acc(0.f, 0.f, 0.f);
		for (int i = 0; i < Ordre; i++)
		{
			acc = acc + point3(TabPC[i] * Bernstein(i, Ordre - 1, t));

		}
		glVertex3f(acc.x, acc.y, acc.z);
	}
	glEnd();

}

void addContrainteBezier2C0(point3 * TabPC1, point3 * TabPC2, const int Ordre1, const int Ordre2)
{
	TabPC2[0] = TabPC1[Ordre1 - 1];
}

void addContrainteBezier2C1(point3 * TabPC1, point3 * TabPC2, const int Ordre1, const int Ordre2)
{
	TabPC2[0] = TabPC1[Ordre1 - 1];
	point3 vecOffset = TabPC1[Ordre1 - 1] - TabPC1[Ordre1 - 2];
	TabPC2[1] = TabPC1[Ordre1 - 1] + vecOffset;
}

void drawEnveloppe(point3 * TabPC, const int Ordre)
{
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < Ordre; i++)
	{
		glVertex3f(TabPC[i].x, TabPC[i].y, TabPC[i].z);
	}
	glEnd();
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

	TabPC[numPoint] = TabPC[numPoint] + point3(tx, ty, 0);

	// Contraintes classe C
	//addContrainteBezier2C1(TabPC, TabPC2, Ordre, Ordre);

	// Enveloppe des points de controles
	glColor3f(1.0, 1.0, 0.0);
	drawEnveloppe(TabPC, Ordre);
	drawEnveloppe(TabPC2, Ordre);

	// Affichage du point de controle courant
	// On se déplace ensuite avec + et - 
	// ° d'un point de contrôle au suivant (+)
	// ° d'un point de contrôle au précédent (-)
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(TabPC[numPoint].x + 0.1f, TabPC[numPoint].y + 0.1f, TabPC[numPoint].z);
	glVertex3f(TabPC[numPoint].x + 0.1f, TabPC[numPoint].y - 0.1f, TabPC[numPoint].z);
	glVertex3f(TabPC[numPoint].x - 0.1f, TabPC[numPoint].y - 0.1f, TabPC[numPoint].z);
	glVertex3f(TabPC[numPoint].x - 0.1f, TabPC[numPoint].y + 0.1f, TabPC[numPoint].z);
	glEnd();

	// Courbe de Bézier
	drawBezier(TabPC, Ordre);
	drawBezier(TabPC2, Ordre);

	glEnd();
	glFlush();
}

/* Au cas ou la fenetre est modifiee ou deplacee */
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//  glOrtho(-20, 20, -20, 20, -10, 10);
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