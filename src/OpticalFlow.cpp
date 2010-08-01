/*
 *  OpticalFlow.cpp
 *  openFrameworks
 *
 *  Created by Oriol Ferrer Mesià on 7/30/10.
 *  Copyright 2010 uri.cat. All rights reserved.
 *
 */

#include "OpticalFlow.h"

OpticalFlow::OpticalFlow(int w_, int h_){
	
	w = w_;
	h = h_;
	
	currentImg = cvCreateImage(cvSize(w, h), 8, 1);
	prevImg = cvCreateImage(cvSize(w, h), 8, 1);
	
	tmp = cvCreateImage(cvSize(w, h), IPL_DEPTH_32F, 1);
	eig = cvCreateImage(cvSize(w, h), IPL_DEPTH_32F, 1);
	CvSize pyr_sz = cvSize( currentImg->width+8, currentImg->height/3 );
	pyrA = cvCreateImage( pyr_sz, IPL_DEPTH_32F, 1 );
	pyrB = cvCreateImage( pyr_sz, IPL_DEPTH_32F, 1 );
	
	equalize = false;
	correlate = false;
	windowSize = 20;
	numFeaturesToTrack = 100;
	quality = 0.001;
	minDist = 7.0;
	
	zeroPoint.x = zeroPoint.y = 0;
	
	for (int i = 0; i < MAX_FEATURES; i++){
		newResults[i].origin = results[i].origin = zeroPoint;
		newResults[i].destination = results[i].destination = zeroPoint;
		newResults[i].direction = results[i].direction = zeroPoint;
	}
	
}

OpticalFlow::~OpticalFlow(){
	
	cvReleaseImage(&pyrA);
	cvReleaseImage(&pyrB);
	cvReleaseImage(&currentImg);
	cvReleaseImage(&prevImg);
	cvReleaseImage(&tmp);
	cvReleaseImage(&eig);
	
}


void OpticalFlow::update(IplImage * newImg){

	if (newImg->nChannels != 1)	//rtfm!
		printf("OpticalFlow:update() >> input image needs to be 1 channel (grayscale)\n");
	
	if (newImg->width != w || newImg->height != h){	//input image neeeds to be downsampled to match working dimensions
		cvResize( newImg, currentImg, CV_INTER_NN );
	}else{
		cvCopy(newImg, currentImg);
	}
	//currentImg now holds a (w,h) 1 channel new image
	
	if (equalize)
		cvEqualizeHist(currentImg, currentImg);
	

	// Optical Flow //
	
//	printf("before # %d numFeaturesToTrack \n", numFeaturesToTrack);
	
	int	numFeatures = numFeaturesToTrack;
	cvGoodFeaturesToTrack(
							currentImg,
							eig,
							tmp,
							cornersA,
							&numFeatures,
							quality,	//quality level
							minDist	//min dist
						  );
	
//	printf("after # %d numFeatures \n", numFeatures);
	
	numFoundFeatures = numFeatures;
	
	cvCalcOpticalFlowPyrLK(
							currentImg,
							prevImg,
							pyrA,
							pyrB,
							cornersA,
							cornersB,
							numFeatures,
							cvSize( windowSize,windowSize ),
							1,
							features_found,
							feature_errors,
							cvTermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, .3 ),
							0
						   );
	
	//fill in our result table
	for( int i = 0; i<numFeatures; i++ ) {
			
		if( features_found[i]==0 || feature_errors[i] > 550 ) {
			continue;
		}

		p2 pa;
		pa.x = cornersA[i].x;
		pa.y = cornersA[i].y;
		
		p2 pb;
		pb.x = cornersB[i].x;
		pb.y = cornersB[i].y;
			
		newResults[i].origin = pb;
		newResults[i].destination = pa;
		newResults[i].direction.x = pa.x - pb.x;	//to check!
		newResults[i].direction.y = pa.y - pb.y;
		
	}
	
	for( int i=0; i<numFeaturesToTrack; i++ ) {
		
		if( features_found[i]==0 || feature_errors[i] > 550 ) 
				continue;
		
		if ( correlate ){	//to check!
			
			float min = FLT_MAX;
			int index = 0;
			for( int j=i+1; j<prevNumFoundFeatures; j++ ) {
				float dist = sqrtf( powf( results[i].origin.x - newResults[j].origin.x, 2.0) + powf( results[i].origin.y - newResults[j].origin.y, 2.0) );
				if (dist < min){
					min = dist;
					index = j;
					if (dist == 0) continue;
				}
			}
			
			// our ghetto way of cancelling out the taken points (put them in the < (i+i) side of the array)
			flow temp = newResults[index];
			newResults[index] = newResults[i];
			newResults[i] = temp;
			results[i] = temp;
		
		}else{
			results[i] = newResults[i];
		}
	}
	
	prevNumFoundFeatures = numFoundFeatures;
	cvCopy(currentImg, prevImg);
	
}


void OpticalFlow::setQuality( float quality_ ){	quality = quality_; }
float OpticalFlow::getQuality(){ return quality; }

void OpticalFlow::setMinDist( float dist ) { minDist = dist; }
float OpticalFlow::getMinDist() {return minDist; }

void OpticalFlow::setNumFeatures( int num ){ 
	if (num < MAX_FEATURES )
		numFeaturesToTrack = num;
	else
		numFeaturesToTrack = MAX_FEATURES;
}

int OpticalFlow::getNumFeatures(){ return numFeaturesToTrack; }

void OpticalFlow::setWindowSize( int winSize ){ windowSize = winSize; }
int OpticalFlow::getWindowSize(){ return windowSize; }

void OpticalFlow::setEqualize( bool eq ){ equalize = eq; }
bool OpticalFlow::getEqualize(){ return equalize; }

void OpticalFlow::setCorrelate( bool corr ){ correlate = corr; }
bool OpticalFlow::getCorrelate(){ return correlate; }

int OpticalFlow::getFoundNumFeatures(){ return numFoundFeatures; }

flow* OpticalFlow::getResults(){ return &results[0]; }
