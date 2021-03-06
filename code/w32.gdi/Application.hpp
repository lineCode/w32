#ifndef _w32_gdi_Application_hpp__
#define _w32_gdi_Application_hpp__

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

#include "__configure__.hpp"
#include <w32.gdi/Dispatcher.hpp>
#include <w32.gdi/Window.hpp>
#include <w32.gdi/Font.hpp>
#include <w32.gdi/Brush.hpp>
#include <w32.gdi/quit.hpp>

namespace w32 { namespace gdi {

    template<class Implementation>
    class Application :
        public Window::Handler< Implementation >
    {
        /* class methods. */
    private:
            // Ask for the preferred font on modern Windows systems.
        static w32::gdi::Font font () {
            w32::gdi::Font::Attributes attributes;
            attributes.face(L"Segoe UI");
            attributes.width(10);
            return (w32::gdi::Font(attributes));
        }

        static w32::gdi::Brush brush () {
            return (w32::gdi::Brush(w32::gdi::SystemColor::buttonFace()));
        }

        static w32::gdi::Window window
            ( const w32::gdi::Font& font, const w32::gdi::Brush& brush )
        {
            const w32::gdi::Rectangle bounds(0, 500, 0, 300);
            w32::gdi::Window window(
                bounds, w32::gdi::Window::Style().overlappedWindow());
            window.font(font);
            window.background(brush);
            window.icon(w32::gdi::Icon::exclamation());
            return (window);
        }

        static Window window ( const Registration& registration,
            const w32::gdi::Font& font, const w32::gdi::Brush& brush )
        {
            const w32::gdi::Rectangle bounds(0, 500, 0, 300);
            w32::gdi::Window window(registration, bounds);
            window.font(font);
            window.background(brush);
            window.icon(w32::gdi::Icon::exclamation());
            return (window);
        }

        /* data. */
    private:
        Dispatcher< Implementation > myDispatcher;

            // Window-owned resources must live longer than the window.
        w32::gdi::Font myFont;
        w32::gdi::Brush myBrush;

            // Create window and controls.
        w32::gdi::Window myWindow;

        /* construction. */
    public:
        Application ( Implementation& implementation )
            : myDispatcher(implementation),
              myFont(font()), myBrush(brush()),
              myWindow(window(myFont, myBrush))
        {
                // Install ourselves as event handler.
            myWindow.handler(implementation);
        }

        Application ( const Registration& registration,
            Implementation& implementation )
            : myDispatcher(implementation),
              myFont(font()), myBrush(brush()),
              myWindow(window(registration, myFont, myBrush))
        {
                // Install ourselves as event handler.
            myWindow.handler(implementation);
        }

        /* methods. */
    public:
        Dispatcher< Implementation >& dispatcher ()
        {
            return (myDispatcher);
        }

        /* helpers. */
    protected:
        template<class T, void(Implementation::*F)(T)>
        delegate<Implementation,T> make_callback ()
        {
            return (callback<Implementation,T,F>());
        }

        Window& window () { return (myWindow); }

        /* event handlers. */
    public:
            // Don't user ask for confirmation.
        bool close () {
            w32::gdi::quit(); return (true);
        }

            // Handle control notification dispatch automagically...
        bool command
            ( w32::gdi::Window& source, ::UINT message )
        {
            return (myDispatcher(source, message));
        }

            // Handle control notification dispatch automagically...
        bool notify
            ( w32::gdi::Window& source, const ::NMHDR * header )
        {
            return (myDispatcher(source, header));
        }
    };

} }

#endif /* _w32_gdi_Application_hpp__ */
