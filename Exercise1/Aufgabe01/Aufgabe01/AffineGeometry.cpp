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

#include "math.h"
#include "AffineGeometry.h"

//-------------------------------------------------
//
// Methods of CoordTuple class in .h-file
//
//-------------------------------------------------

//-------------------------------------------------
//
// Methods of Point class 
//
//-------------------------------------------------
Point::Point(const Point  & p) : CoordTuple(p[0],p[1],p[2],p[3]) {}    // cast constructor

void Point::homogenize()
{
	for (int i = 0; i < 3; i++) c[i] /= c[3];
	c[3] = 1.0;
}

void Point::operator += (const Vector &vec) 
{
	for (int i=0; i<4; i++) c[i] += vec.c[i]; 
}

Point Point::operator + (const Vector &vec) const
{
	return Point(c[0]+vec.c[0],c[1]+vec.c[1],c[2]+vec.c[2],c[3]+vec.c[3]);
}

void Point::operator -= (const Vector &vec) 
{
	for (int i=0; i<4; i++) c[i] -= vec.c[i];
}

Point Point::operator - (const Vector &vec) const
{
	return Point(c[0]-vec.c[0],c[1]-vec.c[1],c[2]-vec.c[2],c[3]-vec.c[3]);
}

Vector Point::operator - (const Point &pnt) const
{
	return Vector(c[0]-pnt.c[0],c[1]-pnt.c[1],c[2]-pnt.c[2],c[3]-pnt.c[3]);
}


Point Point::operator * (double s) const
{
	return Point(c[0] * s, c[1] * s, c[2] * s, c[3] * s);
}

//-------------------------------------------------
//
// Methods of Vector class
//
//-------------------------------------------------

double Vector::getNorm() const
{
	return sqrt(this->getNorm2());
} 

double Vector::getNorm2() const
{
	return c[0]*c[0]+c[1]*c[1]+c[2]*c[2]+c[3]*c[3];
}

void Vector::normalize()
{
	*this/=getNorm();
}

void Vector::operator += (const Vector &v) 
{
	for (int i=0; i<4; i++) c[i] += v.c[i]; 
}

void Vector::operator += (const Point &p) 
{
	for (int i=0; i<4; i++) c[i] += p.c[i]; 
}

Vector Vector::operator + (const Vector &vec)  const
{
	return Vector(c[0]+vec.c[0],c[1]+vec.c[1],c[2]+vec.c[2],c[3]+vec.c[3]);
}

void Vector::operator -= (const Vector &vec) 
{
	for (int i=0; i<4; i++) c[i] -= vec.c[i]; 
}

Vector Vector::operator - (const Vector &vec)  const
{
	return Vector(c[0]-vec.c[0],c[1]-vec.c[1],c[2]-vec.c[2],c[3]-vec.c[3]);
}

Vector Vector::operator - ()  const
{
	return Vector(-c[0],-c[1],-c[2],-c[3]);
}

void Vector::operator *= (double s) 
{
	for (int i=0; i<4; i++) c[i] *= s; 
}

Vector Vector::operator * (double s) const
{
	return Vector(c[0]*s,c[1]*s,c[2]*s,c[3]*s);
}

double Vector::operator * (const Vector &vec)  const
{
	double d = 0.0;
	for (int i=0; i<4; i++) d += c[i] * vec[i];
	return d;
}

Vector Vector::operator ^ (const Vector& vec)  const
{
	return Vector(	c[1] * vec[2] - c[2] * vec[1],
		c[2] * vec[0] - c[0] * vec[2],
		c[0] * vec[1] - c[1] * vec[0],
		0);
}

void Vector::operator /= (double s) 
{
	for (int i=0; i<4; i++) c[i] /= s; 
}

Vector Vector::operator / (double s) const
{
	return Vector(c[0]/s,c[1]/s,c[2]/s,c[3]/s);
}

// output
std::ostream &operator<< (std::ostream &ostr, const Point &u)
{	
	ostr << "point(" << u[0] << u[1] << u[2] << u[3] << ")" << std::endl;
	return ostr;
}

std::ostream &operator<< (std::ostream &ostr, const Vector &u)
{	
	ostr << "vector(" << u[0] << u[1] << u[2] << u[3] << ")" << std::endl;
	return ostr;
}