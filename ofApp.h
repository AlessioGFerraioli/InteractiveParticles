#pragma once

#include "ofMain.h"
#include "particle.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		// i want to initialize a certain number of particles from particle class
		// if i use an array, the size (number of elements i.e. particles) is fixed
		// so i use instead a vector object, which is basically a resizable array with no fixed size
		vector<particle> p;

		// i add a user interface with ofxGui addon
		ofxPanel gui; // the gui
		// sliders
		ofxFloatSlider overallSpeed;
		ofxFloatSlider noiseAmount;
		ofxFloatSlider trail; // amount of trail in particles

		// to use a the cam input
		ofVideoGrabber cam;
		int camW;
		int camH;

		// i will grab a color image frmo the video, this is the variable i use
		ofxCvColorImage colorImg;

				// i will then convert it into grayscale and store in this variable
		ofxCvGrayscaleImage grayImg;
		// image for the background
		ofxCvGrayscaleImage grayBg;
		// image for the difference between the image and backgorudn
		ofxCvGrayscaleImage grayDiff;

		// variable to find the contour of the object to find
		ofxCvContourFinder contourFinder;

		int uniqueIcon; 
		
		// add elements to gui for this interactivity
		ofxIntSlider threshold;  // threshold of the mask
		ofxToggle bLearnBackground; // use this button to capture ("learn") the background
		ofxToggle showCV; // show and not show the webcam and the threshold
		ofxToggle repelOrAttract; // button to change the webcam interation from repel to attract

};
