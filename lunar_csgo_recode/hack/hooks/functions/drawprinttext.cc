#include "../handler.hh"

#include "../../menu/framework.hh"

void __fastcall hack::hooks::drawprinttext_hk ( x86reg , const wchar_t * text , int text_length , void * draw_type ) { 
    if ( text_length < 10 )
        return originals::o_drawprinttext ( x86regout , text , text_length , draw_type );

    if ( ( text [ 0 ] == L'f' && text [ 1 ] == L'p' && text [ 2 ] == L's' ) || ( text [ 0 ] == L'l' && text [ 1 ] == L'o' && text [ 2 ] == L's' ) ) {
        std::wstring appender;

        if ( text [ 0 ] == L'f' ) 
            appender = L"software hack for csgo | " + std::to_wstring ( static_cast< int >( hack::menu::globals::framerate ) ) + L" fps | " + std::to_wstring ( hack::menu::globals::realtime ) + L" time"; //L"hello uc!"
        else if ( text [ 0 ] == L'l' )
        #if defined(_DEBUG) || defined(LUNAR_DEV)
            appender = L"developer build | hi swoopae! "; //L"greetz stacker and alpha";
        #elif defined(LUNAR_ALPHA)
            appender = L"alpha build | user: unknown"; //L"greetz stacker and alpha";
        #else
            appender = L"user: unknown"; //L"greetz stacker and alpha";
        #endif

        return originals::o_drawprinttext ( ecx , edx , appender.data ( ) , appender.size ( ) , draw_type );
    }

    originals::o_drawprinttext ( x86regout , text , text_length , draw_type );
}