////////////////////////////////////////////////////////////////////
//
//	Georg Umlauf, (c) 2012
//
////////////////////////////////////////////////////////////////////

#include "quaternion.h"
#include <iostream>

Quaternion::Quaternion(double Real, Vector Imaginary)
{
	// to be done
	Re = Real;
	Im = Imaginary;


}
Quaternion::Quaternion(double Real, double i, double j, double k)
{
	// to be done
	Re = Real;
	Vector aux = (i, j, k);

	Im = aux;


}
Quaternion::Quaternion() {

	Re = 0.0;
	Vector aux = (0, 0, 0);
	Im = aux;

}


Quaternion::~Quaternion()
{
}


Quaternion Quaternion::operator + (const Quaternion& quat) const{

	Quaternion res;

	res.Re = Re + quat.Re;

	res.Im = Im + quat.Im;

	

	return res;
}

Quaternion Quaternion::operator * (const Quaternion& quat) const {

	Quaternion res;



	res.Re = Re * quat.Re - Im * quat.Im;
	Vector v = Im ^ quat.Im;
	res.Im = quat.Im * (Re)+Im * (quat.Re);
	res.Im +=v;



	return res;

}
	
Quaternion Quaternion::conjugate() const{

	Quaternion Q = *this;
	Vector I = -Q.Im;

	Q.Im = I;
	return Q;
}
double Quaternion::magnitude() const {

	
	//Quaternion aux = q.operator*(conjugate(q));

	double v1 = Im[0] * Im[0];
	double v2 = Im[1] * Im[1];
	double v3 = Im[2] * Im[2];


	return sqrt(Re * Re + v1 + v2 + v3);;

}
Quaternion Quaternion::normalize() const {

	Quaternion Q;
	Vector Qv;

	double d = this->magnitude();

	double v1 = Im[0] / d;
	double v2 = Im[1] / d;
	double v3 = Im[2] / d;
	Qv = (v1, v2, v3);
	
	Q.Re = Re / d;
	Q.Im = Qv;
	return Q;
	
}

