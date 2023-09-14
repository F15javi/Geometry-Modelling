////////////////////////////////////////////////////////////////////
//
//	Georg Umlauf, (c) 2020
//
////////////////////////////////////////////////////////////////////
#pragma once

#include "AffineGeometry.h"

// Class for affine maps, represented as 4x4 matrices M of the form
//
//			   ⎡ R  P ⎤
//         M = ⎢      ⎥
//             ⎣ 0  1 ⎦
//
//  with an orthonormal 3x3 matrix R, a 3d-column vector P and a 3d-row vector 0=(0,0,0). 
class AffineMap 
{
private:
	double m_aatData[4][4] = { 0.0 };

public:
	// constructors
	AffineMap(      double    t=1.0  ); // default constructor: identity
	AffineMap(const double    c[4][4]); // construction with data array
	AffineMap(const AffineMap &mat   ); // copy constructor

	// destructor
	~AffineMap();

	// index operations and related methods
	double &operator () (unsigned i, unsigned j);
	double  operator () (unsigned i, unsigned j) const;
	void    swap		(unsigned i, unsigned j);

	// arithmetic operations
	AffineMap operator + (const AffineMap &mat) const;
	AffineMap operator - (const AffineMap &mat) const;
	AffineMap operator * (const AffineMap &mat) const;
	Vector    operator * (const Vector    &vec) const;
	Point     operator * (const Point     &pnt) const;

	// matrix operations
	void transpose(); // transpose 
	void inverse  (); // inverse 

	// row/column operations
	void         setRow(const CoordTuple4d& r, int i);
	void         setCol(const CoordTuple4d& c, int i);
	CoordTuple4d getRow(                       int i) const;
	CoordTuple4d getCol(                       int i) const;
};

// output
std::ostream &operator<< (std::ostream &ostr, const AffineMap &u);