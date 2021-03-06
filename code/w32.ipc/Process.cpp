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

#include <w32.ipc/Process.hpp>
#include <w32/astring.hpp>
#include <w32/Error.hpp>

namespace {

    ::HANDLE clone ( ::HANDLE handle, ::HANDLE sprocess, ::HANDLE tprocess )
    {
        ::HANDLE duplicate = INVALID_HANDLE_VALUE;
        const ::BOOL result = ::DuplicateHandle(
            sprocess, handle, tprocess, &duplicate,
            0, FALSE, DUPLICATE_SAME_ACCESS
            );
        if ( result == FALSE ) {
            UNCHECKED_WIN32C_ERROR(DuplicateHandle, ::GetLastError());
        }
        return (duplicate);
    }

    ::HANDLE clone ( ::HANDLE handle, ::HANDLE process )
    {
        return (clone(handle, process, process));
    }

    ::HANDLE clone ( ::HANDLE handle )
    {
        return (clone(handle, ::GetCurrentProcess()));
    }

    ::HANDLE create ( ::LPWSTR executable, ::BOOL inherit )
    {
        static const ::DWORD flags = 0;
        //flags = EXTENDED_STARTUPINFO_PRESENT;
        
        ::STARTUPINFOW startup;
        ::ZeroMemory(&startup, sizeof(startup));
        //startup.dwFlags = STARTF_USESTDHANDLES;
        //startup.hStdInput =
        //startup.hStdOutput =
        //startup.hStdError =
        
        ::PROCESS_INFORMATION information;
        const ::BOOL result = ::CreateProcessW(
            0, executable, 0, 0, inherit, flags, 0, 0, &startup, &information);
        if ( result == 0 ) {
            UNCHECKED_WIN32C_ERROR(CreateProcess, ::GetLastError());
        }
        
            // Close the thread handle, it's not necessary.
        w32::Object::claim(information.hThread);
        
        return (information.hProcess);
    }

    ::HANDLE find ( ::DWORD id, ::DWORD access=PROCESS_ALL_ACCESS )
    {
        const ::HANDLE result = ::OpenProcess(access, FALSE, id);
        if ( result == NULL ) {
            UNCHECKED_WIN32C_ERROR(OpenProcess, ::GetLastError());
        }
        return (result);
    }

    ::LPVOID relocate ( ::LPVOID base, ::DWORD offset )
    {
        return (static_cast<::LPBYTE>(base) + offset);
    }

}

namespace w32 { namespace ipc {

    Process Process::current ()
    {
        const Handle handle(Object::claim(::clone(::GetCurrentProcess())));
        return (Process(handle));
    }

    Process::Process ( const Handle& handle )
        : Object(handle)
    {
    }

    Process::Process ( Identifier identifier )
        : Object(Object::claim(::find(identifier)))
    {
    }

    Process::Process ( Identifier identifier, Access access )
        : Object(Object::claim(::find(identifier, access)))
    {
    }

    Process::Process ( const string& executable, bool inherit )
        : Object(Object::claim(::create(
            const_cast<::LPWSTR>(executable.c_str()), inherit?TRUE:FALSE)))
    {
    }

    void Process::priority ( const Priority& priority )
    {
        const ::BOOL result = ::SetPriorityClass(
            handle(), priority.value()
            );
        if ( result == FALSE ) {
            UNCHECKED_WIN32C_ERROR(SetPriorityClass,::GetLastError());
        }
    }

    bool Process::boosted () const
    {
        ::BOOL disabled = FALSE;
        const ::BOOL result = ::GetProcessPriorityBoost(
            handle(), &disabled
            );
        if ( result == FALSE ) {
            UNCHECKED_WIN32C_ERROR(GetProcessPriorityBoost,::GetLastError());
        }
        return (disabled == FALSE);
    }

    void Process::boosted ( bool boost )
    {
        const ::BOOL disable = boost? FALSE : TRUE;
        const ::BOOL result = ::SetProcessPriorityBoost(
            handle(), disable
            );
        if ( result == FALSE ) {
            UNCHECKED_WIN32C_ERROR(SetProcessPriorityBoost,::GetLastError());
        }
    }

    dword Process::handles () const
    {
        ::DWORD count = 0;
        const ::BOOL result = ::GetProcessHandleCount(handle(),&count);
        if ( result == FALSE ) {
            UNCHECKED_WIN32C_ERROR(GetProcessHandleCount,::GetLastError());
        }
        return (count);
    }

    void Process::terminate ( uint status )
    {
        const ::BOOL result = ::TerminateProcess(handle(), status);
        if ( result == FALSE ) {
            UNCHECKED_WIN32C_ERROR(TerminateProcess, ::GetLastError());
        }
    }

    dword Process::status () const
    {
        ::DWORD value = 0;
        const ::BOOL result = ::GetExitCodeProcess(handle(), &value);
        if ( result == FALSE ) {
            UNCHECKED_WIN32C_ERROR(GetExitCodeProcess, ::GetLastError());
        }
        return (value);
    }

    bool Process::alive () const
    {
        return (status() == STILL_ACTIVE);
    }

    void Process::join () const
    {
        Waitable(*this).wait();
    }

    bool Process::join ( const Timespan& timeout ) const
    {
        return (Waitable(*this).wait(timeout));
    }

    bool Process::test () const
    {
        return (Waitable(*this).test());
    }

    pointer Process::acquire ( size_t size )
    {
        const ::LPVOID result = ::VirtualAllocEx
            (handle(), 0, size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
        if ( result == 0 )
        {
            const ::DWORD error = ::GetLastError();
            UNCHECKED_WIN32C_ERROR(VirtualAllocEx, error);
        }
        return (result);
    }

    void Process::release ( pointer base )
    {
        const ::BOOL result = ::VirtualFreeEx
            (handle(), base, 0, MEM_RELEASE);
        if ( result == FALSE )
        {
            const ::DWORD error = ::GetLastError();
            UNCHECKED_WIN32C_ERROR(VirtualFreeEx, error);
        }
    }

    size_t Process::get ( pointer base, void * data, size_t size ) const
    {
        ::SIZE_T transferred = 0;
        const ::BOOL result = ::ReadProcessMemory
            (handle(), base, data, size, &transferred);
        if ( result == FALSE )
        {
            const ::DWORD error = ::GetLastError();
            UNCHECKED_WIN32C_ERROR(ReadProcessMemory, error);
        }
        return (transferred);
    }

    size_t Process::put ( pointer base, const void * data, size_t size )
    {
        ::SIZE_T transferred = 0;
        const ::BOOL result = ::WriteProcessMemory
            (handle(), base, data, size, &transferred);
        if ( result == FALSE )
        {
            const ::DWORD error = ::GetLastError();
            UNCHECKED_WIN32C_ERROR(WriteProcessMemory, error);
        }
        return (transferred);
    }

    mt::Thread Process::call ( pointer code, pointer base )
    {
        typedef ::LPTHREAD_START_ROUTINE CodePointer;
        const ::HANDLE result = ::CreateRemoteThread
            (handle(), 0, 0, reinterpret_cast<CodePointer>(code), base, 0, 0);
        if ( result == 0 )
        {
            const ::DWORD error = ::GetLastError();
            UNCHECKED_WIN32C_ERROR(CreateRemoteThread, error);
        }
        return (mt::Thread(mt::Thread::claim(result)));
    }

    pointer Process::symbol ( pointer base, const astring& name )
    {
            // Read executable core header.
        ::IMAGE_DOS_HEADER prelude;
        get(base, &prelude, sizeof(prelude));
            // Read executable real headers.
        ::IMAGE_NT_HEADERS headers;
        get(relocate(base, prelude.e_lfanew), &headers, sizeof(headers));
        ::IMAGE_DATA_DIRECTORY& data =
            headers.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
            // Read exports dictionary.
        ::IMAGE_EXPORT_DIRECTORY exports;
        get(relocate(base, data.VirtualAddress), &exports, sizeof(exports));
            // Find requested symbol.
        ::CHAR symbol[MAX_MODULE_NAME32+1];
        for ( ::DWORD i = 0; (0 < exports.NumberOfNames); ++i )
        {
                // Read function name's (relative) address.
            ::DWORD offset = 0;
            ::DWORD delta = exports.AddressOfNames + i*sizeof(offset);
            get(relocate(base, delta), &offset, sizeof(offset));
                // Read function name.
            get(relocate(base, offset), symbol, sizeof(symbol));
                // Check if it matches the requested name.
            if ( ::strcmp(symbol, name.data()) == 0 )
            {
                    // Read function's (relative) address.
                offset = 0;
                delta = exports.AddressOfFunctions + i*sizeof(offset);
                get(relocate(base, delta), &offset, sizeof(offset));
                    // Return (relative) address.
                return (relocate(base, offset));
            }
        }
            // Couldn't find it.
        return (0);
    }

    const Process::Priority Process::Priority::higher ()
    {
        return (ABOVE_NORMAL_PRIORITY_CLASS);
    }

    const Process::Priority Process::Priority::lower ()
    {
        return (BELOW_NORMAL_PRIORITY_CLASS);
    }

    const Process::Priority Process::Priority::high ()
    {
        return (HIGH_PRIORITY_CLASS);
    }

    const Process::Priority Process::Priority::idle ()
    {
        return (IDLE_PRIORITY_CLASS);
    }

    const Process::Priority Process::Priority::normal ()
    {
        return (NORMAL_PRIORITY_CLASS);
    }

    const Process::Priority Process::Priority::realtime ()
    {
        return (REALTIME_PRIORITY_CLASS);
    }

    Process::Priority::Priority ( Value value )
        : myValue(value)
    {
    }

    Process::Priority Process::Priority::of ( const Process& process )
    {
        const ::DWORD result = ::GetPriorityClass(process.handle());
        if ( result == 0 ) {
            UNCHECKED_WIN32C_ERROR(GetPriorityClass,::GetLastError());
        }
        return (Priority(result));
    }

    Process::Priority::Value Process::Priority::value () const
    {
        return (myValue);
    }

    Process::Identifier Process::identifier () const
    {
        return (::GetProcessId(handle()));
    }

    Process::Priority Process::priority () const
    {
        return (Priority::of(*this));
    }

    void exit ( dword code )
    {
        ::ExitProcess(code);
    }

    Process::operator Waitable () const
    {
        return Waitable(handle());
    }

    Process::Access::Access ()
        : myMask(0)
    {
    }

    Process::Access& Process::Access::delete_ ()
    {
        myMask |= DELETE; return (*this);
    }

    Process::Access& Process::Access::read_control ()
    {
        myMask |= READ_CONTROL; return (*this);
    }

    Process::Access& Process::Access::synchronize ()
    {
        myMask |= SYNCHRONIZE; return (*this);
    }

    Process::Access& Process::Access::write_dacl ()
    {
        myMask |= WRITE_DAC; return (*this);
    }

    Process::Access& Process::Access::write_owner ()
    {
        myMask |= WRITE_OWNER; return (*this);
    }

    Process::Access& Process::Access::all ()
    {
        myMask |= PROCESS_ALL_ACCESS; return (*this);
    }

    Process::Access& Process::Access::create_process ()
    {
        myMask |= PROCESS_CREATE_PROCESS; return (*this);
    }

    Process::Access& Process::Access::create_thread ()
    {
        myMask |= PROCESS_CREATE_THREAD; return (*this);
    }

    Process::Access& Process::Access::duplicate_handle ()
    {
        myMask |= PROCESS_DUP_HANDLE; return (*this);
    }

    Process::Access& Process::Access::query_information ()
    {
        myMask |= PROCESS_QUERY_INFORMATION; return (*this);
    }

    Process::Access& Process::Access::query_limited_information ()
    {
        myMask |= PROCESS_QUERY_LIMITED_INFORMATION; return (*this);
    }

    Process::Access& Process::Access::set_information ()
    {
        myMask |= PROCESS_SET_INFORMATION; return (*this);
    }

    Process::Access& Process::Access::set_quota ()
    {
        myMask |= PROCESS_SET_QUOTA; return (*this);
    }

    Process::Access& Process::Access::suspend_resume ()
    {
        myMask |= PROCESS_SUSPEND_RESUME; return (*this);
    }

    Process::Access& Process::Access::terminate ()
    {
        myMask |= PROCESS_TERMINATE; return (*this);
    }

    Process::Access& Process::Access::vm_operation ()
    {
        myMask |= PROCESS_VM_OPERATION; return (*this);
    }

    Process::Access& Process::Access::vm_read ()
    {
        myMask |= PROCESS_VM_READ; return (*this);
    }

    Process::Access& Process::Access::vm_write ()
    {
        myMask |= PROCESS_VM_WRITE; return (*this);
    }

    Process::Access::operator Mask () const
    {
        return (myMask);
    }

} }
