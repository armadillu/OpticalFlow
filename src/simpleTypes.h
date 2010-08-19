/*
 *  simpleTypes.h
 *  opencvExample
 *
 *  Created by Oriol Ferrer Mesià on 8/1/10.
 *  Copyright 2010 uri.cat. All rights reserved.
 *
 */

#ifndef SIMPLE_TYPES
#define SIMPLE_TYPES


typedef struct{	float x; float y; }p2;
typedef struct{ p2 origin; p2 destination; p2 direction; float len;}flow;


#endif

