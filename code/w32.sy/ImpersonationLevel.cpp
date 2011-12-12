/*!
 * @file w32/sy/ImpersonationLevel.cpp
 * @author Andre Caron
 */

#include <w32.sy/ImpersonationLevel.hpp>
#include <w32/Error.hpp>

namespace w32 { namespace sy {

    const ImpersonationLevel ImpersonationLevel::anonymous(SecurityAnonymous);
    const ImpersonationLevel
    ImpersonationLevel::identification(SecurityIdentification);
    const ImpersonationLevel
    ImpersonationLevel::impersonation(SecurityImpersonation);
    const ImpersonationLevel ImpersonationLevel::delegation(SecurityDelegation);

    ImpersonationLevel::ImpersonationLevel
        ( ::SECURITY_IMPERSONATION_LEVEL value )
        : myValue(value)
    {
    }

    ImpersonationLevel ImpersonationLevel::of ( const Token& token )
    {
        ::SECURITY_IMPERSONATION_LEVEL information = SecurityAnonymous;
        ::DWORD length = 0;
        const ::BOOL result = ::GetTokenInformation
            (token.handle(), TokenImpersonationLevel,
             &information, sizeof(information), &length);
        if ( result == 0 ) {
            UNCHECKED_WIN32C_ERROR(GetTokenInformation,::GetLastError());
        }
        return (ImpersonationLevel(information));
    }

    ::SECURITY_IMPERSONATION_LEVEL ImpersonationLevel::value () const
    {
        return (myValue);
    }

    bool ImpersonationLevel::operator==
        ( const ImpersonationLevel& other ) const
    {
        return (myValue == other.myValue);
    }

    bool ImpersonationLevel::operator!=
        ( const ImpersonationLevel& other ) const
    {
        return (myValue != other.myValue);
    }

} }
