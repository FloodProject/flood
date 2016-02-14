using Flood.GUI.ControlInternal;
using System;

namespace Flood.GUI.Controls
{
    /// <summary>
    /// ComboBox control.
    /// </summary>
    public class ComboBox : Button
    {
        private readonly Menu m_Menu;
        private readonly Control m_Button;
        private MenuItem m_SelectedItem;

        /// <summary>
        /// Invoked when the selected item has changed.
        /// </summary>
        public event GwenEventHandler ItemSelected;

        /// <summary>
        /// Indicates whether the combo menu is open.
        /// </summary>
        public bool IsOpen { get { return m_Menu != null && !m_Menu.IsHidden; } }

        /// <summary>
        /// Initializes a new instance of the <see cref="ComboBox"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public ComboBox(Control parent)
            : base(parent)
        {
            SetSize(100, 20);
            m_Menu = new Menu(this);
            m_Menu.IsHidden = true;
            m_Menu.IconMarginDisabled = true;
            m_Menu.IsTabable = false;

            DownArrow arrow = new DownArrow(this);
            m_Button = arrow;

            Alignment = Pos.Left | Pos.CenterV;
            Margin = new Margin(3, 0, 0, 0);

            IsTabable = true;
            KeyboardInputEnabled = true;
        }

        /// <summary>
        /// Selected item.
        /// </summary>
        /// <remarks>Not just String property, because items also have internal names.</remarks>
        public Label SelectedItem { get { return m_SelectedItem; } }

        internal override bool IsMenuComponent
        {
            get { return true; }
        }

        /// <summary>
        /// Adds a new item.
        /// </summary>
        /// <param name="label">Item label (displayed).</param>
        /// <param name="name">Item name.</param>
        /// <returns>Newly created control.</returns>
        public virtual MenuItem AddItem(String label, String name = "")
        {
            MenuItem item = m_Menu.AddItem(label, String.Empty);
            item.Name = name;
            item.Selected += OnItemSelected;

            if (m_SelectedItem == null)
                OnItemSelected(item);

            return item;
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Render(Skins.Skin skin)
        {
            skin.DrawComboBox(this, IsDepressed, IsOpen);
        }

        /// <summary>
        /// Internal Pressed implementation.
        /// </summary>
        protected override void OnClicked()
        {
            if (IsOpen)
            {
                GetCanvas().CloseMenus();
                return;
            }

            bool wasMenuHidden = m_Menu.IsHidden;

            GetCanvas().CloseMenus();

            if (wasMenuHidden)
            {
                Open();
            }
        }

        /// <summary>
        /// Removes all items.
        /// </summary>
        public virtual void DeleteAll()
        {
            if (m_Menu != null)
                m_Menu.DeleteAll();
        }

        /// <summary>
        /// Internal handler for item selected event.
        /// </summary>
        /// <param name="control">Event source.</param>
        protected virtual void OnItemSelected(Control control)
        {
            //Convert selected to a menu item
            MenuItem item = control as MenuItem;
            if (null == item) return;

            m_SelectedItem = item;
            Text = m_SelectedItem.Text;
            m_Menu.IsHidden = true;

            if (ItemSelected != null)
                ItemSelected.Invoke(this);

            Focus();
            Invalidate();
        }

        /// <summary>
        /// Lays out the control's interior according to alignment, padding, dock etc.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Layout(Skins.Skin skin)
        {
            m_Button.Position(Pos.Right|Pos.CenterV, 4, 0);
            base.Layout(skin);
        }

        /// <summary>
        /// Handler for losing keyboard focus.
        /// </summary>
        protected override void OnLostKeyboardFocus()
        {
            TextColor = Color.Black;
        }

        /// <summary>
        /// Handler for gaining keyboard focus.
        /// </summary>
        protected override void OnKeyboardFocus()
        {
            //Until we add the blue highlighting again
            TextColor = Color.Black;
        }

        /// <summary>
        /// Opens the combo.
        /// </summary>
        public virtual void Open()
        {
            if (null == m_Menu) return;

            m_Menu.Parent = GetCanvas();
            m_Menu.IsHidden = false;
            m_Menu.BringToFront();

            var p = LocalPosToCanvas(new Vector2i(0,0));

            m_Menu.SetBounds(new Rectangle(p.X, p.Y + Height, Width, m_Menu.Height));
        }

        /// <summary>
        /// Closes the combo.
        /// </summary>
        public virtual void Close()
        {
            if (m_Menu == null) 
                return;

            m_Menu.Hide();
        }

        /// <summary>
        /// Handler for Down Arrow keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>
        /// True if handled.
        /// </returns>
        protected override bool OnKeyDown(bool down)
        {
            if (down)
            {
                var it = m_Menu.Children.FindIndex(x => x == m_SelectedItem);
                if (it + 1 < m_Menu.Children.Count)
                    OnItemSelected(m_Menu.Children[it + 1]);
            }
            return true;
        }

        /// <summary>
        /// Handler for Up Arrow keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>
        /// True if handled.
        /// </returns>
        protected override bool OnKeyUp(bool down)
        {
            if (down)
            {
                var it = m_Menu.Children.FindLastIndex(x => x == m_SelectedItem);
                if (it - 1 >= 0)
                    OnItemSelected(m_Menu.Children[it - 1]);
            }
            return true;
        }

        /// <summary>
        /// Renders the focus overlay.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void RenderFocus(Skins.Skin skin)
        {
            
        }
    }
}
