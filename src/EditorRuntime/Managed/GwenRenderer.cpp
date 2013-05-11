/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Editor/API.h"
#include "Engine/Input/InputManager.h"
#include "Graphics/RenderQueue.h"
#include <vcclr.h>

#using "Editor.Client.dll"
#using "EngineManaged.dll"
#using "EngineBindings.dll"

using namespace Flood::GUI;
using namespace Flood::Editor::Client;
using namespace System::Collections::Generic;

static gcroot<Editor^> gs_EditorInstance = nullptr;

void InitializeGUI(InputManager* inputManager)
{
    gs_EditorInstance = gcnew Editor();
}

void UpdateGUI()
{
    gs_EditorInstance->Update();
}

void ResizeGUI(int x, int y)
{
    gs_EditorInstance->SetSize(x, y);
}

void CloseGUI()
{
    delete(gs_EditorInstance);
    gs_EditorInstance = nullptr;
}

void RenderGUI(RenderBlock& rb)
{
    auto renderBlock = gcnew Flood::RenderBlock(System::IntPtr(&rb));
    gs_EditorInstance->Render(renderBlock);
}

void SetMainWindow(Window* window)
{
    auto NativeWindow = gcnew Flood::Window(System::IntPtr(window));
    gs_EditorInstance->MainWindow->NativeWindow = NativeWindow;
}