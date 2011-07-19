#ifndef _w32_gdi_ThreadEvents_hpp__
#define _w32_gdi_ThreadEvents_hpp__

// Copyright(c) Andre Caron, 2009-2010
//
// This document is covered by the Artistic License 2.0 (Open Source Initiative
// approved license). A copy of the license should have been provided alongside
// this software package (see "license.rtf"). If not, the license is available
// online at "http://www.opensource.org/licenses/artistic-license-2.0.php".

#include "../__configure__.hpp"
#include <w32/gdi/EventQueue.hpp>

namespace w32 { namespace gdi {

    class ThreadEvents :
        public EventQueue
    {
        /* methods. */
    public:
            /*!
             * @brief Blocks until at least one message is pending.
             */
        void wait ();

        /* overrides. */
    public:
        virtual bool next ( Event& event, Filter filter = Filter::any() );
        virtual bool peek ( Event& event, Filter filter = Filter::any() );
        virtual void post ( const Message& message );
    };

} }

#endif /* _w32_gdi_ThreadEvents_hpp__ */
