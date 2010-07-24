// Copyright(c) Andre Caron, 2009-2010
//
// This document is covered by the Artistic License 2.0 (Open Source Initiative
// approved license). A copy of the license should have been provided alongside
// this software package (see "license.rtf"). If not, the license is available
// online at "http://www.opensource.org/licenses/artistic-license-2.0.php".

#include <w32/gdi/ThreadEvents.hpp>
#include <w32/Error.hpp>

namespace w32 { namespace gdi {

    void ThreadEvents::wait ()
    {
        if ( ::WaitMessage() == FALSE ) {
            UNCHECKED_WIN32C_ERROR(WaitMessage,::GetLastError());
        }
    }

    bool ThreadEvents::next ( Event& event, Filter filter )
    {
        const ::BOOL result = ::GetMessage(
            &event.data(), ::HWND(-1), filter.first(), filter.last()
            );
        if ( result == -1 ) {
            UNCHECKED_WIN32C_ERROR(GetMessage,::GetLastError());
        }
        return (result != 0);
    }

    bool ThreadEvents::peek ( Event& event, Filter filter )
    {
        const ::BOOL result = ::PeekMessage(
            &event.data(), ::HWND(-1),
            filter.first(), filter.last(), PM_NOREMOVE
            );
        return (result != 0);
    }

    void ThreadEvents::post ( const Message& message )
    {
        const ::BOOL result = ::PostMessage(
            ::HWND(-1), message.wm(), message.wp(), message.lp()
            );
        if ( result == 0 ) {
            UNCHECKED_WIN32C_ERROR(PostMessage,::GetLastError());
        }
    }

} }
