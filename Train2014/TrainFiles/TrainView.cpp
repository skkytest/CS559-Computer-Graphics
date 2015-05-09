// CS559 Train Project
// TrainView class implementation
// see the header for details
// look for TODO: to see things you want to add/change
// 
#include <Gl/glew.h>
#include <math.h>
#include "TrainView.H"
#include "TrainWindow.H"
#include "DrawObjects.h"

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
		for (size_t i = 0; i < world->points.size(); ++i) {
			if (!doingShadows) {
				if (((int)i) != selectedCube)
					glColor3ub(240, 60, 60);
				else
					glColor3ub(240, 240, 30);
			}
			world->points[i].draw();
		}
	}
	// draw the track
	// TODO: call your own track drawing code
#ifdef EXAMPLE_SOLUTION
	drawTrack(this, doingShadows);
#endif

	DrawObjects newDrawObjects;

	/*
	//This is a try of using shaders

	//load shader first
	static unsigned int shadedCubeShader = 0;
	char* error;
	if (!(shadedCubeShader = loadShader("ShadedCubeTest.vert", "ShadedCubeTest.frag", error))) {
		std::string s = "Can't Load Shader:";
		s += error;
		fl_alert(s.c_str());
	}

	glBindAttribLocation(shadedCubeShader, 0, "position");
	glBindAttribLocation(shadedCubeShader, 1, "color");

	//create avariable to hold our handle to the vertex array object
	GLuint vaoHandle;

	// Wiehin the initialization, create and populate the vertex buffer objects for each attribute
	float positionData[] = {
		-0.5f,-0.5, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};
	float colorData[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	//create the bvuffer objects
	GLuint vboHandles[2];
	glGenBuffers(2, vboHandles);
	GLuint positionBufferHandle = vboHandles[0];
	GLuint colorBufferHandle = vboHandles[1];

	//populate the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);

	//populate the color buffer
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);

	//create and bind to a vertex array object,
	//which stores the relationshop between the buffers and the input attributes

	//create and set-up the vertex array object
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//Enable the vertex attributes arrays
	glEnableVertexAttribArray(0); //vertex position
	glEnableVertexAttribArray(1); //vertex color

	//map index 0 to the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	//map index 1 to the color buffer
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	glUseProgram(shadedCubeShader);

	// Camera/View transformation
	glm::mat4 view;
	GLfloat radius = 10.0f;
	GLfloat camX = sin(this->world->tryTime) * radius;
	GLfloat camZ = cos(this->world->tryTime) * radius;
	view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// Projection 
	glm::mat4 projection;
	projection = glm::perspective(45.0f, (GLfloat)w() / (GLfloat)h(), 0.1f, 100.0f);

	// Get their uniform location
	GLint modelLoc = glGetUniformLocation(shadedCubeShader, "model");
	GLint viewLoc = glGetUniformLocation(shadedCubeShader, "view");
	GLint projLoc = glGetUniformLocation(shadedCubeShader, "projection");
	// Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//in the render function, bind to the vertex array object and call glDrawArrays to initiate rendering
	glBindVertexArray(vaoHandle);

	
	glm::vec3 tri(0.0f, 0.0f, 0.0f);
	glm::mat4 model;
	model = glm::translate(model, tri);
	GLfloat angle = 20.0f;
	model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLES, 0, 3);
	//newDrawObjects.cubes();
	glBindVertexArray(0);
	
	glUseProgram(0);
	*/

	glPushMatrix();
	glTranslated(45, 0, -45);
	if (!doingShadows){
		static unsigned int shadedCubeShader = 0;
		static bool triedCubeShader = false;
		if (!shadedCubeShader && !triedCubeShader) {
			triedCubeShader = true;
			char* error;
			if (!(shadedCubeShader = loadShader("ShadedCubeTest.vert", "ShadedCubeTest.frag", error))) {
				std::string s = "Can't Load Cube Shader:";
				s += error;
				fl_alert(s.c_str());
			}
		}
		glUseProgram(shadedCubeShader);
		newDrawObjects.cubes();
		glUseProgram(0);
	}
	else{
		newDrawObjects.cubes();
	}
	glPopMatrix();


	//draw tree
	newDrawObjects.drawTrees(this, doingShadows);
	//draw track
	newDrawObjects.drawTrack(this, doingShadows);
	//try surface of revolution
	//newDrawObjects.surfRevlution(doingShadows);
	//draw flag
	newDrawObjects.flag(this->world->flagColor, this->world->flagshape, doingShadows);

	if (!tw->trainCam->value()){
		if (this->world->continuity == 1){
			glPushMatrix();
			glTranslatef(this->world->xaxis, this->world->yaxis, this->world->zaxis);
			glRotatef(this->world->angle, 0, 1, 0);
			glRotatef(-this->world->heightAngle, 0, 0, 1);
			glTranslatef(-15, 0, -5);
			if (this->world->model == 2){
				newDrawObjects.drawTrain(this, doingShadows);
			}
			if (this->world->model == 1){
				newDrawObjects.drawTank(this, doingShadows);
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
				newDrawObjects.drawTrain(this, doingShadows);
			}
			if (this->world->model == 1){
				newDrawObjects.drawTank(this, doingShadows);
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
				newDrawObjects.drawTrain(this, doingShadows);
			}
			if (this->world->model == 1){
				newDrawObjects.drawTank(this, doingShadows);
			}
			glPopMatrix();
		}
	}

	//draw billboard
	newDrawObjects.drawBillboard(this, doingShadows);

	newDrawObjects.drawSkybox();
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
