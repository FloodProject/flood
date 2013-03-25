
#include "Editor/API.h"
#include "CLIInterop.h"
#include <vcclr.h>

#using "Editor.Client.dll"
#using "EngineManaged.dll"
#using "EngineBindings.dll"
#using <System.Drawing.dll>

using namespace Flood::GUI; 
using namespace System::Collections::Generic;

static gcroot<Editor::Client::NativeGUI^> gs_GUIInstance = nullptr;

void InitializeGUI(InputManager* inputManager)
{
	gs_GUIInstance = gcnew Editor::Client::NativeGUI(System::IntPtr(inputManager));
}

void UpdateGUI()
{
	gs_GUIInstance->Editor->Update();
}

void ResizeGUI(int x, int y)
{
	gs_GUIInstance->SetSize(x, y);
}

void CloseGUI()
{
	gs_GUIInstance->Close();
	delete(gs_GUIInstance);
	gs_GUIInstance = nullptr; 
}

void RenderGUI(RenderBlock& rb)
{
	auto renderBlock = gcnew Flood::RenderBlock(System::IntPtr(&rb));
	gs_GUIInstance->Render(renderBlock);
}

void SetMainWindow(Window* window)
{
	auto NativeWindow = gcnew Flood::Window(System::IntPtr(window));
	gs_GUIInstance->Editor->MainWindow->NativeWindow = NativeWindow;
}