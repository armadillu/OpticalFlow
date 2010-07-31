#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){


	#ifdef _USE_LIVE_VIDEO
        vidGrabber.setVerbose(true);
        vidGrabber.initGrabber(320,240);
	#else
        vidPlayer.loadMovie("fingers.mov");
        vidPlayer.play();
	#endif

    colorImg.allocate(320,240);
	grayImage.allocate(320,240);
	
	opticalFlow = new OpticalFlow(320, 240);
}

//--------------------------------------------------------------
void testApp::update(){

	ofBackground(100,100,100);

    bool bNewFrame = false;

	#ifdef _USE_LIVE_VIDEO
       vidGrabber.grabFrame();
	   bNewFrame = vidGrabber.isFrameNew();
    #else
        vidPlayer.idleMovie();
        bNewFrame = vidPlayer.isFrameNew();
	#endif

	if (bNewFrame){

		#ifdef _USE_LIVE_VIDEO
            colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
	    #else
            colorImg.setFromPixels(vidPlayer.getPixels(), 320,240);
        #endif

        grayImage = colorImg;

		opticalFlow->update( grayImage.getCvImage() );
	}

}

//--------------------------------------------------------------
void testApp::draw(){

	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetColor(0xffffff);
	colorImg.draw(0,0);
	grayImage.draw(320,00);

	// then draw the contours:

	
	ofSetColor(0xff0000);
	
	int n = opticalFlow->getNumFeatures();
	flow * of = opticalFlow->getResults();
	
	glTranslatef(320, 0, 0);
	glBegin(GL_LINES);
	for (int i = 0; i< n; i++){	

		//ofLine(of[i].origin.x, of[i].origin.y, of[i].destination.x, of[i].destination.y);
		glColor3ub(0, 255, 0);
		glVertex2f(of[i].origin.x, of[i].origin.y);
		
		glColor3ub(255, 0, 0);
		glVertex2f(of[i].destination.x, of[i].destination.y);
	}
	glEnd();


	ofSetColor(0xffffff);
	char reportStr[1024];
	sprintf(reportStr, "fps: %f", ofGetFrameRate());
	ofDrawBitmapString(reportStr, 20, 600);

}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){

	switch (key){
			
		case 'f':
			ofSetFullscreen(true);
			break;

	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

