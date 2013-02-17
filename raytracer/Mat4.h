#ifndef Mat4_h__
#define Mat4_h__

#include "Vector.h"
#include "Globals.h"


/* Somewhat-complete 4x4 matrix class.
 * 
 * Has static methods for creating scale, translation,
 * identity, and rotation martices (about X,Y,Z).
 */
class Mat4
{
public:

	double m[4][4];

	Mat4()
	{
		m[0][0] = m[0][1] = m[0][2] = m[0][3] =
		m[1][0] = m[1][1] = m[1][2] = m[1][3] =
		m[2][0] = m[2][1] = m[2][2] = m[2][3] =
		m[3][0] = m[3][1] = m[3][2] = m[3][3] = 0;
	}

	Mat4(const Mat4 &b)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				m[i][j] = b.m[i][j];
	}

	Mat4(const double m00, const double m01, const double m02, const double m03,
		const double m10, const double m11, const double m12, const double m13,
		const double m20, const double m21, const double m22, const double m23,
		const double m30, const double m31, const double m32, const double m33)
	{
		m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
		m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
		m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
		m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
	}

	const Vector mulPoint(const Vector &v) const
	{
		return Vector(	m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * 1,
						m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * 1,
						m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * 1);
	}

	const Vector mulVector(const Vector &v) const
	{
		return Vector(	m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
						m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
						m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z);
	}

	const Mat4 inverse() const
	{
		Mat4 inv = adjoint();

		const double d = det();

		return inv * (1.0 / d);

	}

	const Mat4 adjoint() const
	{
		return (*this).cofactor().transpose();
	}

	const Mat4 transpose() const
	{
		return Mat4(m[0][0], m[1][0], m[2][0], m[3][0],
					m[0][1], m[1][1], m[2][1], m[3][1],
					m[0][2], m[1][2], m[2][2], m[3][2],
					m[0][3], m[1][3], m[2][3], m[3][3]);
	}

	const Mat4 cofactor() const
	{
		/*	m00 m01 m02 m03
			m10 m11 m12 m13
			m20 m21 m22 m23
			m30 m31 m32 m33 */

		const double c00 =	det3(m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]);
		const double c01 = -det3(m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]);
		const double c02 =	det3(m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]);
		const double c03 = -det3(m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]);

		const double c10 = -det3(m[0][1], m[0][2], m[0][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]);
		const double c11 =	det3(m[0][0], m[0][2], m[0][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]);
		const double c12 = -det3(m[0][0], m[0][1], m[0][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][2], m[3][3]);
		const double c13 =	det3(m[0][0], m[0][1], m[0][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]);

		const double c20 =	det3(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[3][1], m[3][2], m[3][3]);
		const double c21 = -det3(m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[3][0], m[3][2], m[3][3]);
		const double c22 =	det3(m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[3][0], m[3][1], m[3][3]);
		const double c23 = -det3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[3][0], m[3][1], m[3][2]);

		const double c30 = -det3(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3]);
		const double c31 =	det3(m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3]);
		const double c32 = -det3(m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3]);
		const double c33 =	det3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2]);

		return Mat4(c00, c01, c02, c03,
					c10, c11, c12, c13,
					c20, c21, c22, c23,
					c30, c31, c32, c33);
	}

	inline const double det() const
	{
		/*	m00 m01 m02 m03
			m10 m11 m12 m13
			m20 m21 m22 m23
			m30 m31 m32 m33 */

		const double	m00 = m[0][0],
						m01 = m[0][1],
						m02 = m[0][2],
						m03 = m[0][3],
						m10 = m[1][0],
						m11 = m[1][1],
						m12 = m[1][2],
						m13 = m[1][3],
						m20 = m[2][0],
						m21 = m[2][1],
						m22 = m[2][2],
						m23 = m[2][3],
						m30 = m[3][0],
						m31 = m[3][1],
						m32 = m[3][2],
						m33 = m[3][3];

		return	m00 * det3(m11, m12, m13, m21, m22, m23, m31, m32, m33) -
				m10 * det3(m01, m02, m03, m21, m22, m23, m31, m32, m33) +
				m20 * det3(m01, m02, m03, m11, m12, m13, m31, m32, m33) -
				m30 * det3(m01, m02, m03, m11, m12, m13, m21, m22, m23);

	}

	Mat4 &operator *= (const Mat4 &b)
	{
		Mat4 a(*this);

		m[0][0] = a.m[0][0] * b.m[0][0] + a.m[0][1] * b.m[1][0] + a.m[0][2] * b.m[2][0] + a.m[0][3] * b.m[3][0]; // row 0
		m[0][1] = a.m[0][0] * b.m[0][1] + a.m[0][1] * b.m[1][1] + a.m[0][2] * b.m[2][1] + a.m[0][3] * b.m[3][1];
		m[0][2] = a.m[0][0] * b.m[0][2] + a.m[0][1] * b.m[1][2] + a.m[0][2] * b.m[2][2] + a.m[0][3] * b.m[3][2];
		m[0][3] = a.m[0][0] * b.m[0][3] + a.m[0][1] * b.m[1][3] + a.m[0][2] * b.m[2][3] + a.m[0][3] * b.m[3][3];

		m[1][0] = a.m[1][0] * b.m[0][0] + a.m[1][1] * b.m[1][0] + a.m[1][2] * b.m[2][0] + a.m[1][3] * b.m[3][0]; // row 1
		m[1][1] = a.m[1][0] * b.m[0][1] + a.m[1][1] * b.m[1][1] + a.m[1][2] * b.m[2][1] + a.m[1][3] * b.m[3][1];
		m[1][2] = a.m[1][0] * b.m[0][2] + a.m[1][1] * b.m[1][2] + a.m[1][2] * b.m[2][2] + a.m[1][3] * b.m[3][2];
		m[1][3] = a.m[1][0] * b.m[0][3] + a.m[1][1] * b.m[1][3] + a.m[1][2] * b.m[2][3] + a.m[1][3] * b.m[3][3];

		m[2][0] = a.m[2][0] * b.m[0][0] + a.m[2][1] * b.m[1][0] + a.m[2][2] * b.m[2][0] + a.m[2][3] * b.m[3][0]; // row 2
		m[2][1] = a.m[2][0] * b.m[0][1] + a.m[2][1] * b.m[1][1] + a.m[2][2] * b.m[2][1] + a.m[2][3] * b.m[3][1];
		m[2][2] = a.m[2][0] * b.m[0][2] + a.m[2][1] * b.m[1][2] + a.m[2][2] * b.m[2][2] + a.m[2][3] * b.m[3][2];
		m[2][3] = a.m[2][0] * b.m[0][3] + a.m[2][1] * b.m[1][3] + a.m[2][2] * b.m[2][3] + a.m[2][3] * b.m[3][3];

		m[3][0] = a.m[3][0] * b.m[0][0] + a.m[3][1] * b.m[1][0] + a.m[3][2] * b.m[2][0] + a.m[3][3] * b.m[3][0]; // row 3
		m[3][1] = a.m[3][0] * b.m[0][1] + a.m[3][1] * b.m[1][1] + a.m[3][2] * b.m[2][1] + a.m[3][3] * b.m[3][1];
		m[3][2] = a.m[3][0] * b.m[0][2] + a.m[3][1] * b.m[1][2] + a.m[3][2] * b.m[2][2] + a.m[3][3] * b.m[3][2];
		m[3][3] = a.m[3][0] * b.m[0][3] + a.m[3][1] * b.m[1][3] + a.m[3][2] * b.m[2][3] + a.m[3][3] * b.m[3][3];

		return *this;
	}

	Mat4 &operator *= (const double d)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				m[i][j] = m[i][j] * d;

		return *this;
	}

	const Mat4 operator * (const Mat4 &b) const
	{
		return Mat4(*this) *= b;
	}

	const Mat4 operator * (const double d) const
	{
		return Mat4(*this) *= d;
	}

	static Mat4 identity()
	{
		return Mat4(1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1);
	}

	static Mat4 translate(const double x, const double y, const double z)
	{
		return Mat4(1, 0, 0, x,
					0, 1, 0, y,
					0, 0, 1, z,
					0, 0, 0, 1);
	}

	static Mat4 scale(const double x, const double y, const double z)
	{
		return Mat4(x, 0, 0, 0,
					0, y, 0, 0,
					0, 0, z, 0,
					0, 0, 0, 1);
	}

	static Mat4 rotX(const double t)
	{
		return Mat4(1, 0, 0, 0,
					0, cos(t), -sin(t), 0,
					0, sin(t), cos(t), 0,
					0, 0, 0, 1);
	}

	static Mat4 rotY(const double t)
	{
		return Mat4(cos(t), 0, sin(t),	0,
					0,		1, 0,		0,
					-sin(t),0, cos(t),	0,
					0,		0, 0,		1);
	}

	static Mat4 rotZ(const double t)
	{
		return Mat4(cos(t), -sin(t), 0, 0,
					sin(t), cos(t), 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1);
	}

	static Mat4 rotXDeg(const double d)
	{
		const double t = d/180.0*Globals::PI;

		return Mat4(1, 0, 0, 0,
			0, cos(t), -sin(t), 0,
			0, sin(t), cos(t), 0,
			0, 0, 0, 1);
	}

	static Mat4 rotYDeg(const double d)
	{
		const double t = d/180.0*Globals::PI;

		return Mat4(cos(t), 0, sin(t),	0,
			0,		1, 0,		0,
			-sin(t),0, cos(t),	0,
			0,		0, 0,		1);
	}

	static Mat4 rotZDeg(const double d)
	{
		const double t = d/180.0*Globals::PI;

		return Mat4(cos(t), -sin(t), 0, 0,
			sin(t), cos(t), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	}

private:

	inline const double det2(	const double a, const double b,
								const double c, const double d) const
	{
		return a*d-b*c;
	}

	inline const double det3(	const double a1, const double a2, const double a3,
								const double b1, const double b2, const double b3,
								const double c1, const double c2, const double c3) const
	{
		return	a1 * det2(b2, b3, c2, c3) -
				b1 * det2(a2, a3, c2, c3) +
				c1 * det2(a2, a3, b2, b3);
	}

};

#endif // Mat4_h__
