#include "MathLib/Vector2D.h"
#include <cmath>

/******************************************************************************/
/*!
	Default constructor for Vector2D
*/
/******************************************************************************/
Vector2D::Vector2D() : x{ 0.0f }, y{ 0.0f }
{}

/******************************************************************************/
/*!
	Constructor for Vector2D
*/
/******************************************************************************/
Vector2D::Vector2D(float _x, float _y) : x{ _x }, y{ _y }
{}

Vector2D::Vector2D(const Vector2D& rhs) :
	Vector2D{rhs.x, rhs.y}
{
}

Vector2D& Vector2D::operator=(const Vector2D& rhs) {
	if (this != &rhs) {
		x = rhs.x;
		y = rhs.y;
	}

	return *this;
}

/******************************************************************************/
/*!
	Compund addition operator for Vector2D
*/
/******************************************************************************/
Vector2D& Vector2D::operator+=(const Vector2D& rhs)
{
	// Computes the sum of 2 Vector2D's
	x = x + rhs.x;
	y = y + rhs.y;

	return *this;
}

/******************************************************************************/
/*!
	Compound subtraction operator for Vector2D
*/
/******************************************************************************/
Vector2D& Vector2D::operator-=(const Vector2D& rhs)
{
	// Computes the subtracted result of 2 Vector2D's
	x = x - rhs.x;
	y = y - rhs.y;

	return *this;
}

/******************************************************************************/
/*!
	Compound multiplication operator for Vector2D
*/
/******************************************************************************/
Vector2D& Vector2D::operator*=(float rhs)
{
	// Computes the result of Vector2D * float
	x = x * rhs;
	y = y * rhs;

	return *this;
}

/******************************************************************************/
/*!
	Compound division operator for Vector2D
*/
/******************************************************************************/
Vector2D& Vector2D::operator/=(float rhs)
{
	// Computes the result of Vector2D / float
	if (rhs) {
		x = x / rhs;
		y = y / rhs;
	}

	return *this;
}

/******************************************************************************/
/*!
	Unary minus operator for Vector2D
*/
/******************************************************************************/
Vector2D Vector2D::operator-() const
{
	// Inverts the sign of the vector
	Vector2D temp{ -x, -y };

	return temp;
}

/******************************************************************************/
/*!
	Binary addition operator for Vector2D
*/
/******************************************************************************/
Vector2D operator+(const Vector2D& lhs, const Vector2D& rhs)
{
	Vector2D temp;

	// Computes the sum of Vector2D + Vector2D
	temp.x = lhs.x + rhs.x;
	temp.y = lhs.y + rhs.y;

	return temp;
}

/******************************************************************************/
/*!
	Binary subtraction operator for Vector2D
*/
/******************************************************************************/
Vector2D operator-(const Vector2D& lhs, const Vector2D& rhs)
{
	Vector2D temp;

	// Computes the result of Vector2D - Vector2D
	temp.x = lhs.x - rhs.x;
	temp.y = lhs.y - rhs.y;

	return temp;
}

/******************************************************************************/
/*!
	Binary multiplication operator for Vector2D
*/
/******************************************************************************/
Vector2D operator*(const Vector2D& lhs, float rhs)
{
	Vector2D temp;

	// Computes the result of Vector2D * float
	temp.x = lhs.x * rhs;
	temp.y = lhs.y * rhs;

	return temp;
}

/******************************************************************************/
/*!
	Binary multiplication operator for Vector2D (Different sequence)
*/
/******************************************************************************/
Vector2D operator*(float lhs, const Vector2D& rhs)
{
	Vector2D temp;

	// Computes the result of Vector2D * float
	temp = rhs * lhs;

	return temp;
}

/******************************************************************************/
/*!
	Binary division operator for Vector2D
*/
/******************************************************************************/
Vector2D operator/(const Vector2D& lhs, float rhs)
{
	if (rhs) {
		Vector2D temp;

		// Computes the result of Vector2D / float
		temp.x = lhs.x / rhs;
		temp.y = lhs.y / rhs;

		return temp;
	}
	return lhs;
}

/******************************************************************************/
/*!
	Computes normalized vector and assigns result to pResult
*/
/******************************************************************************/
void Vector2DNormalize(Vector2D& pResult, const Vector2D& pVec0)
{
	// Computes the normalized vector (If not / 0)
	if (Vector2DLength(pVec0))
		pResult = pVec0 / Vector2DLength(pVec0);
}

/******************************************************************************/
/*!
	Computes the length of the vector pVec0 and returns the result
*/
/******************************************************************************/
float Vector2DLength(const Vector2D& pVec0)
{
	// Computes the length of pVec0
	return sqrt((pVec0.x * pVec0.x) + (pVec0.y * pVec0.y));
}

/******************************************************************************/
/*!
	Computes the square of pVec0's length and returns the result
*/
/******************************************************************************/
float Vector2DSquareLength(const Vector2D& pVec0)
{
	// Computes the square of the length of pVec0
	return ((pVec0.x * pVec0.x) + (pVec0.y * pVec0.y));
}

/******************************************************************************/
/*!
	Computes the distance between pVec0 and pVec1 and returns the result
*/
/******************************************************************************/
float Vector2DDistance(const Vector2D& pVec0, const Vector2D& pVec1)
{
	Vector2D vector;

	// Computes the distance between pVec0 and pVec1
	vector = pVec1 - pVec0;

	// Returns the distance
	return Vector2DLength(vector);
}

/******************************************************************************/
/*!
	Computes the squared distance between pVec0 and pVec1 and returns the result
*/
/******************************************************************************/
float Vector2DSquareDistance(const Vector2D& pVec0, const Vector2D& pVec1)
{
	Vector2D vector;

	// Computes the distance between pVec0 and pVec1
	vector = pVec1 - pVec0;

	// Returns the squared distance
	return Vector2DSquareLength(vector);
}

/******************************************************************************/
/*!
	Computes the result of the dot product between pVec0 and pVec1 and returns
	the result
*/
/******************************************************************************/
float Vector2DDotProduct(const Vector2D& pVec0, const Vector2D& pVec1)
{
	// Computes the dot product between pVec0 and pVec1
	float magnitude = (pVec0.x * pVec1.x) + (pVec0.y * pVec1.y);

	return magnitude;
}

/******************************************************************************/
/*!
	Computes the result of the cross product between pVec0 and pVec1 and returns
	the result
*/
/******************************************************************************/
float Vector2DCrossProductMag(const Vector2D& pVec0, const Vector2D& pVec1)
{
	// Computes the cross product between pVec0 and pVec1
	float magnitude = (pVec0.x * pVec1.y) - (pVec0.y * pVec1.x);

	return magnitude;
}