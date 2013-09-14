
using System;
using FLDEditor.Services;
using Flood.GUI.Controls;
using Flood.Packages;
using Flood.RPC;

namespace FLDEditor
{
    public class Package : IPackage
    {
        public static PaneManager PaneManager;
        public static Editor Editor;
        public static Canvas canvas;

        public void OnLoad(RPCManager rpcManager)
        {
            if(PaneManager != null)
                throw new Exception();

            PaneManager = new PaneManager();

            rpcManager.ServiceManager.GetCreateImplementation<IPaneManager>(PaneManager);

            Editor = new Editor();

            Editor.Run();
        }

        public void Update()
        {
            Editor.Update();
        }
    }
}
