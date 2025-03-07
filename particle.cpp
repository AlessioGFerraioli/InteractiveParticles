#include "particle.h"

particle::particle()
{
}

void particle::setup()
{
	// initialize with random position
	pos.x = ofRandomWidth();
	pos.y = ofRandomHeight();
	// and random velocity
	vel.x = ofRandom(-4, 4);
	vel.y = ofRandom(-4, 4);

	//initialize the force to zero (will update the force in update())
	frc = ofVec2f(0, 0);

	// initialize the random unique valuye
	uniqueVal.x = ofRandom(-1000, 1000);
	uniqueVal.y = ofRandom(-1000, 1000);

	// initialize random size
	size = ofRandom(3, 5);

	// inizializza drag che conunteracts speed
	drag = ofRandom(0.97, 0.99);

}

void particle::update(float speed, float noise)
{
	// update the force with noise. we use unique val for have a specific force for each particle; also use ofGetElapsedTimeMillis so the force cahanges every frame
	frc.x = ofSignedNoise(uniqueVal.x, ofGetElapsedTimeMillis());
	frc.y = ofSignedNoise(uniqueVal.y, ofGetElapsedTimeMillis());

	// change force by noise factor
	frc *= noise;

	// use the drag to reduce velocity
	vel *= drag;

	// add the repel force
	
	vel += repelFrc; // i use the plus sign to attract instead of repel

	// use the force to increment velocity
	vel += frc;
	
	// RESET AT BORDER
	// if particle hits the border it reappers on the other side
	if (pos.x + vel.x > ofGetWidth()) {
		pos.x -= ofGetWidth();
	}
	else if (pos.x + vel.x < 0) {
		pos.x += ofGetWidth();
	}

	if (pos.y + vel.y > ofGetHeight()) {
		pos.y -= ofGetHeight();
	}
	else if (pos.y + vel.y < 0) {
		pos.y += ofGetHeight();
	}

	
	// update position depending on velocity and speed
	pos += vel*speed;

}

void particle::draw(float sizeDot)
{

	// here we choose how to draw particles
	// draw them as circles, for example
	ofDrawCircle(pos.x, pos.y, size * sizeDot);
}

void particle::repel(vector<ofVec2f> repelPt)
{
	// every frame reset the repel force to zero
	repelFrc.set(0, 0);

	for (int i = 0; i < repelPt.size(); i++) {

		float dist = pos.distance(repelPt[i]); // use the distance function of ofVec2f

		// se la distanza è bassa, la forza agisce, altrimenti no
		if (dist < 150) {

			ofVec2f newRepelPt;
			newRepelPt = repelPt[i] - pos; // calculate the direction and intensity of the repel frc as the difference between the repelpoint and the particle
			repelFrc += newRepelPt* 0.0001;

		}
	}
}
