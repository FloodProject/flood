/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Engine/Input/KeyboardEvents.h>
#include "Device.h"

namespace Flood
{
    enum struct KeyboardEventType;
    enum struct Keys;
    ref class KeyEvent;

    public enum struct Keys
    {
        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,
        Num0 = 48,
        Num1 = 49,
        Num2 = 50,
        Num3 = 51,
        Num4 = 52,
        Num5 = 53,
        Num6 = 54,
        Num7 = 55,
        Num8 = 56,
        Num9 = 57,
        Escape = 256,
        LControl = 257,
        LShift = 258,
        LAlt = 259,
        LSuper = 260,
        RControl = 261,
        RShift = 262,
        RAlt = 263,
        RSuper = 264,
        Menu = 265,
        LBracket = 266,
        RBracket = 267,
        SemiColon = 268,
        Comma = 269,
        Period = 270,
        Quote = 271,
        Slash = 272,
        BackSlash = 273,
        Tilde = 274,
        Equal = 275,
        Dash = 276,
        Space = 277,
        Return = 278,
        Back = 279,
        Tab = 280,
        PageUp = 281,
        PageDown = 282,
        End = 283,
        Home = 284,
        Insert = 285,
        Delete = 286,
        Add = 287,
        Subtract = 288,
        Multiply = 289,
        Divide = 290,
        Left = 291,
        Right = 292,
        Up = 293,
        Down = 294,
        Numpad0 = 295,
        Numpad1 = 296,
        Numpad2 = 297,
        Numpad3 = 298,
        Numpad4 = 299,
        Numpad5 = 300,
        Numpad6 = 301,
        Numpad7 = 302,
        Numpad8 = 303,
        Numpad9 = 304,
        F1 = 305,
        F2 = 306,
        F3 = 307,
        F4 = 308,
        F5 = 309,
        F6 = 310,
        F7 = 311,
        F8 = 312,
        F9 = 313,
        F10 = 314,
        F11 = 315,
        F12 = 316,
        F13 = 317,
        F14 = 318,
        F15 = 319,
        Pause = 320,
        MAX = 321
    };

    public enum struct KeyboardEventType
    {
        KeyPressed = 0,
        KeyReleased = 1,
        KeyText = 2
    };

    public ref class KeyEvent : Flood::InputEvent
    {
    public:
        KeyEvent(::KeyEvent* native);
        KeyEvent(System::IntPtr native);
        KeyEvent(Flood::KeyboardEventType eventType);
        property Flood::KeyboardEventType EventType
        {
            Flood::KeyboardEventType get();
            void set(Flood::KeyboardEventType);
        }
        property Flood::Keys KeyCode
        {
            Flood::Keys get();
            void set(Flood::Keys);
        }
        property bool AltPressed
        {
            bool get();
            void set(bool);
        }
        property bool ShiftPressed
        {
            bool get();
            void set(bool);
        }
        property bool CtrlPressed
        {
            bool get();
            void set(bool);
        }
        property unsigned int Unicode
        {
            unsigned int get();
            void set(unsigned int);
        }
        virtual bool Equals(System::Object^ object) override;
        virtual int GetHashCode() override;
    };
}
