////////////////////////////////////////////////////////////////////
//
//	Georg Umlauf, (c) 2012
//
////////////////////////////////////////////////////////////////////
#pragma once

#include "Quaternion.h"
#include <iostream>

enum class RotationMode {	VIEW_MATRIX_MODE,		// Rotation als Matrix-Produkt 
							VIEW_FORMULA_MODE,		// Rotation über Formel für Matrix 
							VIEW_QUATERNION_MODE,	// Rotation mit Quaternionen 
							MAX_ROTATION_MODE};		// 


class viewSystem 
{
private:
	double       F = 1.0;							// focal length
	Point        EyePoint = Point (0,0,0,1);		// origin of view system
	Vector       ViewDir  = Vector(0,0,1,0);		// z-axis, the view direction
	Vector       ViewUp   = Vector(0,1,0,0);		// y-axis, the up-vector
	RotationMode RotMode  = RotationMode::VIEW_MATRIX_MODE;		// s.o.

	AffineMap RotationMatrix(Vector Vector, double angle);

public:
	// constructors
	viewSystem(                                                                      );
	viewSystem(const double eye[4], const double dir[4], const double up[4], double f);
	viewSystem(const Point  eye   , const Vector dir   , const Vector up   , double f);

	// destructor
	~viewSystem();

	// getter-/setter-methods
	void setData(const double eye[4], const double dir[4], const double up[4], double f);
	void setData(const Point  eye   , const Vector dir   , const Vector up   , double f);

	// rotation mode methods
   void         setMode (RotationMode mode);
   void         stepMode(                 );    // step rotation mode forward
   RotationMode getMode (                 ) const;

	// affine transforms of the view system
	void Zoom     (             double f    );
	void RotateX  (             double angle);	// rotate around the x-axis
	void RotateY  (             double angle);	// rotate around the y-axis
	void RotateZ  (             double angle);	// rotate around the z-axis
	void RotateUp (             double angle);	// rotate around ViewUp
	void RotateDir(             double angle);	// rotate around ViewDir
	void RotateHor(             double angle);	// rotate around ViewHor
	void Rotate   (const Vector    & axis, double angle);	// rotate around arbitrary axis
	void Rotate   (const AffineMap & mat               );	// rotate by rotation matrix mat
	void Rotate   (const Quaternion& q                 );	// rotate by quaternion quat
	void Translate(const Vector    & vec               );	// translate by vec


	// projection methods (in the view system coordinates)
	Point Project (Point Point) const;				// projection in the view system coordinates
	
	// view-to-world/world-to-view transformations
	AffineMap getViewToWorld() const;				// view to world
	AffineMap getWorldToView() const;				// world to view

	// assignment operators
	viewSystem &operator = (const viewSystem &vs);
};

std::ostream &operator<< (std::ostream &ostr, const viewSystem &u);