/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "WxPlatform.h"
#include "wx/app.h"
#include "wx/evtloop.h"
#include "wx/apptrait.h"

FLD_NAMESPACE_BEGIN

//-----------------------------------//

WxPlatform::WxPlatform()
    : windowManager(this)
{
}

//-----------------------------------//

void WxPlatform::init()
{
    wxApp::SetInstance(new wxApp());

    int argc = 0;
    if (!wxEntryStart(argc, /*argv=*/(char**)0))
        return;

    wxTheApp->OnInit();

    auto eventLoop = wxTheApp->GetTraits()->CreateEventLoop();
    wxEventLoopBase::SetActive(eventLoop);

    wxImage::AddHandler(new wxPNGHandler());
}

//-----------------------------------//

WxPlatform::~WxPlatform()
{
    wxTheApp->OnExit();
    wxEntryCleanup();
}

//-----------------------------------//

void WxPlatform::update()
{
    if (wxTheApp->Pending())
        wxTheApp->Dispatch();
    else
        wxTheApp->ProcessIdle();
}

//-----------------------------------//

WindowManager* WxPlatform::getWindowManager()
{
    return &windowManager;
}

//-----------------------------------//

InputManager* WxPlatform::getInputManager()
{
    return &inputManager;
}

//-----------------------------------//

FLD_NAMESPACE_END

WxPlatform* CreateWxPlatformManager()
{
    return AllocateHeap(WxPlatform);
}