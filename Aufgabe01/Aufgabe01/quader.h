////////////////////////////////////////////////////////////////////
//
//	Georg Umlauf, (c) 2012
//
////////////////////////////////////////////////////////////////////
#pragma once

#include "color.h"
#include "viewSystem.h"

class Quader 
{
private:
	Point q[8];	                     // the eight corners of a quader in homogeneous coordinates

public:
	// constructors
	Quader(                              );
	Quader(const Point& l, const Point& u);         // lower left and upper right corner

																	// destructor
	~Quader();

	// setter methods
	void setData(const Point& l, const Point& u);   // lower left and upper right corner

																	// output
	void draw(const viewSystem &view, Color col) const;
	void LERP(const viewSystem& view, Color col, double t) const;
	void SLERP(const viewSystem& view, Color col, double t) const;
	void NLERP(const viewSystem& view, Color col, double t) const;

	AffineMap getWorldToView() const;

};