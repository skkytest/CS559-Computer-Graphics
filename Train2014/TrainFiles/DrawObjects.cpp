#include <math.h>
#include "TrainView.H"
#include "TrainWindow.H"

// we will need OpenGL, and OpenGL needs windows.h
#include <windows.h>
#include "GL/gl.h"
#include "GL/glu.h"

#include"DrawObjects.h"
#include "../Utilities/Texture.H"

//draw trees
void DrawObjects::drawTrees(TrainView* thisTrainView, bool doingShadows){
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

void DrawObjects::surfRevlution(TrainView* thisTrainView, bool doingShadows){

	glPushMatrix();
	glTranslated(-45, 0, -45);
	float k=0,t=0;
	for (float i = 0; i < 7; i += 0.01){
		if (!doingShadows) glColor3f(0.986, 0.584-k, 0.49+k);
		for (double k = 0; k < 2 * 3.14; k += 0.01){
			glBegin(GL_POINTS);
			glVertex3f(i * sin(k), 0.2*i*i, i* cos(k));
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
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	fetchTexture("opengl.jpg", false, false);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, 0.0, -80.0);
	
	glTexCoord2f(1.0, 0.0);
	glVertex3f(64.0, 0.0, -80.0);
	
	glTexCoord2f(1.0, 1.0);
	glVertex3f(64.0, 64.0, -80.0);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, 64.0, -80.0);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}