#include "../handler.hh"

void __fastcall hack::hooks::doanimationevent_hk( x86reg, void * anim_event, int data ) {
	originals::o_doanimationevent( x86regout, anim_event, data );
#ifdef _DEBUG
	std::cout << "func called\n";
#endif /* _DEBUG*/
}