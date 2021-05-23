#pragma once

#include "matrix3x4.hh"

#define pi 3.141593f
#define rad_pi 57.295779513082f;

#define rad2deg(x) ((float)(x) * (float)(180.f / pi))
#define deg2rad(x) ((float)(x) * (float)(pi / 180.f))

#define time_to_ticks(time) ( int ) ( 0.5f + time / hack::game::interfaces::global_vars->m_ipt )
#define ticks_to_time(ticks) hack::game::interfaces::global_vars->m_ipt * ( float ) ( ticks )

namespace sdk::math {
	__forceinline static void angle_vectors ( const c_vector3 & angles , c_vector3 & forward , c_vector3 * right = nullptr , c_vector3 * up = nullptr ) {
		const auto sp = sin ( deg2rad ( angles.x ) ) , cp = cos ( deg2rad ( angles.x ) ) ,
			sy = sin ( deg2rad ( angles.y ) ) , cy = cos ( deg2rad ( angles.y ) ) ,
			sr = sin ( deg2rad ( angles.z ) ) , cr = cos ( deg2rad ( angles.z ) );

		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;

		if ( right ) {
			right->x = -1 * sr * sp * cy + -1 * cr * -sy;
			right->y = -1 * sr * sp * sy + -1 * cr * cy;
			right->z = -1 * sr * cp;
		}

		if ( up ) {
			up->x = cr * sp * cy + -sr * -sy;
			up->y = cr * sp * sy + -sr * cy;
			up->z = cr * cp;
		}
	}

	__forceinline static void vector_angles ( const c_vector3 & fwd , c_vector3 & up , c_vector3 & out ) {
		c_vector3 left = up.cross_product ( fwd );
		left.normalize_place ( );

		auto lgth = fwd.length_2d ( );

		if ( lgth > 0.001f ) {
			out.x = atan2f ( -fwd.z , lgth ) * 180 / math_pi;
			out.y = atan2f ( fwd.y , fwd.x ) * 180 / math_pi;

			float upZ = ( left.y * fwd.x ) - ( left.x * fwd.y );
			out.z = atan2f ( left.z , upZ ) * 180 / math_pi;
		} else {
			out.x = atan2f ( -fwd.z , lgth ) * 180 / math_pi;
			out.y = atan2f ( -left.x , left.y ) * 180 / math_pi;
			out.z = 0;
		}
	}

	__forceinline static void vector_angles ( const c_vector3 & in , c_vector3 & out ) {
		if ( in.z == 0.f && in.x == 0.f ) {
			out.y = 0.f;

			if ( in.z > 0.f )
				out.x = 90.f;
			else
				out.x = 270.f;
		} else {
			out.x = rad2deg ( atan2f ( -in.z , in.length_2d ( ) ) );
			out.y = rad2deg ( atan2f ( in.y , in.x ) );

			if ( out.x < 0.0f )
				out.x += 360.0f;

			if ( out.y < 0.0f )
				out.y += 360.0f;
		}

		out.x -= floorf ( out.x / 360.0f + 0.5f ) * 360.0f;
		out.y -= floorf ( out.y / 360.0f + 0.5f ) * 360.0f;

		if ( out.x > 89.0f )
			out.x = 89.0f;
		else if ( out.x < -89.0f )
			out.x = -89.0f;
	}

	__forceinline static void transform_vector ( const c_vector3 & a , const c_matrix3x4 & b , c_vector3 & out ) {
		out.x = a.dot_product ( b.m_values [ 0 ] ) + b.m_values [ 0 ][ 3 ];
		out.y = a.dot_product ( b.m_values [ 1 ] ) + b.m_values [ 1 ][ 3 ];
		out.z = a.dot_product ( b.m_values [ 2 ] ) + b.m_values [ 2 ][ 3 ];
	}

	__forceinline static c_vector3 calc_angle ( const c_vector3 & from , const c_vector3 & to ) {
		static const auto ang_zero = c_vector3 ( 0.0f , 0.0f , 0.0f );

		const auto delta = from - to;
		if ( delta.length ( ) <= 0.0f )
			return ang_zero;

		if ( delta.z == 0.0f && delta.length ( ) == 0.0f )
			return ang_zero;

		if ( delta.y == 0.0f && delta.x == 0.0f )
			return ang_zero;

		c_vector3 angles;
		angles.x = asinf ( delta.z / delta.length ( ) ) * rad_pi;
		angles.y = atanf ( delta.y / delta.x ) * rad_pi;
		angles.z = 0.0f;

		if ( delta.x >= 0.0f )
			angles.y += 180.0f;

		return angles;
	}

	__forceinline static float get_fov ( const c_vector3 & from , const c_vector3 & to , const c_vector3 & angs ) {
		c_vector3 dir , fw;
		dir = ( to - from ).normalized ( );
		angle_vectors ( angs , fw );
		return std::max ( rad2deg ( std::acos ( fw.dot_product ( dir ) ) ) , 0.f );
	}

	__forceinline static c_vector3 interpolate ( const c_vector3 & from , const c_vector3 & to , const float percent ) {
		return to * percent + from * ( 1.f - percent );
	}

	__forceinline static float interpolate ( const float from , const float to , const float percent ) {
		return to * percent + from * ( 1.f - percent );
	}
}