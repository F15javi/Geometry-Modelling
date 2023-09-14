////////////////////////////////////////////////////////////////////
//
//	Georg Umlauf, (c) 2012
//
////////////////////////////////////////////////////////////////////
#pragma once

#include "AffineGeometry.h"
#include "AffineMap.h"

class Quaternion 
{
	
public:
	double Re;	// real      part
	Vector Im;	// imaginary part

	// constructors/destructors
	Quaternion(double Real, Vector Imaginary);									// default constructor
	Quaternion(double Real, double i, double j, double k);
	Quaternion();

	~Quaternion();

	// further necessary methods and operations...
	Quaternion operator + (const Quaternion& quat) const;
	Quaternion operator * (const Quaternion& quat) const;
	double Product(Quaternion q1, Quaternion q2);
	Quaternion conjugate() const;
	double magnitude() const;
	Quaternion normalize() const;



};