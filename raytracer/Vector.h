#ifndef Vector_h__
#define Vector_h__

#include <istream>


/* A half-baked 3-vector class. */
class Vector
{
public:

	double x, y, z;

	double &r, &g, &b;

	Vector(const double x_, const double y_, const double z_) : x(x_), y(y_), z(z_), r(x), g(y), b(z) {}
	Vector() : x(0), y(0), z(0), r(x), g(y), b(z) {}

	Vector(const Vector &v) : x(v.x), y(v.y), z(v.z), r(x), g(y), b(z)
	{
	}

	Vector &operator *= (const double f)
	{
		x *= f;
		y *= f;
		z *= f;

		return *this;
	}

	const Vector operator * (const double f) const
	{
		return Vector(*this) *= f;
	}


	const Vector operator * (const Vector &rhs) const
	{
		return Vector(x * rhs.x, y * rhs.y, z * rhs.z);
	}

	Vector &operator = (const Vector &rhs)
	{
		if (this != &rhs)
		{
			this->x = rhs.x;
			this->y = rhs.y;
			this->z = rhs.z;
		}

		return *this;
	}

	const Vector operator + (const Vector &rhs) const
	{
		return Vector(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	const Vector operator - (const Vector &rhs) const
	{
		return Vector(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	const Vector operator -() const
	{
		return Vector(-x, -y, -z);
	}

	static const Vector lerp (const Vector &a, const Vector &b, double t)
	{
		return a * (1-t) + b * t;
	}

	inline const double dot(const Vector &b) const
	{
		return x*b.x + y*b.y + z*b.z;
	}

	inline const Vector cross(const Vector &b) const
	{
		return Vector(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);
	}

	inline const double length() const
	{
		return sqrt(lenSquared());
	}

	inline const double lenSquared() const
	{
		return x*x + y*y + z*z;
	}

	inline void normalize()
	{
		const double len = length();

		x /= len;
		y /= len;
		z /= len;
	}

	inline void clamp(const double &minimum, const double &maximum)
	{
		if (x < minimum)
			x = minimum;
		else if (x > maximum)
			x = maximum;

		if (y < minimum)
			y = minimum;
		else if (y > maximum)
			y = maximum;

		if (z < minimum)
			z = minimum;
		else if (z > maximum)
			z = maximum;
	}

	friend std::istream& operator >> (std::istream &in, Vector &v);
};

//void multVectorMatrix (Vector v, Matrix m);

#endif // Vector_h__
