using Flood.GUIv2.ControlInternal;
using System;
using Flood.GUIv2.Panels;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.Controls
{
    /// <summary>
    /// CollapsibleCategory control. Used in CollapsibleList.
    /// </summary>
    public class CollapsibleCategory : CompositeControl
    {
        private readonly Button m_HeaderButton;
        private readonly CollapsibleList m_List;

        /// <summary>
        /// Header text.
        /// </summary>
        public String Text { get { return m_HeaderButton.Text; } set { m_HeaderButton.Text = value; } }

        /// <summary>
        /// Determines whether the category is collapsed (closed).
        /// </summary>
        public bool IsCollapsed { get { return m_HeaderButton.ToggleState; } set { m_HeaderButton.ToggleState = value; } }

        /// <summary>
        /// Invoked when an entry has been selected.
        /// </summary>
        public event Control.GwenEventHandler Selected;

        /// <summary>
        /// Invoked when the category collapsed state has been changed (header button has been pressed).
        /// </summary>
        public event Control.GwenEventHandler Collapsed;

        /// <summary>
        /// Initializes a new instance of the <see cref="CollapsibleCategory"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public CollapsibleCategory(CollapsibleList parent) : base(new RelativePanel())
        {
            throw new NotImplementedException();

            //m_HeaderButton = new CategoryHeaderButton(this);

            //m_HeaderButton.Text = "Category Title"; // [omeg] todo: i18n
            //m_HeaderButton.Alignment = AlignmentFlags.Top;
            //m_HeaderButton.Height = 20;
            //m_HeaderButton.Toggled += OnHeaderToggle;

            //m_List = parent;

            //Padding = new Padding(1, 0, 1, 5);
            ////SetSize(512, 512);
        }

        /// <summary>
        /// Gets the selected entry.
        /// </summary>
        public Button GetSelectedButton()
        {
            throw new NotImplementedException();

            //foreach (Control child in Children)
            //{
            //    CategoryButton button = child as CategoryButton;
            //    if (button == null)
            //        continue;

            //    if (button.ToggleState)
            //        return button;
            //}

            return null;
        }

        /// <summary>
        /// Handler for header button toggle event.
        /// </summary>
        /// <param name="control">Source control.</param>
        protected virtual void OnHeaderToggle(IControl control)
        {
            if (Collapsed != null)
                Collapsed.Invoke(this);
        }

        /// <summary>
        /// Handler for Selected event.
        /// </summary>
        /// <param name="control">Event source.</param>
        protected virtual void OnSelected(IControl control)
        {
            CategoryButton child = control as CategoryButton;
            if (child == null) return;

            if (m_List != null)
            {
                m_List.UnselectAll();
            }
            else
            {
                UnselectAll();
            }

            child.ToggleState = true;

            if (Selected != null)
                Selected.Invoke(this);
        }

        /// <summary>
        /// Returns the total width and height of all children.
        /// </summary>
        /// <remarks>Default implementation returns maximum size of children since the layout is unknown.
        /// Implement this in derived compound controls to properly return their size.</remarks>
        /// <returns></returns>
        public override Vector2i GetChildrenSize()
        {
            throw new NotImplementedException();
           
            //var size = new Vector2i(0, 0);

            //foreach (Control child in Children)
            //{
            //    if (child.IsHidden)
            //        continue;

            //    size.X = Math.Max(size.X, child.Right);
            //    size.Y += child.Bottom - child.Y;
            //}

            //return size;
        }

        /// <summary>
        /// Adds a new entry.
        /// </summary>
        /// <param name="name">Entry name (displayed).</param>
        /// <returns>Newly created control.</returns>
        public Button Add(String name)
        {
            throw new NotImplementedException();

            //CategoryButton button = new CategoryButton(this);
            //button.Text = name;
            //button.Alignment = AlignmentFlags.Top;
            //button.SizeToContents();
            //button.SetSize(button.Width + 4, button.Height + 4);
            //button.Padding = new Padding(5, 2, 2, 2);
            //button.Clicked += OnSelected;

            //return button;
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public override void Render(Skins.Skin skin)
        {
            throw new NotImplementedException();
            skin.DrawCategoryInner(this, m_HeaderButton.ToggleState);
            base.Render(skin);
        }

        /// <summary>
        /// Unselects all entries.
        /// </summary>
        public void UnselectAll()
        {
            throw new NotImplementedException();
            //foreach (Control child in Children)
            //{
            //    CategoryButton button = child as CategoryButton;
            //    if (button == null)
            //        continue;

            //    button.ToggleState = false;
            //}
        }

        /// <summary>
        /// Function invoked after layout.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        //public override void PostLayout(Skins.Skin skin)
        //{
        //    throw new NotImplementedException();
        //    //if (IsCollapsed)
        //    //{
        //    //    Height = m_HeaderButton.Height;
        //    //}
        //    //else
        //    //{
        //    //    SizeToChildren(false, true);
        //    //}

        //    //// alternate row coloring
        //    //bool b = true;
        //    //foreach (Control child in Children)
        //    //{
        //    //    CategoryButton button = child as CategoryButton;
        //    //    if (button == null)
        //    //        continue;

        //    //    button.m_Alt = b;
        //    //    button.UpdateColors();
        //    //    b = !b;
        //    //}
        //}

        public override bool InformFirstDirection(BoxOrientation direction, int size, int availableOtherDir)
        {
            throw new NotImplementedException();
        }
    }
}
