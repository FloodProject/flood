using System;
using System.Collections.Generic;
using System.Linq;
using Flood.GUIv2.Anim;
using Flood.GUIv2.Controls;
using Flood.GUIv2.DragDrop;
using Flood.GUIv2.Input;
using Flood.GUIv2.Panels.Layout;
using Flood.GUIv2.Skins;

namespace Flood.GUIv2
{
    public abstract class CompositeControl : IControl, IDisposable
    {
        private bool m_Disposed;

        protected readonly Panel Panel;

        protected CompositeControl(Panel panel)
        {
            Panel = panel;
            Panel.Parent = this;
        }

        public virtual void Dispose()
        {
            //Debug.Print("Control.Base: Disposing {0} {1:X}", this, GetHashCode());
            if (m_Disposed)
            {
#if DEBUG
                throw new ObjectDisposedException(String.Format("Control.Base [{1:X}] disposed twice: {0}", this, GetHashCode()));
#else
                return;
#endif
            }
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!disposing)
                return;

            Panel.Dispose();

            if (InputHandler.HoveredControl == this)
                InputHandler.HoveredControl = null;
            if (InputHandler.KeyboardFocus == this)
                InputHandler.KeyboardFocus = null;
            if (InputHandler.MouseFocus == this)
                InputHandler.MouseFocus = null;

            DragAndDrop.ControlDeleted(this);
            GUIv2.ToolTip.ControlDeleted(this);
            Animation.Cancel(this);

            m_Disposed = true;
        }

        #region panel wrapping

        //todo: remove this when control serialization is working and test don't need it anymore
        public List<IControl> Children
        {
            get { return Panel.Children; }
        }

        public event Control.GwenEventHandler HoverEnter { add { Panel.HoverEnter += value; } remove { Panel.HoverEnter -= value; } }
        public event Control.GwenEventHandler HoverLeave { add { Panel.HoverLeave += value; } remove { Panel.HoverLeave -= value; } }
        public event Control.GwenEventHandler BoundsChanged { add { Panel.BoundsChanged += value; } remove { Panel.BoundsChanged -= value; } }
        public event Control.ParentChangedEventHandler ParentChanged;
        
        private IControl m_Parent;

        public virtual void OnCanvasChanged(Canvas canvas)
        {
            m_Skin = canvas.Skin;
            Panel.OnCanvasChanged(canvas);
        }

        public IControl Parent
        {
            get { return m_Parent; }
            set
            {
                if (value != null && Parent != null)
                    throw new Exception("Parent should be null.");

                var oldCanvas = GetCanvas();
                m_Parent = value;
                var canvas = GetCanvas();

                if (canvas != null && canvas != oldCanvas)
                    OnCanvasChanged(canvas);

                if (ParentChanged != null)
                    ParentChanged.Invoke(this, value);
            }
        }

        protected Skins.Skin m_Skin;
        public virtual Skin Skin { get { return m_Skin; } }

        public IControl ToolTip { get { return Panel.ToolTip; } set { Panel.ToolTip = value; } }

        public virtual bool IsMenuComponent
        {
            get
            {
                if (Parent == null)
                    return false;
                return Parent.IsMenuComponent;
            }
        }

        public virtual bool ShouldClip { get { return true; } }

        public Margin Margin
        {
            get { return Panel.Margin; }
            set
            {
                if (Margin == value)
                    return;
                Panel.Margin = value;
                InvalidateParent();
            }
        }

        public bool IsOnTop
        {
            get
            {
                return Parent != null && Parent is Panel &&
                    this == ((Panel)Parent).Children.First();
            }
        }
        public object UserData { get { return Panel.UserData; } set { Panel.UserData = value; } }
        public bool IsHovered { get { return InputHandler.HoveredControl == this; } }
        public bool HasFocus { get { return InputHandler.KeyboardFocus == this; } }
        public bool IsDisabled
        {
            get { return Panel.IsDisabled; }
            set { Panel.IsDisabled = value; }
        }
        public bool IsHidden
        {
            get { return Panel.IsHidden; }
            set { Panel.IsHidden = value; }
        }
        public bool RestrictToParent
        {
            get { return Panel.RestrictToParent; } 
            set { Panel.RestrictToParent = value; }
        }
        public bool MouseInputEnabled
        {
            get { return Panel.MouseInputEnabled; }
            set { Panel.MouseInputEnabled = value; }
        }
        public bool KeyboardInputEnabled
        {
            get { return Panel.KeyboardInputEnabled; }
            set { Panel.KeyboardInputEnabled = value; }
        }
        public Cursor Cursor
        {
            get { return Panel.Cursor; }
            set { Panel.Cursor = value; }
        }
        public bool IsTabable
        {
            get { return Panel.IsTabable; }
            set { Panel.IsTabable = value; }
        }
        public bool ShouldDrawBackground
        {
            get { return Panel.ShouldDrawBackground; }
            set { Panel.ShouldDrawBackground = value; }
        }
        public bool ShouldCacheToTexture
        {
            get { return Panel.ShouldCacheToTexture; }
            set { Panel.ShouldCacheToTexture = value; }
        }
        public string Name
        {
            get { return Panel.Name; }
            set { Panel.Name = value; }
        }

        public MarginFlags MarginFlags { get { return Panel.MarginFlags; } }
        public PaddingFlags PaddingFlags { get { return Panel.PaddingFlags; } }
        public bool IsRenderable { get; private set; }
        public int Proportion
        {
            get { return Panel.Proportion; }
            set { Panel.Proportion = value; }
        }
        public ExpansionFlags Expansion
        {
            get { return Panel.Expansion; }
            set { Panel.Expansion = value; }
        }
        public AlignmentFlags Alignment
        {
            get { return Panel.Alignment; }
            set { Panel.Alignment = value; }
        }
        public void DelayedDelete()
        {
            GetCanvas().AddDelayedDelete(this);
        }

        public Color PaddingOutlineColor { get { return Panel.PaddingOutlineColor; } set { Panel.PaddingOutlineColor = value; } }
        public Color MarginOutlineColor { get { return Panel.MarginOutlineColor; } set { Panel.MarginOutlineColor = value; } }
        public Color BoundsOutlineColor { get { return Panel.BoundsOutlineColor; } set { Panel.BoundsOutlineColor = value; } }

        public Rectangle Bounds { get { return Panel.Bounds; } }

        public Rectangle RenderBounds { get { return Panel.RenderBounds; } }

        public Vector2i MinimumSize { get { return Panel.MinimumSize; } set { Panel.MinimumSize = value; } }

        public Vector2i BestSize { get { return Panel.BestSize; } set { Panel.BestSize = value; } }

        public Vector2i MaximumSize { get { return Panel.MaximumSize; } set { Panel.MaximumSize = value; } }
        public bool ShouldDrawHover { get { return Panel.ShouldDrawHover; } }

        public virtual bool AccelOnlyFocus { get { return false; } }
        public virtual bool NeedsInputChars { get { return false; } }

        public bool IsVisible { get; private set; }
        public int X
        {
            get { return Panel.X; }
            set { Panel.X = value; }
        }
        public int Y
        {
            get { return Panel.Y; }
            set { Panel.Y = value; }
        }
        public int Width
        {
            get { return Panel.Width; }
            set { Panel.Width = value; }
        }
        public int Height
        {
            get { return Panel.Height; }
            set { Panel.Height = value; }
        }
        public int Bottom
        {
            get { return Panel.Bottom; }
        }
        public int Right
        {
            get { return Panel.Right; }
        }

        public int RenderHeight { get { return Panel.RenderHeight; } }
        public int RenderWidth { get { return Panel.RenderWidth; } }

        public float AspectRatio
        {
            get { return Panel.AspectRatio; }
        }

        void IControl.UpdateRenderBounds()
        {
            ((IControl)Panel).UpdateRenderBounds();
        }

        public bool SetBounds(Rectangle bounds)
        {
            return SetBounds(bounds.X, bounds.Y, bounds.Width, bounds.Height);
        }

        public bool SetBounds(float x, float y, float width, float height)
        {
            return SetBounds((int)x, (int)y, (int)width, (int)height);
        }

        public bool SetBounds(int x, int y, int width, int height)
        {
            return Panel.SetBounds(x, y, width, height);
        }

        public virtual void OnBoundsChanged(Rectangle oldBounds)
        {
            //Anything that needs to update on size changes
            //Iterate my children and tell them I've changed
            if (Parent != null)
                Parent.OnChildBoundsChanged(oldBounds, this);


            if (Bounds.Width != oldBounds.Width || Bounds.Height != oldBounds.Height)
            {
                Invalidate();
            }

            Redraw();
            ((IControl)this).UpdateRenderBounds();
        }

        public virtual void OnScaleChanged()
        {
            Panel.OnScaleChanged();
        }

        public virtual void OnChildBoundsChanged(Rectangle oldChildBounds, IControl child)
        {
            Panel.OnChildBoundsChanged(oldChildBounds, child);
        }

        public bool SetSize(int width, int height)
        {
            return SetBounds(X, Y, width, height);
        }

        public void MoveBy(int x, int y)
        {
            SetBounds(X + x, Y + y, Width, Height);
        }

        public void MoveTo(float x, float y)
        {
            MoveTo((int)x, (int)y);
        }

        public void MoveTo(int x, int y)
        {
            Panel.MoveTo(x, y);
        }

        public void SetPosition(float x, float y)
        {
            SetPosition((int)x, (int)y);
        }

        public void SetPosition(int x, int y)
        {
            SetBounds(x, y, Width, Height);
        }

        #endregion

        public override string ToString()
        {
            if (this is Controls.MenuItem)
                return "[MenuItem: " + (this as Controls.MenuItem).Text + "]";
            if (this is Label)
                return "[Label: " + (this as Label).Text + "]";
            return GetType().ToString();
        }

        public virtual Canvas GetCanvas()
        {
            IControl canvas = Parent;
            if (canvas == null)
                return null;

            return canvas.GetCanvas();
        }

        public void CloseMenus()
        {
            Panel.CloseMenus();
        }

        public virtual void UpdateColors()
        {
            Panel.UpdateColors();
        }

        bool IControl.HandleAccelerator(string accelerator)
        {
            return HandleAccelerator(accelerator);
        }

        public void AddAccelerator(string accelerator, Control.GwenEventHandler handler)
        {
            Panel.AddAccelerator(accelerator, handler);
        }

        public void AddAccelerator(string accelerator)
        {
            Panel.AddAccelerator(accelerator);
        }

        public void SetToolTipText(string text)
        {
            Panel.SetToolTipText(text);
        }

        public Package DragAndDrop_GetPackage(int x, int y)
        {
            return Panel.DragAndDrop_GetPackage(x, y);
        }

        public bool DragAndDrop_Draggable()
        {
            return Panel.DragAndDrop_Draggable();
        }

        public virtual void DragAndDrop_SetPackage(bool draggable, string name = "", object userData = null)
        {
            Panel.DragAndDrop_SetPackage(draggable, name, userData);
        }

        public virtual bool DragAndDrop_ShouldStartDrag()
        {
            return Panel.DragAndDrop_ShouldStartDrag();
        }

        public virtual void DragAndDrop_StartDragging(Package package, int x, int y)
        {
            Panel.DragAndDrop_StartDragging(package, x, y);
        }

        public virtual void DragAndDrop_EndDragging(bool success, int x, int y)
        {
            Panel.DragAndDrop_EndDragging(success, x, y);
        }

        public bool DragAndDrop_HandleDrop(Package p, int x, int y)
        {
            return Panel.DragAndDrop_HandleDrop(p, x, y);
        }

        public void DragAndDrop_HoverEnter(Package p, int x, int y)
        {
            Panel.DragAndDrop_HoverEnter(p, x, y);
        }

        public void DragAndDrop_HoverLeave(Package p)
        {
            Panel.DragAndDrop_HoverLeave(p);
        }

        public void DragAndDrop_Hover(Package p, int x, int y)
        {
            Panel.DragAndDrop_Hover(p, x, y);
        }

        public bool DragAndDrop_CanAcceptPackage(Package p)
        {
            return Panel.DragAndDrop_CanAcceptPackage(p);
        }

        public virtual void OnPaste(IControl @from)
        {
            Panel.OnPaste(from);
        }

        public virtual void OnCopy(IControl @from)
        {
            Panel.OnCopy(from);
        }

        public virtual void OnCut(IControl @from)
        {
            Panel.OnCut(from);
        }

        public virtual void OnSelectAll(IControl @from)
        {
            Panel.OnSelectAll(from);
        }

        public void InputCopy(IControl @from)
        {
            OnCopy(from);
        }

        public void InputPaste(IControl @from)
        {
            OnPaste(from);
        }

        public void InputCut(IControl @from)
        {
            OnCut(from);
        }

        public void InputSelectAll(IControl @from)
        {
            OnSelectAll(from);
        }

        public virtual bool OnChar(char chr)
        {
            return false;
        }

        bool IControl.InputChar(Char chr)
        {
            return OnChar(chr);
        }

        public virtual bool OnKeyPressed(Keys key, bool down = true)
        {
            return Panel.OnKeyPressed(key, down);
        }

        bool IControl.InputKeyPressed(Keys key, bool down)
        {
            return OnKeyPressed(key, down);
        }

        public virtual bool OnKeyTab(bool down)
        {
            return Panel.OnKeyTab(down);
        }

        public virtual bool OnKeySpace(bool down)
        {
            return Panel.OnKeySpace(down);
        }

        public virtual bool OnKeyReturn(bool down)
        {
            return Panel.OnKeyReturn(down);
        }

        public virtual bool OnKeyBackspace(bool down)
        {
            return Panel.OnKeyBackspace(down);
        }

        public virtual bool OnKeyDelete(bool down)
        {
            return Panel.OnKeyDelete(down);
        }

        public virtual bool OnKeyRight(bool down)
        {
            return Panel.OnKeyRight(down);
        }

        public virtual bool OnKeyLeft(bool down)
        {
            return Panel.OnKeyLeft(down);
        }

        public virtual bool OnKeyHome(bool down)
        {
            return Panel.OnKeyHome(down);
        }

        public virtual bool OnKeyEnd(bool down)
        {
            return Panel.OnKeyEnd(down);
        }

        public virtual bool OnKeyUp(bool down)
        {
            return Panel.OnKeyUp(down);
        }

        public virtual bool OnKeyDown(bool down)
        {
            return Panel.OnKeyDown(down);
        }

        public virtual bool OnKeyEscape(bool down)
        {
            return Panel.OnKeyEscape(down);
        }

        public virtual bool OnMouseWheeled(int delta)
        {
            return Panel.OnMouseWheeled(delta);
        }

        bool IControl.InputMouseWheeled(int delta)
        {
            return OnMouseWheeled(delta);
        }

        public virtual void OnMouseMoved(int x, int y, int dx, int dy)
        {
            Panel.OnMouseMoved(x, y, dx, dy);
        }

        void IControl.InputMouseMoved(int x, int y, int dx, int dy)
        {
            OnMouseMoved(x, y, dx, dy);
        }

        public virtual void OnMouseClickedLeft(int x, int y, bool down)
        {
            Panel.OnMouseClickedLeft(x, y, down);
        }

        void IControl.InputMouseClickedLeft(int x, int y, bool down)
        {
            OnMouseClickedLeft(x, y, down);
        }

        public virtual void OnMouseClickedRight(int x, int y, bool down)
        {
            Panel.OnMouseClickedRight(x, y, down);
        }

        void IControl.InputMouseClickedRight(int x, int y, bool down)
        {
            OnMouseClickedRight(x, y, down);
        }

        public virtual void OnMouseDoubleClickedLeft(int x, int y)
        {
            Panel.OnMouseDoubleClickedLeft(x, y);
        }

        void IControl.InputMouseDoubleClickedLeft(int x, int y)
        {
            OnMouseDoubleClickedLeft(x, y);
        }

        void IControl.InputMouseDoubleClickedRight(int x, int y)
        {
            OnMouseDoubleClickedRight(x, y);
        }

        public virtual void OnMouseDoubleClickedRight(int x, int y)
        {
            Panel.OnMouseDoubleClickedRight(x, y);
        }

        public void OnMouseEntered()
        {
            Panel.OnMouseEntered();
        }

        void IControl.InputMouseEntered()
        {
            OnMouseEntered();
        }

        public virtual void OnMouseLeft()
        {
            Panel.OnMouseLeft();
        }

        void IControl.InputMouseLeft()
        {
            OnMouseLeft();
        }

        public virtual void Touch()
        {
            if (Parent != null)
                Parent.OnChildTouched(this);
        }

        public virtual void OnChildTouched(IControl control)
        {
            Touch();
        }

        public virtual void RenderFocus(Skin skin)
        {
            Panel.RenderFocus(skin);
        }

        public virtual void RenderUnder(Skin skin)
        {
            Panel.RenderUnder(skin);
        }

        public virtual void RenderOver(Skin skin)
        {
            Panel.RenderOver(skin);
        }

        public void Anim_WidthIn(float length, float delay = 0, float ease = 1)
        {
            Panel.Anim_WidthIn(length, delay, ease);
        }

        public void Anim_HeightIn(float length, float delay, float ease)
        {
            Panel.Anim_HeightIn(length, delay, ease);
        }

        public void Anim_WidthOut(float length, bool hide, float delay, float ease)
        {
            Panel.Anim_WidthOut(length, hide, delay, ease);
        }

        public void Anim_HeightOut(float length, bool hide, float delay, float ease)
        {
            Panel.Anim_HeightOut(length, hide, delay, ease);
        }

        public bool HandleAccelerator(String accelerator)
        {
            return Panel.HandleAccelerator(accelerator);
        }

        #region layout

        public virtual void ReduceToMinSize()
        {
            Panel.ReduceToMinSize();
        }

        public virtual bool InformFirstDirection(BoxOrientation direction, int size, int availableOtherDir)
        {
            return Panel.InformFirstDirection(direction, size, availableOtherDir);
        }

        public Vector2i GetMaxSizeWithBorder()
        {
            return Panel.GetMaxSizeWithBorder();
        }

        public void UpdateCursor()
        {
            Panel.UpdateCursor();
        }

        public void Enable()
        {
            Panel.Enable();
        }

        public void Disable()
        {
            Panel.Disable();
        }

        public void Hide()
        {
            Panel.Hide();
        }

        public void Show()
        {
            Panel.Show();
        }

        public void Focus()
        {
            if (InputHandler.KeyboardFocus == this)
                return;

            if (InputHandler.KeyboardFocus != null)
                InputHandler.KeyboardFocus.OnLostKeyboardFocus();

            InputHandler.KeyboardFocus = this;
            OnKeyboardFocus();
            Redraw();
        }

        public void Blur()
        {
            if (InputHandler.KeyboardFocus != this)
                return;

            InputHandler.KeyboardFocus = null;
            OnLostKeyboardFocus();
            Redraw();
        }

        public virtual void OnKeyboardFocus()
        {
            Panel.OnKeyboardFocus();
        }

        public virtual void OnLostKeyboardFocus()
        {
            Panel.OnLostKeyboardFocus();
        }

        public void SendToBack()
        {
            Panel.SendToBack();
        }

        public void BringToFront()
        {
            Panel.BringToFront();
        }

        public void BringNextToControl(IControl child, bool behind)
        {
            Panel.BringNextToControl(child, behind);
        }

        public virtual void Redraw()
        {
            Panel.Redraw();
        }

        public virtual void Layout(Skin skin)
        {
            Panel.Layout(skin);
        }

        public void RecurseLayout(Skin skin, int level = 0)
        {
            Panel.RecurseLayout(skin, level);
        }

        public void PostLayout(Skin skin)
        {
            Panel.PostLayout(skin);
        }

        #endregion
        
        #region child management

        protected void AddChild(IControl child)
        {
            Panel.AddChild(child);
        }

        public IControl FindChildByName(String name, bool recursive = false)
        {
            return Panel.FindChildByName(name, recursive);
        }

        public void InvalidateChildren(bool recursive = false)
        {
            Panel.InvalidateChildren(recursive);
        }

        public virtual void Invalidate()
        {
            Panel.Invalidate();
        }

        public void InvalidateParent()
        {
            if (Parent != null)
                Parent.Invalidate();
        }

        public void SetDimension(Vector2i pos, Vector2i size)
        {
            Panel.SetDimension(pos, size);
        }

        public Vector2i GetMinSizeWithBorder()
        {
            return Panel.GetMinSizeWithBorder();
        }

        public Vector2i GetSizeWithBorder()
        {
            return Panel.GetSizeWithBorder();
        }

        public virtual Vector2i GetChildrenSize()
        {
            return Panel.GetChildrenSize();
        }

        public IControl GetControlAt(int x, int y)
        {
            var ctrl = Panel.GetControlAt(x, y);
            if (ReferenceEquals(ctrl, Panel))
                return this;
            return ctrl;
        }

        public Vector2i LocalPosToCanvas(Vector2i pnt)
        {
            return Panel.LocalPosToCanvas(pnt);
        }

        public Vector2i CanvasPosToLocal(Vector2i pnt)
        {
            return Panel.CanvasPosToLocal(pnt);
        }

        #endregion

        #region render

        public virtual void Render(Skins.Skin skin)
        {
        }
        public void DoCacheRender(Skin skin, IControl master)
        {
            Panel.DoCacheRender(skin, master);
        }

        public void DoRender(Skin skin)
        {
            Panel.DoRender(skin);
        }

        public bool DrawDebugOutlines
        {
            get
            {
                return Panel.DrawDebugOutlines;
            }
            set
            {
                Panel.DrawDebugOutlines = value;
            }
        }
        
        #endregion
    }
}
