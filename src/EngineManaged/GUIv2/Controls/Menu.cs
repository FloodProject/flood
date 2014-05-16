using Flood.GUIv2.ControlInternal;
using System;
using System.Linq;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.Controls
{
    /// <summary>
    /// Popup menu.
    /// </summary>
    public class Menu : ScrollControl
    {
        private bool m_DisableIconMargin;
        private bool m_DeleteOnClose;

        public override bool IsMenuComponent { get { return true; } }
        
        public bool IconMarginDisabled { get { return m_DisableIconMargin; } set { m_DisableIconMargin = value; } }
        
        /// <summary>
        /// Determines whether the menu should be disposed on close.
        /// </summary>
        public bool DeleteOnClose { get { return m_DeleteOnClose; } set { m_DeleteOnClose = value; } }

        /// <summary>
        /// Determines whether the menu should open on mouse hover.
        /// </summary>
        protected virtual bool ShouldHoverOpenMenu { get { return true; } }

        /// <summary>
        /// Initializes a new instance of the <see cref="Menu"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public Menu(Control parent)
            : base(parent)
        {
            SetBounds(0, 0, 10, 10);
            Margin = Margin.Two;
            IconMarginDisabled = false;

            AutoHideBars = true;
            EnableScroll(false, true);
            DeleteOnClose = false;
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public override void Render(Skins.Skin skin)
        {
            skin.DrawMenu(this, IconMarginDisabled);
        }

        /// <summary>
        /// Renders under the actual control (shadows etc).
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public override void RenderUnder(Skins.Skin skin)
        {
            throw new NotImplementedException();

            base.RenderUnder(skin);
            skin.DrawShadow(this);
        }

        /// <summary>
        ///  Opens the menu.
        /// </summary>
        /// <param name="pos">Unused.</param>
        public void Open(Pos pos)
        {
            IsHidden = false;
            BringToFront();
            var mouse = Input.InputHandler.MousePosition;
            SetPosition(mouse.X, mouse.Y);
        }

        /// <summary>
        /// Lays out the control's interior according to alignment, padding, dock etc.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public override void Layout(Skins.Skin skin)
        {
            throw new NotImplementedException();

            //int childrenHeight = Children.Sum(child => child != null ? child.Height : 0);

            //if (Y + childrenHeight > GetCanvas().Height)
            //    childrenHeight = GetCanvas().Height - Y;

            //SetSize(Width, childrenHeight);

            //base.Layout(skin);
        }

        /// <summary>
        /// Adds a new menu item.
        /// </summary>
        /// <param name="text">Item text.</param>
        /// <returns>Newly created control.</returns>
        public virtual MenuItem AddItem(String text)
        {
            return AddItem(text, String.Empty);
        }

        /// <summary>
        /// Adds a new menu item.
        /// </summary>
        /// <param name="text">Item text.</param>
        /// <param name="iconName">Icon texture name.</param>
        /// <param name="accelerator">Accelerator for this item.</param>
        /// <returns>Newly created control.</returns>
        public virtual MenuItem AddItem(String text, String iconName, String accelerator = "")
        {
            MenuItem item = new MenuItem(this);
            item.Margin = Margin.Four;
            item.SetText(text);
            item.SetImage(iconName);
            item.SetAccelerator(accelerator);

            OnAddItem(item);

            return item;
        }

        /// <summary>
        /// Add item handler.
        /// </summary>
        /// <param name="item">Item added.</param>
        protected virtual void OnAddItem(MenuItem item)
        {
            item.TextMargin = new Margin(IconMarginDisabled ? 0 : 24, 0, 16, 0);
            item.Alignment = AlignmentFlags.Top;
            item.SizeToContents();
            item.TextAlignment = AlignmentFlags.CenterVertical | AlignmentFlags.Left;
            item.HoverEnter += OnHoverItem;

            // Do this here - after Top Docking these values mean nothing in layout
            int w = item.Width + 10 + 32;
            if (w < Width) w = Width;
            SetSize(w, Height);
        }

        /// <summary>
        /// Closes all submenus.
        /// </summary>
        public virtual void CloseAll()
        {
            throw new NotImplementedException();

            ////System.Diagnostics.Debug.Print("Menu.CloseAll: {0}", this);
            //Children.ForEach(child => { if (child is MenuItem) (child as MenuItem).CloseMenu(); });
        }

        /// <summary>
        /// Indicates whether any (sub)menu is open.
        /// </summary>
        /// <returns></returns>
        public virtual bool IsMenuOpen()
        {
            throw new NotImplementedException();

            //return Children.Any(child => { if (child is MenuItem) return (child as MenuItem).IsMenuOpen; return false; });
        }

        /// <summary>
        /// Mouse hover handler.
        /// </summary>
        /// <param name="control">Event source.</param>
        protected virtual void OnHoverItem(IControl control)
        {
            if (!ShouldHoverOpenMenu) return;

            MenuItem item = control as MenuItem;
            if (null == item) return;
            if (item.IsMenuOpen) return;

            CloseAll();
            item.OpenMenu();
        }

        /// <summary>
        /// Closes the current menu.
        /// </summary>
        public virtual void Close()
        {
            //System.Diagnostics.Debug.Print("Menu.Close: {0}", this);
            IsHidden = true;
            if (DeleteOnClose)
            {
                DelayedDelete();
            }
        }

        /// <summary>
        /// Closes all submenus and the current menu.
        /// </summary>
        public override void CloseMenus()
        {
            //System.Diagnostics.Debug.Print("Menu.CloseMenus: {0}", this);
            base.CloseMenus();
            CloseAll();
            Close();
        }

        /// <summary>
        /// Adds a divider menu item.
        /// </summary>
        public virtual void AddDivider()
        {
            MenuDivider divider = new MenuDivider(this);
            divider.Alignment = AlignmentFlags.Top;
            divider.Margin = new Margin(IconMarginDisabled ? 0 : 24, 0, 4, 0);
        }
    }
}
