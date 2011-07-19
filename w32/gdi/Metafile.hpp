#ifndef _w32_gdi_Metafile_hpp__
#define _w32_gdi_Metafile_hpp__

// Copyright(c) Andre Caron, 2009-2010
//
// This document is covered by the Artistic License 2.0 (Open Source Initiative
// approved license). A copy of the license should have been provided alongside
// this software package (see "license.rtf"). If not, the license is available
// online at "http://www.opensource.org/licenses/artistic-license-2.0.php".

#include "__configure__.hpp"
#include <w32/Reference.hpp>

namespace w32 { namespace gdi {

        /*!
         * @brief Macro of GDI drawing commands.
         */
    class Metafile
    {
        /* nested types. */
    public:
        typedef Reference< ::HMETAFILE, void(*)(::HMETAFILE) > Handle;

        /* class methods. */
    public:
        static Handle claim ( ::HMETAFILE object );
        static Handle proxy ( ::HMETAFILE object );

        /* data. */
    private:
        Handle myHandle;

        /* construction. */
    public:
        explicit Metafile ( const Handle& handle );

        /* methods. */
    public:
            /*!
             * @brief Obtains the metafile's handle.
             */
        const Handle& handle () const;
    };

} }

#endif /* _w32_gdi_Metafile_hpp__ */
