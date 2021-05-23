#include "../handler.hh"

#include "../../modules/rage/tickbase.hh"

#include <intrin.h>

bool __fastcall hack::hooks::writeucmddeltatobuffer_hk ( x86reg , int a1 , void * a2 , int a3 , int a4 , bool a5 ) {
	return hack::modules::rage::tickbase::in_writeucmddelta ( ecx , edx , a1 , a2 , a3 , a4 , a5 );
}