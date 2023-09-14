////////////////////////////////////////////////////////////////////
//
//	Georg Umlauf, (c) 2012
//
////////////////////////////////////////////////////////////////////

#include "Quader.h"
#include "glut.h"
#define M_PI           90

Point EyePoint(0, 0, 0, 1);	// EyePoint
Vector ViewDir(0, 0, -1, 0);	// ViewDir
Vector ViewUp(0, 1, 0, 0); // ViewUp


// constructors
Quader::Quader()
{
}

Quader::Quader(const Point& l, const Point& u)
{
	for (int i=0, j=0; i<4; i++, j=(i>1)?2:0) {
		q[2*i  ][0] = l[0]; q[i+j  ][1] = l[1]; q[i  ][2] = l[2];  q[i  ][3] = 1;
		q[2*i+1][0] = u[0]; q[i+j+2][1] = u[1]; q[i+4][2] = u[2];  q[i+4][3] = 1;
	}
}

// destructor
Quader::~Quader()
{
}

// setter
void Quader::setData(const Point& l, const Point& u)
{
	Quader buf(l,u);
	*this = buf;
}

inline void glColor3f (const Color& c                  ) { glColor3f(c.r, c.g, c.b); }
inline void glVertex2f(const Point& Q                  ) { glVertex2f(Q[0], Q[1]); }
inline void glLine2f  (const Point& Q1, const Point& Q2) { glVertex2f(Q1); glVertex2f(Q2); }

// output
void Quader::draw(const viewSystem &view, Color col) const 
{
   AffineMap mat = view.getWorldToView();

	Point Q[8];
	for (int i=0; i<8; i++) Q[i] = view.Project(mat*q[i]);	// transform and project corners
	
	// draw all 12 edges
   glColor3f(col);
   glBegin  (GL_LINES);
   glLine2f (Q[0],Q[1]);
   glLine2f (Q[0],Q[2]);
   glLine2f (Q[0],Q[4]);
   glLine2f (Q[1],Q[5]);
   glLine2f (Q[1],Q[3]);
   glLine2f (Q[2],Q[3]);
   glLine2f (Q[2],Q[6]);
   glLine2f (Q[3],Q[7]);
   glLine2f (Q[4],Q[5]);
   glLine2f (Q[4],Q[6]);
   glLine2f (Q[5],Q[7]);
   glLine2f (Q[6],Q[7]);
   glEnd    ();
}
//	

void Quader::LERP(const viewSystem& view, Color col, double t) const
{
	AffineMap mat = view.getWorldToView();


	


	Point p1[8];
	Point p2[8];
	Point lerp[8];
	for (int i = 0; i < 8; i++) {

		p1[i] = getWorldToView() * q[i];
		p2[i] = mat * (q[i]);

		lerp[i] = (p1[i] * (1 - t)) + (p2[i] * t);

		lerp[i] = view.Project(lerp[i]);	// transform and project corners
		p1[i] = view.Project(p1[i]);
		p2[i] = view.Project(p2[i]);

	}
	glColor3f(col);
	glBegin(GL_LINES);
	glLine2f(p1[0], p1[1]);
	glLine2f(p1[0], p1[2]);
	glLine2f(p1[0], p1[4]);
	glLine2f(p1[1], p1[5]);
	glLine2f(p1[1], p1[3]);
	glLine2f(p1[2], p1[3]);
	glLine2f(p1[2], p1[6]);
	glLine2f(p1[3], p1[7]);
	glLine2f(p1[4], p1[5]);
	glLine2f(p1[4], p1[6]);
	glLine2f(p1[5], p1[7]);
	glLine2f(p1[6], p1[7]);
	glEnd();

	glColor3f(col);
	glBegin(GL_LINES);
	glLine2f(p2[0], p2[1]);
	glLine2f(p2[0], p2[2]);
	glLine2f(p2[0], p2[4]);
	glLine2f(p2[1], p2[5]);
	glLine2f(p2[1], p2[3]);
	glLine2f(p2[2], p2[3]);
	glLine2f(p2[2], p2[6]);
	glLine2f(p2[3], p2[7]);
	glLine2f(p2[4], p2[5]);
	glLine2f(p2[4], p2[6]);
	glLine2f(p2[5], p2[7]);
	glLine2f(p2[6], p2[7]);
	glEnd();

	glColor3f(1,1,1);
	glBegin(GL_LINES);
	glLine2f(lerp[0], lerp[1]);
	glLine2f(lerp[0], lerp[2]);
	glLine2f(lerp[0], lerp[4]);
	glLine2f(lerp[1], lerp[5]);
	glLine2f(lerp[1], lerp[3]);
	glLine2f(lerp[2], lerp[3]);
	glLine2f(lerp[2], lerp[6]);
	glLine2f(lerp[3], lerp[7]);
	glLine2f(lerp[4], lerp[5]);
	glLine2f(lerp[4], lerp[6]);
	glLine2f(lerp[5], lerp[7]);
	glLine2f(lerp[6], lerp[7]);
	glEnd();

}
void Quader::SLERP(const viewSystem& view, Color col, double t) const
{
	AffineMap mat = view.getWorldToView();


	


	Point Q[8];
	Point p1[8];
	Point p2[8];

	Point slerp[8];


	for (int i = 0; i < 8; i++) {
				
		
		p1[i] = getWorldToView() * q[i];
		p2[i] = mat * q[i];




		Quaternion Q1;
		Q1.Re = 0;
		Q1.Im[0] = p1[i][0];
		Q1.Im[1] = p1[i][1];
		Q1.Im[2] = p1[i][2];

		Quaternion Q3 = Q1.normalize();
		Quaternion Q2;
		Q2.Re = 0;
		Q2.Im[0] = p2[i][0];
		Q2.Im[1] = p2[i][1];
		Q2.Im[2] = p2[i][2];
		Quaternion Q4 = Q2.normalize();
		Vector aux;
		double Theta = acos(Q3.Im * Q4.Im);
		//double sinHalfTheta = sqrt(1.0 - cosHalfTheta * cosHalfTheta);


		printf("%f\n", Theta);
		aux = Q3.Im * (sin((1 - t) * Theta) / sin(Theta)) + Q4.Im * (sin(t * Theta) / sin(Theta));
	

		Q[i][0] = aux[0];
		Q[i][1] = aux[1];
		Q[i][2] = aux[2];




		slerp[i] = view.Project(mat * Q[i]);	// transform and project corners
		p1[i] = view.Project(p1[i]);
		p2[i] = view.Project(p2[i]);
	}

	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
	glLine2f(p1[0], p1[1]);
	glLine2f(p1[0], p1[2]);
	glLine2f(p1[0], p1[4]);
	glLine2f(p1[1], p1[5]);
	glLine2f(p1[1], p1[3]);
	glLine2f(p1[2], p1[3]);
	glLine2f(p1[2], p1[6]);
	glLine2f(p1[3], p1[7]);
	glLine2f(p1[4], p1[5]);
	glLine2f(p1[4], p1[6]);
	glLine2f(p1[5], p1[7]);
	glLine2f(p1[6], p1[7]);
	glEnd();

	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glLine2f(p2[0], p2[1]);
	glLine2f(p2[0], p2[2]);
	glLine2f(p2[0], p2[4]);
	glLine2f(p2[1], p2[5]);
	glLine2f(p2[1], p2[3]);
	glLine2f(p2[2], p2[3]);
	glLine2f(p2[2], p2[6]);
	glLine2f(p2[3], p2[7]);
	glLine2f(p2[4], p2[5]);
	glLine2f(p2[4], p2[6]);
	glLine2f(p2[5], p2[7]);
	glLine2f(p2[6], p2[7]);
	glEnd();


	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glLine2f(slerp[0], slerp[1]);
	glLine2f(slerp[0], slerp[2]);
	glLine2f(slerp[0], slerp[4]);
	glLine2f(slerp[1], slerp[5]);
	glLine2f(slerp[1], slerp[3]);
	glLine2f(slerp[2], slerp[3]);
	glLine2f(slerp[2], slerp[6]);
	glLine2f(slerp[3], slerp[7]);
	glLine2f(slerp[4], slerp[5]);
	glLine2f(slerp[4], slerp[6]);
	glLine2f(slerp[5], slerp[7]);
	glLine2f(slerp[6], slerp[7]);
	glEnd();

}
void Quader::NLERP(const viewSystem& view, Color col, double t) const
{
	AffineMap mat = view.getWorldToView();





	Point Q[8];
	Point p1[8];
	Point p2[8];

	Point nlerp[8];

	double degre = t * 180 / M_PI;

	for (int i = 0; i < 8; i++) {


		p1[i] = getWorldToView() * q[i];
		p2[i] = mat * (q[i]);




		Quaternion Q1;
		Q1.Re = 0;
		Q1.Im[0] = p1[i][0];
		Q1.Im[1] = p1[i][1];
		Q1.Im[2] = p1[i][2];
		Q1.normalize();
		Quaternion Q2;
		Q2.Re = 0;
		Q2.Im[0] = p2[i][0];
		Q2.Im[1] = p2[i][1];
		Q2.Im[2] = p2[i][2];
		Q2.normalize();
		Vector aux;
		double Theta = acos(Q1.Im * Q2.Im);
		//double sinHalfTheta = sqrt(1.0 - cosHalfTheta * cosHalfTheta);


		printf("%f\n", Theta);
		aux = Q1.Im * (sin((1 - t) * Theta) / sin(Theta)) + Q2.Im * (sin(t * Theta) / sin(Theta));

		Q[i][0] = aux[0];
		Q[i][1] = aux[1];
		Q[i][2] = aux[2];




		nlerp[i] = view.Project(mat * Q[i]);	// transform and project corners
		p1[i] = view.Project(p1[i]);
		p2[i] = view.Project(p2[i]);
	}

	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
	glLine2f(p1[0], p1[1]);
	glLine2f(p1[0], p1[2]);
	glLine2f(p1[0], p1[4]);
	glLine2f(p1[1], p1[5]);
	glLine2f(p1[1], p1[3]);
	glLine2f(p1[2], p1[3]);
	glLine2f(p1[2], p1[6]);
	glLine2f(p1[3], p1[7]);
	glLine2f(p1[4], p1[5]);
	glLine2f(p1[4], p1[6]);
	glLine2f(p1[5], p1[7]);
	glLine2f(p1[6], p1[7]);
	glEnd();

	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glLine2f(p2[0], p2[1]);
	glLine2f(p2[0], p2[2]);
	glLine2f(p2[0], p2[4]);
	glLine2f(p2[1], p2[5]);
	glLine2f(p2[1], p2[3]);
	glLine2f(p2[2], p2[3]);
	glLine2f(p2[2], p2[6]);
	glLine2f(p2[3], p2[7]);
	glLine2f(p2[4], p2[5]);
	glLine2f(p2[4], p2[6]);
	glLine2f(p2[5], p2[7]);
	glLine2f(p2[6], p2[7]);
	glEnd();


	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glLine2f(nlerp[0], nlerp[1]);
	glLine2f(nlerp[0], nlerp[2]);
	glLine2f(nlerp[0], nlerp[4]);
	glLine2f(nlerp[1], nlerp[5]);
	glLine2f(nlerp[1], nlerp[3]);
	glLine2f(nlerp[2], nlerp[3]);
	glLine2f(nlerp[2], nlerp[6]);
	glLine2f(nlerp[3], nlerp[7]);
	glLine2f(nlerp[4], nlerp[5]);
	glLine2f(nlerp[4], nlerp[6]);
	glLine2f(nlerp[5], nlerp[7]);
	glLine2f(nlerp[6], nlerp[7]);
	glEnd();

}
AffineMap Quader::getWorldToView() const
{
	AffineMap M;
	// AUFGABE01
	AffineMap Rt;


	Vector ViewHor = ViewDir ^ ViewUp;

	Rt.setRow(ViewDir, 0);
	Rt.setRow(ViewUp, 1);
	Rt.setRow(ViewHor, 2);
	Vector mul = Rt * EyePoint;

	M.setRow(ViewDir, 0);
	M.setRow(ViewUp, 1);
	M.setRow(ViewHor, 2);
	M.setCol(mul, 3);
	return M;
}