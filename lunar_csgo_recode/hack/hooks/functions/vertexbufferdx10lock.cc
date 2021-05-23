#include "../handler.hh"

#include "../../menu/config.hh"

void __fastcall hack::hooks::vertexbufferdx10lock_hk ( x86reg , int max_vertex_count , bool append , void * unk ) {
  /* lol no */

	originals::o_vertexbufferlock ( x86regout , max_vertex_count , append , unk );
}