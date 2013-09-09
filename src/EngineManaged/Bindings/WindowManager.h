/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Engine/Window/WindowManager.h>

namespace Flood
{
    enum struct DirectoryDialogFlags;
    enum struct FileDialogFlags;
    ref class Window;
    ref class WindowManager;
    value struct WindowSettings;

    /// <summary>
    /// Specifies flags for opening file dialogs.
    /// </summary>
    [System::Flags]
    public enum struct FileDialogFlags
    {
        Open = 2,
        Save = 4,
        AllowMultipleFiles = 8
    };

    /// <summary>
    /// Specifies flags for opening directory dialogs.
    /// </summary>
    [System::Flags]
    public enum struct DirectoryDialogFlags
    {
        MustExist = 2
    };

    /// <summary>
    /// Window manager is responsible for managing the native windows in the
    /// platform. It also provides support for platform-specific dialogs.
    /// </summary>
    public ref class WindowManager : ICppInstance
    {
    public:

        property ::WindowManager* NativePtr;
        property System::IntPtr Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        WindowManager(::WindowManager* native);
        WindowManager(System::IntPtr native);
        WindowManager();

        /// <summary>
        /// Creates a new window.
        /// </summary>
        virtual Flood::Window^ CreateWindow(Flood::WindowSettings settings);

        /// <summary>
        /// Opens a file selection dialog.
        /// </summary>
        virtual Flood::Window^ OpenFileDialog(System::String^ wildcard, Flood::FileDialogFlags flags);

        /// <summary>
        /// Opens a directory selection dialog.
        /// </summary>
        virtual Flood::Window^ OpenDirectoryDialog(System::String^ wildcard, Flood::DirectoryDialogFlags flags);

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;

    };
}
