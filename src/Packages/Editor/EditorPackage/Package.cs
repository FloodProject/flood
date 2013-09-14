
using System;
using System.Threading;
using Editor.Services;
using Flood.GUI.Controls;
using Flood.Packages;
using Flood.RPC;

namespace Editor
{
    public class Package : IPackage
    {
        public static PaneManager PaneManager;
        public static EditorBase Editor;
        public static Canvas canvas;

        public void OnLoad(RPCManager rpcManager)
        {
            if(PaneManager != null)
                throw new Exception();

            PaneManager = new PaneManager();

            rpcManager.ServiceManager.GetCreateImplementation<IPaneManager>(PaneManager);

            Editor = new EditorBase();

            Editor.Run();
        }

        public void Update()
        {
            Editor.Update();
        }
    }
}
