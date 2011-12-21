#ifndef _w32_gdi_configure_hpp__
#define _w32_gdi_configure_hpp__

// Copyright(c) Andre Caron, 2009-2010
//
// This document is covered by the Artistic License 2.0 (Open Source Initiative
// approved license). A copy of the license should have been provided alongside
// this software package (see "license.rtf"). If not, the license is available
// online at "http://www.opensource.org/licenses/artistic-license-2.0.php".

#include "../w32/__configure__.hpp"

    // API headers.
#ifndef _INC_WINDOWSX
#   include <WindowsX.h>
#endif
#ifndef _INC_COMMCTRL
#   include <CommCtrl.h>
#endif
#ifndef _PRSHT_H_
#   include <PrSht.h>
#endif

    // Remove conflicting macros.
#if (defined(PropertySheet))
#   undef PropertySheet
#endif

#ifdef _MSC_VER
#   pragma comment ( lib, "ComCtl32.lib" )
#endif

#endif /* _w32_gdi_configure_hpp__ */