#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){

	ofSetVerticalSync(true );
	ofEnableAlphaBlending();
	ofEnableSmoothing();

	#ifdef _USE_LIVE_VIDEO
        vidGrabber.setVerbose(true);
        vidGrabber.initGrabber(CAM_WIDTH,CAM_HEIGHT);
		vidGrabber.setDesiredFrameRate(8);
	#else
        vidPlayer.loadMovie("fingers.mov");
        vidPlayer.play();
	#endif

    colorImg.allocate(CAM_WIDTH,CAM_HEIGHT);
	grayImage.allocate(CAM_WIDTH,CAM_HEIGHT);
	
	opticalFlow = new OpticalFlow(PROCESSING_WIDTH, PROCESSING_HEIGHT);
	opticalFlow->setMinDist(7);
	opticalFlow->setQuality(0.0001);
	opticalFlow->setNumFeatures(30);
	opticalFlow->setEqualize(false);
	opticalFlow->setCorrelate(false);
	opticalFlow->setWindowSize(10);
	
	for (int i = 0; i < MAX_FEATURES; i++){
		colors[i].r = ofRandom(0, 1);
		colors[i].g = ofRandom(0, 1);
		colors[i].b = ofRandom(0, 1);
		colors[i].a = 1;
	}
	
	ofSetWindowShape(4 * PROCESSING_WIDTH, 2 * PROCESSING_HEIGHT);
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
            colorImg.setFromPixels(vidGrabber.getPixels(), CAM_WIDTH,CAM_HEIGHT);
	    #else
            colorImg.setFromPixels(vidPlayer.getPixels(), CAM_WIDTH,CAM_HEIGHT);
        #endif

        grayImage = colorImg;
		opticalFlow->update( grayImage.getCvImage() );
	}

}

//--------------------------------------------------------------
void testApp::draw(){

	glScalef(2, 2, 1);
	
	ofSetColor(0xffffff);
	colorImg.draw(0,0, PROCESSING_WIDTH, PROCESSING_HEIGHT);
	grayImage.draw(PROCESSING_WIDTH,0, PROCESSING_WIDTH, PROCESSING_HEIGHT);
	
	ofSetColor(0xff0000);
	
	int n = opticalFlow->getFoundNumFeatures();
	flow * of = opticalFlow->getResults();
	
	glLineWidth(1);
	glTranslatef(PROCESSING_WIDTH, 0, 0);
	glBegin(GL_LINES);
	
	for (int i = 0; i< n; i++){	
		//ofLine(of[i].origin.x, of[i].origin.y, of[i].destination.x, of[i].destination.y);
		glColor4f(colors[i].r, colors[i].g, colors[i].b, 0);
		glVertex2f(of[i].origin.x, of[i].origin.y);
		
		glColor4f(colors[i].r, colors[i].g, colors[i].b, 1);
		glVertex2f(of[i].destination.x, of[i].destination.y);
	}
	glEnd();

	ofSetColor(0xffffff);
	char reportStr[1024];
	sprintf(reportStr, "fps: %f", ofGetFrameRate());
	ofDrawBitmapString(reportStr, 20, 20);

}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){

	switch (key){
		case 's':
			#ifdef _USE_LIVE_VIDEO
			vidGrabber.videoSettings();
			#endif
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

