// Copyright (c) 2009-2012, Andre Caron (andre.l.caron@gmail.com)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <w32.http/Library.hpp>
#include <w32/Error.hpp>

namespace {

        // Update as necessary to reflect most recent version!
    const w32::http::Library::Version
        default_version = w32::http::Library::Version::_2();

}

namespace w32 { namespace http {

    Library::Library ()
        : myVersion(::default_version), myFlags(HTTP_INITIALIZE_SERVER)
    {
        const ::ULONG result = ::HttpInitialize(myVersion.data(), myFlags, 0);
        if ( result != NO_ERROR ) {
            UNCHECKED_WIN32C_ERROR(HttpInitialize, result);
        }
    }

    Library::Library ( const Version& version )
        : myVersion(version), myFlags(HTTP_INITIALIZE_SERVER)
    {
        const ::ULONG result = ::HttpInitialize(myVersion.data(), myFlags, 0);
        if ( result != NO_ERROR ) {
            UNCHECKED_WIN32C_ERROR(HttpInitialize, result);
        }
    }

    Library::~Library ()
    {
        const ::ULONG result = ::HttpTerminate(myFlags, 0);
        if ( result != NO_ERROR ) {
            std::cerr << "[[[ " << Error(result) << " ]]]" << std::endl;
        }
    }

    const Library::Version& Library::version () const
    {
        return (myVersion);
    }

    const Library::Version Library::Version::_1 ()
    {
        const Data data = HTTPAPI_VERSION_1; return (data);
    }

    const Library::Version Library::Version::_2 ()
    {
        const Data data = HTTPAPI_VERSION_2; return (data);
    }

    Library::Version::Version ( const Data& data )
        : myData(data)
    {
    }

    Library::Version::Data& Library::Version::data ()
    {
        return (myData);
    }

    const Library::Version::Data& Library::Version::data () const
    {
        return (myData);
    }

    ushort Library::Version::major () const
    {
        return (myData.HttpApiMajorVersion);
    }

    ushort Library::Version::minor () const
    {
        return (myData.HttpApiMinorVersion);
    }

    bool Library::Version::operator== ( const Version& rhs ) const
    {
        return ((major() == rhs.major()) &&
                (minor() == rhs.minor()));
    }

    bool Library::Version::operator!= ( const Version& rhs ) const
    {
        return (!(*this == rhs));
    }

} }
