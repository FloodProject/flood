using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flood.Editor
{
    public interface UndoOperation<TDoc> where TDoc : Document<TDoc>
    {

        void Do(TDoc document);

        void Undo(TDoc document);

    }
}
