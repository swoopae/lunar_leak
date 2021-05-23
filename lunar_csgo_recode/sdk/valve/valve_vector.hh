#pragma once

namespace sdk::valve {
	class c_util_vector {
	public:
		unsigned memory;
	private:
		char pad [ 8 ];
	public:
		unsigned int count;
		unsigned elements;

		inline void * base ( ) {
			return ( void * ) memory;
		}

		inline int get_count ( ) {
			return count;
		}
	};
}