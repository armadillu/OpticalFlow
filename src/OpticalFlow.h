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
#include "simpleTypes.h"

#define	MAX_FEATURES	500


class OpticalFlow{

	public:
	
		OpticalFlow(int w, int h);
		~OpticalFlow();
		
		void update(IplImage * newFrame);	//expects a 1 channel (grayscale) iplimage
		
		void setQuality( float quality_ );
		float getQuality();

		void setMinDist( float dist );
		float getMinDist();
		
		void setNumFeatures( int num );	//features to look for
		int getNumFeatures();			

		int getFoundNumFeatures();	//number of features returned by opencv

		void setWindowSize( int winSize );
		int getWindowSize();
		
		void setEqualize( bool eq );
		bool getEqualize();

		void setCorrelate( bool corr );	//correlate cross-finds the current motion vectors
										//with the previous ones, so indexes stay consistent
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
		int		numFoundFeatures;
		int		prevNumFoundFeatures;
	
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

