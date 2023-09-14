#include "viewSystem.h"
#include <iostream>





Point Lerp(Point q, double t) 
{
	Point lerp;

	Point Q;


	lerp = (q * (1 - t)) + (Q * t);

	return lerp;
}