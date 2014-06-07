using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.Panels
{

    public class BoxPanel : Panel
    {
        private Orientation m_Orientation;
        public Orientation Orientation
        {
            get { return m_Orientation; }
            set 
            {
                if(value == m_Orientation) 
                    return;

                m_Orientation = value;
                Invalidate();
            }
        }

        //this should only be called if control is Fit
        public override void LayoutSizes()
        {
            layoutMinSize = new Vector2i(MinNotSet, MinNotSet);
            layoutBestSize = new Vector2i(MinNotSet, MinNotSet);
            layoutMaxSize = new Vector2i(MinNotSet, MinNotSet);
            InheritedExpandHorizontal = false;
            InheritedExpandVertical = false;
            foreach (var child in Children)
            {
                if(child.IsHidden) continue;
                if(child.IsExpandHorizontal) InheritedExpandHorizontal = true; 
                if(child.IsExpandVertical) InheritedExpandVertical = true; 
                layoutMinSize = UpdateSize(layoutMinSize, child.LayoutMinSize);
                layoutBestSize = UpdateSize(layoutBestSize, child.LayoutBestSize);
                layoutMaxSize = UpdateSize(layoutMaxSize, child.LayoutMaxSize);
            }
        }

        private Vector2i UpdateSize(Vector2i thisSize, Vector2i childSize)
        {
            var thisMajor = MajorDirection(thisSize);
            var childMajor = MajorDirection(childSize);

            if(thisMajor != MaxNotSet)
                thisMajor = (childMajor == MaxNotSet)
                    ? childMajor 
                    : thisMajor + childMajor;

            var thisMinor = Math.Max(MinorDirection(thisSize), MinorDirection(childSize));

            var x = IsVertical() ? thisMinor : thisMajor;
            var y = IsVertical() ? thisMajor : thisMinor;

            return new Vector2i(x, y);
        }

        private bool IsVertical()
        {
            return Orientation == Orientation.Vertical;
        }

        public int MajorDirection(Vector2i vector)
        {
            return IsVertical() ? vector.Y : vector.X;
        }

        public int MinorDirection(Vector2i vector)
        {
            return IsVertical() ? vector.X : vector.Y;
        }


        public BoxPanel(Orientation orientation = Orientation.Vertical, ExpansionFlags expansion = ExpansionFlags.Fit)
            :base(expansion)
        {
            Orientation = orientation;
            new BoxSizer(this);
        }

        public void AddChild(IControl child, int index)
        {
            InsertChild(child, index);
            Invalidate();
        }

        public bool RemoveChild(int index, bool dispose)
        {
            try
            {
                RemoveChild(Children[index], dispose);
                Invalidate();
                return true;
            }
            catch (ArgumentOutOfRangeException)
            {
                return false;
            }
        }

        public bool SwapChildren(int first, int second)
        {
            try
            {
                var tmp = Children[second];
                Children[second] = Children[first];
                Children[first] = tmp;
                Invalidate();
                return true;

            }
            catch (ArgumentOutOfRangeException)
            {
                return false;
            }
        }

        public bool SwapChildren(IControl first, IControl second)
        {
            var element1 = Children.IndexOf(first);
            var element2 = Children.IndexOf(second);

            if(element1 < 0 || element2 < 0)
                return false;

            return SwapChildren(element1, element2);
        }


    }
}
