#pragma once

#include "vector.hh"

class c_matrix3x4 {
public:
	float m_values [ 3 ][ 4 ];

	c_matrix3x4 ( void ) {
		m_values [ 0 ][ 0 ] = 0.0f; m_values [ 0 ][ 1 ] = 0.0f; m_values [ 0 ][ 2 ] = 0.0f; m_values [ 0 ][ 3 ] = 0.0f;
		m_values [ 1 ][ 0 ] = 0.0f; m_values [ 1 ][ 1 ] = 0.0f; m_values [ 1 ][ 2 ] = 0.0f; m_values [ 1 ][ 3 ] = 0.0f;
		m_values [ 2 ][ 0 ] = 0.0f; m_values [ 2 ][ 1 ] = 0.0f; m_values [ 2 ][ 2 ] = 0.0f; m_values [ 2 ][ 3 ] = 0.0f;
	}

	c_matrix3x4 (
		float m00 , float m01 , float m02 , float m03 ,
		float m10 , float m11 , float m12 , float m13 ,
		float m20 , float m21 , float m22 , float m23 ) {
		m_values [ 0 ][ 0 ] = m00; m_values [ 0 ][ 1 ] = m01; m_values [ 0 ][ 2 ] = m02; m_values [ 0 ][ 3 ] = m03;
		m_values [ 1 ][ 0 ] = m10; m_values [ 1 ][ 1 ] = m11; m_values [ 1 ][ 2 ] = m12; m_values [ 1 ][ 3 ] = m13;
		m_values [ 2 ][ 0 ] = m20; m_values [ 2 ][ 1 ] = m21; m_values [ 2 ][ 2 ] = m22; m_values [ 2 ][ 3 ] = m23;
	}

	void init ( const c_vector3 & x , const c_vector3 & y , const c_vector3 & z , const c_vector3 & origin ) {
		m_values [ 0 ][ 0 ] = x.x; m_values [ 0 ][ 1 ] = y.x; m_values [ 0 ][ 2 ] = z.x; m_values [ 0 ][ 3 ] = origin.x;
		m_values [ 1 ][ 0 ] = x.y; m_values [ 1 ][ 1 ] = y.y; m_values [ 1 ][ 2 ] = z.y; m_values [ 1 ][ 3 ] = origin.y;
		m_values [ 2 ][ 0 ] = x.z; m_values [ 2 ][ 1 ] = y.z; m_values [ 2 ][ 2 ] = z.z; m_values [ 2 ][ 3 ] = origin.z;
	}

	c_matrix3x4 ( const c_vector3 & x , const c_vector3 & y , const c_vector3 & z , const c_vector3 & origin ) {
		init ( x , y , z , origin );
	}

	inline void set_origin ( c_vector3 const & p ) {
		m_values [ 0 ][ 3 ] = p.x;
		m_values [ 1 ][ 3 ] = p.y;
		m_values [ 2 ][ 3 ] = p.z;
	}

	inline void invalidate ( void ) {
		for ( int i = 0; i < 3; i++ ) {
			for ( int j = 0; j < 4; j++ ) {
				m_values [ i ][ j ] = std::numeric_limits<float>::infinity ( );
			}
		}
	}

	c_vector3 get_x_axis ( void ) const {
		return at ( 0 );
	}

	c_vector3 get_y_axis ( void ) const {
		return at ( 1 );
	}

	c_vector3 get_z_axis ( void ) const {
		return at ( 2 );
	}

	c_vector3 origin ( void ) const {
		return at ( 3 );
	}

	c_vector3 at ( int i ) const {
		return c_vector3 { m_values [ 0 ][ i ], m_values [ 1 ][ i ], m_values [ 2 ][ i ] };
	}

	float * operator[]( int i ) {
		return m_values [ i ];
	}

	const float * operator[]( int i ) const {
		return m_values [ i ];
	}

	float * base ( ) {
		return &m_values [ 0 ][ 0 ];
	}

	const float * base ( ) const {
		return &m_values [ 0 ][ 0 ];
	}

	const bool operator==( c_matrix3x4 matrix ) const {
		return
			m_values [ 0 ][ 0 ] == matrix [ 0 ][ 0 ] && m_values [ 0 ][ 1 ] == matrix [ 0 ][ 1 ] && m_values [ 0 ][ 2 ] == matrix [ 0 ][ 2 ] && m_values [ 0 ][ 3 ] == matrix [ 0 ][ 3 ] &&
			m_values [ 1 ][ 0 ] == matrix [ 1 ][ 0 ] && m_values [ 1 ][ 1 ] == matrix [ 1 ][ 1 ] && m_values [ 1 ][ 2 ] == matrix [ 1 ][ 2 ] && m_values [ 1 ][ 3 ] == matrix [ 1 ][ 3 ] &&
			m_values [ 2 ][ 0 ] == matrix [ 2 ][ 0 ] && m_values [ 2 ][ 1 ] == matrix [ 2 ][ 1 ] && m_values [ 2 ][ 2 ] == matrix [ 2 ][ 2 ] && m_values [ 2 ][ 3 ] == matrix [ 2 ][ 3 ];
	}
};

class v_matrix {
public:
	v_matrix ( );
	v_matrix (
		float m00 , float m01 , float m02 , float m03 ,
		float m10 , float m11 , float m12 , float m13 ,
		float m20 , float m21 , float m22 , float m23 ,
		float m30 , float m31 , float m32 , float m33
	);

	void init (
		float m00 , float m01 , float m02 , float m03 ,
		float m10 , float m11 , float m12 , float m13 ,
		float m20 , float m21 , float m22 , float m23 ,
		float m30 , float m31 , float m32 , float m33
	);

	// array access
	inline float * operator[]( int i ) {
		return m [ i ];
	}

	inline const float * operator[]( int i ) const {
		return m [ i ];
	}

	// pointer to m[0][0]
	inline float * base ( ) {
		return &m [ 0 ][ 0 ];
	}

	inline const float * base ( ) const {
		return &m [ 0 ][ 0 ];
	}

public:
	float		m [ 4 ][ 4 ];
};