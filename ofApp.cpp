#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	
	//setup the cam
	camW = 640;
	camH = 480;
	cam.setup(camW, camH);

	// initialize the ofxCv images
	colorImg.allocate(camW, camH);
	grayImg.allocate(camW, camH);
	grayBg.allocate(camW, camH);
	grayDiff.allocate(camW, camH);

	// set vertical sync
	ofSetVerticalSync(true); // this ensures the screen is drawn in sync with the vertical refresh (for example, when using a projector the image is produced vertically from top to bottom, so this ensures the frame rate is synced with that)


	// by default, the background refreshes at each frame
	// we can disable this to have a "trail" effect of the particle movements
	ofSetBackgroundAuto(false);

	// if frame rate is not explicitly set, the computer tries to use always the highest frame rate it can
	// but this is variable, because changes in performance changes the frame rate, changing the actual
	// speed of the particles
	// so we set up a defined and fixed framerate
	ofSetFrameRate(60);

	// setup the gui
	gui.setup();
	// setup the overallSpeed value with name speed and initial value 1, min value 0 and max value 3
	gui.add(overallSpeed.setup("speed", 1, 0, 3));
	// and noiseAmount slider
	gui.add(noiseAmount.setup("noise", 1, 0, 3));
	// and trail slider
	gui.add(trail.setup("trail", 6.0, 0, 20));
	// threshold
	gui.add(threshold.setup("threshold", 80, 0, 255));
	// button for learning the background
	gui.add(bLearnBackground.setup("capture bg", false));
	// button for showing the webcam
	gui.add(showCV.setup("show webcam", false));
	// buttonn to change forces from repel or attractive
	gui.add(repelOrAttract.setup("repulsive force", false));
	// color sliders
	gui.add(color[0].setup("color1", ofColor(45, 237, 38), ofColor(0,0,0), ofColor(255, 255, 255)));
	gui.add(color[1].setup("color2", ofColor(219, 2, 60), ofColor(0, 0, 0), ofColor(255, 255, 255)));
	gui.add(color[2].setup("color3", ofColor(245, 96, 47), ofColor(0, 0, 0), ofColor(255, 255, 255)));
	gui.add(color[3].setup("color4", ofColor(187, 26, 219), ofColor(0, 0, 0), ofColor(255, 255, 255)));


	// create a certain number of particle
	// to add values to a vector use the function assign
	// i initialize 1000 particles and store them in the vector
	int num_particles = 1000;
	p.assign(num_particles, particle());

	// loop to setup all the particle

	for (int i = 0; i < p.size(); i++) {
		p[i].setup();
	}

}


//--------------------------------------------------------------
void ofApp::update(){
	
	// print the frame rate
	ofSetWindowTitle("FPS:" + ofToString(ofGetFrameRate()));

	// this is to ask to the video player if the frame is a new frame
	bool bNewFrame = false;


	cam.update();
	bNewFrame = cam.isFrameNew();

	if (bNewFrame == true) {

		// copy the input in colorImg
		colorImg.setFromPixels(cam.getPixels());
		// i need to flip it horizontally because i want it to be mirrored
		colorImg.mirror(false, true);
		// copy it in gray img
		grayImg = colorImg;
		

		// if button is pressed, save the frame in brackgorund image
		if (bLearnBackground == true) {
			grayBg = grayImg;
			bLearnBackground = false;
		}

		// difference between background and iamge
		grayDiff.absDiff(grayBg, grayImg);
		// do the threshold
		grayDiff.threshold(threshold);

		// find contours of thresholded differences, params: min size of a blob, max size of a blob, max number of blobs, bool of if i want holes inside the blobs
		contourFinder.findContours(grayDiff, 100, (camW * camH)*0.9, 10, true);


	}

	// i use the blob points found by the contour finder to define the repel points
	vector<ofVec2f>blobPts;
	for(int i=0;i<contourFinder.nBlobs;i++) { // iterate over the blobs
		for (int j = 0; j < contourFinder.blobs[i].pts.size(); j++) { // for each blob, iterate over its points
			// append the points of the blob in the blobPts vector, but we scale them to be the size of the screen
			blobPts.push_back(contourFinder.blobs[i].pts[j] * ofGetWidth() / 640);
		}
	}


	for (int i = 0; i < p.size(); i++) {
		// update the repel points with the current blobpoints
		p[i].repel(blobPts);
		p[i].update(overallSpeed,noiseAmount,repelOrAttract);
	}

}

//--------------------------------------------------------------
void ofApp::draw(){

	// create a rectangle as a background for the particles
	// the opacity of the rectangle will be controlled by the trail slider
	// in pratica funziona così
	// il frame precedente non viene cancellato automaticamente perché abbiamo disabilitato questa cosa con "ofSetBackgroundAuto(false)"
	// bensì lo "copriamo" con questi rettangoli che vengono disegnati a ogni frame
	// però l'opacità del rettangolo non è 100%, il che vuol dire che più i frame sono "vecchi", da più successivi rettangoli sono coperti
	// e dunque più vecchi sono i frame meno sono visibili
	// creando questo effetto di traccia del movimento
	ofSetColor(0, 0, 0, trail);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	// draw white particles
	
	for (int i = 0; i < p.size(); i++) {
		ofSetColor(color[p[i].uniqueCol]);
		p[i].draw(1);
	}


	if(showCV) {
		// FOR SEEING WHAT THE IMAGES PRODUCED LOOK LIKE:
		ofSetHexColor(0xffffff);
		grayDiff.draw(10, 10);
		// draw the contours:
		ofFill();
		ofSetHexColor(0x333333);
		ofDrawRectangle(660, 10, 640, 480);
		contourFinder.draw(660, 10);
	}

	

	// draw the gui
	gui.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
