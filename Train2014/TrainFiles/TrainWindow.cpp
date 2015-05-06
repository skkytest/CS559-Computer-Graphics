// CS559 Train Project -
// Train Window class implementation
// - note: this is code that a student might want to modify for their project
//   see the header file for more details
// - look for the TODO: in this file
// - also, look at the "TrainView" - its the actual OpenGL window that
//   we draw into
//
// Written by Mike Gleicher, October 2008
//

#include "TrainWindow.H"
#include "TrainView.H"
#include "CallBacks.H"

#include <FL/fl.h>
#include <FL/Fl_Box.h>

// for using the real time clock
#include <time.h>



/////////////////////////////////////////////////////
TrainWindow::TrainWindow(const int x, const int y) : Fl_Double_Window(x,y,800,600,"Train and Roller Coaster")
{
	// make all of the widgets
	begin();	// add to this widget
	{
		int pty=5;			// where the last widgets were drawn

		trainView = new TrainView(5,5,590,590);
		trainView->tw = this;
		trainView->world = &world;
		this->resizable(trainView);

		// to make resizing work better, put all the widgets in a group
		widgets = new Fl_Group(600,5,190,590);
		widgets->begin();

		runButton = new Fl_Button(605,pty,60,20,"Run");
		togglify(runButton);

		Fl_Button* fb = new Fl_Button(700,pty,25,20,"@>>");
		fb->callback((Fl_Callback*)forwCB,this);
		Fl_Button* rb = new Fl_Button(670,pty,25,20,"@<<");
		rb->callback((Fl_Callback*)backCB,this);
		
		arcLength = new Fl_Button(730,pty,65,20,"ArcLength");
		togglify(arcLength,1);
  
		pty+=25;
		speed = new Fl_Value_Slider(655,pty,140,20,"speed");
		speed->range(0,5);
		speed->value(2);
		speed->align(FL_ALIGN_LEFT);
		speed->type(FL_HORIZONTAL);

		pty += 30;

		// camera buttons - in a radio button group
		Fl_Group* camGroup = new Fl_Group(600,pty,195,20);
		camGroup->begin();
		worldCam = new Fl_Button(605, pty, 60, 20, "World");
        worldCam->type(FL_RADIO_BUTTON);		// radio button
        worldCam->value(1);			// turned on
        worldCam->selection_color((Fl_Color)3); // yellow when pressed
		worldCam->callback((Fl_Callback*)damageCB,this);

		trainCam = new Fl_Button(670, pty, 60, 20, "Train");
        trainCam->type(FL_RADIO_BUTTON);
        trainCam->value(0);
        trainCam->selection_color((Fl_Color)3);
		trainCam->callback((Fl_Callback*)damageCB,this);

		topCam = new Fl_Button(735, pty, 60, 20, "Top");
        topCam->type(FL_RADIO_BUTTON);
        topCam->value(0);
        topCam->selection_color((Fl_Color)3);
		topCam->callback((Fl_Callback*)damageCB,this);
		camGroup->end();

		pty += 30;

		// browser to select spline types
		// TODO: make sure these choices are the same as what the code supports
		splineBrowser = new Fl_Browser(605,pty,160,75,"Spline Type");
		splineBrowser->type(2);		// select
		splineBrowser->callback((Fl_Callback*)damageCB,this);
		splineBrowser->add("Linear");
		splineBrowser->add("Cardinal Cubic");
		splineBrowser->add("Cubic B-Spline");
		splineBrowser->select(2);
		splineBrowser->callback((Fl_Callback*)changeContinuity, this);

		pty += 110;

		//to change tension of the splines
		tension = new Fl_Value_Slider(655, pty, 140, 20, "tension");
		tension->range(0, 5);
		tension->value(2);
		tension->align(FL_ALIGN_LEFT);
		tension->type(FL_HORIZONTAL);

		pty += 40;


		// add and delete points
		Fl_Button* ap = new Fl_Button(605,pty,80,20,"Add Point");
		ap->callback((Fl_Callback*)addPointCB,this);
		Fl_Button* dp = new Fl_Button(690,pty,80,20,"Delete Point");
		dp->callback((Fl_Callback*)deletePointCB,this);

		pty += 25;
		// reset the points
		resetButton = new Fl_Button(735,pty,60,20,"Reset");
		resetButton->callback((Fl_Callback*)resetCB,this);
		Fl_Button* loadb = new Fl_Button(605,pty,60,20,"Load");
		loadb->callback((Fl_Callback*) loadCB, this);
		Fl_Button* saveb = new Fl_Button(670,pty,60,20,"Save");
		saveb->callback((Fl_Callback*) saveCB, this);

		pty += 25;
		// roll the points
		Fl_Button* rx = new Fl_Button(605,pty,30,20,"R+X");
		rx->callback((Fl_Callback*)rpxCB,this);
		Fl_Button* rxp = new Fl_Button(635,pty,30,20,"R-X");
		rxp->callback((Fl_Callback*)rmxCB,this);
		Fl_Button* rz = new Fl_Button(670,pty,30,20,"R+Z");
		rz->callback((Fl_Callback*)rpzCB,this);
		Fl_Button* rzp = new Fl_Button(700,pty,30,20,"R-Z");
		rzp->callback((Fl_Callback*)rmzCB,this);

		pty+=30;

		// TODO: add widgets for all of your fancier features here
#ifdef EXAMPLE_SOLUTION
		makeExampleWidgets(this,pty);
#endif

		// browser to select model (train or tank)
		modelBrowser = new Fl_Browser(605, pty, 80, 75, "Model Type");
		modelBrowser->type(2);		// select
		modelBrowser->callback((Fl_Callback*)damageCB, this);
		modelBrowser->add("tank(main)");
		modelBrowser->add("train(secondary)");
		modelBrowser->select(1);
		modelBrowser->callback((Fl_Callback*)changeModel, this);

		// bowser to select track model
		trackBrowser = new Fl_Browser(700, pty, 80, 75, "Track Type");
		trackBrowser->type(2);		// select
		trackBrowser->callback((Fl_Callback*)damageCB, this);
		trackBrowser->add("line");
		trackBrowser->add("wood track");
		trackBrowser->add("road");
		trackBrowser->select(2);
		trackBrowser->callback((Fl_Callback*)changeTrack, this);

		pty += 110;


		// physics
		Fl_Button* physics = new Fl_Button(605, pty, 80, 20, "physics");
		physics->type(FL_TOGGLE_BUTTON);		// toggle
		physics->value(0);		// turned off
		physics->selection_color((Fl_Color)3); // yellow when pressed	
		physics->callback((Fl_Callback*)getPhysics, this);


		// we need to make a little phantom widget to have things resize correctly
		Fl_Box* resizebox = new Fl_Box(600,595,200,5);
		widgets->resizable(resizebox);


		widgets->end();
	}
	end();	// done adding to this widget

	// set up callback on idle
	Fl::add_idle((void (*)(void*))runButtonCB,this);
}

// handy utility to make a button into a toggle
void TrainWindow::togglify(Fl_Button* b, int val)
{
    b->type(FL_TOGGLE_BUTTON);		// toggle
    b->value(val);		// turned off
    b->selection_color((Fl_Color)3); // yellow when pressed	
	b->callback((Fl_Callback*)damageCB,this);
}

void TrainWindow::damageMe()
{
	if (trainView->selectedCube >= ((int)world.points.size()))
		trainView->selectedCube = 0;
	trainView->damage(1);
}

/////////////////////////////////////////////////////
// this will get called (approximately) 30 times per second
// if the run button is pressed
void TrainWindow::advanceTrain(float dir)
{
	// TODO: make this work for your train
#ifdef EXAMPLE_SOLUTION
	// note - we give a little bit more example code here than normal,
	// so you can see how this works

	if (arcLength->value()) {
		float vel = ew.physics->value() ? physicsSpeed(this) * (float)speed->value() : dir * (float)speed->value();
		world.trainU += arclenVtoV(world.trainU, vel, this);
	} else {
		world.trainU +=  dir * ((float)speed->value() * .1f);
	}

	float nct = static_cast<float>(world.points.size());
	if (world.trainU > nct) world.trainU -= nct;
	if (world.trainU < 0) world.trainU += nct;
#endif
	float speed = (float)this->speed->value();
	if (this->trainCam->value()){
		speed /= 3;
	}

	//local parameter to get tension from Window
	float ts = (float)this->tension->value() / 5.0+0.5;

	float standLength = 70.1706;

	bool arcFlag;
	if (this->arcLength->value()){
		arcFlag = true;
	}
	else{
		arcFlag = false;
	}

	if (this->world.continuity == 1){
		//printf("%f \n", this->world.trainU);
		int pre = int(this->world.trainU);
		ControlPoint pres = this->world.points[pre];
		ControlPoint next;
		if (pre == this->world.points.size() - 1) next = this->world.points[0];
		else next = this->world.points[pre + 1];
		double length = sqrt((pres.pos.x - next.pos.x)*(pres.pos.x - next.pos.x) + (pres.pos.z - next.pos.z)*(pres.pos.z - next.pos.z));

		float rate = 1;
		if (!arcFlag){
			rate = length / standLength;
		}

		//parameter help to set trainCamera
		this->world.viewx = next.pos.x;
		this->world.viewy = next.pos.y;
		this->world.viewz = next.pos.z;

		float xdis = next.pos.x - pres.pos.x;
		float ydis = next.pos.y - pres.pos.y;
		float zdis = next.pos.z - pres.pos.z;
		if (xdis >= 0 && zdis >= 0){
			this->world.angle = 180 - atan(zdis / xdis) * 360 / 6.28;
		}
		else if (xdis > 0 && zdis < 0){
			zdis = (-1) * zdis;
			this->world.angle = (-1)*(180 - atan(zdis / xdis) * 360 / 6.28);
		}
		else if (xdis < 0 && zdis < 0){
			xdis = (-1) * xdis;
			zdis = (-1) * zdis;
			this->world.angle = (-1)*(atan(zdis / xdis) * 360 / 6.28);
		}
		else{
			xdis = (-1)*xdis;
			this->world.angle = atan(zdis / xdis) * 360 / 6.28;
		}
		this->world.heightAngle = atan(ydis / (sqrt(xdis*xdis + zdis*zdis))) * 360 / 6.28;


		//control the speed of the train
		float dis = 3 * speed*rate;
		this->world.distance += dis;
		this->world.xaxis = pres.pos.x + (next.pos.x - pres.pos.x) * (this->world.distance / length);
		this->world.yaxis = pres.pos.y + (next.pos.y - pres.pos.y) * (this->world.distance / length);
		this->world.zaxis = pres.pos.z + (next.pos.z - pres.pos.z) * (this->world.distance / length);
		//printf("%f %f %f", this->world.angle, this->world.xaxis, this->world.zaxis);
		if (this->world.distance / length >= 0.95){
			this->world.distance = 0.0;
			this->world.trainU += 1;
		}
		if (this->world.trainU == this->world.points.size())
			this->world.trainU = 0;
	}

	if (this->world.continuity == 2 )
	{
		int i = int(this->world.trainU);
		int p;
		//P[0]
		if (i == 0) p = this->world.points.size() - 1;
		else p = i - 1;
		float p0x = this->world.points[p].pos.x;
		float p0y = this->world.points[p].pos.y;
		float p0z = this->world.points[p].pos.z;
		//p[1]
		float p1x = this->world.points[i].pos.x;
		float p1y = this->world.points[i].pos.y;
		float p1z = this->world.points[i].pos.z;

		//printf("%f %f %f \n", this->world.points[i].orient.x, this->world.points[i].orient.y, this->world.points[i].orient.z);

		//P[2]
		p = (i + 1) % (world.points.size());
		float p2x = this->world.points[p].pos.x;
		float p2y = this->world.points[p].pos.y;
		float p2z = this->world.points[p].pos.z;
		//P[3]
		p = (i + 2) % (world.points.size());
		float p3x = this->world.points[p].pos.x;
		float p3y = this->world.points[p].pos.y;
		float p3z = this->world.points[p].pos.z;

		//calculate distance between P1 and P2
		float length = sqrt((p1x - p2x)*(p1x - p2x) + (p1y - p2y)*(p1y - p2y) + (p1z - p2z)*(p1z - p2z));

		
		float rate = 1;
		if (arcFlag == true){
			rate = length / standLength;
			if (rate < 1) rate = 1;
		}
		

		if (this->trainCam->value()){
			speed /= 2;
		}
		
		//calculate the physics
		//parameter to set the offset of the speed when we use physics
		float phys;
		if (this->world.physics){
			//calculate the present Y-axis
			float preY;
			float py = this->world.distance;
			if (p2y > p1y){
				preY = ((-ts)*p0x + (2 - ts) * p1x + (ts - 2)*p2x + ts*p3x)*py*py*py + (2 * ts * p0x + (ts - 3) * p1x + (3 - 2 * ts) * p2x - ts*p3x)*py*py + ((-ts)*p0x + ts*p2x)*py + 2 * p1y;
				phys = (0.5*length + p2y - preY) / (1.5*length)*2.5;
			}
			else if (p2y < p1y){
				preY = ((-ts)*p0x + (2 - ts) * p1x + (ts - 2)*p2x + ts*p3x)*py*py*py + (2 * ts * p0x + (ts - 3) * p1x + (3 - 2 * ts) * p2x - ts*p3x)*py*py + ((-ts)*p0x + ts*p2x)*py + 2 * p1y;
				phys = (p1y - preY + 0.5*length) / (1.5*length)*2.5;
			}
			else{
				phys = 2.5;
			}
		}
		else{
			phys = 2.5;
		}

	
		//local parameter to get the date in the world->distance
		float t;

		//boolean parameter to decide whether satisfy the requirement to go to the next point
		//only need to be calculated under arc-length seneriao
		bool run;

		if (arcFlag){
			float s; //calculate arc-length point
			
			this->world.delta += (0.05*speed/rate/4)*phys;
			t = this->world.distance;
			s = t + this->world.delta;

			//calculate distance between present point and present point+delta
			//calculate matrix
			float txx, tyy, tzz;
			float rxx, ryy, rzz;
			txx = (((-ts)*p0x + (2 - ts) * p1x + (ts - 2)*p2x + ts*p3x)*t*t*t + (2 * ts * p0x + (ts - 3) * p1x + (3 - 2 * ts) * p2x - ts*p3x)*t*t + ((-ts)*p0x + ts*p2x)*t + p1x);
			tyy = (((-ts)*p0y + (2 - ts) * p1y + (ts - 2)*p2y + ts*p3y)*t*t*t + (2 * ts * p0y + (ts - 3) * p1y + (3 - 2 * ts) * p2y - ts*p3y)*t*t + ((-ts)*p0y + ts*p2y)*t + p1y);
			tzz = (((-ts)*p0z + (2 - ts) * p1z + (ts - 2)*p2z + ts*p3z)*t*t*t + (2 * ts * p0z + (ts - 3) * p1z + (3 - 2 * ts) * p2z - ts*p3z)*t*t + ((-ts)*p0z + ts*p2z)*t + p1z);


			rxx = (((-ts)*p0x + (2 - ts) * p1x + (ts - 2)*p2x + ts*p3x)*s*s*s + (2 * ts * p0x + (ts - 3) * p1x + (3 - 2 * ts) * p2x - ts*p3x)*s*s + ((-ts)*p0x + ts*p2x)*s + p1x);
			ryy = (((-ts)*p0y + (2 - ts) * p1y + (ts - 2)*p2y + ts*p3y)*s*s*s + (2 * ts * p0y + (ts - 3) * p1y + (3 - 2 * ts) * p2y - ts*p3y)*s*s + ((-ts)*p0y + ts*p2y)*s + p1y);
			rzz = (((-ts)*p0z + (2 - ts) * p1z + (ts - 2)*p2z + ts*p3z)*s*s*s + (2 * ts * p0z + (ts - 3) * p1z + (3 - 2 * ts) * p2z - ts*p3z)*s*s + ((-ts)*p0z + ts*p2z)*s + p1z);

			
			float deltaDis = sqrt((txx-rxx)*(txx-rxx)+(tyy-ryy)*(tyy-ryy)+(tzz-rzz)*(tzz-rzz));

			if (deltaDis > 1){
				this->world.distance += this->world.delta;
				this->world.delta = 0;
				run = true;
			}
			else{
				run = false;
			}
		}
		else{

			this->world.distance += (0.05*speed/2)*phys;
			t = world.distance;
			run = true;
		}

		if (run){

			t = this->world.distance;

			//r is used to calculate the rotate angle of the train
			float r;
			//r= world.distance + 0.001;
			r = t + 0.001;

			//calculate the matrix
			float tx, ty, tz;
			float rx, ry, rz;
			tx = (((-ts)*p0x + (2 - ts) * p1x + (ts - 2)*p2x + ts*p3x)*t*t*t + (2 * ts * p0x + (ts - 3) * p1x + (3 - 2 * ts) * p2x - ts*p3x)*t*t + ((-ts)*p0x + ts*p2x)*t + p1x);
			ty = (((-ts)*p0y + (2 - ts) * p1y + (ts - 2)*p2y + ts*p3y)*t*t*t + (2 * ts * p0y + (ts - 3) * p1y + (3 - 2 * ts) * p2y - ts*p3y)*t*t + ((-ts)*p0y + ts*p2y)*t + p1y);
			tz = (((-ts)*p0z + (2 - ts) * p1z + (ts - 2)*p2z + ts*p3z)*t*t*t + (2 * ts * p0z + (ts - 3) * p1z + (3 - 2 * ts) * p2z - ts*p3z)*t*t + ((-ts)*p0z + ts*p2z)*t + p1z);

			rx = (((-ts)*p0x + (2 - ts) * p1x + (ts - 2)*p2x + ts*p3x)*r*r*r + (2 * ts * p0x + (ts - 3) * p1x + (3 - 2 * ts) * p2x - ts*p3x)*r*r + ((-ts)*p0x + ts*p2x)*r + p1x);
			ry = (((-ts)*p0y + (2 - ts) * p1y + (ts - 2)*p2y + ts*p3y)*r*r*r + (2 * ts * p0y + (ts - 3) * p1y + (3 - 2 * ts) * p2y - ts*p3y)*r*r + ((-ts)*p0y + ts*p2y)*r + p1y);
			rz = (((-ts)*p0z + (2 - ts) * p1z + (ts - 2)*p2z + ts*p3z)*r*r*r + (2 * ts * p0z + (ts - 3) * p1z + (3 - 2 * ts) * p2z - ts*p3z)*r*r + ((-ts)*p0z + ts*p2z)*r + p1z);

			this->world.xaxis = tx;
			this->world.yaxis = ty;
			this->world.zaxis = tz;

			float xdis = rx - tx;
			float ydis = ry - ty;
			float zdis = rz - tz;

			//parameter help to set trainCamera
			this->world.viewx = tx + 30 * xdis;
			this->world.viewy = ty + 30 * ydis;
			this->world.viewz = tz + 30 * zdis;

			if (xdis >= 0 && zdis >= 0){
				this->world.angle = 180 - atan(zdis / xdis) * 360 / 6.28;
			}
			else if (xdis > 0 && zdis < 0){
				this->world.angle = (-1)*(180 - atan((-1)*zdis / xdis) * 360 / 6.28);
			}
			else if (xdis < 0 && zdis < 0){
				this->world.angle = (-1)*(atan(zdis / xdis) * 360 / 6.28);
			}
			else{
				this->world.angle = atan((-1)*zdis / xdis) * 360 / 6.28;
			}
			this->world.heightAngle = atan(ydis / (sqrt(xdis*xdis + zdis*zdis))) * 360 / 6.28;

			if (t > 1.0){
				this->world.distance -= 1.0;
				this->world.trainU += 1;	
			}
			
			
			if (this->world.trainU == world.points.size()) this->world.trainU = 0;
			
		}
	}

	if (this->world.continuity == 3){

		int i = int(this->world.trainU);
		int number = this->world.points.size();
		//P[0]
		float p0x = this->world.points[i%number].pos.x;
		float p0y = this->world.points[i%number].pos.y;
		float p0z = this->world.points[i%number].pos.z;
		//p[1]
		float p1x = this->world.points[(i + 1) % number].pos.x;
		float p1y = this->world.points[(i + 1) % number].pos.y;
		float p1z = this->world.points[(i + 1) % number].pos.z;
		//P[2]
		float p2x = this->world.points[(i + 2) % number].pos.x;
		float p2y = this->world.points[(i + 2) % number].pos.y;
		float p2z = this->world.points[(i + 2) % number].pos.z;
		//P[3]
		float p3x = this->world.points[(i + 3) % number].pos.x;
		float p3y = this->world.points[(i + 3) % number].pos.y;
		float p3z = this->world.points[(i + 3) % number].pos.z;

		if (this->trainCam->value()){
			speed /= 2;
		}

		float length = sqrt((p0x-p1x)*(p0x-p1x)+(p0y-p1y)*(p0y-p1y)+(p0z-p1z)*(p0z-p1z));

		float rate = 1;
		if (arcFlag == true){
			rate = length / standLength;
			if (rate < 1) rate = 1;
		}

		//local parameter to get the date in the world->distance
		float t;

		//boolean parameter to decide whether satisfy the requirement to go to the next point
		//only need to be calculated under arc-length seneriao
		bool run;

		if (arcFlag){
			float s; //calculate arc-length point

			this->world.delta += 0.022*speed*2;
			t = this->world.distance;
			s = t + this->world.delta;

			//calculate distance between present point and present point+delta
			//calculate matrix
			float txx, tyy, tzz;
			float rxx, ryy, rzz;
			txx = 0.5*(((-1)*p0x + 3 * p1x + (-3)*p2x + p3x)*t*t*t + (2 * p0x - 5 * p1x + 4 * p2x - p3x)*t*t + ((-1)*p0x + p2x)*t + 2 * p1x);
			tyy = 0.5*(((-1)*p0y + 3 * p1y + (-3)*p2y + p3y)*t*t*t + (2 * p0y - 5 * p1y + 4 * p2y - p3y)*t*t + ((-1)*p0y + p2y)*t + 2 * p1y);
			tzz = 0.5*(((-1)*p0z + 3 * p1z + (-3)*p2z + p3z)*t*t*t + (2 * p0z - 5 * p1z + 4 * p2z - p3z)*t*t + ((-1)*p0z + p2z)*t + 2 * p1z);

			rxx = 0.5*(((-1)*p0x + 3 * p1x + (-3)*p2x + p3x)*s*s*s + (2 * p0x - 5 * p1x + 4 * p2x - p3x)*s*s + ((-1)*p0x + p2x)*s + 2 * p1x);
			ryy = 0.5*(((-1)*p0y + 3 * p1y + (-3)*p2y + p3y)*s*s*s + (2 * p0y - 5 * p1y + 4 * p2y - p3y)*s*s + ((-1)*p0y + p2y)*s + 2 * p1y);
			rzz = 0.5*(((-1)*p0z + 3 * p1z + (-3)*p2z + p3z)*s*s*s + (2 * p0z - 5 * p1z + 4 * p2z - p3z)*s*s + ((-1)*p0z + p2z)*s + 2 * p1z);

			float deltaDis = sqrt((txx - rxx)*(txx - rxx) + (tyy - ryy)*(tyy - ryy) + (tzz - rzz)*(tzz - rzz));

			if (deltaDis > 1){
				this->world.distance += this->world.delta;
				this->world.delta = 0;
				run = true;
			}
			else{
				run = false;
			}
		}
		else{
			this->world.distance += 0.08*speed;
			t = world.distance;
			run = true;
		}

		if (run){
			//r is used to calculate the rotate angle of the train
			float r;
			if (!arcFlag){
				r = world.distance + 0.05;
			}
			else{
				r = world.distance + world.delta;
			}

			float px, py, pz;
			float rx, ry, rz;
			px = 1.0 / 6 * (((-1)*p0x + 3 * p1x + (-3)*p2x + p3x)*t*t*t + (3 * p0x - 6 * p1x + 3 * p2x)*t*t + ((-3)*p0x + 3 * p2x)*t + p0x + 4 * p1x + p2x);
			py = 1.0 / 6 * (((-1)*p0y + 3 * p1y + (-3)*p2y + p3y)*t*t*t + (3 * p0y - 6 * p1y + 3 * p2y)*t*t + ((-3)*p0y + 3 * p2y)*t + p0y + 4 * p1y + p2y);
			pz = 1.0 / 6 * (((-1)*p0z + 3 * p1z + (-3)*p2z + p3z)*t*t*t + (3 * p0z - 6 * p1z + 3 * p2z)*t*t + ((-3)*p0z + 3 * p2z)*t + p0z + 4 * p1z + p2z);

			rx = 1.0 / 6 * (((-1)*p0x + 3 * p1x + (-3)*p2x + p3x)*r*r*r + (3 * p0x - 6 * p1x + 3 * p2x)*r*r + ((-3)*p0x + 3 * p2x)*r + p0x + 4 * p1x + p2x);
			ry = 1.0 / 6 * (((-1)*p0y + 3 * p1y + (-3)*p2y + p3y)*r*r*r + (3 * p0y - 6 * p1y + 3 * p2y)*r*r + ((-3)*p0y + 3 * p2y)*r + p0y + 4 * p1y + p2y);
			rz = 1.0 / 6 * (((-1)*p0z + 3 * p1z + (-3)*p2z + p3z)*r*r*r + (3 * p0z - 6 * p1z + 3 * p2z)*r*r + ((-3)*p0z + 3 * p2z)*r + p0z + 4 * p1z + p2z);

			this->world.xaxis = px;
			this->world.yaxis = py;
			this->world.zaxis = pz;

			float xdis = rx - px;
			float ydis = ry - py;
			float zdis = rz - pz;

			// parameter help to set trainCamera
			this->world.viewx = px + 30 * xdis;
			this->world.viewy = py + 30 * ydis;
			this->world.viewz = pz + 30 * zdis;

			if (xdis >= 0 && zdis >= 0){
				this->world.angle = 180 - atan(zdis / xdis) * 360 / 6.28;
			}
			else if (xdis > 0 && zdis < 0){
				this->world.angle = (-1)*(180 - atan((-1)*zdis / xdis) * 360 / 6.28);
			}
			else if (xdis < 0 && zdis < 0){
				this->world.angle = (-1)*(atan(zdis / xdis) * 360 / 6.28);
			}
			else{
				this->world.angle = atan((-1)*zdis / xdis) * 360 / 6.28;
			}
			this->world.heightAngle = atan(ydis / (sqrt(xdis*xdis + zdis*zdis))) * 360 / 6.28;


			if (t >= 1){
				this->world.distance -= 1;
				this->world.trainU += 1;
			}

			if (this->world.trainU == world.points.size()) this->world.trainU = 0;
		}
	}
}