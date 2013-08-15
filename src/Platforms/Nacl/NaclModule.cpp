/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Log.h"
#include "Core/String.h"

#include <ppapi/c/pp_errors.h>
#include <ppapi/c/pp_module.h>
#include <ppapi/c/pp_var.h>
#include <ppapi/c/ppb.h>
#include <ppapi/c/ppb_core.h>
#include <ppapi/c/ppb_console.h>
#include <ppapi/c/ppb_instance.h>
#include <ppapi/c/ppb_messaging.h>
#include <ppapi/c/ppb_var.h>
#include <ppapi/c/ppp.h>
#include <ppapi/c/ppp_instance.h>
#include <ppapi/c/ppp_messaging.h>

#include <cstring>

static PP_Instance ppInstance = 0;

static PP_Bool Instance_DidCreate(PP_Instance instance, uint32_t argc,
                                  const char* argn[], const char* argv[])
{
    ppInstance = instance;
    return PP_TRUE;
}

static void Instance_DidDestroy(PP_Instance instance)
{
}

static void Instance_DidChangeView(PP_Instance instance, PP_Resource viewResource)
{
}

static void Instance_DidChangeFocus(PP_Instance instance, PP_Bool hasFocus)
{
}

static PP_Bool Instance_HandleDocumentLoad(PP_Instance instance,
                                           PP_Resource urlLoader)
{
    // NaCl modules do not need to handle the document load function.
    return PP_FALSE;
}

static PPB_Core* ppbCoreInterface = 0;
static PPB_Console* ppbConsoleInterface = 0;
static PPB_Messaging* ppbMessagingInterface = 0;
static PPB_Var* ppbVarInterface = 0;

PP_Time GetTime()
{
    return ppbCoreInterface->GetTime();
}

static PP_LogLevel GetPPLogLevel(LogLevel logLevel)
{
    PP_LogLevel level;
    switch(logLevel)
    {
    case LogLevel::Debug:
        level = PP_LOGLEVEL_TIP;
        break;
    case LogLevel::Info:
        level = PP_LOGLEVEL_LOG;
        break;
    case LogLevel::Warn:
        level = PP_LOGLEVEL_WARNING;
        break;
    case LogLevel::Assert:
    case LogLevel::Error:
        level = PP_LOGLEVEL_ERROR;
        break;
    }

    return level;
}

static void LogConsole(LogEntry* entry)
{
    PP_LogLevel level = GetPPLogLevel(entry->level);

    const String& msg = entry->struct;
    PP_Var var = ppbVarInterface->VarFromUtf8(msg.data(), msg.size());

    ppbConsoleInterface->Log(ppInstance, level, var);

    ppbVarInterface->Release(var);
}

PP_EXPORT int32_t PPP_InitializeModule(PP_Module module,
                                       PPB_GetInterface getBrowser)
{
    ppbCoreInterface = (PPB_Core*)(getBrowser(PPB_CORE_INTERFACE));
    ppbConsoleInterface = (PPB_Console*)(getBrowser(PPB_CONSOLE_INTERFACE));
    ppbMessagingInterface = (PPB_Messaging*)(getBrowser(PPB_MESSAGING_INTERFACE));
    ppbVarInterface = (PPB_Var*)(getBrowser(PPB_VAR_INTERFACE));

    LogAddHandler(LogGetDefault(), LogConsole);
    LogInfo("Initializing Native Client module");

    return PP_OK;
}

PP_EXPORT void PPP_ShutdownModule()
{
    LogInfo("Shutting down Native Client module");
    LogRemoveHandler(LogGetDefault(), LogConsole);
}

PP_EXPORT const void* PPP_GetInterface(const char* interface)
{
    if (strcmp(interface, PPP_INSTANCE_INTERFACE) != 0)
        return 0;

    static PPP_Instance instance_interface =
    {
        &Instance_DidCreate,
        &Instance_DidDestroy,
        &Instance_DidChangeView,
        &Instance_DidChangeFocus,
        &Instance_HandleDocumentLoad,
    };

    return &instance_interface;
}

int main() {}