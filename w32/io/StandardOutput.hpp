#ifndef _w32_io_StandardOutput_hpp__
#define _w32_io_StandardOutput_hpp__

// Copyright(c) Andre Caron, 2009-2010
//
// This document is covered by the Artistic License 2.0 (Open Source Initiative
// approved license). A copy of the license should have been provided alongside
// this software package (see "license.rtf"). If not, the license is available
// online at "http://www.opensource.org/licenses/artistic-license-2.0.php".

#include "__configure__.hpp"
#include <w32/NotCopyable.hpp>
#include <w32/io/OutputStream.hpp>

namespace w32 { namespace io {

    class W32_IO_EXPORT StandardOutput :
        public OutputStream
    {
        /* nested types. */
    public:
        class Redirection;

        /* construction. */
    public:
        StandardOutput ();
    };

    class W32_IO_EXPORT StandardOutput::Redirection :
        private w32::NotCopyable
    {
        /* data. */
    private:
        Handle myPredecessor;

        /* construction. */
    public:
        Redirection ( OutputStream& output );
        ~Redirection ();
    };

} }

#endif /* _w32_io_StandardOutput_hpp__ */
