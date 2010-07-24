#ifndef _w32_gdi_MappingMode_hpp__
#define _w32_gdi_MappingMode_hpp__

// Copyright(c) Andre Caron, 2009-2010
//
// This document is covered by the Artistic License 2.0 (Open Source Initiative
// approved license). A copy of the license should have been provided alongside
// this software package (see "license.rtf"). If not, the license is available
// online at "http://www.opensource.org/licenses/artistic-license-2.0.php".

#include "__configure__.hpp"
#include <w32/NotCopyable.hpp>

namespace w32 { namespace gdi {

    class DeviceContext;

        /*!
         * @brief Coordinate-translation mode.
         */
    class W32_GDI_EXPORT MappingMode
    {
        /* nested types. */
    public:
        typedef int Value;

        class W32_GDI_EXPORT Selection :
            private NotCopyable
        {
            /* data. */
        private:
            ::HDC myDevice;
            int myPredecessor;

            /* construction. */
        public:
            Selection ( const DeviceContext& device, const MappingMode& mode );
            ~Selection ();
        };

        /* class data. */
    public:
            /*!
             * @brief Untranslated device units.
             */
        static const MappingMode text ();

        /* class methods. */
    public:
        static MappingMode of ( const DeviceContext& device );

        /* data. */
    private:
        Value myValue;

        /* construction. */
    private:
        MappingMode ( Value value );

        /* methods. */
    public:
            /*!
             * @brief Obtains the mapping mode's identifier.
             */
        Value value () const;
    };

} }

#endif /* _w32_gdi_MappingMode_hpp__ */
