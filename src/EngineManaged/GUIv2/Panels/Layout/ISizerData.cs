using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flood.GUIv2.Panels.Layout
{
    interface ISizerData
    {
        Vector2i InnerLayoutMinSize { get; set; }
        Vector2i InnerLayoutBestSize { get; set; }
        Vector2i InnerLayoutMaxSize { get; set; }
    }
}
