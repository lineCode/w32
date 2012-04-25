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

#include "User.hpp"

#include "Identifier.hpp"
#include "Token.hpp"

#include <w32/Error.hpp>

namespace w32 { namespace sy {

    User::User ( const Token& token )
        : myData(0)
    {
        ::DWORD size = 0;
        ::BOOL status = ::GetTokenInformation
            (token.handle(), ::TokenUser, 0, 0, &size);
        ::DWORD error = ::GetLastError();
        while ((status == 0) && (error == ERROR_INSUFFICIENT_BUFFER))
        {
            myData = operator new(size);
            ::ZeroMemory(myData, size);
            status = ::GetTokenInformation
                (token.handle(), ::TokenUser, myData, size, &size);
            error = ::GetLastError();
        }
        if (status == 0) {
            UNCHECKED_WIN32C_ERROR(GetTokenInformation, error);
        }
    }

    User::~User ()
    {
        operator delete(myData); myData = 0;
    }

    User::Data& User::data ()
    {
        return (*static_cast<Data*>(myData));
    }

    const User::Data& User::data () const
    {
        return (*static_cast<const Data*>(myData));
    }

    dword User::attributes () const
    {
        return (data().User.Attributes);
    }

    Identifier User::identifier () const
    {
        return (Identifier(data().User.Sid));
    }

} }