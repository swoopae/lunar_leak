#pragma once

#include <cmath>
#include <algorithm>

#define math_pi 3.14159265358979323846264338327950288

class c_quaternion {
public:
	float x , y , z , w;
};

class c_vector2 {
public:
	using vec_t = float;
	vec_t x , y;

	constexpr c_vector2 ( vec_t x = 0.f , vec_t y = 0.f ) : x ( std::move ( x ) ) , y ( std::move ( y ) ) { }

	c_vector2 & operator=( const c_vector2 & vec ) {
		auto res = c_vector2 ( );

		if ( !this )
			return res;

		this->x = vec.x; this->y = vec.y;

		return *this;
	}

	c_vector2 operator-( ) const {
		return c_vector2 ( -this->x , -this->y );
	}

	c_vector2 operator+( const c_vector2 & v ) const {
		return c_vector2 ( x + v.x , y + v.y );
	}

	c_vector2 operator+( const float & v ) const {
		return c_vector2 ( x + v , y + v );
	}

	c_vector2 operator+( const int & v ) const {
		return c_vector2 ( x + v , y + v );
	}

	c_vector2 operator-( const c_vector2 & v ) const {
		return c_vector2 ( this->x - v.x , this->y - v.y );
	}

	c_vector2 operator*( float fl ) const {
		return c_vector2 ( this->x * fl , this->y * fl );
	}

	c_vector2 operator*( const c_vector2 & v ) const {
		return c_vector2 ( this->x * v.x , this->y * v.y );
	}

	c_vector2 operator/( float fl ) const {
		return c_vector2 ( this->x / fl , this->y / fl );
	}

	c_vector2 operator-( float fl ) const {
		return c_vector2 ( this->x - fl , this->y - fl );
	}

	c_vector2 operator-( int fl ) const {
		return c_vector2 ( this->x - fl , this->y - fl );
	}

	c_vector2 operator/( const c_vector2 & v ) const {
		return c_vector2 ( this->x / v.x , this->y / v.y );
	}
};

class c_vector3 {
public:
	using vec_t = float;
	vec_t x , y , z;

	constexpr c_vector3 ( vec_t x = 0.f , vec_t y = 0.f , vec_t z = 0.f ) : x ( std::move ( x ) ) , y ( std::move ( y ) ) , z ( std::move ( z ) ) { }

	constexpr void init ( ) {
		this->x = this->y = this->z = 0.0f;
	}

	void init ( float x , float y , float z ) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void clamp ( ) {
		this->x = std::clamp ( this->x , -89.0f , 89.0f );
		this->y = std::clamp ( std::remainder ( this->y , 360.0f ) , -180.0f , 180.0f );
		this->z = 0.f;
	}

	bool is_valid ( ) {
		return std::isfinite ( this->x ) && std::isfinite ( this->y ) && std::isfinite ( this->z );
	}

	bool is_zero ( ) {
		return c_vector3 ( this->x , this->y , this->z ) == c_vector3 ( 0.0f , 0.0f , 0.0f );
	}

	void invalidate ( ) {
		this->x = this->y = this->z = std::numeric_limits< float >::infinity ( );
	}

	void clear ( ) {
		this->x = this->y = this->z = 0.0f;
	}

	float & operator[]( int i ) {
		return ( ( float * ) this ) [ i ];
	}

	float operator[]( int i ) const {
		return ( ( float * ) this ) [ i ];
	}

	void zero ( ) {
		this->x = this->y = this->z = 0.0f;
	}

	bool operator==( const c_vector3 & src ) const {
		return ( src.x == this->x ) && ( src.y == y ) && ( src.z == z );
	}

	bool operator!=( const c_vector3 & src ) const {
		return ( src.x != this->x ) || ( src.y != y ) || ( src.z != z );
	}

	c_vector3 & operator+=( const c_vector3 & v ) {
		this->x += v.x; this->y += v.y; this->z += v.z;

		return *this;
	}

	c_vector3 & operator-=( const c_vector3 & v ) {
		this->x -= v.x; this->y -= v.y; this->z -= v.z;

		return *this;
	}

	c_vector3 & operator*=( float fl ) {
		this->x *= fl;
		this->y *= fl;
		this->z *= fl;

		return *this;
	}

	c_vector3 & operator*=( const c_vector3 & v ) {
		this->x *= v.x;
		this->y *= v.y;
		this->z *= v.z;

		return *this;
	}

	c_vector3 & operator/=( const c_vector3 & v ) {
		this->x /= v.x;
		this->y /= v.y;
		this->z /= v.z;

		return *this;
	}

	c_vector3 & operator+=( float fl ) {
		this->x += fl;
		this->y += fl;
		this->z += fl;

		return *this;
	}

	c_vector3 & operator/=( float fl ) {
		this->x /= fl;
		this->y /= fl;
		this->z /= fl;

		return *this;
	}

	c_vector3 & operator-=( float fl ) {
		this->x -= fl;
		this->y -= fl;
		this->z -= fl;

		return *this;
	}

	c_vector3 a_normalize ( ) {
		const auto l = length ( );

		if ( l > 0 ) {
			x /= l;
			y /= l;
			z /= l;
		}

		return *this;
	}

	void normalize ( ) {
		*this = normalized ( );
	}

	c_vector3 normalized ( ) const {
		auto res = *this;
		auto l = res.length ( );

		if ( l != 0.0f )
			res /= l;
		else
			res.x = res.y = res.z = 0.0f;

		return res;
	}

	void normalize_place ( ) {
		auto res = *this;
		auto radius = std::sqrtf ( x * x + y * y + z * z );
		auto iradius = 1.0f / ( radius + FLT_EPSILON );

		res.x *= iradius;
		res.y *= iradius;
		res.z *= iradius;
	}

	float dist_to ( const c_vector3 & vec ) const {
		c_vector3 delta;

		delta.x = this->x - vec.x;
		delta.y = this->y - vec.y;
		delta.z = this->z - vec.z;

		return delta.length ( );
	}

	float dist_to_sqr ( const c_vector3 & vec ) const {
		c_vector3 delta;

		delta.x = this->x - vec.x;
		delta.y = this->y - vec.y;
		delta.z = this->z - vec.z;

		return delta.length_sqr ( );
	}

	float dot_product ( const c_vector3 & vec ) const {
		return this->x * vec.x + this->y * vec.y + this->z * vec.z;
	}

	float dot_product ( const float * other ) const {
		return this->x * other [ 0 ] + this->y * other [ 1 ] + this->z * other [ 2 ];
	}

	c_vector3 cross_product ( const c_vector3 & vec ) const {
		return c_vector3 ( this->y * vec.z - this->z * vec.y , this->z * vec.x - this->x * vec.z , this->x * vec.y - this->y * vec.x );
	}

	float length ( ) const {
		return std::sqrtf ( this->x * this->x + this->y * this->y + this->z * this->z );
	}

	float length_sqr ( ) const {
		return this->x * this->x + this->y * this->y + this->z * this->z;
	}

	float length_2d ( ) const {
		return std::sqrtf ( this->x * this->x + this->y * this->y );
	}

	c_vector3 & operator=( const c_vector3 & vec ) {
		auto res = c_vector3 ( );

		if ( !this )
			return res;

		this->x = vec.x; this->y = vec.y; this->z = vec.z;

		return *this;
	}

	c_vector3 operator-( ) const {
		return c_vector3 ( -this->x , -this->y , -this->z );
	}

	c_vector3 operator+( const c_vector3 & v ) const {
		return c_vector3 ( x + v.x , y + v.y , z + v.z );
	}
	c_vector3 operator-( const c_vector3 & v ) const {
		return c_vector3 ( this->x - v.x , this->y - v.y , this->z - v.z );
	}

	c_vector3 operator*( float fl ) const {
		return c_vector3 ( this->x * fl , this->y * fl , this->z * fl );
	}

	c_vector3 operator*( const c_vector3 & v ) const {
		return c_vector3 ( this->x * v.x , this->y * v.y , this->z * v.z );
	}

	c_vector3 operator/( float fl ) const {
		return c_vector3 ( this->x / fl , this->y / fl , this->z / fl );
	}

	c_vector3 operator/( const c_vector3 & v ) const {
		return c_vector3 ( this->x / v.x , this->y / v.y , this->z / v.z );
	}
};

__forceinline c_vector3 operator*( float lhs , const c_vector3 & rhs ) {
	return rhs * lhs;
}

__forceinline c_vector3 operator/( float lhs , const c_vector3 & rhs ) {
	return rhs / lhs;
}