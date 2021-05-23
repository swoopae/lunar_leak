#pragma once

namespace sdk::valve {
	class c_classmap {
	public:
		virtual void add ( const char * mapname , const char * classname , int size , void* factory = 0 ) = 0;
		virtual char const * lookup ( const char * classname ) = 0;
		virtual void * create_entity_by_name ( const char * mapname ) = 0;
		virtual int	get_class_entity ( const char * classname ) = 0;
	};
}