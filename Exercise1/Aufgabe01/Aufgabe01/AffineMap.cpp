////////////////////////////////////////////////////////////////////
//
//	Georg Umlauf, (c) 2020
//
////////////////////////////////////////////////////////////////////

#include "AffineMap.h"

// constructors
AffineMap::AffineMap (double t) 
{
	for (int j=0; j<4; j++) 
		for (int i=0; i<4; i++) 
			m_aatData[i][j] = (i==j)? t: 0.0;
}

AffineMap::AffineMap (const double aatData[4][4]) 
{
	for (int j=0; j<4; j++) 
		for (int i=0; i<4; i++) 
			m_aatData[i][j] = aatData[i][j];
}

AffineMap::AffineMap (const AffineMap &mat) 
{
	for (int j=0; j<4; j++) 
		for (int i=0; i<4; i++) 
			m_aatData[i][j] = mat(i,j);
}

// destructor	
AffineMap::~AffineMap () 
{	// nothing to do here ...
}

// index operations and related methods
double &AffineMap::operator () (unsigned i, unsigned j) 
{
	return m_aatData[i%4][j%4];
}

double AffineMap::operator () (unsigned i, unsigned j) const 
{
	return m_aatData[i%4][j%4];
}

void AffineMap::swap (unsigned i, unsigned j) 
{
	i %= 4;
	j %= 4;
	double tmp      = m_aatData[i][j];
	m_aatData[i][j] = m_aatData[j][i];
	m_aatData[j][i] = tmp;
}

// arithmetic operations
AffineMap AffineMap::operator + (const AffineMap &mat) const
{
	AffineMap buf(0.0);
	for (int i=0; i<4; i++) 
		for (int j=0; j<4; j++) 
			buf(i,j) = m_aatData[i][j] + mat.m_aatData[i][j];
	return buf;
}

AffineMap AffineMap::operator - (const AffineMap &mat) const
{
	AffineMap buf(0.0);
	for (int i=0; i<4; i++) 
		for (int j=0; j<4; j++) 
			buf(i,j) = m_aatData[i][j] - mat.m_aatData[i][j];
	return buf;
}

AffineMap AffineMap::operator * (const AffineMap &mat)  const
{
	AffineMap buf(0.0);
	for (int i=0; i<4; i++)			// row i
		for (int j=0; j<4; j++)		// column j
			for (int k=0; k<4; k++)	// k
				buf(i,j) += m_aatData[i][k] * mat.m_aatData[k][j];
	return buf;
}

Vector AffineMap::operator * (const Vector &vec)  const
{
	Vector buf(0,0,0,0);
	for (int j=0; j<4; j++)		// column j
		for (int i=0; i<4; i++)	// row i
			buf[i] += m_aatData[i][j] * vec[j];
	return buf;
}

Point AffineMap::operator * (const Point &pnt)  const
{
	Point buf(0,0,0,0);
	for (int j=0; j<4; j++)		// column j
		for (int i=0; i<4; i++)	// row i
			buf[i] += m_aatData[i][j] * pnt[j];
	return buf;
}

// matrix operations
void AffineMap::transpose()
{ // transpose submatrix (a_ij) for i,j = r,...,l.
	for( int i=0; i<4; i++)				// row i
		for (int j=i+1; j<4; j++)	   // column j
			swap(i,j);
}

void AffineMap::inverse()
// Computes the inverse of a 4x4 matrix M of the form 
//             ⎡ R  P ⎤
//         M = ⎢      ⎥,
//             ⎣ 0  1 ⎦
// with an orthonormal 3x3 matrix R, a 3d-column vector P and a 3d-row vector 0=(0,0,0).
//
// The inverse if computed as
//            ⎡ Rᵗ -Rᵗ·P ⎤
//      M⁻¹ = ⎢          ⎥ .
//            ⎣  0   1   ⎦
//
// The steps in the computation are 
//			 ⎡ R  P ⎤		⎡ R  0 ⎤		⎡ Rᵗ  0 ⎤		⎡ Rᵗ -Rᵗ·P ⎤
//		M = ⎢      ⎥	⇒¹	⎢      ⎥	⇒²	⎢       ⎥	⇒³	⎢          ⎥ = M⁻¹ .
//			 ⎣ 0  1 ⎦		⎣ 0  1 ⎦		⎣ 0   1 ⎦		⎣  0   1   ⎦
//
{
	AffineMap&   M = *this;
	CoordTuple4d P = M.getCol(3);	// copy P from M
	CoordTuple4d b(0,0,0,1);

	M.setCol   (     b, 3);			// step ⇒¹: remove P from M		-> the upper left 3x3 block of M is R
	M.transpose(         );			// step ⇒²: transpose M				-> the upper left 3x3 block of M is Rᵗ 
	M.setCol   (-(M*P), 3);			// step ⇒³: insert -Rᵗ·P into M	-> M⁻¹
}

// row/column operations
void AffineMap::setRow(const CoordTuple4d& r, int i) 
{
	for (int j=0; j<4; j++) m_aatData[i%4][j] = r[j]; // column j
}

void AffineMap::setCol(const CoordTuple4d& c, int j) 
{
	for (int i=0; i<4; i++) m_aatData[i][j%4] = c[i];// row i
}

CoordTuple4d AffineMap::getCol(int j) const
{
	Vector vec;
	for (int i=0; i<4; i++) vec[i] = m_aatData[i][j%4]; // row i
	return vec;
}

CoordTuple4d AffineMap::getRow(int i) const
{
	Vector vec;
	for (int j=0; j<4; j++) vec[j] = m_aatData[i%4][j]; // column j
	return vec;
}

// output
std::ostream &operator<< (std::ostream &ostr, const AffineMap &u)
{	
	ostr << "Affine Map:\n";
	for (int i = 0; i < 4; i++) {
		ostr << "[";
		for (int j = 0; j < 4; j++) {
			ostr << u(i, j);
			if (j != 3) ostr << ", ";
		}
		ostr << "]\n";
	}
	return ostr;
}