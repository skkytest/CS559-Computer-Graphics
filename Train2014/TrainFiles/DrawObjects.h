//This file draw all kinds of Objects
//tree
//train
//tank
//track


class DrawObjects{
public:
	void drawTrees(TrainView* , bool);
	void drawTrack(TrainView*, bool);
	void drawTrain(TrainView*, bool);
	void drawTank(TrainView*, bool);
	void surfRevlution(bool);
	void drawBillboard(TrainView*, bool);
	void cubes();
	void drawSkybox();
	void flag(float, float, bool);
};