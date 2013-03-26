using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Flood.Editor.Client.Extensions;
using Flood.GUI;
using Mono.Addins;

namespace Flood.Editor.Client
{

    public static class IconManager
    {
        static readonly Dictionary<int,StockIconCodon> IconStock = new Dictionary<int,StockIconCodon>();
        static readonly Dictionary<int,Bitmap> IconData = new Dictionary<int,Bitmap>();

        static IconManager()
        {
            AddinManager.AddExtensionNodeHandler(
                "/Flood/Editor/Client/StockIcons",
                delegate(object sender, ExtensionNodeEventArgs args)
                    {
                        var iconCodon = (StockIconCodon) args.ExtensionNode;
                        switch (args.Change)
                        {
                            case ExtensionChange.Add:
                                AddStockIcon(iconCodon);
                                break;
                            case ExtensionChange.Remove:
                                RemoveStockIcon(iconCodon);
                                break;
                        }
                    });

            foreach (ExtensionNode node in AddinManager.GetExtensionNodes ("/Flood/Editor/Client/StockIcons")) {
                AddStockIcon((StockIconCodon)node);
            }
        }

        private static void AddStockIcon(StockIconCodon iconCodon)
        {
            var hash = iconCodon.GetHashCode();
            if (!IconStock.ContainsKey(hash))
            {
                IconStock.Add(hash,iconCodon);
            }
        }

        private static void RemoveStockIcon(StockIconCodon iconCodon)
        {
            var hash = iconCodon.GetHashCode();
            if (IconStock.ContainsKey(hash))
                IconStock.Remove(hash);
        }

        public static Bitmap GetIconData(string iconId, IconSize iconSize)
        {
            var hash = StockIconCodon.ComputeHashCode(iconId, iconSize);
            if (IconData.ContainsKey(hash))
                return IconData[hash];

            if (!IconStock.ContainsKey(hash))
               throw new Exception("Icon "+iconId+" not available!");

            var iconCodon = IconStock[hash];

            if (!string.IsNullOrEmpty (iconCodon.Resource) || !string.IsNullOrEmpty (iconCodon.File)) {
                Bitmap bitmap;
                System.IO.Stream stream;
                if (iconCodon.Resource != null)
                    stream = iconCodon.Addin.GetResource (iconCodon.Resource);
                else
                    stream = File.OpenRead (iconCodon.Addin.GetFilePath (iconCodon.File));
                using (stream) {
                    if (stream == null || stream.Length < 0) {
                        throw new Exception(string.Format("Did not find resource '{0}' in addin '{1}' for icon '{2}'", 
                                                    iconCodon.Resource, iconCodon.Addin.Id, iconCodon.StockId));
                    }
                    bitmap = (Bitmap) System.Drawing.Image.FromStream(stream);
                }
                

                IconData[hash] = bitmap;
            }
            
            return IconData[hash];
        }
    }
}
