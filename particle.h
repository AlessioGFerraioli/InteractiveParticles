#pragma once
#include "ofMain.h"

class particle
{
public:
	particle(); //constructor

	// methods
	void setup(string iconPath);
	void update(float speed, float noise, bool repulsive);
	void draw(float sizeDot);
	void repel(vector<ofVec2f>repelPt); // use these points to repel particles from the silouhette in the video

	// attributes
	ofVec2f pos;
	ofVec2f vel;
	ofVec2f frc; // forces that affect movement
	ofVec2f repelFrc;
	float drag;

	ofVec2f uniqueVal; // this will be used in conjuction with the noise, each particle has a unique value for the noise 
	float size;

	int uniqueIcon; // number identifying the icon to use for theparticle


	ofImage icon; // image to use instead of the dot

	string iconPath; // the path of the image

};

