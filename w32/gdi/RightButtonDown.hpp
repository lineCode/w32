#ifndef _win32c_gdi_RightButtonDown_hpp__
#define _win32c_gdi_RightButtonDown_hpp__

// Copyright(c) Andre Caron, 2009-2010
//
// This document is covered by the Artistic License 2.0 (Open Source Initiative
// approved license). A copy of the license should have been provided alongside
// this software package (see "license.rtf"). If not, the license is available
// online at "http://www.opensource.org/licenses/artistic-license-2.0.php".

#include <win32c/gdi/MouseNotification.hpp>
#include <Windows.h>

namespace win32c { namespace gdi {

    class RightButtonDown :
        public MouseNotification
    {
        /* construction. */
    public:
        RightButtonDown ( const Point& point );
        RightButtonDown ( ::WPARAM wparam, ::LPARAM lparam );
    };

} }

#endif /* _win32c_gdi_RightButtonDown_hpp__ */
