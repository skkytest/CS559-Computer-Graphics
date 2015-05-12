#include <math.h>
#include "TrainView.H"
#include "TrainWindow.H"
#include <Gl/glew.h>
#include "Utilities/3DUtils.H"
#include <Fl/fl.h>
#include <FL/fl_ask.h>
// we will need OpenGL, and OpenGL needs windows.h
#include <windows.h>
#include "GL/gl.h"
#include "GL/glu.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ShaderTools.h"

#include"DrawObjects.h"
#include "../Utilities/Texture.H"
#include "SOIL.h"

#include <time.h>

//draw trees
void DrawObjects::drawTrees(TrainView* thisTrainView, bool doingShadows){
	/*
	glPushMatrix();
	if (!doingShadows) glColor3f(1, 0.647, 0);
	for (float j = 0; j < 2.5 * 3.14; j += 0.01f)
	{
		glBegin(GL_LINES);
		glVertex3f(cos(j), 0, sin(j));
		glVertex3f(cos(j), 17, sin(j));
		glEnd();
	}
	if (!doingShadows) glColor3f(0.180, 0.545, 0.341);
	for (float j = 0; j < 2.5 * 3.14; j += 0.01f)
	{
		glBegin(GL_TRIANGLES);
		glVertex3f(0, 10, 0);
		glVertex3f(10 * cos(j), 10, 10 * sin(j));
		glVertex3f(0, 30, 0);
		glEnd();
	}
	*/
	glPushMatrix();
	glTranslated(-90, 0, 0);
	if (!doingShadows) glColor3f(1, 0.647, 0);
	for (float j = 0; j < 2.5 * 3.14; j += 0.01f)
	{
		glBegin(GL_LINES);
		glVertex3f(cos(j), 0, sin(j));
		glVertex3f(cos(j), 17, sin(j));
		glEnd();
	}
	if (!doingShadows) glColor3f(0.180, 0.545, 0.341);
	for (float j = 0; j < 2.5 * 3.14; j += 0.01f)
	{
		glBegin(GL_TRIANGLES);
		glVertex3f(0, 10, 0);
		glVertex3f(10 * cos(j), 10, 10 * sin(j));
		glVertex3f(0, 30, 0);
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslated(90, 0, 0);
	if (!doingShadows) glColor3f(1, 0.647, 0);
	for (float j = 0; j < 2.5 * 3.14; j += 0.01f)
	{
		glBegin(GL_LINES);
		glVertex3f(cos(j), 0, sin(j));
		glVertex3f(cos(j), 17, sin(j));
		glEnd();
	}
	if (!doingShadows) glColor3f(0.180, 0.545, 0.341);
	for (float j = 0; j < 2.5 * 3.14; j += 0.01f)
	{
		glBegin(GL_TRIANGLES);
		glVertex3f(0, 10, 0);
		glVertex3f(10 * cos(j), 10, 10 * sin(j));
		glVertex3f(0, 30, 0);
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, -90);
	if (!doingShadows) glColor3f(1, 0.647, 0);
	for (float j = 0; j < 2.5 * 3.14; j += 0.01f)
	{
		glBegin(GL_LINES);
		glVertex3f(cos(j), 0, sin(j));
		glVertex3f(cos(j), 17, sin(j));
		glEnd();
	}
	if (!doingShadows) glColor3f(0.180, 0.545, 0.341);
	for (float j = 0; j < 2.5 * 3.14; j += 0.01f)
	{
		glBegin(GL_TRIANGLES);
		glVertex3f(0, 10, 0);
		glVertex3f(10 * cos(j), 10, 10 * sin(j));
		glVertex3f(0, 30, 0);
		glEnd();
	}
	glPopMatrix();

	glPopMatrix();
}

void DrawObjects::drawTrack(TrainView* thisTrainView, bool doingShadows){
	//draw track with c0 coutinuity
	if (thisTrainView->world->continuity == 1){
		glPushMatrix();
		for (int i = 0; i < (thisTrainView->world->points.size()); i++){

			float presentX = thisTrainView->world->points[i].pos.x;
			float presentY = thisTrainView->world->points[i].pos.y;
			float presentZ = thisTrainView->world->points[i].pos.z;
			int p = i + 1;
			if (i == (thisTrainView->world->points.size()) - 1) p = 0;
			float nextX = thisTrainView->world->points[p].pos.x;
			float nextY = thisTrainView->world->points[p].pos.y;
			float nextZ = thisTrainView->world->points[p].pos.z;
			glBegin(GL_LINES);
			if (!doingShadows) glColor3f(0.867, 0.427, 0.133);
			glVertex3f(presentX, presentY, presentZ);
			glVertex3f(nextX, nextY, nextZ);
			glEnd();

			double length = sqrt((presentX - nextX)*(presentX - nextX) + (presentZ - nextZ)*(presentZ - nextZ));
			double xbias, ybias, zbias;
			float xdis = nextX - presentX;
			float ydis = nextY - presentY;
			float zdis = nextZ - presentZ;
			float theAngle;
			float highAngle;
			if (xdis >= 0 && zdis >= 0){
				theAngle = 180 - atan(zdis / xdis) * 360 / 6.28;
			}
			else if (xdis > 0 && zdis < 0){
				zdis = (-1) * zdis;
				theAngle = (-1)*(180 - atan(zdis / xdis) * 360 / 6.28);
			}
			else if (xdis < 0 && zdis < 0){
				xdis = (-1) * xdis;
				zdis = (-1) * zdis;
				theAngle = (-1)*(atan(zdis / xdis) * 360 / 6.28);
			}
			else{
				xdis = (-1)*xdis;
				theAngle = atan(zdis / xdis) * 360 / 6.28;
			}
			highAngle = atan(ydis / (sqrt(xdis*xdis + zdis*zdis))) * 360 / 6.28;


			if (thisTrainView->world->trackType == 2){
				for (int times = 0; times / length < 0.98; times++){
					glPushMatrix();
					times = times + 5;
					xbias = presentX + (nextX - presentX) * (times / length);
					ybias = presentY + (nextY - presentY) * (times / length);
					zbias = presentZ + (nextZ - presentZ) * (times / length);
					glTranslatef(xbias, ybias - 4, zbias);
					glRotatef(90 + theAngle, 0, 1, 0);
					glRotatef(highAngle, 1, 0, 0);
					glTranslatef(-5, 0, -1);
					glBegin(GL_QUADS);
					if (!doingShadows) glColor3f(0.867, 0.427, 0.133);
					glVertex3f(0, 4, 0);
					glVertex3f(10, 4, 0);
					glVertex3f(10, 4, 2);
					glVertex3f(0, 4, 2);
					glEnd();
					glPopMatrix();
				}
			}
		}
		glPopMatrix();
	}

	//draw track with c1 continuity
	/*
							  [-ts 2-ts ts-2  ts ][P(0)]
							  [2ts ts-3 3-2ts -ts][P(1)]
	p(t) = 1/2 * [t^3 t^2 t 1][-ts  0    ts    0 ][P(2)]
							  [ 0   1    0     0 ][P(3)]

	P(1) and P(2) are present and next control points
	*/

	//local parameter to get tension from Window
	float ts = (float)thisTrainView->tw->tension->value() / 5.0+0.5;

	if (thisTrainView->world->continuity == 2){
		glPushMatrix();
		if (!doingShadows) glColor3f(0.867, 0.427, 0.133);
		for (int i = 0; i < (thisTrainView->world->points.size()); i++){
			int p;
			//P[0]
			if (i == 0) p = thisTrainView->world->points.size() - 1;
			else p = i - 1;
			float p0x = thisTrainView->world->points[p].pos.x;
			float p0y = thisTrainView->world->points[p].pos.y;
			float p0z = thisTrainView->world->points[p].pos.z;
			//p[1]
			float p1x = thisTrainView->world->points[i].pos.x;
			float p1y = thisTrainView->world->points[i].pos.y;
			float p1z = thisTrainView->world->points[i].pos.z;
			//P[2]
			p = (i + 1) % (thisTrainView->world->points.size());
			float p2x = thisTrainView->world->points[p].pos.x;
			float p2y = thisTrainView->world->points[p].pos.y;
			float p2z = thisTrainView->world->points[p].pos.z;
			//P[3]
			p = (i + 2) % (thisTrainView->world->points.size());
			float p3x = thisTrainView->world->points[p].pos.x;
			float p3y = thisTrainView->world->points[p].pos.y;
			float p3z = thisTrainView->world->points[p].pos.z;

			//calculate distance between P1 and P2
			float length = sqrt((p1x - p2x)*(p1x - p2x) + (p1y - p2y)*(p1y - p2y) + (p1z - p2z)*(p1z - p2z));

			//calculate the matrix
			float block = 0.0; // use this parameter to calculate where I need to put a wood of track in the thisTrainView->world
			// because when you prolong the track the ratio of distance may change, so we need to figure out a way to fix it
			//record where the last wood is
			float lastBlockX = p1x;
			float lastBlockY = p1y;
			float lastBlockZ = p1z;

			for (float t = 0; t < 1; t += 0.001){
				float px, py, pz;
				px = (((-ts)*p0x + (2-ts) * p1x + (ts-2)*p2x + ts*p3x)*t*t*t + (2*ts * p0x + (ts-3) * p1x + (3-2*ts) * p2x - ts*p3x)*t*t + ((-ts)*p0x + ts*p2x)*t +  p1x);
				py = (((-ts)*p0y + (2-ts) * p1y + (ts-2)*p2y + ts*p3y)*t*t*t + (2*ts * p0y + (ts-3) * p1y + (3-2*ts) * p2y - ts*p3y)*t*t + ((-ts)*p0y + ts*p2y)*t +  p1y);
				pz = (((-ts)*p0z + (2-ts) * p1z + (ts-2)*p2z + ts*p3z)*t*t*t + (2*ts * p0z + (ts-3) * p1z + (3-2*ts) * p2z - ts*p3z)*t*t + ((-ts)*p0z + ts*p2z)*t +  p1z);

				glBegin(GL_POINTS);
				glVertex3f(px, py, pz);
				glEnd();

				if (thisTrainView->world->trackType == 3){
					float p = t + 0.001;
					float tx, ty, tz;
					tx = (((-ts)*p0x + (2 - ts) * p1x + (ts - 2)*p2x + ts*p3x)*p*p*p + (2 * ts * p0x + (ts - 3) * p1x + (3 - 2 * ts) * p2x - ts*p3x)*p*p + ((-ts)*p0x + ts*p2x)*p + p1x);
					ty = (((-ts)*p0y + (2 - ts) * p1y + (ts - 2)*p2y + ts*p3y)*p*p*p + (2 * ts * p0y + (ts - 3) * p1y + (3 - 2 * ts) * p2y - ts*p3y)*p*p + ((-ts)*p0y + ts*p2y)*p + p1y);
					tz = (((-ts)*p0z + (2 - ts) * p1z + (ts - 2)*p2z + ts*p3z)*p*p*p + (2 * ts * p0z + (ts - 3) * p1z + (3 - 2 * ts) * p2z - ts*p3z)*p*p + ((-ts)*p0z + ts*p2z)*p + p1z);
					glBegin(GL_QUADS);
					//if (!doingShadows) glColor3f(0.867, 0.427, 0.133);
					glVertex3f(tx, ty, tz + 5);
					glVertex3f(tx, ty, tz - 5);
					glVertex3f(px, py, pz - 5);
					glVertex3f(px, py, pz + 5);
					glEnd();
					glPopMatrix();
				}

				block += 0.001;
				if (block * length >= 6){
					float xdis = px - lastBlockX;
					float ydis = py - lastBlockY;
					float zdis = pz - lastBlockZ;
					//to calculate the rotate angle
					float theAngle;  // this parameter is the rotate angle in space X-Z
					float highAngle; // this parameter is the rotate angle in space X-Y
					if (xdis >= 0 && zdis >= 0){
						theAngle = 180 - atan(zdis / xdis) * 360 / 6.28;
					}
					else if (xdis > 0 && zdis < 0){
						theAngle = (-1)*(180 - atan((-1)*zdis / xdis) * 360 / 6.28);
					}
					else if (xdis < 0 && zdis < 0){
						theAngle = (-1)*(atan(zdis / xdis) * 360 / 6.28);
					}
					else{
						theAngle = atan((-1)*zdis / xdis) * 360 / 6.28;
					}
					highAngle = atan(ydis / (sqrt(xdis*xdis + zdis*zdis))) * 360 / 6.28;

					if (thisTrainView->world->trackType == 2){

						glPushMatrix();
						glTranslatef(px, py, pz);
						glRotatef(90 + theAngle, 0, 1, 0);
						glRotatef(highAngle, 1, 0, 0);
						glTranslatef(-5, 0, -1);
						glBegin(GL_QUADS);
						//if (!doingShadows) glColor3f(0.867, 0.427, 0.133);
						glVertex3f(0, 0, 0);
						glVertex3f(10, 0, 0);
						glVertex3f(10, 0, 2);
						glVertex3f(0, 0, 2);
						glEnd();
						glPopMatrix();

					}


					block = 0;

					lastBlockX = px;
					lastBlockY = py;
					lastBlockZ = pz;
				}
			}
		}
		glPopMatrix();
	}

	//draw track with c1 continuity
	/*
	[-1  3 -3  1][P0]
	Q(u) = 1/6[u^3 u^2 u 1][ 3 -6  3  0][P1]
	[-3  0  3  0][P2]
	[ 1  4  1  0][P3]
	*/
	if (thisTrainView->world->continuity == 3){
		glPushMatrix();

		for (int i = 0; i < (thisTrainView->world->points.size()); i++){
			int number = thisTrainView->world->points.size();
			//P[0]
			if (i == number) i = 0;
			float p0x = thisTrainView->world->points[i%number].pos.x;
			float p0y = thisTrainView->world->points[i%number].pos.y;
			float p0z = thisTrainView->world->points[i%number].pos.z;
			//p[1]
			float p1x = thisTrainView->world->points[(i + 1) % number].pos.x;
			float p1y = thisTrainView->world->points[(i + 1) % number].pos.y;
			float p1z = thisTrainView->world->points[(i + 1) % number].pos.z;
			//P[2]
			float p2x = thisTrainView->world->points[(i + 2) % number].pos.x;
			float p2y = thisTrainView->world->points[(i + 2) % number].pos.y;
			float p2z = thisTrainView->world->points[(i + 2) % number].pos.z;
			//P[3]
			float p3x = thisTrainView->world->points[(i + 3) % number].pos.x;
			float p3y = thisTrainView->world->points[(i + 3) % number].pos.y;
			float p3z = thisTrainView->world->points[(i + 3) % number].pos.z;


			//save the distance between two point
			float length;

			//record where the last wood is
			float lastBlockX = 1.0 / 6 * (((-1)*p0x + 3 * p1x + (-3)*p2x + p3x)*0.01*0.01*0.01 + (3 * p0x - 6 * p1x + 3 * p2x)*0.01*0.01 + ((-3)*p0x + 3 * p2x)*0.01 + p0x + 4 * p1x + p2x);
			float lastBlockY = 1.0 / 6 * (((-1)*p0y + 3 * p1y + (-3)*p2y + p3y)*0.01*0.01*0.01 + (3 * p0y - 6 * p1y + 3 * p2y)*0.01*0.01 + ((-3)*p0y + 3 * p2y)*0.01 + p0y + 4 * p1y + p2y);
			float lastBlockZ = 1.0 / 6 * (((-1)*p0z + 3 * p1z + (-3)*p2z + p3z)*0.01*0.01*0.01 + (3 * p0z - 6 * p1z + 3 * p2z)*0.01*0.01 + ((-3)*p0z + 3 * p2z)*0.01 + p0z + 4 * p1z + p2z);

			for (float t = 0; t < 1; t += 0.001){
				float px, py, pz;
				px = 1.0 / 6 * (((-1)*p0x + 3 * p1x + (-3)*p2x + p3x)*t*t*t + (3 * p0x - 6 * p1x + 3 * p2x)*t*t + ((-3)*p0x + 3 * p2x)*t + p0x + 4 * p1x + p2x);
				py = 1.0 / 6 * (((-1)*p0y + 3 * p1y + (-3)*p2y + p3y)*t*t*t + (3 * p0y - 6 * p1y + 3 * p2y)*t*t + ((-3)*p0y + 3 * p2y)*t + p0y + 4 * p1y + p2y);
				pz = 1.0 / 6 * (((-1)*p0z + 3 * p1z + (-3)*p2z + p3z)*t*t*t + (3 * p0z - 6 * p1z + 3 * p2z)*t*t + ((-3)*p0z + 3 * p2z)*t + p0z + 4 * p1z + p2z);


				glBegin(GL_POINTS);
				if (!doingShadows) glColor3f(0.867, 0.427, 0.133);
				glVertex3f(px, py, pz);
				glEnd();

				length = sqrt((lastBlockX - px)*(lastBlockX - px) + (lastBlockY - py)*(lastBlockY - py) + (lastBlockZ - pz)*(lastBlockZ - pz));

				if (length > 5 || t == 0.02){
					float xdis = px - lastBlockX;
					float ydis = py - lastBlockY;
					float zdis = pz - lastBlockZ;
					//to calculate the rotate angle
					float theAngle;  // this parameter is the rotate angle in space X-Z
					float highAngle; // this parameter is the rotate angle in space X-Y
					if (xdis >= 0 && zdis >= 0){
						theAngle = 180 - atan(zdis / xdis) * 360 / 6.28;
					}
					else if (xdis > 0 && zdis < 0){
						theAngle = (-1)*(180 - atan((-1)*zdis / xdis) * 360 / 6.28);
					}
					else if (xdis < 0 && zdis < 0){
						theAngle = (-1)*(atan(zdis / xdis) * 360 / 6.28);
					}
					else{
						theAngle = atan((-1)*zdis / xdis) * 360 / 6.28;
					}
					highAngle = atan(ydis / (sqrt(xdis*xdis + zdis*zdis))) * 360 / 6.28;

					if (thisTrainView->world->trackType == 2){

						glPushMatrix();
						glTranslatef(px, py, pz);
						glRotatef(90 + theAngle, 0, 1, 0);
						glRotatef(highAngle, 1, 0, 0);
						glTranslatef(-5, 0, -1);
						glBegin(GL_QUADS);
						if (!doingShadows) glColor3f(0.867, 0.427, 0.133);
						glVertex3f(0, 0, 0);
						glVertex3f(10, 0, 0);
						glVertex3f(10, 0, 2);
						glVertex3f(0, 0, 2);
						glEnd();
						glPopMatrix();

					}
					lastBlockX = px;
					lastBlockY = py;
					lastBlockZ = pz;

				}
			}
		}

		glPopMatrix();
	}
}

void DrawObjects::drawTrain(TrainView* thisTrainView, bool doingShadows){
	glPushMatrix();
	//for the body part
	glBegin(GL_QUADS);
	// front
	//(x-axis, z-axis, y-axis)
	if (!doingShadows) glColor3f(0.968f, 0.219f, 0.035f);
	glVertex3f(0, 4, 0);
	glVertex3f(0, 14, 0);
	glVertex3f(0, 14, 10);
	glVertex3f(0, 4, 10);

	// back
	if (!doingShadows) glColor3f(0.968f, 0.219f, 0.035f);
	glVertex3f(30, 4, 0);
	glVertex3f(30, 14, 0);
	glVertex3f(30, 14, 10);
	glVertex3f(30, 4, 10);

	// top
	if (!doingShadows) glColor3f(0.067f, 0.761f, 0.937f);
	glVertex3f(0, 14, 0);
	glVertex3f(30, 14, 0);
	glVertex3f(30, 14, 10);
	glVertex3f(0, 14, 10);

	// bottom
	if (!doingShadows) glColor3f(0.067f, 0.761f, 0.937f);
	glVertex3f(0, 4, 0);
	glVertex3f(30, 4, 0);
	glVertex3f(30, 4, 10);
	glVertex3f(0, 4, 10);

	// left
	if (!doingShadows) glColor3f(0.965f, 0.580f, 0.102f);
	glVertex3f(0, 4, 0);
	glVertex3f(0, 14, 0);
	glVertex3f(30, 14, 0);
	glVertex3f(30, 4, 0);

	// right
	if (!doingShadows) glColor3f(0.965f, 0.580f, 0.102f);
	glVertex3f(0, 4, 10);
	glVertex3f(0, 14, 10);
	glVertex3f(30, 14, 10);
	glVertex3f(30, 4, 10);
	glEnd();

	//for the higher part
	glBegin(GL_QUADS);
	// front
	//(x-axis, z-axis, y-axis)
	if (!doingShadows) glColor3f(0.965f, 0.580f, 0.102f);
	glVertex3f(20, 14, 0);
	glVertex3f(20, 22, 0);
	glVertex3f(20, 22, 10);
	glVertex3f(20, 14, 10);

	// back
	if (!doingShadows) glColor3f(0.968f, 0.219f, 0.035f);
	glVertex3f(30, 14, 0);
	glVertex3f(30, 22, 0);
	glVertex3f(30, 22, 10);
	glVertex3f(30, 14, 10);

	// top
	if (!doingShadows) glColor3f(0.067f, 0.761f, 0.937f);
	glVertex3f(20, 22, 0);
	glVertex3f(30, 22, 0);
	glVertex3f(30, 22, 10);
	glVertex3f(20, 22, 10);

	// bottom
	if (!doingShadows) glColor3f(0.067f, 0.761f, 0.937f);
	glVertex3f(20, 14, 0);
	glVertex3f(30, 14, 0);
	glVertex3f(30, 14, 10);
	glVertex3f(20, 14, 10);

	// left
	if (!doingShadows) glColor3f(0.965f, 0.580f, 0.102f);
	glVertex3f(20, 14, 0);
	glVertex3f(30, 14, 0);
	glVertex3f(30, 22, 0);
	glVertex3f(20, 22, 0);

	// right
	if (!doingShadows) glColor3f(0.965f, 0.580f, 0.102f);
	glVertex3f(20, 14, 10);
	glVertex3f(30, 14, 10);
	glVertex3f(30, 22, 10);
	glVertex3f(20, 22, 10);
	glEnd();


	//for the wheels part
	//left and front wheel
	glPushMatrix();
	glTranslated(7.5, 4, 10);
	glScaled(3, 3, 1);
	for (float i = 0; i < 1; i += 0.1f)
	{
		for (float j = 0; j < 8 * 3.14; j += 0.01f)
		{
			glBegin(GL_QUADS);
			if (!doingShadows) glColor3f(0, 0, 0);
			glVertex3f(cos(j), sin(j), i);
			glVertex3f(cos(j + 1), sin(j + 1), i);
			glVertex3f(cos(j + 1), sin(j + 1), i + 1);
			glVertex3f(cos(j), sin(j), i + 1);
			glEnd();
		}
	}
	glPopMatrix();

	glPushMatrix();
	glTranslated(7.5, 4, 12);
	glScaled(3, 3, 1);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0, 0);
		glVertex3f(0.0, 0.0, 0);
		glVertex3f(cos(k), sin(k), 0);
		glVertex3f(cos(k + 0.05), sin(k + 0.05), 0);
		glEnd();
	}
	glPopMatrix();

	//left and back wheel
	glPushMatrix();
	glTranslated(22.5, 4, 10);
	glScaled(3, 3, 1);
	for (float i = 0; i < 1; i += 0.1f)
	{
		for (float j = 0; j < 8 * 3.14; j += 0.01f)
		{
			glBegin(GL_QUADS);
			if (!doingShadows) glColor3f(0, 0, 0);
			glVertex3f(cos(j), sin(j), i);
			glVertex3f(cos(j + 1), sin(j + 1), i);
			glVertex3f(cos(j + 1), sin(j + 1), i + 1);
			glVertex3f(cos(j), sin(j), i + 1);
			glEnd();
		}
	}
	glPopMatrix();

	glPushMatrix();
	glTranslated(22.5, 4, 12);
	glScaled(3, 3, 1);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0, 0);
		glVertex3f(0.0, 0.0, 0);
		glVertex3f(cos(k), sin(k), 0);
		glVertex3f(cos(k + 0.05), sin(k + 0.05), 0);
		glEnd();
	}
	glPopMatrix();

	//right and front wheel
	glPushMatrix();
	glTranslated(7.5, 4, -2);
	glScaled(3, 3, 1);
	for (float i = 0; i < 1; i += 0.1f)
	{
		for (float j = 0; j < 8 * 3.14; j += 0.01f)
		{
			glBegin(GL_QUADS);
			if (!doingShadows) glColor3f(0, 0, 0);
			glVertex3f(cos(j), sin(j), i);
			glVertex3f(cos(j + 1), sin(j + 1), i);
			glVertex3f(cos(j + 1), sin(j + 1), i + 1);
			glVertex3f(cos(j), sin(j), i + 1);
			glEnd();
		}
	}
	glPopMatrix();

	glPushMatrix();
	glTranslated(7.5, 4, -2);
	glScaled(3, 3, 1);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0, 0);
		glVertex3f(0.0, 0.0, 0);
		glVertex3f(cos(k), sin(k), 0);
		glVertex3f(cos(k + 0.05), sin(k + 0.05), 0);
		glEnd();
	}
	glPopMatrix();


	//right and back wheel
	glPushMatrix();
	glTranslated(22.5, 4, -2);
	glScaled(3, 3, 1);
	for (float i = 0; i < 1; i += 0.1f)
	{
		for (float j = 0; j < 8 * 3.14; j += 0.01f)
		{
			glBegin(GL_QUADS);
			if (!doingShadows) glColor3f(0, 0, 0);
			glVertex3f(cos(j), sin(j), i);
			glVertex3f(cos(j + 1), sin(j + 1), i);
			glVertex3f(cos(j + 1), sin(j + 1), i + 1);
			glVertex3f(cos(j), sin(j), i + 1);
			glEnd();
		}
	}
	glPopMatrix();

	glPushMatrix();
	glTranslated(22.5, 4, -2);
	glScaled(3, 3, 1);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0);
		glVertex3f(cos(k), sin(k), 0);
		glVertex3f(cos(k + 0.05), sin(k + 0.05), 0);
		glEnd();
	}
	glPopMatrix();


	//for the funnel part
	glPushMatrix();
	glTranslated(7.5, 14, 5);
	glScaled(3, 1, 3);
	if (!doingShadows) glColor3f(1.0, 0.0, 0.0);
	for (float j = 0; j < 8 * 3.14; j += 0.01f)
	{
		glBegin(GL_LINES);
		glVertex3f(cos(j), 0, sin(j));
		glVertex3f(cos(j), 6, sin(j));
		glEnd();
	}

	glPopMatrix();


	glPushMatrix();
	glTranslated(7.5, 19, 5);
	glScaled(3, 1, 3);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(cos(k), 0, sin(k));
		glVertex3f(cos(k + 0.05), 0, sin(k + 0.05));
		glEnd();
	}
	glPopMatrix();


	glPopMatrix();
}

void DrawObjects::drawTank(TrainView* thisTrainView, bool doingShadows){
	glPushMatrix();
	//for the bootom part
	glBegin(GL_QUADS);
	// bottom
	//(x-axis, z-axis, y-axis)
	if (!doingShadows) glColor3f(0.968f, 0.219f, 0.035f);
	glVertex3f(3, 3, 2);
	glVertex3f(27, 3, 2);
	glVertex3f(27, 3, 8);
	glVertex3f(3, 3, 8);
	//top
	if (!doingShadows) glColor3f(0.968f, 0.219f, 0.035f);
	glVertex3f(3, 10, 2);
	glVertex3f(27, 10, 2);
	glVertex3f(27, 10, 8);
	glVertex3f(3, 10, 8);
	//front
	if (!doingShadows) glColor3f(0.968f, 0.219f, 0.035f);
	glVertex3f(3, 3, 2);
	glVertex3f(3, 10, 2);
	glVertex3f(3, 10, 8);
	glVertex3f(3, 3, 8);
	//back
	if (!doingShadows) glColor3f(0.968f, 0.219f, 0.035f);
	glVertex3f(27, 3, 2);
	glVertex3f(27, 10, 2);
	glVertex3f(27, 10, 8);
	glVertex3f(27, 3, 8);
	//left
	if (!doingShadows) glColor3f(0.968f, 0.219f, 0.035f);
	glVertex3f(3, 3, 8);
	glVertex3f(3, 10, 8);
	glVertex3f(27, 10, 8);
	glVertex3f(27, 3, 8);
	//right
	if (!doingShadows) glColor3f(0.968f, 0.219f, 0.035f);
	glVertex3f(3, 3, 2);
	glVertex3f(3, 10, 2);
	glVertex3f(27, 10, 2);
	glVertex3f(27, 3, 2);

	glEnd();
	glPopMatrix();

	glPushMatrix();
	//for middle 
	glBegin(GL_QUADS);
	//left
	if (!doingShadows) glColor3f(0.067f, 0.761f, 0.937f);
	glVertex3f(7, 13, 10);
	glVertex3f(27, 13, 10);
	glVertex3f(30, 10, 10);
	glVertex3f(0, 10, 10);
	//right
	if (!doingShadows) glColor3f(0.067f, 0.761f, 0.937f);
	glVertex3f(7, 13, 0);
	glVertex3f(27, 13, 0);
	glVertex3f(30, 10, 0);
	glVertex3f(0, 10, 0);
	//bottom
	if (!doingShadows) glColor3f(0.067f, 0.761f, 0.937f);
	glVertex3f(0, 10, 10);
	glVertex3f(30, 10, 10);
	glVertex3f(30, 10, 0);
	glVertex3f(0, 10, 0);
	//top
	if (!doingShadows) glColor3f(0.067f, 0.761f, 0.937f);
	glVertex3f(7, 13, 0);
	glVertex3f(27, 13, 0);
	glVertex3f(27, 13, 10);
	glVertex3f(7, 13, 10);
	//front
	if (!doingShadows) glColor3f(0.067f, 0.761f, 0.937f);
	glVertex3f(7, 13, 0);
	glVertex3f(7, 13, 10);
	glVertex3f(0, 10, 10);
	glVertex3f(0, 10, 0);
	//back
	if (!doingShadows) glColor3f(0.067f, 0.761f, 0.937f);
	glVertex3f(27, 13, 0);
	glVertex3f(27, 13, 10);
	glVertex3f(30, 10, 10);
	glVertex3f(30, 10, 0);

	glEnd();
	glPopMatrix();

	//for top
	glPushMatrix();
	glBegin(GL_QUADS);
	//right
	if (!doingShadows) glColor3f(0.965f, 0.580f, 0.102f);
	glVertex3f(13, 18, 0);
	glVertex3f(21, 18, 0);
	glVertex3f(21, 13, 0);
	glVertex3f(13, 13, 0);
	//left
	if (!doingShadows) glColor3f(0.965f, 0.580f, 0.102f);
	glVertex3f(13, 18, 10);
	glVertex3f(21, 18, 10);
	glVertex3f(21, 13, 10);
	glVertex3f(13, 13, 10);
	//top
	if (!doingShadows) glColor3f(0.965f, 0.580f, 0.102f);
	glVertex3f(13, 18, 0);
	glVertex3f(21, 18, 0);
	glVertex3f(21, 18, 10);
	glVertex3f(13, 18, 10);
	//bottom
	if (!doingShadows) glColor3f(0.965f, 0.580f, 0.102f);
	glVertex3f(13, 18, 0);
	glVertex3f(21, 18, 0);
	glVertex3f(21, 13, 10);
	glVertex3f(13, 13, 10);
	//front
	if (!doingShadows) glColor3f(0.965f, 0.580f, 0.102f);
	glVertex3f(13, 18, 0);
	glVertex3f(13, 18, 10);
	glVertex3f(13, 13, 10);
	glVertex3f(13, 13, 0);
	//back
	if (!doingShadows) glColor3f(0.965f, 0.580f, 0.102f);
	glVertex3f(21, 18, 0);
	glVertex3f(21, 18, 10);
	glVertex3f(21, 13, 10);
	glVertex3f(21, 13, 0);

	glEnd();
	glPopMatrix();


	//the barrel part
	glPushMatrix();
	glTranslatef(13, 15.5, 5);
	glRotatef(83, 0, 0, 1);
	if (!doingShadows) glColor3f(1.0, 0.0, 0.0);
	for (float j = 0; j < 2.5 * 3.14; j += 0.01f)
	{
		glBegin(GL_LINES);
		glVertex3f(cos(j), 0, sin(j));
		glVertex3f(cos(j), 17, sin(j));
		glEnd();
	}
	glPopMatrix();

	//wheel
	//left
	glPushMatrix();
	glBegin(GL_QUADS);
	if (!doingShadows) glColor3f(0.749, 0.749, 0.749);
	glVertex3f(0, 10, 10);
	glVertex3f(30, 10, 10);
	glVertex3f(25, 5, 10);
	glVertex3f(5, 5, 10);

	if (!doingShadows) glColor3f(0, 0.5, 1);
	glVertex3f(0, 10, 10);
	glVertex3f(0, 10, 8);
	glVertex3f(0, 5, 8);
	glVertex3f(0, 5, 10);

	if (!doingShadows) glColor3f(0, 0.5, 1);
	glVertex3f(30, 10, 10);
	glVertex3f(30, 10, 8);
	glVertex3f(30, 5, 8);
	glVertex3f(30, 5, 10);

	if (!doingShadows) glColor3f(0, 0.5, 1);
	glVertex3f(0, 5, 8);
	glVertex3f(0, 5, 10);
	glVertex3f(5, 0, 10);
	glVertex3f(5, 0, 8);

	if (!doingShadows) glColor3f(0, 0.5, 1);
	glVertex3f(30, 5, 8);
	glVertex3f(30, 5, 10);
	glVertex3f(25, 0, 10);
	glVertex3f(25, 0, 8);

	if (!doingShadows) glColor3f(0, 0.5, 1);
	glVertex3f(25, 0, 8);
	glVertex3f(25, 0, 10);
	glVertex3f(5, 0, 10);
	glVertex3f(5, 0, 8);

	//right
	if (!doingShadows) glColor3f(0.749, 0.749, 0.749);
	glVertex3f(0, 10, 0);
	glVertex3f(30, 10, 0);
	glVertex3f(25, 6, 0);
	glVertex3f(5, 6, 0);

	if (!doingShadows) glColor3f(0, 0.5, 1);
	glVertex3f(0, 10, 2);
	glVertex3f(0, 10, 0);
	glVertex3f(0, 5, 0);
	glVertex3f(0, 5, 2);

	if (!doingShadows) glColor3f(0, 0.5, 1);
	glVertex3f(30, 10, 2);
	glVertex3f(30, 10, 0);
	glVertex3f(30, 5, 0);
	glVertex3f(30, 5, 2);

	if (!doingShadows) glColor3f(0, 0.5, 1);
	glVertex3f(0, 5, 0);
	glVertex3f(0, 5, 2);
	glVertex3f(5, 0, 2);
	glVertex3f(5, 0, 0);

	if (!doingShadows) glColor3f(0, 0.5, 1);
	glVertex3f(30, 5, 0);
	glVertex3f(30, 5, 2);
	glVertex3f(25, 0, 2);
	glVertex3f(25, 0, 0);

	if (!doingShadows) glColor3f(0, 0.5, 1);
	glVertex3f(25, 0, 0);
	glVertex3f(25, 0, 2);
	glVertex3f(5, 0, 2);
	glVertex3f(5, 0, 0);

	glEnd();
	glPopMatrix();

	//rounds
	//LEFT PART-------------------------------------
	//one round start--------------------------------
	glPushMatrix();
	glTranslatef(7, 3, 8);
	glRotatef(90, 1, 0, 0);
	glScaled(3, 1, 3);
	if (!doingShadows) glColor3f(0.0, 0.0, 0.0);
	for (float j = 0; j < 2.0 * 3.14; j += 0.01f)
	{
		glBegin(GL_LINES);
		glVertex3f(cos(j), 0, sin(j));
		glVertex3f(cos(j), 2, sin(j));
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(7, 3, 8.2);
	glRotatef(90, 1, 0, 0);
	glScaled(3, 1, 3);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(cos(k), 0, sin(k));
		glVertex3f(cos(k + 0.05), 0, sin(k + 0.05));
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(7, 3, 9.8);
	glRotatef(90, 1, 0, 0);
	glScaled(3, 1, 3);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(cos(k), 0, sin(k));
		glVertex3f(cos(k + 0.05), 0, sin(k + 0.05));
		glEnd();
	}
	glPopMatrix();
	//one round over--------------------------------------

	//second---------------------------------------
	glPushMatrix();
	glTranslatef(14, 3, 8);
	glRotatef(90, 1, 0, 0);
	glScaled(3, 1, 3);
	if (!doingShadows) glColor3f(0.0, 0.0, 0.0);
	for (float j = 0; j < 2.0 * 3.14; j += 0.01f)
	{
		glBegin(GL_LINES);
		glVertex3f(cos(j), 0, sin(j));
		glVertex3f(cos(j), 2, sin(j));
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(14, 3, 8.2);
	glRotatef(90, 1, 0, 0);
	glScaled(3, 1, 3);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(cos(k), 0, sin(k));
		glVertex3f(cos(k + 0.05), 0, sin(k + 0.05));
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(14, 3, 9.8);
	glRotatef(90, 1, 0, 0);
	glScaled(3, 1, 3);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(cos(k), 0, sin(k));
		glVertex3f(cos(k + 0.05), 0, sin(k + 0.05));
		glEnd();
	}
	glPopMatrix();

	//third-----------------------------------
	glPushMatrix();
	glTranslatef(21, 3, 8);
	glRotatef(90, 1, 0, 0);
	glScaled(3, 1, 3);
	if (!doingShadows) glColor3f(0.0, 0.0, 0.0);
	for (float j = 0; j < 2.0 * 3.14; j += 0.01f)
	{
		glBegin(GL_LINES);
		glVertex3f(cos(j), 0, sin(j));
		glVertex3f(cos(j), 2, sin(j));
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(21, 3, 8.2);
	glRotatef(90, 1, 0, 0);
	glScaled(3, 1, 3);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(cos(k), 0, sin(k));
		glVertex3f(cos(k + 0.05), 0, sin(k + 0.05));
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(21, 3, 9.8);
	glRotatef(90, 1, 0, 0);
	glScaled(3, 1, 3);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(cos(k), 0, sin(k));
		glVertex3f(cos(k + 0.05), 0, sin(k + 0.05));
		glEnd();
	}
	glPopMatrix();

	//fourth--------------------------------------
	glPushMatrix();
	glTranslatef(27.45, 6.1, 8);
	glRotatef(90, 1, 0, 0);
	glScaled(2.5, 1, 2.5);
	if (!doingShadows) glColor3f(0.0, 0.0, 0.0);
	for (float j = 0; j < 2.0 * 3.14; j += 0.01f)
	{
		glBegin(GL_LINES);
		glVertex3f(cos(j), 0, sin(j));
		glVertex3f(cos(j), 2, sin(j));
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(27.45, 6.1, 8.2);
	glRotatef(90, 1, 0, 0);
	glScaled(2.5, 1, 2.5);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(cos(k), 0, sin(k));
		glVertex3f(cos(k + 0.05), 0, sin(k + 0.05));
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(27.45, 6.1, 9.8);
	glRotatef(90, 1, 0, 0);
	glScaled(2.5, 1, 2.5);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(cos(k), 0, sin(k));
		glVertex3f(cos(k + 0.05), 0, sin(k + 0.05));
		glEnd();
	}
	glPopMatrix();

	//fifth---------------------------------------
	glPushMatrix();
	glTranslatef(2.3, 6.1, 8);
	glRotatef(90, 1, 0, 0);
	glScaled(2.3, 1, 2.3);
	if (!doingShadows) glColor3f(0.0, 0.0, 0.0);
	for (float j = 0; j < 2.0 * 3.14; j += 0.01f)
	{
		glBegin(GL_LINES);
		glVertex3f(cos(j), 0, sin(j));
		glVertex3f(cos(j), 2, sin(j));
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.3, 6.1, 8.2);
	glRotatef(90, 1, 0, 0);
	glScaled(2.3, 1, 2.3);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(cos(k), 0, sin(k));
		glVertex3f(cos(k + 0.05), 0, sin(k + 0.05));
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.3, 6.1, 9.8);
	glRotatef(90, 1, 0, 0);
	glScaled(2.3, 1, 2.3);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(cos(k), 0, sin(k));
		glVertex3f(cos(k + 0.05), 0, sin(k + 0.05));
		glEnd();
	}
	glPopMatrix();


	//right part

	//first one--------------------------------------
	glPushMatrix();
	glTranslatef(7, 3, 0.2);
	glRotatef(90, 1, 0, 0);
	glScaled(3, 1, 3);
	if (!doingShadows) glColor3f(0.0, 0.0, 0.0);
	for (float j = 0; j < 2.0 * 3.14; j += 0.01f)
	{
		glBegin(GL_LINES);
		glVertex3f(cos(j), 0, sin(j));
		glVertex3f(cos(j), 2, sin(j));
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(7, 3, 0.2);
	glRotatef(90, 1, 0, 0);
	glScaled(3, 1, 3);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(cos(k), 0, sin(k));
		glVertex3f(cos(k + 0.05), 0, sin(k + 0.05));
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(7, 3, 2);
	glRotatef(90, 1, 0, 0);
	glScaled(3, 1, 3);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(cos(k), 0, sin(k));
		glVertex3f(cos(k + 0.05), 0, sin(k + 0.05));
		glEnd();
	}
	glPopMatrix();
	//one round over--------------------------------------

	//second---------------------------------------
	glPushMatrix();
	glTranslatef(14, 3, 0.2);
	glRotatef(90, 1, 0, 0);
	glScaled(3, 1, 3);
	if (!doingShadows) glColor3f(0.0, 0.0, 0.0);
	for (float j = 0; j < 2.0 * 3.14; j += 0.01f)
	{
		glBegin(GL_LINES);
		glVertex3f(cos(j), 0, sin(j));
		glVertex3f(cos(j), 2, sin(j));
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(14, 3, 0.2);
	glRotatef(90, 1, 0, 0);
	glScaled(3, 1, 3);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(cos(k), 0, sin(k));
		glVertex3f(cos(k + 0.05), 0, sin(k + 0.05));
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(14, 3, 2);
	glRotatef(90, 1, 0, 0);
	glScaled(3, 1, 3);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(cos(k), 0, sin(k));
		glVertex3f(cos(k + 0.05), 0, sin(k + 0.05));
		glEnd();
	}
	glPopMatrix();

	//third-----------------------------------
	glPushMatrix();
	glTranslatef(21, 3, 0.2);
	glRotatef(90, 1, 0, 0);
	glScaled(3, 1, 3);
	if (!doingShadows) glColor3f(0.0, 0.0, 0.0);
	for (float j = 0; j < 2.0 * 3.14; j += 0.01f)
	{
		glBegin(GL_LINES);
		glVertex3f(cos(j), 0, sin(j));
		glVertex3f(cos(j), 2, sin(j));
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(21, 3, 0.2);
	glRotatef(90, 1, 0, 0);
	glScaled(3, 1, 3);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(cos(k), 0, sin(k));
		glVertex3f(cos(k + 0.05), 0, sin(k + 0.05));
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(21, 3, 2);
	glRotatef(90, 1, 0, 0);
	glScaled(3, 1, 3);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(cos(k), 0, sin(k));
		glVertex3f(cos(k + 0.05), 0, sin(k + 0.05));
		glEnd();
	}
	glPopMatrix();

	//fourth--------------------------------------
	glPushMatrix();
	glTranslatef(27.45, 6.1, 0.2);
	glRotatef(90, 1, 0, 0);
	glScaled(2.5, 1, 2.5);
	if (!doingShadows) glColor3f(0.0, 0.0, 0.0);
	for (float j = 0; j < 2.0 * 3.14; j += 0.01f)
	{
		glBegin(GL_LINES);
		glVertex3f(cos(j), 0, sin(j));
		glVertex3f(cos(j), 2, sin(j));
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(27.45, 6.1, 0.2);
	glRotatef(90, 1, 0, 0);
	glScaled(2.5, 1, 2.5);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(cos(k), 0, sin(k));
		glVertex3f(cos(k + 0.05), 0, sin(k + 0.05));
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(27.45, 6.1, 2);
	glRotatef(90, 1, 0, 0);
	glScaled(2.5, 1, 2.5);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(cos(k), 0, sin(k));
		glVertex3f(cos(k + 0.05), 0, sin(k + 0.05));
		glEnd();
	}
	glPopMatrix();

	//fifth---------------------------------------
	glPushMatrix();
	glTranslatef(2.3, 6.1, 0.2);
	glRotatef(90, 1, 0, 0);
	glScaled(2.3, 1, 2.3);
	if (!doingShadows) glColor3f(0.0, 0.0, 0.0);
	for (float j = 0; j < 2.0 * 3.14; j += 0.01f)
	{
		glBegin(GL_LINES);
		glVertex3f(cos(j), 0, sin(j));
		glVertex3f(cos(j), 2, sin(j));
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.3, 6.1, 0.2);
	glRotatef(90, 1, 0, 0);
	glScaled(2.3, 1, 2.3);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(cos(k), 0, sin(k));
		glVertex3f(cos(k + 0.05), 0, sin(k + 0.05));
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.3, 6.1, 2);
	glRotatef(90, 1, 0, 0);
	glScaled(2.3, 1, 2.3);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(cos(k), 0, sin(k));
		glVertex3f(cos(k + 0.05), 0, sin(k + 0.05));
		glEnd();
	}
	glPopMatrix();

	glPopMatrix();
}

void DrawObjects::drawCoaster(TrainView* thisTrainView, bool doingShadows){
	glPushMatrix();
	int x = 10;
	int z = 5;
	glTranslated(x, 0, z);
	glRotated(180, 0, 1, 0);
	//interior
	if (!doingShadows)
		glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_QUADS);//base
	glVertex3f(-10, 0.1, -4.9);
	glVertex3f(-10, 0.1, 4.9);
	glVertex3f(10, 0.1, 4.9);
	glVertex3f(10, 0.1, -4.9);
	glEnd();
	glBegin(GL_QUADS);//wall
	glVertex3f(-10, 0, -4.9);
	glVertex3f(-10, 5, -4.9);
	glVertex3f(2, 5, -4.9);
	glVertex3f(10, 0, -4.9);
	glEnd();
	glBegin(GL_QUADS);//wall
	glVertex3f(-10, 0, 4.9);
	glVertex3f(-10, 5, 4.9);
	glVertex3f(2, 5, 4.9);
	glVertex3f(10, 0, 4.9);
	glEnd();

	//exterior
	if (!doingShadows)
		glColor3f(0.8, 0.8, 0.8);
	glBegin(GL_QUADS);//wall
	glVertex3f(-10, 0, -5);
	glVertex3f(-10, 5, -5);
	glVertex3f(2, 5, -5);
	glVertex3f(10, 0, -5);
	glEnd();
	glBegin(GL_QUADS);//wall-seat-edge
	glVertex3f(-4, 5, -5);
	glVertex3f(-4, 7, -5);
	glVertex3f(-3, 7, -5);
	glVertex3f(-19.0 / 7.0, 5, -5);
	glEnd();

	glBegin(GL_QUADS);//wall
	glVertex3f(-10, 0, 5);
	glVertex3f(-10, 5, 5);
	glVertex3f(2, 5, 5);
	glVertex3f(10, 0, 5);
	glEnd();
	glBegin(GL_QUADS);//wall-seat-edge
	glVertex3f(-4, 5, 5);
	glVertex3f(-4, 7, 5);
	glVertex3f(-3, 7, 5);
	glVertex3f(-19.0 / 7.0, 5, 5);
	glEnd();

	glBegin(GL_QUADS);//back
	glVertex3f(-10, 0, 5);
	glVertex3f(-10, 0, -5);
	glVertex3f(-10, 7, -5);
	glVertex3f(-10, 7, 5);
	glEnd();

	glBegin(GL_QUADS);//front
	glVertex3f(10, 0, 5);
	glVertex3f(2, 5, 5);
	glVertex3f(2, 5, -5);
	glVertex3f(10, 0, -5);
	glEnd();

	glBegin(GL_QUADS);//wall--back-seat-edge
	glVertex3f(-10, 5, -5);
	glVertex3f(-10, 7, -5);
	glVertex3f(-9, 7, -5);
	glVertex3f(-61.0 / 7.0, 5, -5);
	glEnd();

	glBegin(GL_QUADS);//wall-back-seat-edge
	glVertex3f(-10, 5, 5);
	glVertex3f(-10, 7, 5);
	glVertex3f(-9, 7, 5);
	glVertex3f(-61.0 / 7.0, 5, 5);
	glEnd();

	glBegin(GL_QUADS);//wall-back-seat-edge
	glVertex3f(-10, 5, -5);
	glVertex3f(-10, 7, -5);
	glVertex3f(-9, 7, -5);
	glVertex3f(-61.0 / 7.0, 5, -5);
	glEnd();

	glBegin(GL_QUADS);//wall-back-seat-edge
	glVertex3f(-10, 5, 5);
	glVertex3f(-10, 7, 5);
	glVertex3f(-9, 7, 5);
	glVertex3f(-61.0 / 7.0, 5, 5);
	glEnd();

	//seats
	if (!doingShadows)
		glColor3f(0.2, 0.2, 0.2);
	glBegin(GL_QUADS);//seat-back
	glVertex3f(-4, 0, 5);
	glVertex3f(-4, 7, 5);
	glVertex3f(-4, 7, -5);
	glVertex3f(-4, 0, -5);
	glEnd();
	glBegin(GL_QUADS);//seat-top
	glVertex3f(-4, 7, 5);
	glVertex3f(-4, 7, -5);
	glVertex3f(-3, 7, -5);
	glVertex3f(-3, 7, 5);
	glEnd();
	glBegin(GL_QUADS);//seat-seat
	glVertex3f(-3, 7, 5);
	glVertex3f(-3, 7, -5);
	glVertex3f(-2, 0, -5);
	glVertex3f(-2, 0, 5);
	glEnd();



	glBegin(GL_QUADS);//seat-top
	glVertex3f(-10, 7, 5);
	glVertex3f(-10, 7, -5);
	glVertex3f(-9, 7, -5);
	glVertex3f(-9, 7, 5);
	glEnd();
	glBegin(GL_QUADS);//seat-seat
	glVertex3f(-9, 7, 5);
	glVertex3f(-9, 7, -5);
	glVertex3f(-8, 0, -5);
	glVertex3f(-8, 0, 5);
	glEnd();

	glPopMatrix();
}

void DrawObjects::surfRevlution(bool doingShadows){

	glPushMatrix();
	glTranslated(-45, 0, -45);
	float k=0,t=0;
	for (float i = 0; i < 7; i += 0.01){
		if (!doingShadows) glColor3f(0.986, 0.584-k, 0.49+k);
		for (double k = 0; k < 2 * 3.14; k += 0.01){
			glBegin(GL_QUAD_STRIP);
			glVertex3f(i * sin(k), 0.2*i*i, i* cos(k));
			float i1 = i + 0.01;
			glVertex3f(i1 * sin(k), 0.2*i1*i1, i1* cos(k));
			glEnd();
		}
		//change color
		t += 0.01;
		if (t-1>0){
			t = 0;
			k += 0.03;
		}
	}
	glPopMatrix();

	glPushMatrix();
	glTranslated(-45, 0, -45);
	float k1 = 0, t1 = 0;
	for (float i = 0; i < 7; i += 0.01){
		if (!doingShadows) glColor3f(0.986, 0.584 - k1, 0.49 + k1);
		for (double k = 0; k < 2 * 3.14; k += 0.01){
			glBegin(GL_POINTS);
			glVertex3f(i * sin(k), 19.6 - 0.2*i*i, i* cos(k));
			glEnd();
		}
		//change color
		t1 += 0.01;
		if (t1 - 1>0){
			t1 = 0;
			k1 += 0.03;
		}
	}
	glPopMatrix();

}

void DrawObjects::drawBillboard(TrainView* thisTrainView, bool doingShadows) {

	//opengl
	glColor3f(1.0f, 1.0f, 1.0f);

	glPushMatrix();
	GLUquadric* cylQuad = gluNewQuadric();

	glTranslatef(180, 0, -180);
	glRotatef(-90, 1, 0, 0);
	gluCylinder(cylQuad, 2.0, 2.0, 60, 100, 100);

	glTranslatef(-60, 0, 0);
	gluCylinder(cylQuad, 2.0, 2.0, 60, 100, 100);

	glPopMatrix();


	
	glEnable(GL_TEXTURE_2D);
	fetchTexture("ilikeit.jpg", false, false);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);
	
	glTexCoord2f(1.0, 0.0);
	glVertex3f(180.0, 20.0, -180.0);
	
	glTexCoord2f(1.0, 1.0);
	glVertex3f(180.0, 60.0, -180.0);
	
	glTexCoord2f(0.0, 1.0);
	glVertex3f(120.0, 60.0, -180.0);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(120.0, 20.0, -180.0);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	//i like it
	glColor3f(1.0f, 1.0f, 1.0f);

	glPushMatrix();
	GLUquadric* like = gluNewQuadric();

	glTranslatef(100, 0, -180);
	glRotatef(-90, 1, 0, 0);
	gluCylinder(like, 2.0, 2.0, 60, 100, 100);

	glTranslatef(-60, 0, 0);
	gluCylinder(like, 2.0, 2.0, 60, 100, 100);

	glPopMatrix();
	

	
	glEnable(GL_TEXTURE_2D);
	fetchTexture("art.jpg", false, false);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(100.0, 20.0, -180.0);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(100.0, 60.0, -180.0);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(40.0, 60.0, -180.0);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(40.0, 20.0, -180.0);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	//This is art

	glColor3f(1.0f, 1.0f, 1.0f);

	glPushMatrix();
	GLUquadric* art = gluNewQuadric();

	glTranslatef(20, 0, -180);
	glRotatef(-90, 1, 0, 0);
	gluCylinder(art, 2.0, 2.0, 60, 100, 100);

	glTranslatef(-60, 0, 0);
	gluCylinder(art, 2.0, 2.0, 60, 100, 100);

	glPopMatrix();



	glEnable(GL_TEXTURE_2D);
	fetchTexture("opengl.jpg", false, false);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(20.0, 20.0, -180.0);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(20.0, 60.0, -180.0);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(-40.0, 60.0, -180.0);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(-40.0, 20.0, -180.0);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void DrawObjects::cubes(){

	glBegin(GL_QUADS);
	//front
	glNormal3f(0,0,1);
	glVertex3f(0, 4, 0);
	glVertex3f(0, 14, 0);
	glVertex3f(0, 14, 10);
	glVertex3f(0, 4, 10);

	// back
	glNormal3f(0,0,-1);
	glVertex3f(30, 4, 0);
	glVertex3f(30, 14, 0);
	glVertex3f(30, 14, 10);
	glVertex3f(30, 4, 10);

	// top
	glNormal3f(0,1,0);
	glVertex3f(0, 14, 0);
	glVertex3f(30, 14, 0);
	glVertex3f(30, 14, 10);
	glVertex3f(0, 14, 10);

	// bottom
	glNormal3f(0,-1,0);
	glVertex3f(0, 4, 0);
	glVertex3f(30, 4, 0);
	glVertex3f(30, 4, 10);
	glVertex3f(0, 4, 10);

	// left
	glNormal3f(-1,0,0);
	glVertex3f(0, 4, 0);
	glVertex3f(0, 14, 0);
	glVertex3f(30, 14, 0);
	glVertex3f(30, 4, 0);

	// right
	glNormal3f(1,0,0);
	glVertex3f(0, 4, 10);
	glVertex3f(0, 14, 10);
	glVertex3f(30, 14, 10);
	glVertex3f(30, 4, 10);
	glEnd();
	
	//this is how we control the color of this world right...
	//Throught a long I finally realize the reason all kinds of things change color automatically 
	//The normal of a surface is important,so We need to set it correctlly
	//However I don't have time to add all the normal to the original surface, so I used this tricky way
	glNormal3f(0, 1, 0);

}

void DrawObjects::drawSkybox(bool draw) {
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_BACK, GL_LINE);
	glColorMaterial(GL_BACK, GL_AMBIENT_AND_DIFFUSE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	int skyboxSize = 1000;
	for (int face = 0; face < 4; face++){
		switch (face){
		case 3: fetchTexture("iceflats_ft.tga", false, false); break;
		case 2: fetchTexture("iceflats_rt.tga", false, false); break;
		case 1: fetchTexture("iceflats_bk.tga", false, false); break;
		case 0: fetchTexture("iceflats_lf.tga", false, false); break;
		}
		glPushMatrix();
		glRotatef(90 * face, 0, 1, 0);

		glBegin(GL_QUADS);

		glTexCoord2f(1.0, 1.0);
		glVertex3f(-skyboxSize, skyboxSize / 2, -skyboxSize - 2);

		glTexCoord2f(0.0, 1.0);
		glVertex3f(-skyboxSize, skyboxSize / 2, skyboxSize + 2);

		glTexCoord2f(0.0, 0.0);
		glVertex3f(-skyboxSize, skyboxSize / -2, skyboxSize + 2);

		glTexCoord2f(1.0, 0.0);
		glVertex3f(-skyboxSize, skyboxSize / -2, -skyboxSize - 2);

		glEnd();
		glPopMatrix();
	}

	int floorboxSize = skyboxSize + 2;
	//up
	fetchTexture("iceflats_up.tga", false, false);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(floorboxSize, skyboxSize / 2 - 2, -floorboxSize);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(floorboxSize, skyboxSize / 2 - 2, floorboxSize);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-floorboxSize, skyboxSize / 2 - 2, floorboxSize);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-floorboxSize, skyboxSize / 2 - 2, -floorboxSize);
	glEnd();

	//dn
	fetchTexture("iceflats_dn.tga", false, false);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-floorboxSize, skyboxSize / -2 + 2, -floorboxSize);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-floorboxSize, skyboxSize / -2 + 2, floorboxSize);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(floorboxSize, skyboxSize / -2 + 2, skyboxSize);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(floorboxSize, skyboxSize / -2 + 2, -floorboxSize);
	glEnd();

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_TEXTURE_2D);

}

void DrawObjects::flag(float flagColor, float flagShape, bool doingShadows){
	//SYSTEMTIME sysTime;
	//GetSystemTime(&sysTime);
	glNormal3f(0, 1, 0);

	glPushMatrix();
	glTranslatef(-60,0,-90);
	glScalef(2, 2, 2);
	for (float j = 0; j < 2. * 3.14; j += 0.01f)
	{
		glBegin(GL_LINES);
		if (!doingShadows) glColor3f(1, 0.749, 0.49);
		glVertex3f(cos(j), 0, sin(j));
		glVertex3f(cos(j), 25, sin(j));
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-60,25,-90);
	glScalef(2, 2, 2);
	glBegin(GL_LINES);
	if (!doingShadows) glColor3f(1, 0.749, 0.49);
	for (float i = 0; i < 2 * 3.14; i += 0.01){
		glVertex3f(0, 0, 0);
		glVertex3f(cos(i), 0, sin(i));
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-60, 0, -90);
	glScalef(2, 2, 2);
	for (float j = 0; j < 10; j += 0.01f)
	{
		if (!doingShadows) glColor3f(0.870, 0.246 + flagColor, 0.258);
		glBegin(GL_LINES);
		glVertex3f(j, 17, sin(j - flagShape));
		glVertex3f(j, 25, sin(j - flagShape));
		glEnd();
	}
	glPopMatrix();
}

void DrawObjects::drawPlatform(TrainView* thisTrainView, bool doingShadows) {


	glColor3f(0.5f, 0.5f, 0.5f);

	glEnable(GL_TEXTURE_2D);
	fetchTexture("concrete.jpg", false, false);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, NULL);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(200.0, 0.0, 200.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(200.0, 0.0, -200.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-200.0, 0.0, -200.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-200.0, 0.0, 200.0);
	glEnd();
	
	for (int side = 0; side < 4; side++){
	glPushMatrix();
	glRotated(side * 90, 0, 1, 0);
		fetchTexture("concrete-slope.jpg", false, false);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-200.0, 0.0, -200.0);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(200.0, 0.0, -200.0);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(200.0, -10.0, -220.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-200.0, -10.0, -220.0);
		glEnd();
		fetchTexture("concrete-edge.jpg", false, false);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-200.0, -10.0, -220.0);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(200.0, -10.0, -220.0);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(200.0, -100.0, -220.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-200.0, -100.0, -220.0);
		glEnd();
		fetchTexture("concrete-slope.jpg", false, false);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-200.0, -100.0, -220.0);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(200.0, -100.0, -220.0);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(200.0, -110.0, -200.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-200.0, -110.0, -200.0);
		glEnd();

		fetchTexture("concrete-corner.jpg", false, false);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-200.0, 0.0, -200.0);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-200.0, -10.0, -220.0);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-220.0, -10.0, -200.0);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-200.0, 0.0, -200.0);
		glEnd();
		fetchTexture("concrete-corner-edge.jpg", false, false);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-200.0, -10.0, -220.0);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-200.0, -100.0, -220.0);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-220.0, -100.0, -200.0);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-220.0, -10.0, -200.0);
		glEnd();
		fetchTexture("concrete-corner.jpg", false, false);
		glBegin(GL_QUADS);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-200.0, -100.0, -220.0);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-200.0, -110.0, -200.0);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-200.0, -110.0, -200.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-220.0, -100.0, -200.0);
		glEnd();


		//propellers
		glPushMatrix();
		GLUquadric* cylQuad = gluNewQuadric();

		glTranslatef(190, -115, 190);
		glPushMatrix();
			glTranslated(0, 10, 0);
			glRotated(90, 1, 0, 0);
			gluCylinder(cylQuad, 5.0, 5.0, 10, 100, 100);
		glPopMatrix();
		SYSTEMTIME sysTime;
		GetSystemTime(&sysTime);
		double time = sysTime.wMilliseconds;
		if (thisTrainView->tw->runButton->value())
			glRotated(360.0 / 1000.0*time, 0, 1, 0);

		for (int fan = 0; fan < 4; fan++){
			glPushMatrix();
			glRotated(90 * fan, 0, 1, 0);
			glBegin(GL_QUADS);
			glVertex3f(0, 0, 0);
			glVertex3f(100, 0, 10);
			glVertex3f(110, 0, 0);
			glVertex3f(100, 0, -10);
			glEnd();
			glPopMatrix();
		}
		glPopMatrix();
	glPopMatrix();
	}

	fetchTexture("concrete.jpg", false, false);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(200.0, -110.0, 200.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(200.0, -110.0, -200.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-200.0, -110.0, -200.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-200.0, -110.0, 200.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

}

void DrawObjects::drawJet(TrainView* thisTrainView, bool doingShadows){
	//draw exaust
	glPushMatrix();
		glTranslated(0, -110, 0);
		glRotated(90, 1, 0, 0);
		glColor3d(0, 0, 0);
		GLUquadric* cylQuad = gluNewQuadric();
		gluCylinder(cylQuad, 35.0, 35.0, 10, 100, 100);
	glPopMatrix();

	double vertices[140][360][2];
	double height = 140;
	double points_in_rung = 360;
	double PI = 3.14159265;

	//generate the points
	for (int rungs = 0; rungs < height; rungs++){
		double radius = (-0.8*(rungs - 60.0)*(rungs - 60.0) + 5000.0) / 100.0; // r = (-0.8*(x-60)^2 +5000)/100
		for (int point = 0; point < points_in_rung; point++){
			double x = 1.0;
			double z = 1.0;
			x = radius * cos(point*PI / 180.0);
			z = radius*sin(point*PI / 180.0);
			vertices[rungs][point][0] = x;
			vertices[rungs][point][1] = z;
		}
	}
	glPushMatrix();
	glTranslated(0,-110,0);

	
	int lastStepHeight = 0;
	//draw the points
	for (int rung = 0; rung < height; rung+=1){
		double drung = rung;
		glBegin(GL_LINE_STRIP);
		//rgba(98, 101, 173, 0.62):0 -> rgba(255, 210, 142, 1):100
		double r = 98;
		double g = 101;
		double b = 173;
		double a = 0.32;
		if (rung != 0){ //gradient
			r += (255 - 98) * (drung/height);
			g += (210 - 101) * (drung/height);
			b += (142 - 173) * (drung/height);
			a += (1 - 0.32) * (drung/height);
		}
		glColor4d(r/255, g/255, b/255, a);

		//calculate wigglness
		double variability = 0;
		if (thisTrainView->tw->runButton->value())
			variability = ((double)(rand() % 10 + 10)) / 100.0;
		//shift in y, accounts for wigglness and opacity of bluer section
		lastStepHeight += 1.0 / a + variability;
		//draw rung
		for (int point = 0; point < points_in_rung; point++){
			glVertex3d(vertices[rung][point][0],-lastStepHeight, vertices[rung][point][1]);//-drung/2.0*variability
		}
		glEnd();
	}
	glPopMatrix();
}

void DrawObjects::ShaderCube(TrainView* thisTrainView, bool doingShadows){
	
	static unsigned int cube = 0;
	if (!thisTrainView->world->cubeShader){
		char* error;
		if (!(cube = loadShader("shaders/cube.vert", "shaders/cube.frag", error))) {
			std::string s = "Can't Load Shader:";
			s += error;
			fl_alert(s.c_str());
		}
		thisTrainView->world->cubeShader = !thisTrainView->world->cubeShader;
	}
	glBindAttribLocation(cube, 0, "position");
	glBindAttribLocation(cube, 1, "vertexColor");

	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,1,0,0,
		0.5f, -0.5f, -0.5f, 0,1,0,
		0.5f, 0.5f, -0.5f, 1,0,0,
		0.5f, 0.5f, -0.5f, 1,0,0,
		-0.5f, 0.5f, -0.5f, 0,1,0,
		-0.5f, -0.5f, -0.5f, 1,0,0,

		-0.5f, -0.5f, 0.5f, 1,0,0,
		0.5f, -0.5f, 0.5f, 0,1,0,
		0.5f, 0.5f, 0.5f, 0,0,1,
		0.5f, 0.5f, 0.5f, 0,0,1,
		-0.5f, 0.5f, 0.5f, 0,1,0,
		-0.5f, -0.5f, 0.5f, 1,0,0,

		-0.5f, 0.5f, 0.5f, 1,0,0,
		-0.5f, 0.5f, -0.5f, 0,1,0,
		-0.5f, -0.5f, -0.5f, 0,0,1,
		-0.5f, -0.5f, -0.5f, 0,0,1
		-0.5f, -0.5f, 0.5f, 0,1,0,
		-0.5f, 0.5f, 0.5f, 1,0,0,

		0.5f, 0.5f, 0.5f, 1,0,0,
		0.5f, 0.5f, -0.5f, 0,1,0,
		0.5f, -0.5f, -0.5f, 0,0,1,
		0.5f, -0.5f, -0.5f, 0,0,1,
		0.5f, -0.5f, 0.5f, 0,1,0,
		0.5f, 0.5f, 0.5f, 1,0,0,

		-0.5f, -0.5f, -0.5f, 1,0,0,
		0.5f, -0.5f, -0.5f, 0,1,0,
		0.5f, -0.5f, 0.5f, 0,0,1,
		0.5f, -0.5f, 0.5f,0,0,1,
		-0.5f, -0.5f, 0.5f,0,1,0,
		-0.5f, -0.5f, -0.5f, 1,0,0,

		-0.5f, 0.5f, -0.5f, 1,0,0,
		0.5f, 0.5f, -0.5f, 0,1,0,
		0.5f, 0.5f, 0.5f, 0,0,1,
		0.5f, 0.5f, 0.5f, 0,0,1,
		-0.5f, 0.5f, 0.5f,0,1,0,
		-0.5f, 0.5f, -0.5f,1,0,0
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO

	glUseProgram(cube);

	// Camera/View transformation
	glm::mat4 view;
	view = glm::translate(view,glm::vec3(-thisTrainView->arcball.eyeX, -thisTrainView->arcball.eyeY, -thisTrainView->arcball.eyeZ));
	// Projection 
	glm::mat4 projection;
	//projection = glm::perspective(arcball.fieldOfView, (GLfloat)w() / (GLfloat)h(), 0.1f, 1000.0f);
	projection = glm::perspective(thisTrainView->arcball.fieldOfView, (GLfloat)thisTrainView->w() / (GLfloat)thisTrainView->h(), 0.1f, 3000.0f);

	// Rotate
	glm::mat4 model;

	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++)
			model[i][j] = thisTrainView->arcball.rotateMatrix[i][j];
	}
	model = glm::translate(model, glm::vec3(60, 20, -130));
	model = glm::scale(model, glm::vec3(20, 20, 20));
	
	

	// Get their uniform location
	GLint modelLoc = glGetUniformLocation(cube, "model");
	GLint viewLoc = glGetUniformLocation(cube, "view");
	GLint projLoc = glGetUniformLocation(cube, "projection");
	// Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// Draw container
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glUseProgram(0);
	

/*
	static unsigned int cube = 0;
	char* error;
	if (!(cube = loadShader("shaders/cube.vert", "shaders/cube.frag", error))) {
		std::string s = "Can't Load Shader:";
		s += error;
		fl_alert(s.c_str());
	}

	glBindAttribLocation(cube, 0, "position");

	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO


	// Load and create a texture 
	GLuint texture1;
	GLuint texture2;
	// ====================
	// Texture 1
	// ====================
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image("textures/front.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	// ===================
	// Texture 2
	// ===================
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	image = SOIL_load_image("textures/front.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glUniform1i(glGetUniformLocation(cube, "ourTexture1"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(cube, "ourTexture2"), 1);

	glUseProgram(cube);

	// Camera/View transformation
	glm::mat4 view;
	view = glm::translate(view, glm::vec3(-thisTrainView->arcball.eyeX, -thisTrainView->arcball.eyeY, -thisTrainView->arcball.eyeZ));
	// Projection 
	glm::mat4 projection;
	//projection = glm::perspective(arcball.fieldOfView, (GLfloat)w() / (GLfloat)h(), 0.1f, 1000.0f);
	projection = glm::perspective(thisTrainView->arcball.fieldOfView, (GLfloat)thisTrainView->w() / (GLfloat)thisTrainView->h(), 0.1f, 2000.0f);

	// Rotate
	glm::mat4 model;
	//model = glm::translate(model, glm::vec3(-10, 20, -20));
	
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++)
			model[i][j] = thisTrainView->arcball.rotateMatrix[i][j];
	}
	model = glm::scale(model, glm::vec3(10, 10, 10));

	// Get their uniform location
	GLint modelLoc = glGetUniformLocation(cube, "model");
	GLint viewLoc = glGetUniformLocation(cube, "view");
	GLint projLoc = glGetUniformLocation(cube, "projection");
	// Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	
	// Draw container
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	*/
}