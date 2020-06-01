#include "Matrix3x3.h"
#define _USE_MATH_DEFINES
#include <cmath>

/******************************************************************************/
/*!
	Default constructor for Matrix3x3
*/
/******************************************************************************/
Matrix3x3::Matrix3x3() : m00{ 0 }, m01{ 0 }, m02{ 0 },
								m10{ 0 }, m11{ 0 }, m12{ 0 },
								m20{ 0 }, m21{ 0 }, m22{ 0 }
{}

/******************************************************************************/
/*!
	Constructor for Matrix3x3
*/
/******************************************************************************/
Matrix3x3::Matrix3x3(const float* pArr) : m00{ pArr[0] }, m01{ pArr[1] }, m02{ pArr[2] },
m10{ pArr[3] }, m11{ pArr[4] }, m12{ pArr[5] },
m20{ pArr[6] }, m21{ pArr[7] }, m22{ pArr[8] }
{}

/******************************************************************************/
/*!
	Constructor for Matrix3x3 (Different parameters)
*/
/******************************************************************************/
Matrix3x3::Matrix3x3(float _00, float _01, float _02,
							float _10, float _11, float _12,
							float _20, float _21, float _22) : m00{ _00 }, m01{ _01 }, m02{ _02 },
															   m10{ _10 }, m11{ _11 }, m12{ _12 },
															   m20{ _20 }, m21{ _21 }, m22{ _22 }
{}

/******************************************************************************/
/*!
	Assignment operator for Matrix3x3
*/
/******************************************************************************/
Matrix3x3& Matrix3x3::operator=(const Matrix3x3& rhs)
{
	// Assigning rhs to "this"
	m00 = rhs.m00;
	m01 = rhs.m01;
	m02 = rhs.m02;
	m10 = rhs.m10;
	m11 = rhs.m11;
	m12 = rhs.m12;
	m20 = rhs.m20;
	m21 = rhs.m21;
	m22 = rhs.m22;

	return *this;
}

/******************************************************************************/
/*!
	Performs compound multiplication for Matrix3x3 and returns a reference to
	the result
*/
/******************************************************************************/
Matrix3x3& Matrix3x3::operator *= (const Matrix3x3& rhs)
{
	// Multiplying "this" with rhs
	m00 = (m00 * rhs.m00) + (m01 * rhs.m10) + (m02 * rhs.m20);
	m01 = (m00 * rhs.m01) + (m01 * rhs.m11) + (m02 * rhs.m21);
	m02 = (m00 * rhs.m02) + (m01 * rhs.m12) + (m02 * rhs.m22);
	m10 = (m10 * rhs.m00) + (m11 * rhs.m10) + (m12 * rhs.m20);
	m11 = (m10 * rhs.m01) + (m11 * rhs.m11) + (m12 * rhs.m21);
	m12 = (m10 * rhs.m02) + (m11 * rhs.m12) + (m12 * rhs.m22);
	m20 = (m20 * rhs.m00) + (m21 * rhs.m10) + (m22 * rhs.m20);
	m21 = (m20 * rhs.m01) + (m21 * rhs.m11) + (m22 * rhs.m21);
	m22 = (m20 * rhs.m02) + (m21 * rhs.m12) + (m22 * rhs.m22);

	return *this;
}

/******************************************************************************/
/*!
	Performs multiplication for Matrix3x3 and returns a copy of the result
*/
/******************************************************************************/
Matrix3x3 operator * (const Matrix3x3& lhs, const Matrix3x3& rhs)
{
	Matrix3x3 temp;

	// Multiplying "lhs" with "rhs" and storing the result in a temp variable
	temp.m00 = (lhs.m00 * rhs.m00) + (lhs.m01 * rhs.m10) + (lhs.m02 * rhs.m20);
	temp.m01 = (lhs.m00 * rhs.m01) + (lhs.m01 * rhs.m11) + (lhs.m02 * rhs.m21);
	temp.m02 = (lhs.m00 * rhs.m02) + (lhs.m01 * rhs.m12) + (lhs.m02 * rhs.m22);
	temp.m10 = (lhs.m10 * rhs.m00) + (lhs.m11 * rhs.m10) + (lhs.m12 * rhs.m20);
	temp.m11 = (lhs.m10 * rhs.m01) + (lhs.m11 * rhs.m11) + (lhs.m12 * rhs.m21);
	temp.m12 = (lhs.m10 * rhs.m02) + (lhs.m11 * rhs.m12) + (lhs.m12 * rhs.m22);
	temp.m20 = (lhs.m20 * rhs.m00) + (lhs.m21 * rhs.m10) + (lhs.m22 * rhs.m20);
	temp.m21 = (lhs.m20 * rhs.m01) + (lhs.m21 * rhs.m11) + (lhs.m22 * rhs.m21);
	temp.m22 = (lhs.m20 * rhs.m02) + (lhs.m21 * rhs.m12) + (lhs.m22 * rhs.m22);

	return temp;
}

/******************************************************************************/
/*!
	This operator multiplies the matrix pMtx with the vector rhs and returns
	the result as a Vector2D
*/
/******************************************************************************/
Vector2D  operator * (const Matrix3x3& pMtx, const Vector2D& rhs)
{
	Vector2D temp;

	// Multiplies pMtx (3x3) with rhs (2x1)
	temp.x = (pMtx.m00 * rhs.x) + (pMtx.m01 * rhs.y) + pMtx.m02;
	temp.y = (pMtx.m10 * rhs.x) + (pMtx.m11 * rhs.y) + pMtx.m12;

	return temp;
}

/******************************************************************************/
/*!
	This function sets the matrix pResult to the identity matrix
*/
/******************************************************************************/
void Mtx33Identity(Matrix3x3& pResult)
{
	// Assigns identity matrix to pResult
	pResult.m00 = 1; pResult.m01 = 0; pResult.m02 = 0;
	pResult.m10 = 0; pResult.m11 = 1; pResult.m12 = 0;
	pResult.m20 = 0; pResult.m21 = 0; pResult.m22 = 1;
}

/******************************************************************************/
/*!
	This function creates a translation matrix from x & y and saves it
	in pResult
*/
/******************************************************************************/
void Mtx33Translate(Matrix3x3& pResult, float x, float y)
{
	// Resets pResult
	Mtx33Identity(pResult);
	// Assigns x & y respectively to create translation matrix
	pResult.m02 = x;
	pResult.m12 = y;
}

/******************************************************************************/
/*!
	This function creates a scaling matrix from x & y and saves it
	in pResult
*/
/******************************************************************************/
void Mtx33Scale(Matrix3x3& pResult, float x, float y)
{
	//Resets pResult
	Mtx33Identity(pResult);
	// Assigns x & y respectively to create scaling matrix
	pResult.m00 = x;
	pResult.m11 = y;
}

/******************************************************************************/
/*!
	This matrix creates a rotation matrix from "angle" whose value is
	in radian. Saves the resultant matrix in pResult.
*/
/******************************************************************************/
void Mtx33RotRad(Matrix3x3& pResult, float angle)
{
	// Resets pResult
	Mtx33Identity(pResult);
	// Assigns angle respectively to create rotation matrix in rad
	pResult.m00 = cosf(angle);
	pResult.m01 = -sinf(angle);
	pResult.m10 = sinf(angle);
	pResult.m11 = cosf(angle);
}

/******************************************************************************/
/*!
	This matrix creates a rotation matrix from "angle" whose value is
	in degrees. Saves the resultant matrix in pResult.
*/
/******************************************************************************/
void Mtx33RotDeg(Matrix3x3& pResult, float angle)
{
	// Resets pResult
	Mtx33Identity(pResult);
	// Assigns angle respectively to create rotation matrix in degrees
	pResult.m00 = cosf(angle * static_cast<float>((M_PI / 180)));
	pResult.m01 = -sinf(angle * static_cast<float>((M_PI / 180)));
	pResult.m10 = sinf(angle * static_cast<float>((M_PI / 180)));
	pResult.m11 = cosf(angle * static_cast<float>((M_PI / 180)));
}

/******************************************************************************/
/*!
	This functions sets the transpose matrix of pMtx and saves
	it in pResult.
*/
/******************************************************************************/
void Mtx33Transpose(Matrix3x3& pResult, const Matrix3x3& pMtx)
{
	// Transposes pMtx and stores result in pResult
	pResult.m00 = pMtx.m00; pResult.m01 = pMtx.m10; pResult.m02 = pMtx.m20;
	pResult.m10 = pMtx.m01; pResult.m11 = pMtx.m11; pResult.m12 = pMtx.m21;
	pResult.m20 = pMtx.m02; pResult.m21 = pMtx.m12; pResult.m22 = pMtx.m22;
}

/******************************************************************************/
/*!
	This function calculates the inverse matrix of pMtx and saves the
	result in pResult. If the matrix inversion fails, pResult will be
	set to NULL.
*/
/******************************************************************************/
void Mtx33Inverse(Matrix3x3* pResult, float* determinant, const Matrix3x3& pMtx)
{
	// Compute the determinant of the matrix pMtx
	*determinant = (pMtx.m00 * ((pMtx.m11 * pMtx.m22) - (pMtx.m12 * pMtx.m21))) -
		(pMtx.m01 * ((pMtx.m10 * pMtx.m22) - (pMtx.m12 * pMtx.m20))) +
		(pMtx.m02 * ((pMtx.m10 * pMtx.m21) - (pMtx.m11 * pMtx.m20)));
	// If determinant is equal to 0, set pResult to NULL as there
	// does not exist an inverse matrix for pMtx
	if (!*determinant)
		pResult = NULL;
	else
	{
		Matrix3x3 Cofactor, temp;
		// Calculate the Co-Factor matrix
		Cofactor.m00 = ((pMtx.m11 * pMtx.m22) - (pMtx.m12 * pMtx.m21));
		Cofactor.m01 = -((pMtx.m10 * pMtx.m22) - (pMtx.m12 * pMtx.m20));
		Cofactor.m02 = ((pMtx.m10 * pMtx.m21) - (pMtx.m11 * pMtx.m20));
		Cofactor.m10 = -((pMtx.m01 * pMtx.m22) - (pMtx.m02 * pMtx.m21));
		Cofactor.m11 = ((pMtx.m00 * pMtx.m22) - (pMtx.m02 * pMtx.m20));
		Cofactor.m12 = -((pMtx.m00 * pMtx.m21) - (pMtx.m01 * pMtx.m20));
		Cofactor.m20 = ((pMtx.m01 * pMtx.m12) - (pMtx.m02 * pMtx.m11));
		Cofactor.m21 = -((pMtx.m00 * pMtx.m12) - (pMtx.m02 * pMtx.m10));
		Cofactor.m22 = ((pMtx.m00 * pMtx.m11) - (pMtx.m01 * pMtx.m10));

		// Transpose the matrix
		Mtx33Transpose(temp, Cofactor);

		// Calculating inverse matrix after transposing Co-Factor matrix
		// by multiplying it by (1 / determinant)
		pResult->m00 = (1 / *determinant) * temp.m00;
		pResult->m01 = (1 / *determinant) * temp.m01;
		pResult->m02 = (1 / *determinant) * temp.m02;
		pResult->m10 = (1 / *determinant) * temp.m10;
		pResult->m11 = (1 / *determinant) * temp.m11;
		pResult->m12 = (1 / *determinant) * temp.m12;
		pResult->m20 = (1 / *determinant) * temp.m20;
		pResult->m21 = (1 / *determinant) * temp.m21;
		pResult->m22 = (1 / *determinant) * temp.m22;
	}
}