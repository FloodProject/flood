using System;

namespace Flood
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
