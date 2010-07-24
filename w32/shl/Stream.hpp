#ifndef _win_Stream_hpp__
#define _win_Stream_hpp__

// Copyright(c) Andre Caron, 2009-2010
//
// This document is covered by the Artistic License 2.0 (Open Source Initiative
// approved license). A copy of the license should have been provided alongside
// this software package (see "license.rtf"). If not, the license is available
// online at "http://www.opensource.org/licenses/artistic-license-2.0.php".

#include "__configure__.hpp"
#include <w32/shl/Path.hpp>

namespace w32 { namespace shl {

    class W32_SHL_EXPORT Stream :
        public com::Wrapper< ::IStream >
    {
        /* class methods. */
    public:
        static Stream open ( const shl::Path& path );

        /* construction. */
    public:
        explicit Stream ( const Ptr& backend );

        /* methods. */
    public:
        ulong read ( void * buffer, ulong bytes );
        ulong write ( const void * buffer, ulong bytes );
    };

} }

template<> inline w32::com::Guid
    w32::com::guidof<::IStream> ()
{
    return (IID_IStream);
}


#endif /* _win_Stream_hpp__ */
