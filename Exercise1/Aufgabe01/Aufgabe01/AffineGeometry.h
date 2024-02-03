//-------------------------------------------------------------------
// 
// Affine Geometry
// 
// Class for affine points and vectors in homogeneous coordinates 
//
// (c) Georg Umlauf, 2014
// (c) Georg Umlauf, 2018: added const-specifiers
//
//-------------------------------------------------------------------

#pragma once

#include "Color.h"
#include <iostream>

// Forward-Deklarationen
template <class T,unsigned D> class CoordTuple;			// D-dimensional coordinate-tuple of type T  
class Point;											// affine point  in homogeneous coordinates
class Vector;											// affine vector in homogeneous coordinates

// D-coordinate tupel class ------------------
template <class T, unsigned D=2> class CoordTuple
{
protected:
	T c[D] = { 0.0 };

public:
	CoordTuple(T x=T(0), T y=T(0), T z=T(0), T w=T(0));
	CoordTuple(const            T       crd[D]);
	CoordTuple(const CoordTuple<T,D> &crd     );

	// index operations
	T& operator [] (unsigned i);
	T  operator [] (unsigned i) const;

	// assignment operations
	CoordTuple<T,D>& operator = (const CoordTuple<T,D> &crd);

	// comparison operations
	bool operator == (const CoordTuple<T,D> &crd) const;
	bool operator != (const CoordTuple<T,D> &crd) const;
};
// output
template <class T, unsigned D=2> std::ostream &operator<< (std::ostream &ostr, const CoordTuple<T,D> &u);

typedef CoordTuple<double, 4> CoordTuple4d;

// Point class -------------------------
class Point : public CoordTuple4d
{
	friend class Vector;
public:
	Point(const double p[4]) : CoordTuple(p) {}
	Point(double x=0.0, double y=0.0, double z=0.0, double w=1.0) : CoordTuple( x, y, z, w) {}
	Point(const Point  & p);                     // copy constructor

	// homogeneity-related methods
	void homogenize();									// this->c = this->c/c[3]

	// arithmetic operations
	void    operator += (const Vector &v);			// (*this) += v
	void    operator -= (const Vector &v);			// (*this) -= v
	Point   operator +  (const Vector &v) const;	// result = (*this) + v
	Point   operator -  (const Vector &v) const;	// result = (*this) - v
	Vector  operator -  (const Point  &p) const;	// result = (*this) - p
	Point   operator * (double s) const;

};

// Vector class ------------------------
class Vector : public CoordTuple4d
{
	friend class Point;
	friend class Line;
public:
	Vector(double x=0.0, double y=0.0, double z=0.0, double w=0.0) : CoordTuple(x, y, z, w ) {};
	Vector(const double        v[4]) : CoordTuple(v[0],v[1],v[2],v[3]) {};
	Vector(const CoordTuple4d& v   ) : CoordTuple(v[0],v[1],v[2],v[3]) {};
	Vector(const Vector&       v   ) : CoordTuple(v[0],v[1],v[2],v[3]) {};      // copy constructor

	// norm-related methods
	double  getNorm      () const;						// Euclidean norm of vector
	double  getNorm2     () const;						// squared Euclidean norm of vector 
	void    normalize    ();								// normalize     the vector

	// arithmetic operations
	void    operator += (const Vector &v);				// (*this) =  (*this) + v
	void    operator += (const Point  &p);				// (*this) =  (*this) + p
	void    operator -= (const Vector &v);				// (*this) =  (*this) - v
	Vector  operator +  (const Vector &v) const;		// result  =  (*this) + v
	Vector  operator -  (const Vector &v) const;		// result  =  (*this) - v
	Vector  operator -  (               ) const;		// result  = -(*this)
	void    operator *= (      double  s);				// (*this) =  (*this) * s
	void    operator /= (      double  s);				// (*this) =  (*this) / s
	Vector  operator *  (      double  s) const;		// result  =  (*this) * s
	Vector  operator /  (      double  s) const;		// result  =  (*this) / s
	double  operator *  (const Vector &v) const;		// result  =  (*this) * v
	Vector  operator ^  (const Vector &v) const;		// result  =  (*this) x v // Attention: "^"-opertor has lower priority than e.g. "+"-operator, i.e. use brackets!!!
};

// output
std::ostream &operator<< (std::ostream &ostr, const Point &u);
std::ostream &operator<< (std::ostream &ostr, const Vector &u);

// Methods of CoordTuple class ----
template <class T, unsigned D> CoordTuple<T,D>::CoordTuple(T x, T y, T z, T w)
{
	c[0] = x;
	c[1] = y;
	if (D > 2) {
		c[2] = z;
		c[3] = w;
		for (int i = 4; i < D; i++) c[i] = T(0);
	}
}

template <class T, unsigned D> CoordTuple<T,D>::CoordTuple(const T crd[D])
{
	for (int i=0; i<D; i++) c[i] = crd[i];
}

template <class T, unsigned D> CoordTuple<T,D>::CoordTuple(const CoordTuple<T,D>& crd)
{
	for (int i=0; i<D; i++) c[i] = crd.c[i];
}

template <class T, unsigned D> T& CoordTuple<T,D>::operator[](unsigned i) 
{	// access to i-th coordinate
	return c[i%D]; 		
}

template <class T, unsigned D> T CoordTuple<T,D>::operator[](unsigned i) const
{	// access to i-th coordinate
	return c[i%D]; 		
}

template <class T, unsigned D> CoordTuple<T,D>& CoordTuple<T,D>::operator=(const CoordTuple<T,D>& crd) 
{	
	if (this!=&crd) for (int i=0; i<D; i++) c[i] = crd.c[i];
	return *this; 		
}

template <class T, unsigned D> bool CoordTuple<T,D>::operator==(const CoordTuple<T,D>& crd) const 
{	
	if (this==&crd) return true;
	for (int i=0; i<D; i++) if (c[i]!=crd[i]) return false; // test for equality is problematic!
	return true; 		
}

template <class T, unsigned D> bool CoordTuple<T,D>::operator!=(const CoordTuple<T,D>& crd) const 
{	
	return !(*this==crd); 		
}

// output
template <class T, unsigned D> std::ostream &operator<< (std::ostream &ostr, const CoordTuple<T,D> &u)
{	
	ostr << "CoordTuple(";
	for (int i = 0; i < D; i++) {
		ostr << u[i];
		if (i != D - 1) ostr << ", "; 
	}
	ostr << ")\n";
	return ostr;
}
