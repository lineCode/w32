// Copyright(c) Andre Caron, 2009-2010
//
// This document is covered by the Artistic License 2.0 (Open Source Initiative
// approved license). A copy of the license should have been provided alongside
// this software package (see "license.rtf"). If not, the license is available
// online at "http://www.opensource.org/licenses/artistic-license-2.0.php".

#include <w32.gdi/Animation.hpp>

namespace w32 { namespace gdi {

    const Animation Animation::blend ()
    {
        return (Animation(AW_BLEND));
    }

    const Animation Animation::expand ()
    {
        return (Animation(AW_CENTER));
    }

    const Animation Animation::fade ()
    {
        return (AW_BLEND);
    }

    const Animation Animation::collapse ()
    {
        return (AW_CENTER);
    }

    Animation::Animation ( Value value )
        : myValue(value)
    {
    }

    Animation::Value Animation::value () const
    {
        return (myValue);
    }

} }