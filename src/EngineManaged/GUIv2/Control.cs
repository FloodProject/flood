using Flood.GUIv2.Anim;
using Flood.GUIv2.Controls;
using Flood.GUIv2.DragDrop;
using Flood.GUIv2.Input;
using Flood.GUIv2.Panels.Layout;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Flood.GUIv2
{
    /// <summary>
    /// Base control class.
    /// A control is generally a small window which processes user input
    /// and/or displays one or more item of data.
    /// </summary>
    public abstract class Control : IDisposable, IControl
    {
        public const int SizeNotSet = -1;
        // this REALLY needs to be replaced with control-specific events
        /// <summary>
        /// Delegate used for all control event handlers.
        /// </summary>
        /// <param name="control">Event source.</param>
        public delegate void GwenEventHandler(IControl control);

        #region Properties

        private bool m_Disposed;
        private IControl m_Parent;
        private IControl m_ToolTip;
        private Rectangle m_Bounds;
        private Rectangle m_RenderBounds;
        private Margin m_Margin;
        private String m_Name;
        private bool m_RestrictToParent;
        private bool m_Disabled;
        private bool m_Hidden;
        private bool m_MouseInputEnabled;
        private bool m_KeyboardInputEnabled;
        private bool m_DrawBackground;
        private Cursor m_Cursor;
        private bool m_Tabable;
        private bool m_CacheTextureDirty;
        private bool m_CacheToTexture;
        private Package m_DragAndDrop_Package;
        private object m_UserData;
        private bool m_DrawDebugOutlines;

        public event GwenEventHandler HoverEnter;
        public event GwenEventHandler HoverLeave;
        public event GwenEventHandler BoundsChanged;

        public delegate void ParentChangedEventHandler(IControl control, IControl parent);
        public event ParentChangedEventHandler ParentChanged;


        public virtual void OnCanvasChanged(Canvas canvas)
        {
            m_Skin = canvas.Skin;
        }

        /// <summary>
        /// Accelerator map.
        /// </summary>
        private readonly Dictionary<String, GwenEventHandler> m_Accelerators;

        public const int MaxCoord = 4096; // added here from various places in code

        /// <summary>
        /// Gets or sets a value indicating whether this control needs layout
        /// </summary>
        protected bool NeedsLayout { get; set; }

        protected bool CacheTextureDirty
        {
            get { return m_CacheTextureDirty; }
            set { m_CacheTextureDirty = value; }
        }

        public bool Disposed
        {
            get { return m_Disposed; }
        }

        public IControl Parent
        {
            get { return m_Parent; }
            set
            {
                if(value != null && Parent != null)
                    throw new Exception("Parent should be null.");
                
                var oldCanvas = GetCanvas();
                m_Parent = value;
                var canvas = GetCanvas();

                if(canvas != null && canvas != oldCanvas)
                    OnCanvasChanged(canvas);

                if (ParentChanged != null)
                    ParentChanged.Invoke(this, value);
            }
        }

        protected Skins.Skin m_Skin;
        public virtual Skins.Skin Skin
        {
            get { return m_Skin; }
        }


        //todo: since this is no longer a child of this it currently doesn't render or anything else
        public IControl ToolTip
        {
            get { return m_ToolTip; }
            set
            {
                throw new NotImplementedException();

                m_ToolTip = value;
                if (m_ToolTip != null)
                {
                    m_ToolTip.Parent = this;
                    m_ToolTip.IsHidden = true;
                }
            }
        }

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
            get { return m_Margin; }
            set
            {
                if (m_Margin == value)
                    return;
                UpdateMarginFlags(value);
                m_Margin = value;
                Invalidate();
                InvalidateParent();
            }
        }

        /// <summary>
        /// Helper method to update margin flags when margin changes.
        /// </summary>
        private void UpdateMarginFlags(Margin margin)
        {
            MarginFlags = 0;
            MarginFlags |= margin.Top > 0 ? MarginFlags.Top : 0;
            MarginFlags |= margin.Bottom > 0 ? MarginFlags.Bottom : 0;
            MarginFlags |= margin.Right > 0 ? MarginFlags.Right : 0;
            MarginFlags |= margin.Left > 0 ? MarginFlags.Left : 0;
        }

        public virtual bool IsOnTop { get { return Parent != null && Parent is Panel && 
            this == ((Panel)Parent).Children.First(); } } 

        public object UserData { get { return m_UserData; } set { m_UserData = value; } }

        public bool IsHovered { get { return InputHandler.HoveredControl == this; } }

        public bool HasFocus { get { return InputHandler.KeyboardFocus == this; } }

        public bool IsDisabled { get { return m_Disabled; } set { m_Disabled = value; } }

        public virtual bool IsHidden { get { return m_Hidden; } set { if (value == m_Hidden) return; m_Hidden = value; Invalidate(); } }

        public bool RestrictToParent { get { return m_RestrictToParent; } set { m_RestrictToParent = value; } }

        public bool MouseInputEnabled { get { return m_MouseInputEnabled; } set { m_MouseInputEnabled = value; } }

        public bool KeyboardInputEnabled { get { return m_KeyboardInputEnabled; } set { m_KeyboardInputEnabled = value; } }

        public Cursor Cursor { get { return m_Cursor; } set { m_Cursor = value; } }

        public bool IsTabable { get { return m_Tabable; } set { m_Tabable = value; } }

        public bool ShouldDrawBackground { get { return m_DrawBackground; } set { m_DrawBackground = value; } }

        public bool ShouldCacheToTexture { get { return m_CacheToTexture; } set { m_CacheToTexture = value; /*Children.ForEach(x => x.ShouldCacheToTexture=value);*/ } }

        public String Name { get { return m_Name; } set { m_Name = value; } }

        public Rectangle Bounds { get { return m_Bounds; } private set { m_Bounds = value; } }

        public Rectangle RenderBounds { get { return m_RenderBounds; } }

        public Vector2i MinimumSize { get { return m_MinimumSize; } set { m_MinimumSize = value; } }

        public Vector2i BestSize { get { return m_BestSize; } set { m_BestSize = value; } }

        public Vector2i MaximumSize { get { return m_MaximumSize; } set { m_MaximumSize = value; } }

        private Vector2i m_MinimumSize = new Vector2i(SizeNotSet, SizeNotSet);
        private Vector2i m_MaximumSize = new Vector2i(SizeNotSet, SizeNotSet);
        private Vector2i m_BestSize = new Vector2i(SizeNotSet, SizeNotSet);

        public bool ShouldDrawHover { get { return InputHandler.MouseFocus == this || InputHandler.MouseFocus == null; } }

        public virtual bool AccelOnlyFocus { get { return false; } }
        public virtual bool NeedsInputChars { get { return false; } }

        public bool IsVisible
        {
            get
            {
                if (IsHidden)
                    return false;

                if (Parent != null)
                    return Parent.IsVisible;

                return true;
            }
        }

        public int X { get { return m_Bounds.X; } set { SetPosition(value, Y); } }

        public int Y { get { return m_Bounds.Y; } set { SetPosition(X, value); } }

        // todo: Bottom/Right includes margin but X/Y not?

        public int Width { get { return Bounds.Width; } set { SetSize(value, Height); } }
        public int Height { get { return Bounds.Height; } set { SetSize(Width, value); } }
        public int Bottom { get { return Bounds.GetBottom() + Margin.Bottom; } }
        public int Right { get { return Bounds.GetRight() + Margin.Right; } }

        public int RenderHeight
        {
            get { return Height + Margin.Bottom + Margin.Top; }
        }

        public int RenderWidth
        {
            get { return Width + Margin.Left + Margin.Right; }
        }

        // todo: check if there's no loss of precision here because of resizes
        public float AspectRatio { get { return ((float)Width)/Height; } }

        public virtual bool DrawDebugOutlines
        {

            get { return m_DrawDebugOutlines; }
            set
            {
                m_DrawDebugOutlines = value;
            }
        }

        public Color PaddingOutlineColor { get; set; }
        public Color MarginOutlineColor { get; set; }
        public Color BoundsOutlineColor { get; set; }

        #endregion

        #region Constructors

        /// <summary>
        /// Initializes a new instance of the <see cref="Control"/> class.
        /// </summary>
        protected Control()
        {
            Expansion = ExpansionFlags.NotSet;
            Alignment = AlignmentFlags.NotSet;
            m_Accelerators = new Dictionary<string, GwenEventHandler>();

            m_Hidden = false;
            m_Bounds = new Rectangle(0, 0, 10, 10);
            m_Margin = Margin.Zero;

            RestrictToParent = false;

            MouseInputEnabled = true;
            KeyboardInputEnabled = false;

            Invalidate();
            Cursor = Cursors.Default;
            //ToolTip = null;
            IsTabable = false;
            ShouldDrawBackground = true;
            m_Disabled = false;
            CacheTextureDirty = true;
            m_CacheToTexture = false;

            BoundsOutlineColor = Color.Red;
            MarginOutlineColor = Color.Green;
            PaddingOutlineColor = Color.Blue;
        }

        /// <summary>
        /// Performs application-defined tasks associated with freeing, releasing, or resetting unmanaged resources.
        /// </summary>
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
        /// <summary>
        /// Performs application-defined tasks associated with freeing, releasing, or resetting unmanaged resources.
        /// </summary>
        protected virtual void Dispose(bool disposing)
        {
            if (!disposing)
                return;

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

#if DEBUG
        ~Control()
        {
            //throw new InvalidOperationException(String.Format("IDisposable object finalized [{1:X}]: {0}", this, GetHashCode()));
            //Debug.Print(String.Format("IDisposable object finalized: {0}", GetType()));
        }
#endif

        public void DelayedDelete()
        {
            GetCanvas().AddDelayedDelete(this);
        }

        #endregion

        public override string ToString()
        {
            if (this is ControlInternal.Text)
                return "[Text: " + (this as ControlInternal.Text).String + "]";
            return GetType().ToString();
        }

        public virtual Canvas GetCanvas()
        {
            IControl canvas = Parent;
            if (canvas == null)
                return null;

            return canvas.GetCanvas();
        }

        public virtual void CloseMenus()
        {
        }

        #region Bounds

        void IControl.UpdateRenderBounds()
        {
            m_RenderBounds.X = 0;
            m_RenderBounds.Y = 0;

            m_RenderBounds.Width = m_Bounds.Width;
            m_RenderBounds.Height = m_Bounds.Height;
        }

        public bool SetBounds(Rectangle bounds)
        {
            return SetBounds(bounds.X, bounds.Y, bounds.Width, bounds.Height);
        }

        public bool SetBounds(float x, float y, float width, float height)
        {
            return SetBounds((int)x, (int)y, (int)width, (int)height);
        }

        public virtual bool SetBounds(int x, int y, int width, int height)
        {
            if (Bounds.X == x &&
                Bounds.Y == y &&
                Bounds.Width == width &&
                Bounds.Height == height)
                return false;

            Rectangle oldBounds = Bounds;

            m_Bounds.X = x;
            m_Bounds.Y = y;

            m_Bounds.Width = ClampWidth(width);
            m_Bounds.Height = ClampHeight(height);

            OnBoundsChanged(oldBounds);

            if (BoundsChanged != null)
                BoundsChanged.Invoke(this);

            return true;
        }

        private int ClampWidth(int width)
        {
            if (MinimumSize.X != SizeNotSet && width < MinimumSize.X)
                return MinimumSize.X;
            if (MaximumSize.X != SizeNotSet && width > MaximumSize.X)
                return MaximumSize.X;
            return width;
        }

        private int ClampHeight(int height)
        {
            if (MinimumSize.Y != SizeNotSet && height < MinimumSize.Y)
                return MinimumSize.Y;
            if (MaximumSize.Y != SizeNotSet && height > MaximumSize.Y)
                return MaximumSize.Y;
            return height;
        }


        public virtual void OnBoundsChanged(Rectangle oldBounds)
        {
            //Anything that needs to update on size changes
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
        }

        public virtual void OnChildBoundsChanged(Rectangle oldChildBounds, IControl child)
        {

        }

        #endregion

        #region Sizing

        public bool SetSize(int width, int height)
        {
            return SetBounds(X, Y, width, height);
        }


        #endregion

        #region Positioning

        public void MoveBy(int x, int y)
        {
            //todo: remove this method and replace with relative position panel where neccessary
            throw new NotImplementedException();
            SetBounds(X + x, Y + y, Width, Height);
        }

        public void MoveTo(float x, float y)
        {
            //todo: remove this method and replace with relative position panel where neccessary
            throw new NotImplementedException();
            MoveTo((int)x, (int)y);
        }

        public virtual void MoveTo(int x, int y)
        {
            //todo: remove this method and replace with relative position panel where neccessary
            throw new NotImplementedException();
            //if (RestrictToParent && (Parent != null))
            //{
            //    IControl parent = Parent;
            //    if (x < 0)
            //        x = 0;
            //    if (y < 0)
            //        y = 0;
            //    if (x + Width > parent.Width - parent.Margin.Right)
            //        x = parent.Width - parent.Margin.Right - Width - Padding.Right;
            //    if (y + Height + Padding.Bottom > parent.Height - parent.Margin.Bottom)
            //        y = parent.Height - parent.Margin.Bottom - Height - Padding.Bottom;
            //}

            //SetBounds(x, y, Width, Height);
        }

        public void SetPosition(float x, float y)
        {
            //todo: remove this method and replace with relative position panel where neccessary
            throw new NotImplementedException();
            SetPosition((int)x, (int)y);
        }

        public void SetPosition(int x, int y)
        {
            //todo: remove this method and replace with relative position panel where neccessary
            throw new NotImplementedException();
            SetBounds(x, y, Width, Height);
        }

        //todo: remove this when its clear the replacements are working
        //public void Position(Pos pos, int xpadding = 0, int ypadding = 0)
        //{
        //    throw new NotImplementedException();
        //    int w = Parent.Width;
        //    int h = Parent.Height;
        //    Padding padding = Parent.Padding;

        //    int x = X;
        //    int y = Y;
        //    if (0 != (pos & Pos.Left)) x = padding.Left + xpadding;
        //    if (0 != (pos & Pos.Right)) x = w - Width - padding.Right - xpadding;
        //    if (0 != (pos & Pos.CenterH))
        //        x = (int)(padding.Left + xpadding + (w - Width - padding.Left - padding.Right) * 0.5f);

        //    if (0 != (pos & Pos.Top)) y = padding.Top + ypadding;
        //    if (0 != (pos & Pos.Bottom)) y = h - Height - padding.Bottom - ypadding;
        //    if (0 != (pos & Pos.CenterV))
        //        y = (int)(padding.Top + ypadding + (h - Height - padding.Bottom - padding.Top) * 0.5f);

        //    SetPosition(x, y);
        //}

        public virtual IControl GetControlAt(int x, int y)
        {
            if (IsHidden)
                return null;

            if (x < 0 || y < 0 || x >= Width || y >= Height)
                return null;

            if (!MouseInputEnabled)
                return null;

            return this;
        }

        public Vector2i LocalPosToCanvas(Vector2i pnt)
        {
            if (Parent != null)
            {
                int x = pnt.X + X;
                int y = pnt.Y + Y;

                return Parent.LocalPosToCanvas(new Vector2i(x, y));
            }

            return pnt;
        }

        public Vector2i CanvasPosToLocal(Vector2i pnt)
        {
            if (m_Parent != null)
            {
                int x = pnt.X - X;
                int y = pnt.Y - Y;

                return Parent.CanvasPosToLocal(new Vector2i(x, y));
            }

            return pnt;
        }

        #endregion

        #region Layout

        public virtual void Layout(Skins.Skin skin)
        {
            if (skin.Renderer.CTT != null && ShouldCacheToTexture)
                skin.Renderer.CTT.CreateControlCacheTexture(this);
        }

        public virtual void RecurseLayout(Skins.Skin skin, int level = 0 )
        {
            Console.WriteLine("{0}+ {1} W: {2} H: {3}", new string(' ', level), GetType().Name, Bounds.Width, Bounds.Height);
            if (IsHidden)
                return;

            if (NeedsLayout)
            {
                NeedsLayout = false;
                Layout(skin);
            }

            PostLayout(skin);

            if (IsTabable)
            {
                if (GetCanvas().FirstTab == null)
                    GetCanvas().FirstTab = this;
                if (GetCanvas().NextTab == null)
                    GetCanvas().NextTab = this;
            }

            if (InputHandler.KeyboardFocus == this)
            {
                GetCanvas().NextTab = null;
            }

            Console.WriteLine("{0}- {1} W: {2} H: {3}", new string(' ', level), GetType().Name, Bounds.Width, Bounds.Height);

        }

        public virtual void PostLayout(Skins.Skin skin)
        {

        }

        public void InvalidateParent()
        {
            if (Parent != null)
                Parent.Invalidate();
        }
        public virtual void InvalidateChildren(bool recursive = false)
        {
        }

        public virtual void Invalidate()
        {
            if(NeedsLayout)
                return;
            //todo: check if this is not causing layout problems or infinite loops
            if (Parent != null)
                Parent.Invalidate();
            NeedsLayout = true;
            CacheTextureDirty = true;
        }

        public void SetDimension(Vector2i pos, Vector2i size)
        {
            if (Expansion == ExpansionFlags.Shaped)
            {
                var rWidth = (int)(size.Y * AspectRatio);
                if (rWidth > size.X)
                {
                    var rHeight = (int)(size.Y / AspectRatio);
                    if (Alignment == AlignmentFlags.CenterVertical)
                        pos.Y += (size.Y - rHeight) / 2;
                    else if (Alignment == AlignmentFlags.Bottom)
                        pos.Y += (size.Y - rHeight);
                    size.Y = rHeight;
                }
                else if (rWidth < size.X)
                {
                    if (Alignment == AlignmentFlags.CenterHorizontal)
                        pos.X += (size.X - rWidth) / 2;
                    else if (Alignment == AlignmentFlags.Bottom)
                        pos.X += (size.X - rWidth);
                    size.X = rWidth;
                }
            }
            SetBounds(pos.X, pos.Y, size.X, size.Y);
        }

        public virtual Vector2i GetMinSizeWithBorder()
        {
            SetBestSize();
            var minW = (MinimumSize.X == SizeNotSet) ? BestSize.X : MinimumSize.X;
            var minH = (MinimumSize.Y == SizeNotSet) ? BestSize.Y : MinimumSize.Y;
            var width = minW + Margin.Left + Margin.Right;
            var height = minH + Margin.Top + Margin.Bottom;
            return new Vector2i(width, height);
        }

        public virtual Vector2i GetSizeWithBorder()
        {
            var width = Width + Margin.Left + Margin.Right;
            var height = Height + Margin.Top + Margin.Bottom;
            return new Vector2i(width, height);
        }
        
        public virtual void ReduceToMinSize()
        {
            SetBestSize();
            var minW = (MinimumSize.X == SizeNotSet) ? BestSize.X : MinimumSize.X;
            var minH = (MinimumSize.Y == SizeNotSet) ? BestSize.Y : MinimumSize.Y;
            SetSize(minW, minH);
        }

        public abstract bool InformFirstDirection(BoxOrientation direction, int size, int availableOtherDir);


        protected virtual void SetBestSize()
        {
            //todo: figure out if there's any way to get a better best size
            BestSize = new Vector2i(Width, Height);
        }

        public virtual Vector2i GetMaxSizeWithBorder()
        {
            var width = MaximumSize.X + Margin.Left + Margin.Right;
            var height = MaximumSize.Y + Margin.Top + Margin.Bottom;
            return new Vector2i(width, height);
        }

        public virtual MarginFlags MarginFlags { get; private set; }
        public virtual PaddingFlags PaddingFlags { get; private set; }

        public virtual bool IsRenderable { get { return true; } }


        //TODO : set all these properties
        public int Proportion { get; set; }
        private ExpansionFlags m_Expansion;
        public ExpansionFlags Expansion
        {
            get
            {
                return m_Expansion;
            } 
            set
            {
                if (m_Expansion == value)
                    return;
                m_Expansion = value;
                Invalidate();
                InvalidateParent();
            }
        }
        private AlignmentFlags m_Align;
        public AlignmentFlags Alignment
        {
            get { return m_Align; } 
            set
            {
                if(m_Align == value)
                    return;
                m_Align = value; 
                Invalidate();
                InvalidateParent();
            }
        }

        #endregion

        #region Cursors

        public void UpdateCursor()
        {
            Platform.Neutral.SetCursor(m_Cursor);
        }

        #endregion

        #region Status

        public void Enable()
        {
            IsDisabled = false;
        }

        public void Disable()
        {
            IsDisabled = true;
        }

        public  void Hide()
        {
            IsHidden = true;
        }

        public void Show()
        {
            IsHidden = false;
        }

        #endregion

        #region Focus

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

        }
        public virtual void OnLostKeyboardFocus()
        {

        }

        #endregion

        #region Z-order

        public void SendToBack()
        {
            var parent = Parent as Panel; 
            if (parent == null)
                return;
            if (parent.Children.Count == 0)
                return;
            if (parent.Children.First() == this)
                return;

            parent.Children.Remove(this);
            parent.Children.Insert(0, this);

            InvalidateParent();
        }

        public void BringToFront()
        {
            var parent = Parent as Panel; 
            if (parent == null)
                return;
            if (parent.Children.Last() == this)
                return;

            parent.Children.Remove(this);
            parent.Children.Add(this);
            InvalidateParent();
            Redraw();
        }

        #endregion

        #region Tab order

        public void BringNextToControl(IControl child, bool behind)
        {
            var parent = Parent as Panel; 
            if (parent == null)
                return;

            parent.Children.Remove(this);

            // todo: validate
            int idx = parent.Children.IndexOf(child);
            if (idx == parent.Children.Count - 1)
            {
                BringToFront();
                return;
            }

            if (behind)
            {
                ++idx;

                if (idx == parent.Children.Count)
                {
                    BringToFront();
                    return;
                }
            }

            parent.Children.Insert(idx, this);
            InvalidateParent();
        }

        #endregion

        #region Rendering

        public virtual void Render(Skins.Skin skin)
        {
        }

        public virtual void DoCacheRender(Skins.Skin skin, IControl master)
        {
            Renderers.Renderer render = skin.Renderer;
            Renderers.ICacheToTexture cache = render.CTT;

            if (cache == null)
                return;

            Vector2i oldRenderOffset = render.RenderOffset;
            Rectangle oldRegion = render.ClipRegion;

            if (this != master)
            {
                render.AddRenderOffset(Bounds);
                render.AddClipRegion(Bounds);
            }
            else
            {
                render.RenderOffset = new Vector2i(0, 0);
                render.ClipRegion = new Rectangle(0, 0, Width, Height);
            }

            if (CacheTextureDirty && render.ClipRegionVisible)
            {
                render.StartClip();

                if (ShouldCacheToTexture)
                    cache.SetupCacheTexture(this);

                //Render myself first
                //var old = render.ClipRegion;
                //render.ClipRegion = Bounds;
                //var old = render.RenderOffset;
                //render.RenderOffset = new Point(Bounds.X, Bounds.Y);
                Render(skin);
                //render.RenderOffset = old;
                //render.ClipRegion = old;

                if (ShouldCacheToTexture)
                {
                    cache.FinishCacheTexture(this);
                    CacheTextureDirty = false;
                }
            }

            render.ClipRegion = oldRegion;
            render.StartClip();
            render.RenderOffset = oldRenderOffset;

            if (ShouldCacheToTexture)
                cache.DrawCachedControlTexture(this);
        }

        public virtual void DoRender(Skins.Skin skin)
        {
            Renderers.Renderer render = skin.Renderer;

            if (render.CTT != null && ShouldCacheToTexture)
            {
                DoCacheRender(skin, this);
                return;
            }

            RenderMyself(skin, Bounds);

            if (DrawDebugOutlines)
                skin.DrawDebugOutlines(this);
        }

        /// <summary>
        /// Recursive rendering logic.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        /// <param name="clipRect">Clipping rectangle.</param>
        private void RenderMyself(Skins.Skin skin, Rectangle clipRect)
        {
            Renderers.Renderer render = skin.Renderer;
            Vector2i oldRenderOffset = render.RenderOffset;

            render.AddRenderOffset(clipRect);

            RenderUnder(skin);

            Rectangle oldRegion = render.ClipRegion;

            if (ShouldClip)
            {
                render.AddClipRegion(clipRect);

                if (!render.ClipRegionVisible)
                {
                    render.RenderOffset = oldRenderOffset;
                    render.ClipRegion = oldRegion;
                    return;
                }

                render.StartClip();
            }

            //Render myself first
            Render(skin);

            render.ClipRegion = oldRegion;
            render.StartClip();
            RenderOver(skin);

            RenderFocus(skin);

            render.RenderOffset = oldRenderOffset;
        }

        #endregion

        #region Drawing

        public virtual void Redraw()
        {
            UpdateColors();
            CacheTextureDirty = true;
            if (Parent != null)
                Parent.Redraw();
        }

        public virtual void UpdateColors()
        {
        }

        #endregion

        #region Accelerators

        /// <summary>
        /// Default accelerator handler.
        /// </summary>
        protected virtual void OnAccelerator()
        {
            throw new NotImplementedException();
        }

        public virtual bool HandleAccelerator(String accelerator)
        {
            if (HandleAcceleratorAux(accelerator)) return true;

            return false;
        }

        protected bool HandleAcceleratorAux(string accelerator)
        {
            if (InputHandler.KeyboardFocus == this || !AccelOnlyFocus)
            {
                if (m_Accelerators.ContainsKey(accelerator))
                {
                    m_Accelerators[accelerator].Invoke(this);
                    return true;
                }
            }
            return false;
        }

        /// <summary>
        /// Adds keyboard accelerator.
        /// </summary>
        /// <param name="accelerator">Accelerator text.</param>
        /// <param name="handler">Handler.</param>
        public void AddAccelerator(String accelerator, GwenEventHandler handler)
        {
            accelerator = accelerator.Trim().ToUpperInvariant();
            m_Accelerators[accelerator] = handler;
        }

        /// <summary>
        /// Default accelerator handler.
        /// </summary>
        /// <param name="control">Event source.</param>
        private void DefaultAcceleratorHandler(IControl control)
        {
            OnAccelerator();
        }

        /// <summary>
        /// Adds keyboard accelerator with a default handler.
        /// </summary>
        /// <param name="accelerator">Accelerator text.</param>
        public void AddAccelerator(String accelerator)
        {
            m_Accelerators[accelerator] = DefaultAcceleratorHandler;
        }

        #endregion

        #region Context-sensitive help

        /// <summary>
        /// Creates a tooltip for the control.
        /// </summary>
        /// <param name="text">Tooltip text.</param>
        public void SetToolTipText(String text)
        {
            var tooltip = new Label
                              {
                                  AutoSizeToContents = true,
                                  Text = text,
                                  TextColorOverride = Skin.Colors.TooltipText,
                                  Margin = new Margin(5, 3, 5, 3)
                              };
            tooltip.SizeToContents();

            ToolTip = tooltip;
        }

        #endregion

        #region Drag and drop

        // giver
        public Package DragAndDrop_GetPackage(int x, int y)
        {
            return m_DragAndDrop_Package;
        }

        // giver
        public bool DragAndDrop_Draggable()
        {
            if (m_DragAndDrop_Package == null)
                return false;

            return m_DragAndDrop_Package.IsDraggable;
        }

        // giver
        public void DragAndDrop_SetPackage(bool draggable, String name = "", object userData = null)
        {
            if (m_DragAndDrop_Package == null)
            {
                m_DragAndDrop_Package = new Package
                                            {
                                                IsDraggable = draggable, 
                                                Name = name, 
                                                UserData = userData
                                            };
            }
        }

        // giver
        public virtual bool DragAndDrop_ShouldStartDrag()
        {
            return true;
        }

        // giver
        public virtual void DragAndDrop_StartDragging(Package package, int x, int y)
        {
            package.HoldOffset = CanvasPosToLocal(new Vector2i(x, y));
            package.DrawControl = this;
        }

        // giver
        public virtual void DragAndDrop_EndDragging(bool success, int x, int y)
        {
        }

        // receiver
        public virtual bool DragAndDrop_HandleDrop(Package p, int x, int y)
        {
            DragAndDrop.SourceControl.Parent = this;
            return true;
        }

        // receiver
        public virtual void DragAndDrop_HoverEnter(Package p, int x, int y)
        {

        }

        // receiver
        public virtual void DragAndDrop_HoverLeave(Package p)
        {

        }

        // receiver
        public virtual void DragAndDrop_Hover(Package p, int x, int y)
        {

        }

        // receiver
        public virtual bool DragAndDrop_CanAcceptPackage(Package p)
        {
            return false;
        }

        #endregion

        #region Clipboard events

        public virtual void OnPaste(IControl from)
        {
        }

        public virtual void OnCopy(IControl from)
        {
        }

        public virtual void OnCut(IControl from)
        {
        }

        public virtual void OnSelectAll(IControl from)
        {
        }

        public void InputCopy(IControl from)
        {
            OnCopy(from);
        }

        public void InputPaste(IControl from)
        {
            OnPaste(from);
        }

        public void InputCut(IControl from)
        {
            OnCut(from);
        }

        public void InputSelectAll(IControl from)
        {
            OnSelectAll(from);
        }

        #endregion

        #region Text input events

        public virtual bool OnChar(Char chr)
        {
            return false;
        }

        bool IControl.InputChar(Char chr)
        {
            return OnChar(chr);
        }

        #endregion

        #region Keyboard events

        /// <summary>
        /// Handler for keyboard events.
        /// </summary>
        /// <param name="key">Key pressed.</param>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>True if handled.</returns>
        public virtual bool OnKeyPressed(Keys key, bool down = true)
        {
            bool handled = false;
            switch (key)
            {
                case Keys.Tab: handled = OnKeyTab(down); break;
                case Keys.Space: handled = OnKeySpace(down); break;
                case Keys.Home: handled = OnKeyHome(down); break;
                case Keys.End: handled = OnKeyEnd(down); break;
                case Keys.Return: handled = OnKeyReturn(down); break;
                case Keys.Back: handled = OnKeyBackspace(down); break;
                case Keys.Delete: handled = OnKeyDelete(down); break;
                case Keys.Right: handled = OnKeyRight(down); break;
                case Keys.Left: handled = OnKeyLeft(down); break;
                case Keys.Up: handled = OnKeyUp(down); break;
                case Keys.Down: handled = OnKeyDown(down); break;
                case Keys.Escape: handled = OnKeyEscape(down); break;
            }

            if (!handled && Parent != null)
                Parent.OnKeyPressed(key, down);

            return handled;
        }

        bool IControl.InputKeyPressed(Keys key, bool down)
        {
            return OnKeyPressed(key, down);
        }

        /// <summary>
        /// Handler for keyboard events.
        /// </summary>
        /// <param name="key">Key pressed.</param>
        /// <returns>True if handled.</returns>
        protected bool OnKeyReleased(Keys key)
        {
            return OnKeyPressed(key, false);
        }

        public virtual bool OnKeyTab(bool down)
        {
            if (!down)
                return true;

            if (GetCanvas().NextTab != null)
            {
                GetCanvas().NextTab.Focus();
                Redraw();
            }

            return true;
        }

        public virtual bool OnKeySpace(bool down) { return false; }

        public virtual bool OnKeyReturn(bool down) { return false; }

        public virtual bool OnKeyBackspace(bool down) { return false; }

        public virtual bool OnKeyDelete(bool down) { return false; }

        public virtual bool OnKeyRight(bool down) { return false; }

        public virtual bool OnKeyLeft(bool down) { return false; }

        public virtual bool OnKeyHome(bool down) { return false; }

        public virtual bool OnKeyEnd(bool down) { return false; }

        public virtual bool OnKeyUp(bool down) { return false; }

        public virtual bool OnKeyDown(bool down) { return false; }

        public virtual bool OnKeyEscape(bool down) { return false; }

        #endregion

        #region Mouse events

        public virtual bool OnMouseWheeled(int delta)
        {
            if (Parent != null)
                return Parent.OnMouseWheeled(delta);

            return false;
        }

        bool IControl.InputMouseWheeled(int delta)
        {
            return OnMouseWheeled(delta);
        }

        public virtual void OnMouseMoved(int x, int y, int dx, int dy)
        {

        }

        void IControl.InputMouseMoved(int x, int y, int dx, int dy)
        {
            OnMouseMoved(x, y, dx, dy);
        }

        public virtual void OnMouseClickedLeft(int x, int y, bool down)
        {
        }

        void IControl.InputMouseClickedLeft(int x, int y, bool down)
        {
            OnMouseClickedLeft(x, y, down);
        }

        public virtual void OnMouseClickedRight(int x, int y, bool down)
        {
        }

        void IControl.InputMouseClickedRight(int x, int y, bool down)
        {
            OnMouseClickedRight(x, y, down);
        }

        public virtual void OnMouseDoubleClickedLeft(int x, int y)
        {
            OnMouseClickedLeft(x, y, true); // [omeg] should this be called?
        }

        void IControl.InputMouseDoubleClickedLeft(int x, int y)
        {
            OnMouseDoubleClickedLeft(x, y);
        }

        public virtual void OnMouseDoubleClickedRight(int x, int y)
        {
            OnMouseClickedRight(x, y, true); // [omeg] should this be called?
        }

        void IControl.InputMouseDoubleClickedRight(int x, int y)
        {
            OnMouseDoubleClickedRight(x, y);
        }

        public virtual void OnMouseEntered()
        {
            if (HoverEnter != null)
                HoverEnter.Invoke(this);

            if (ToolTip != null)
                GUIv2.ToolTip.Enable(this);
            else if (Parent != null && Parent.ToolTip != null)
                GUIv2.ToolTip.Enable(Parent);

            Redraw();
        }

        void IControl.InputMouseEntered()
        {
            OnMouseEntered();
        }

        public virtual void OnMouseLeft()
        {
            if (HoverLeave != null)
                HoverLeave.Invoke(this);

            if (ToolTip != null)
                GUIv2.ToolTip.Disable(this);

            Redraw();
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

        #endregion

        #region Render events

        /// <summary>
        /// Renders the focus overlay.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public virtual void RenderFocus(Skins.Skin skin)
        {
            if (InputHandler.KeyboardFocus != this)
                return;
            if (!IsTabable)
                return;

            skin.DrawKeyboardHighlight(this, RenderBounds, 3);
        }

        /// <summary>
        /// Renders under the actual control (shadows etc).
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public virtual void RenderUnder(Skins.Skin skin)
        {

        }

        /// <summary>
        /// Renders over the actual control (overlays).
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public virtual void RenderOver(Skins.Skin skin)
        {

        }

        #endregion

        #region Animation

        public void Anim_WidthIn(float length, float delay = 0.0f, float ease = 1.0f)
        {
            Animation.Add(this, new Anim.Size.Width(0, Width, length, false, delay, ease));
            Width = 0;
        }

        public void Anim_HeightIn(float length, float delay, float ease)
        {
            Animation.Add(this, new Anim.Size.Height(0, Height, length, false, delay, ease));
            Height = 0;
        }

        public void Anim_WidthOut(float length, bool hide, float delay, float ease)
        {
            Animation.Add(this, new Anim.Size.Width(Width, 0, length, hide, delay, ease));
        }

        public void Anim_HeightOut(float length, bool hide, float delay, float ease)
        {
            Animation.Add(this, new Anim.Size.Height(Height, 0, length, hide, delay, ease));
        }

        #endregion
    }
}
