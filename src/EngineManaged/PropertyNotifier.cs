using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EngineManaged
{
    public class PropertyAttribute : Attribute
    {
        
    }


    public delegate bool PropertyChanged(object obj, string propertyName, object oldValue, object newValue);

    public interface IPropertyNotifier
    {
        event PropertyChanged PropertyChanged;
    }
}
