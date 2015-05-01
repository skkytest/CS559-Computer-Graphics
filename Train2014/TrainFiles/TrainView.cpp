// CS559 Train Project
// TrainView class implementation
// see the header for details
// look for TODO: to see things you want to add/change
// 
#include <math.h>
#include "TrainView.H"
#include "TrainWindow.H"

#include "Utilities/3DUtils.H"

#include <Fl/fl.h>

// we will need OpenGL, and OpenGL needs windows.h
#include <windows.h>
#include "GL/gl.h"
#include "GL/glu.h"

#ifdef EXAMPLE_SOLUTION
#include "TrainExample/TrainExample.H"
#endif


TrainView::TrainView(int x, int y, int w, int h, const char* l) : Fl_Gl_Window(x,y,w,h,l)
{
	mode( FL_RGB|FL_ALPHA|FL_DOUBLE | FL_STENCIL );

	resetArcball();
}

void TrainView::resetArcball()
{
	// set up the camera to look at the world
	// these parameters might seem magical, and they kindof are
	// a little trial and error goes a long way
	arcball.setup(this,40,250,.2f,.4f,0);
}

// FlTk Event handler for the window
// TODO: if you want to make the train respond to other events 
// (like key presses), you might want to hack this.
int TrainView::handle(int event)
{
	// see if the ArcBall will handle the event - if it does, then we're done
	// note: the arcball only gets the event if we're in world view
	if (tw->worldCam->value())
		if (arcball.handle(event)) return 1;

	// remember what button was used
	static int last_push;

	switch(event) {
		case FL_PUSH:
			last_push = Fl::event_button();
			if (last_push == 1) {
				doPick();
				damage(1);
				return 1;
			};
			break;
		case FL_RELEASE:
			damage(1);
			last_push=0;
			return 1;
		case FL_DRAG:
			if ((last_push == 1) && (selectedCube >=0)) {
				ControlPoint* cp = &world->points[selectedCube];

				double r1x, r1y, r1z, r2x, r2y, r2z;
				getMouseLine(r1x,r1y,r1z, r2x,r2y,r2z);

				double rx, ry, rz;
				mousePoleGo(r1x,r1y,r1z, r2x,r2y,r2z, 
						  static_cast<double>(cp->pos.x), 
						  static_cast<double>(cp->pos.y),
						  static_cast<double>(cp->pos.z),
						  rx, ry, rz,
						  (Fl::event_state() & FL_CTRL) != 0);
				cp->pos.x = (float) rx;
				cp->pos.y = (float) ry;
				cp->pos.z = (float) rz;
				damage(1);
			}
			break;
			// in order to get keyboard events, we need to accept focus
		case FL_FOCUS:
			return 1;
		case FL_ENTER:	// every time the mouse enters this window, aggressively take focus
				focus(this);
				break;
		case FL_KEYBOARD:
		 		int k = Fl::event_key();
				int ks = Fl::event_state();
				if (k=='p') {
					if (selectedCube >=0) 
						printf("Selected(%d) (%g %g %g) (%g %g %g)\n",selectedCube,
							world->points[selectedCube].pos.x,world->points[selectedCube].pos.y,world->points[selectedCube].pos.z,
							world->points[selectedCube].orient.x,world->points[selectedCube].orient.y,world->points[selectedCube].orient.z);
					else
						printf("Nothing Selected\n");
					return 1;
				};
				break;
	}

	return Fl_Gl_Window::handle(event);
}

// this is the code that actually draws the window
// it puts a lot of the work into other routines to simplify things
void TrainView::draw()
{

	glViewport(0,0,w(),h());

	// clear the window, be sure to clear the Z-Buffer too
	//glClearColor(0,0,.3f,0);		// background should be blue
	glClearColor(0.529, 0.808, 0.922, 0);

	// we need to clear out the stencil buffer since we'll use
	// it for shadows
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH);

	// Blayne prefers GL_DIFFUSE
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// prepare for projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	setProjection();		// put the code to set up matrices here

	// TODO: you might want to set the lighting up differently
	// if you do, 
	// we need to set up the lights AFTER setting up the projection

	// enable the lighting
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	// top view only needs one light
	if (tw->topCam->value()) {
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
	} else {
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
	}
	// set the light parameters
	GLfloat lightPosition1[] = {0,1,1,0}; // {50, 200.0, 50, 1.0};
	GLfloat lightPosition2[] = {1, 0, 0, 0};
	GLfloat lightPosition3[] = {0, -1, 0, 0};
	GLfloat yellowLight[] = {0.5f, 0.5f, .1f, 1.0};
	GLfloat whiteLight[] = {1.0f, 1.0f, 1.0f, 1.0};
	GLfloat blueLight[] = {.1f,.1f,.3f,1.0};
	GLfloat grayLight[] = {.3f, .3f, .3f, 1.0};

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, grayLight);

	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, yellowLight);

	glLightfv(GL_LIGHT2, GL_POSITION, lightPosition3);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, blueLight);



	// now draw the ground plane
	setupFloor();
	glDisable(GL_LIGHTING);
	drawFloor(200,10);
	glEnable(GL_LIGHTING);
	setupObjects();

	// we draw everything twice - once for real, and then once for
	// shadows
	drawStuff();

	// this time drawing is for shadows (except for top view)
	if (!tw->topCam->value()) {
		setupShadows();
		drawStuff(true);
		unsetupShadows();
	}
	
}

// note: this sets up both the Projection and the ModelView matrices
// HOWEVER: it doesn't clear the projection first (the caller handles
// that) - its important for picking
void TrainView::setProjection()
{
	// compute the aspect ratio (we'll need it)
	float aspect = static_cast<float>(w()) / static_cast<float>(h());

	if (tw->worldCam->value())
		arcball.setProjection(false);
	else if (tw->topCam->value()) {
		float wi,he;
		if (aspect >= 1) {
			wi = 110;
			he = wi/aspect;
		} else {
			he = 110;
			wi = he*aspect;
		}
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-wi,wi,-he,he,-200,200);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(90,1,0,0);
	}
	else {
		// TODO: put code for train view projection here!
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45, aspect, 0.1, 1000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(this->world->xaxis, this->world->yaxis + 5, this->world->zaxis, this->world->viewx, this->world->viewy + 5, this->world->viewz, 0, 1, 0);
	}
}

// this draws all of the stuff in the world
// NOTE: if you're drawing shadows, DO NOT set colors 
// (otherwise, you get colored shadows)
// this gets called twice per draw - once for the objects, once for the shadows
// TODO: if you have other objects in the world, make sure to draw them
void TrainView::drawStuff(bool doingShadows)
{
	// draw the control points
	// don't draw the control points if you're driving 
	// (otherwise you get sea-sick as you drive through them)
	if (!tw->trainCam->value()) {
		for(size_t i=0; i<world->points.size(); ++i) {
			if (!doingShadows) {
				if ( ((int) i) != selectedCube)
					glColor3ub(240,60,60);
				else
					glColor3ub(240,240,30);
			}
			world->points[i].draw();
		}
	}
	// draw the track
	// TODO: call your own track drawing code
#ifdef EXAMPLE_SOLUTION
	drawTrack(this, doingShadows);
#endif
	
	drawTrack(this, doingShadows);
	
	//draw my trees
	drawTrees(this, doingShadows);

	// draw the train
	// TODO: call your own train drawing code
#ifdef EXAMPLE_SOLUTION
	// don't draw the train if you're looking out the front window
	if (!tw->trainCam->value())
		drawTrain(this, doingShadows);
#endif
	if (!tw->trainCam->value()){
		if (this->world->continuity == 1){
			glPushMatrix();
			glTranslatef(this->world->xaxis, this->world->yaxis, this->world->zaxis);
			glRotatef(this->world->angle, 0, 1, 0);
			glRotatef(-this->world->heightAngle, 0, 0, 1);
			glTranslatef(-15, 0, -5);
			if (this->world->model == 2){
				drawTrain(this, doingShadows);
			}
			if (this->world->model == 1){
				drawTank(this, doingShadows);
			}
			glPopMatrix();
		}
		if (this->world->continuity == 2){
			glPushMatrix();
			glTranslatef(this->world->xaxis, this->world->yaxis, this->world->zaxis);
			glRotatef(this->world->angle, 0, 1, 0);
			glRotatef(-this->world->heightAngle, 0, 0, 1);
			glTranslatef(-15, 0, -5);
			if (this->world->model == 2){
				drawTrain(this, doingShadows);
			}
			if (this->world->model == 1){
				drawTank(this, doingShadows);
			}
			glPopMatrix();
		}
		if (this->world->continuity == 3){
			glPushMatrix();
			glTranslatef(this->world->xaxis, this->world->yaxis, this->world->zaxis);
			glRotatef(this->world->angle, 0, 1, 0);
			glRotatef(-this->world->heightAngle, 0, 0, 1);
			glTranslatef(-15, 0, -5);
			if (this->world->model == 2){
				drawTrain(this, doingShadows);
			}
			if (this->world->model == 1){
				drawTank(this, doingShadows);
			}
			glPopMatrix();
		}
	}
}

//draw trees
void TrainView::drawTrees(TrainView* thisTrainView, bool doingShadows)
{
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
		glVertex3f(10*cos(j), 10, 10*sin(j));
		glVertex3f(0,30,0);
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

//draw track
void TrainView::drawTrack(TrainView* thisTrainView, bool doingShadows){
	
	//draw track with c0 coutinuity
	if (thisTrainView->world->continuity == 1){
		glPushMatrix();
		for (int i = 0; i < (world->points.size() ); i++){

			float presentX = world->points[i].pos.x;
			float presentY = world->points[i].pos.y;
			float presentZ = world->points[i].pos.z;
			int p = i + 1;
			if (i == (world->points.size()) - 1) p = 0;
			float nextX = world->points[p].pos.x;
			float nextY = world->points[p].pos.y;
			float nextZ = world->points[p].pos.z;
			glBegin(GL_LINES);
			if (!doingShadows) glColor3f(0.867, 0.427, 0.133);
			glVertex3f(presentX, presentY, presentZ);
			glVertex3f(nextX, nextY, nextZ);
			glEnd();

			double length = sqrt((presentX - nextX)*(presentX - nextX) + (presentZ - nextZ)*(presentZ - nextZ));
			double xbias, ybias,zbias;
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


			if (this->world->trackType == 2){
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
						      [-1  3 -3  1][P(0)]				
						      [ 2 -5  4 -1][P(1)]
	p(t) = 1/2 * [t^3 t^2 t 1][-1  0  1  0][P(2)]
						      [ 0  2  0  0][P(3)]
	
	P(1) and P(2) are present and next control points
	*/
	
	if (thisTrainView->world->continuity == 2){
		glPushMatrix();
		if (!doingShadows) glColor3f(0.867, 0.427, 0.133);
		for (int i = 0; i < (world->points.size()); i++){
			int p;
			//P[0]
			if (i == 0) p = world->points.size() - 1;
			else p = i - 1;
			float p0x = world->points[p].pos.x;
			float p0y = world->points[p].pos.y;
			float p0z = world->points[p].pos.z;
			//p[1]
			float p1x = world->points[i].pos.x;
			float p1y = world->points[i].pos.y;
			float p1z = world->points[i].pos.z;
			//P[2]
			p = (i + 1) % (world->points.size());
			float p2x = world->points[p].pos.x;
			float p2y = world->points[p].pos.y;
			float p2z = world->points[p].pos.z;
			//P[3]
			p = (i + 2) % (world->points.size());
			float p3x = world->points[p].pos.x;
			float p3y = world->points[p].pos.y;
			float p3z = world->points[p].pos.z;

			//calculate distance between P1 and P2
			float length = sqrt((p1x - p2x)*(p1x - p2x) + (p1y - p2y)*(p1y - p2y) + (p1z - p2z)*(p1z - p2z));

			//calculate the matrix
			float block=0.0; // use this parameter to calculate where I need to put a wood of track in the world
							 // because when you prolong the track the ratio of distance may change, so we need to figure out a way to fix it
			//record where the last wood is
			float lastBlockX = p1x;
			float lastBlockY = p1y;
			float lastBlockZ = p1z;

			for (float t = 0; t < 1; t += 0.001){
				float px, py, pz;
				px = 0.5*(((-1)*p0x + 3 * p1x + (-3)*p2x + p3x)*t*t*t + (2 * p0x - 5 * p1x + 4 * p2x - p3x)*t*t + ((-1)*p0x + p2x)*t + 2 * p1x);
				py = 0.5*(((-1)*p0y + 3 * p1y + (-3)*p2y + p3y)*t*t*t + (2 * p0y - 5 * p1y + 4 * p2y - p3y)*t*t + ((-1)*p0y + p2y)*t + 2 * p1y);
				pz = 0.5*(((-1)*p0z + 3 * p1z + (-3)*p2z + p3z)*t*t*t + (2 * p0z - 5 * p1z + 4 * p2z - p3z)*t*t + ((-1)*p0z + p2z)*t + 2 * p1z);

				glBegin(GL_POINTS);

				glVertex3f(px, py, pz);
				glEnd();

				if (this->world->trackType == 3){
					float p = t + 0.001;
					float tx, ty, tz;
					tx = 0.5*(((-1)*p0x + 3 * p1x + (-3)*p2x + p3x)*p*p*p + (2 * p0x - 5 * p1x + 4 * p2x - p3x)*p*p + ((-1)*p0x + p2x)*p + 2 * p1x);
					ty = 0.5*(((-1)*p0y + 3 * p1y + (-3)*p2y + p3y)*p*p*p + (2 * p0y - 5 * p1y + 4 * p2y - p3y)*p*p + ((-1)*p0y + p2y)*p + 2 * p1y);
					tz = 0.5*(((-1)*p0z + 3 * p1z + (-3)*p2z + p3z)*p*p*p + (2 * p0z - 5 * p1z + 4 * p2z - p3z)*p*p + ((-1)*p0z + p2z)*p + 2 * p1z);
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

					if (this->world->trackType == 2){

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

		for (int i = 0; i < (world->points.size()); i++){
			int number = world->points.size();
			//P[0]
			if (i == number) i = 0;
			float p0x = world->points[i%number].pos.x;
			float p0y = world->points[i%number].pos.y;
			float p0z = world->points[i%number].pos.z;
			//p[1]
			float p1x = world->points[(i + 1) % number].pos.x;
			float p1y = world->points[(i + 1) % number].pos.y;
			float p1z = world->points[(i + 1) % number].pos.z;
			//P[2]
			float p2x = world->points[(i + 2) % number].pos.x;
			float p2y = world->points[(i + 2) % number].pos.y;
			float p2z = world->points[(i + 2) % number].pos.z;
			//P[3]
			float p3x = world->points[(i + 3) % number].pos.x;
			float p3y = world->points[(i + 3) % number].pos.y;
			float p3z = world->points[(i + 3) % number].pos.z;


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

					if (this->world->trackType == 2){

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

// drwa train
void TrainView::drawTrain(TrainView* thisTrainView, bool doingShadows){
	
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
				glVertex3f(cos(j),0,sin(j));
				glVertex3f(cos(j),6,sin(j));
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

void TrainView::drawTank(TrainView* thisTrainView, bool doingShadows){
	glPushMatrix();
	//for the bootom part
	glBegin(GL_QUADS);
	// bottom
	//(x-axis, z-axis, y-axis)
	if (!doingShadows) glColor3f(0.968f, 0.219f, 0.035f);
	glVertex3f(3,3,2);
	glVertex3f(27,3,2);
	glVertex3f(27,3,8);
	glVertex3f(3,3,8);
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
	glVertex3f(7,13,10);
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
	glTranslatef(13,15.5,5);
	glRotatef(83,0,0,1);
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
	if (!doingShadows) glColor3f(0.749,0.749,0.749);
	glVertex3f(0, 10,10);
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
	glRotatef(90,1,0,0);
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

// this tries to see which control point is under the mouse
// (for when the mouse is clicked)
// it uses OpenGL picking - which is always a trick
// TODO: if you want to pick things other than control points, or you
// changed how control points are drawn, you might need to change this
void TrainView::doPick()
{
	make_current();		// since we'll need to do some GL stuff

	int mx = Fl::event_x(); // where is the mouse?
	int my = Fl::event_y();

	// get the viewport - most reliable way to turn mouse coords into GL coords
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	// set up the pick matrix on the stack - remember, FlTk is
	// upside down!
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity ();
	gluPickMatrix((double)mx, (double)(viewport[3]-my), 5, 5, viewport);

	// now set up the projection
	setProjection();

	// now draw the objects - but really only see what we hit
	GLuint buf[100];
	glSelectBuffer(100,buf);
	glRenderMode(GL_SELECT);
	glInitNames();
	glPushName(0);

	// draw the cubes, loading the names as we go
	for(size_t i=0; i<world->points.size(); ++i) {
		glLoadName((GLuint) (i+1));
		world->points[i].draw();
	}

	// go back to drawing mode, and see how picking did
	int hits = glRenderMode(GL_RENDER);
	if (hits) {
		// warning; this just grabs the first object hit - if there
		// are multiple objects, you really want to pick the closest
		// one - see the OpenGL manual 
		// remember: we load names that are one more than the index
		selectedCube = buf[3]-1;
	} else // nothing hit, nothing selected
		selectedCube = -1;

	printf("Selected Cube %d\n",selectedCube);
}

// CVS Header - if you don't know what this is, don't worry about it
// This code tells us where the original came from in CVS
// Its a good idea to leave it as-is so we know what version of
// things you started with
// $Header: /p/course/cs559-gleicher/private/CVS/TrainFiles/TrainView.cpp,v 1.10 2009/11/08 21:34:13 gleicher Exp $
