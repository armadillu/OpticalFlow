/*
 *  OpticalFlow.h
 *  openFrameworks
 *
 *  Created by Oriol Ferrer Mesià on 7/30/10.
 *  Copyright 2010 uri.cat. All rights reserved.
 *
 */

#pragma once

#include "cv.h"

#define	MAX_FEATURES	400


typedef struct{	float x; float y; }p2;
typedef struct{ p2 origin; p2 destination; p2 direction; }flow;


class OpticalFlow{

	public:
	
		OpticalFlow(int w, int h);
		~OpticalFlow();
		
		void update(IplImage * newFrame);	//expects a 1 channel (grayscale) iplimage
		
		void setQuality( float quality_ );
		float getQuality();

		void setMinDist( float dist );
		float getMinDist();
		
		void setNumFeatures( int num );
		int getNumFeatures();

		void setWindowSize( int winSize );
		int getWindowSize();
		
		void setEqualize( bool eq );
		bool getEqualize();

		void setCorrelate( bool corr );
		bool getCorrelate();

		flow* getResults();

	private:
	
		IplImage * currentImg;
		IplImage * prevImg;
		IplImage * tmp;
		IplImage * eig;
	
		IplImage* pyrA;
		IplImage* pyrB;
	
		int		w, h;
	
		int		windowSize;
		int		numFeaturesToTrack;
		float	quality;
		float	minDist;
	
		bool	equalize;
		bool	correlate;
	
		CvPoint2D32f cornersA[ MAX_FEATURES ];
		CvPoint2D32f cornersB[ MAX_FEATURES ];
	
		char features_found[ MAX_FEATURES ];
		float feature_errors[ MAX_FEATURES ];

		flow results[ MAX_FEATURES ];
		flow newResults[ MAX_FEATURES ];

		//holds (0,0)
		p2 zeroPoint;

};

