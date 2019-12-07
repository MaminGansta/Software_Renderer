

#define PI 3.14159265359

template <typename T>
struct Vec2
{
	union
	{
		struct { T x, y; };
		T raw[2];
	};

	inline Vec2(): x(0), y(0){}
	inline Vec2(T x, T y) : x(x), y(y) {}

	inline Vec2<T> & operator + (Vec2<T> other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	inline Vec2<T>& operator * (float scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	inline T& operator [] (int inx)
	{
		return raw[inx];
	}
};

template <typename T>
struct Vec3
{
	union
	{
		struct { T x, y, z; };
		T raw[3];
	};

	inline Vec3() : x(0), y(0), z(0) {}
	inline Vec3(T x, T y, T z) : x(x), y(y), z(z) {}
	//inline Vec3(Matrix44f matrix) : x(matrix[0][0] / matrix[3][0]), y(matrix[1][0] / matrix[3][0]), z(matrix[2][0] / matrix[3][0]) {}


	inline Vec3<T>& operator += (Vec3<T> other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	inline Vec3<T>& operator -= (Vec3<T> other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	inline Vec3<T>& operator *= (float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	inline Vec3<T> operator + (Vec3<T> other)
	{
		Vec3<T> tmp;
		tmp.x = x + other.x;
		tmp.y = y + other.y;
		tmp.z = z + other.z;
		return tmp;
	}

	inline Vec3<T> operator - (Vec3<T> other)
	{
		Vec3<T> tmp;
		tmp.x = x - other.x;
		tmp.y = y - other.y;
		tmp.z = z - other.z;
		return tmp;
	}

	inline Vec3<T> operator * (float scalar)
	{
		Vec3<T> tmp;
		tmp.x = x * scalar;
		tmp.y = y * scalar;
		tmp.z = z * scalar;
		return tmp;
	}

	inline T& operator [] (int inx)
	{
		return raw[inx];
	}

	float norm()
	{
		float a = x * x;
		float b = y * y;
		float c = z * z;
		return std::sqrt(a + b + c); 
	}

	inline Vec3<T>& normalize()
	{
		Vec3<T> temp = (*this) * (1.0f / norm());
		return temp;
	}

	inline void normalizeYourself()
	{
		*this *= (1.0f / norm());
	}

};

typedef Vec2<int> Vert2i;
typedef Vec2<float> Vert2f;
typedef Vec2<int> Vec2i;
typedef Vec2<float> Vec2f;

typedef Vec3<int> Vert3i;
typedef Vec3<float> Vert3f;
typedef Vec3<int> Vec3i;
typedef Vec3<float> Vec3f;



template <typename T>
inline float dproduct(Vec3<T> a, Vec3<T> b)
{
	float res = 0;
	for (int i = 0; i < 3; i++)
		res += a[i] * b[i];

	return res;
}

template <typename T>
inline int dproduct(Vec2<T> a, Vec2<T> b)
{
	int res = 0;
	for (int i = 0; i < 2; i++)
		res += a[i] * b[i];

	return res;
}

template <typename T>
inline void cproduct(Vec3<T> a, Vec3<T> b, Vec3<T>* res)
{
	res->x = a.y * b.z - a.z * b.y;
	res->y = a.z * b.x - a.x * b.z;
	res->z = a.x * b.y - a.y * b.x;
}

template <typename T>
Vec3<T> cross(Vec3<T> v1, Vec3<T> v2) {
	return Vec3<T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

#define identity 0
#define empty 1

template<typename T>
class Matrix44
{
public:
	T x[4][4] = { {0, 0, 0, 0}, { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 } };

	Matrix44(int kind)
	{
		if (kind == identity)
			for (int i = 0; i < 4; i++)
				x[i][i] = 1;
	}

	Matrix44(T a, T b, T c, T d, T e, T f, T g, T h,
		T i, T j, T k, T l, T m, T n, T o, T p)
	{
		x[0][0] = a;
		x[0][1] = b;
		x[0][2] = c;
		x[0][3] = d;
		x[1][0] = e;
		x[1][1] = f;
		x[1][2] = g;
		x[1][3] = h;
		x[2][0] = i;
		x[2][1] = j;
		x[2][2] = k;
		x[2][3] = l;
		x[3][0] = m;
		x[3][1] = n;
		x[3][2] = o;
		x[3][3] = p;
	}
	
	Matrix44(Vec3f v) 
	{

		x[0][0] = v.x;
		x[1][0] = v.y;
		x[2][0] = v.z;
		x[3][0] = 1.f;
	}

	const T* operator [] (uint8_t i) const { return x[i]; }
	T* operator [] (uint8_t i) { return x[i]; }

	// Multiply the current matrix with another matrix (rhs)
	Matrix44 operator * (const Matrix44& v) const
	{
		Matrix44 tmp(identity);
		multiply(*this, v, tmp);

		return tmp;
	}

	static void multiply(const Matrix44<T>& a, const Matrix44& b, Matrix44& c)
	{
		for (uint8_t i = 0; i < 4; ++i)
		{
			for (uint8_t j = 0; j < 4; ++j)
			{
				c[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] +
					a[i][2] * b[2][j] + a[i][3] * b[3][j];
			}
		}
	}

	Matrix44 transposed() const
	{
		Matrix44 t;
		for (uint8_t i = 0; i < 4; ++i) {
			for (uint8_t j = 0; j < 4; ++j) {
				t[i][j] = x[j][i];
			}
		}
		return t;
	}

	// point-matrix multiplication
	template<typename S>
	void multVecMatrix(const Vec3<S>& src, Vec3<S>& dst) const
	{
		S a, b, c, w;

		a = src[0] * x[0][0] + src[1] * x[1][0] + src[2] * x[2][0] + x[3][0];
		b = src[0] * x[0][1] + src[1] * x[1][1] + src[2] * x[2][1] + x[3][1];
		c = src[0] * x[0][2] + src[1] * x[1][2] + src[2] * x[2][2] + x[3][2];
		w = src[0] * x[0][3] + src[1] * x[1][3] + src[2] * x[2][3] + x[3][3];

		dst.x = a / w;
		dst.y = b / w;
		dst.z = c / w;
	}

	// vector-matrix multiplication
	template<typename S>
	void multDirMatrix(const Vec3<S>& src, Vec3<S>& dst) const
	{
		S a, b, c;

		a = src[0] * x[0][0] + src[1] * x[1][0] + src[2] * x[2][0];
		b = src[0] * x[0][1] + src[1] * x[1][1] + src[2] * x[2][1];
		c = src[0] * x[0][2] + src[1] * x[1][2] + src[2] * x[2][2];

		dst.x = a;
		dst.y = b;
		dst.z = c;
	}


	Matrix44 inverse()
	{
		int i, j, k;
		Matrix44 s;
		Matrix44 t(*this);

		// Forward elimination
		for (i = 0; i < 3; i++) {
			int pivot = i;

			T pivotsize = t[i][i];

			if (pivotsize < 0)
				pivotsize = -pivotsize;

			for (j = i + 1; j < 4; j++) {
				T tmp = t[j][i];

				if (tmp < 0)
					tmp = -tmp;

				if (tmp > pivotsize) {
					pivot = j;
					pivotsize = tmp;
				}
			}

			if (pivotsize == 0) {
				// Cannot invert singular matrix
				return Matrix44();
			}

			if (pivot != i) {
				for (j = 0; j < 4; j++) {
					T tmp;

					tmp = t[i][j];
					t[i][j] = t[pivot][j];
					t[pivot][j] = tmp;

					tmp = s[i][j];
					s[i][j] = s[pivot][j];
					s[pivot][j] = tmp;
				}
			}

			for (j = i + 1; j < 4; j++) {
				T f = t[j][i] / t[i][i];

				for (k = 0; k < 4; k++) {
					t[j][k] -= f * t[i][k];
					s[j][k] -= f * s[i][k];
				}
			}
		}

		// Backward substitution
		for (i = 3; i >= 0; --i) {
			T f;

			if ((f = t[i][i]) == 0) {
				// Cannot invert singular matrix
				return Matrix44();
			}

			for (j = 0; j < 4; j++) {
				t[i][j] /= f;
				s[i][j] /= f;
			}

			for (j = 0; j < i; j++) {
				f = t[j][i];

				for (k = 0; k < 4; k++) {
					t[j][k] -= f * t[i][k];
					s[j][k] -= f * s[i][k];
				}
			}
		}

		return s;
	}

	Matrix44<T>& invertYourself()
	{
		*this = inverse();
		return *this;
	}

	Vec3f toVec3()
	{
		return  Vec3f(x[0][0] / x[3][0], x[1][0] / x[3][0], x[2][0] / x[3][0]);
		//return  Vec3f(x[0][0], x[1][0], x[2][0]);
	}

};

typedef Matrix44<float> Matrix44f;




//Matrix44f lookAt(Vec3f& from, Vec3f& to, Vec3f tmp = Vec3f(0, 1, 0))
//{
//	Vec3f forward = (from - to).normalize();
//	Vec3f right = cross(tmp.normalize(), forward);
//	Vec3f up = cross(forward, right);
//
//	Matrix44f camToWorld(empty);
//
//	camToWorld[0][0] = right.x;
//	camToWorld[0][1] = right.y;
//	camToWorld[0][2] = right.z;
//	camToWorld[1][0] = up.x;
//	camToWorld[1][1] = up.y;
//	camToWorld[1][2] = up.z;
//	camToWorld[2][0] = forward.x;
//	camToWorld[2][1] = forward.y;
//	camToWorld[2][2] = forward.z;
//
//	camToWorld[3][0] = from.x;
//	camToWorld[3][1] = from.y;
//	camToWorld[3][2] = from.z;
//
//	return camToWorld;
//}

Matrix44f lookAt(Vec3f eye, Vec3f center, Vec3f up = Vec3f(0, 1, 0))
{
	Vec3f z = (eye - center).normalize();
	Vec3f x = cross(up, z).normalize();
	Vec3f y = cross(z, x).normalize();
	Matrix44f res = Matrix44f(identity);
	for (int i = 0; i < 3; i++) {
		res[0][i] = x[i];
		res[1][i] = y[i];
		res[2][i] = z[i];
		res[i][3] = -center[i];
	}
	return res;
}

Matrix44f viewport(int x, int y, int w, int h) {
	Matrix44f m(identity);
	m[0][3] = x + w / 2.f;
	m[1][3] = y + h / 2.f;
	m[2][3] = depth / 2.f;

	m[0][0] = w / 2.f;
	m[1][1] = h / 2.f;
	m[2][2] = depth / 2.f;
	return m;
}



bool barycentric(Vec3f A, Vec3f B, Vec3f C, Vert3i P, Vec3f* out) {
	Vec3f s[2];
	for (int i = 0; i < 2; i++) {
		s[i][0] = C[i] - A[i];
		s[i][1] = B[i] - A[i];
		s[i][2] = A[i] - P[i];
	}
	Vec3f u = cross(s[0], s[1]);
	if (std::abs(u[2]) > 1e-2)
	{
		out->x = 1.0f - (u.x + u.y) / u.z;
		out->y = u.y / u.z;
		out->z = u.x / u.z;
		if (out->x >= 0 && out->y >= 0 && out->z >= 0)
			return true;
	}
	return false;
}
