using System;
using System.Collections.Generic;
using System.Linq;
using Flood.GUIv2.Controls;
using Flood.GUIv2.Input;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2
{
    public abstract class Panel : Control, IPanel
    {

        private bool m_InheritedExpandVertical; 

        public bool InheritedExpandVertical 
        {
            get { return m_InheritedExpandVertical;}
            set
            {
                if (!base.IsExpandVertical)
                    m_InheritedExpandVertical = value;
            }
        }
        
        private bool m_InheritedExpandHorizontal; 

        public bool InheritedExpandHorizontal 
        {
            get { return m_InheritedExpandHorizontal;}
            set
            {
                if (!base.IsExpandHorizontal)
                    m_InheritedExpandHorizontal = value;
            }
        }

        public override bool IsExpandVertical
        {
            get { return base.IsExpandVertical || InheritedExpandVertical; }
            set { base.IsExpandVertical = value; }
        }

        public override bool IsExpandHorizontal
        {
            get { return base.IsExpandHorizontal || InheritedExpandHorizontal; }
            set { base.IsExpandHorizontal = value; }
        }

        public override bool IsFit { get; set; }

        /// <summary>
        /// List of children.
        /// </summary>
        private readonly List<IControl> m_Children;

        /// <summary>
        /// List of children.
        /// </summary>
        public List<IControl> Children
        {
            get { return m_Children; }
        }

        protected Vector2i layoutMinSize;
        protected Vector2i layoutBestSize;
        protected Vector2i layoutMaxSize;

        public override Vector2i LayoutMinSize
        {
            get
            {
                if (Expansion == ExpansionFlags.Shaped)
                    return base.LayoutMinSize;
                var renderLayoutMinSize = layoutMinSize.TryToAdd(MarginSizes);
                var minX = (RenderMinimumSize.X > renderLayoutMinSize.X)
                            ? RenderMinimumSize.X : renderLayoutMinSize.X;
                var minY = (RenderMinimumSize.Y > renderLayoutMinSize.Y)
                            ? RenderMinimumSize.Y : renderLayoutMinSize.Y;

                return new Vector2i(minX, minY);
            }
        }

        public override Vector2i LayoutBestSize
        {
            get
            {
                if (Expansion == ExpansionFlags.Shaped)
                    return base.LayoutBestSize;

                return layoutBestSize.TryToAdd(MarginSizes);
            }
        }


        public override bool IsVisible
        {
            get
            {
                if (IsHidden)
                    return false;

                if (LogicParent != null)
                    return LogicParent.IsVisible;

                return true;
            }
        }
        public override Vector2i LayoutMaxSize
        {
            get
            {
                if (Expansion == ExpansionFlags.Shaped || (Expansion & (ExpansionFlags.Fill | ExpansionFlags.Expand)) != 0)
                    return base.LayoutMaxSize;
               
                var renderLayoutMaxSize = layoutMaxSize.TryToAdd(MarginSizes);
                var maxX = (RenderMaximumSize.X < renderLayoutMaxSize.X)
                            ? RenderMaximumSize.X : renderLayoutMaxSize.X;
                var maxY = (RenderMaximumSize.Y < renderLayoutMaxSize.Y)
                            ? RenderMaximumSize.Y : renderLayoutMaxSize.Y; 

                return new Vector2i(maxX, maxY);
            }
        }


        public override bool DrawDebugOutlines
        {

            get { return base.DrawDebugOutlines; }
            set
            {
                if (base.DrawDebugOutlines == value)
                    return;
                base.DrawDebugOutlines = value;
                foreach (Control child in Children)
                {
                    child.DrawDebugOutlines = value;
                }
            }
        }

        public void Invalidate()
        {
            var parent = LogicParent as IPanel;
            //todo: check if this is not causing layout problems or infinite loops
            if (parent != null)
                parent.Invalidate();
            if (NeedsLayout)
                return;
            NeedsLayout = true;
            CacheTextureDirty = true;
        }

        public void InvalidateChildren(bool recursive = false)
        {
            foreach (var child in Children.Where(child => child is IPanel).Cast<IPanel>())
            {
                child.Invalidate();
                if (recursive)
                    child.InvalidateChildren(true);
            }
        }

        public IControl LogicParent
        {
            get
            {
                if (Parent != null && Parent is CompositeControl)
                    return Parent.Parent;
                return Parent;
            }
        }

        /// <summary>
        /// Gets the associated sizer.
        /// </summary>
        /// <remarks>
        /// The sizer is set by the sizer constructor when it is passed a reference to this.
        /// The coupling must be done that way.
        /// </remarks>
        public Sizer Sizer { get; internal set; }

        /// <summary>
        /// Initializes a base instance of the <see cref="Panel"/> class. 
        /// This is a top level panel, inheriting from a canvas and should be 
        /// the root for all child panels in this canvas.
        /// </summary>
        /// <param name="expansion">The type of <see cref="ExpansionFlags"/> of this panel.</param>
        protected Panel(ExpansionFlags expansion = ExpansionFlags.Expand)
        {
            m_Children = new List<IControl>();
            Expansion = expansion;
        }



        public override void OnCanvasChanged(Canvas canvas)
        {
            base.OnCanvasChanged(canvas);
            foreach (var child in Children)
                child.OnCanvasChanged(canvas);
        }

        protected override void Dispose(bool disposing)
        {
            if (!disposing)
                return;

            for (var i = Children.Count - 1; i >= 0; i--)
                ((IDisposable)Children[i]).Dispose();

            Children.Clear();

            base.Dispose(true);
        }

        public override void CloseMenus()
        {
            //Debug.Print("Base.CloseMenus: {0}", this);

            // todo: not very efficient with the copying and recursive closing, maybe store currently open menus somewhere (canvas)?
            var childrenCopy = m_Children.FindAll(x => true);
            foreach (IControl child in childrenCopy)
            {
                child.CloseMenus();
            }
        }

        #region Child management

        /// <summary>
        /// Finds a child by name.
        /// </summary>
        /// <param name="name">Child name.</param>
        /// <param name="recursive">Determines whether the search should be recursive.</param>
        /// <returns>Found control or null.</returns>
        public virtual IControl FindChildByName(String name, bool recursive = false)
        {
            IControl b = Children.Find(x => x.Name == name);
            if (b != null)
                return b;

            if (recursive)
            {
                foreach (IControl child in m_Children)
                {
                    var panel = child as Panel;
                    if (panel != null)
                        b = panel.FindChildByName(name, true);

                    var composite = child as CompositeControl;
                    if (composite != null)
                        b = composite.FindChildByName(name, true);

                    if (b != null)
                        return b;
                }
            }
            return null;
        }

        /// <summary>
        /// Attaches specified control as a child of this panel.
        /// </summary>
        /// <param name="child">Control to be added as a child.</param>
        public virtual void AddChild(IControl child)
        {
            Children.Add(child);
            OnChildAdded(child);
            var parent = child.Parent as Panel;
            if (parent != null)
                parent.RemoveChild(child, false);

            child.Parent = this;
        }

        /// <summary>
        /// Attaches specified control as a child of this one.
        /// </summary>
        /// <param name="child">Control to be added as a child.</param>
        /// <param name="index">Position where to insert child</param>
        public virtual void InsertChild(IControl child, int index)
        {

            m_Children.Insert(index, child);
            OnChildAdded(child);

            var parent = child.Parent as Panel;
            if (parent != null)
                parent.RemoveChild(child, false);

            child.Parent = this;
        }

        public override void MoveTo(int x, int y)
        {
            //todo: remove this method and replace with relative position panel where neccessary
            throw new NotImplementedException();
            //var parent = LogicParent;
            //if (RestrictToParent && (parent != null))
            //{
            //    if (x - Padding.Left < parent.Margin.Left)
            //        x = parent.Margin.Left + Padding.Left;
            //    if (y - Padding.Top < parent.Margin.Top)
            //        y = parent.Margin.Top + Padding.Top;
            //    if (x + Width + Padding.Right > parent.Width - parent.Margin.Right)
            //        x = parent.Width - parent.Margin.Right - Width - Padding.Right;
            //    if (y + Height + Padding.Bottom > parent.Height - parent.Margin.Bottom)
            //        y = parent.Height - parent.Margin.Bottom - Height - Padding.Bottom;
            //}

            //SetBounds(x, y, Width, Height);
        }
        /// <summary>
        /// Detaches specified control from this one.
        /// </summary>
        /// <param name="child">Child to be removed.</param>
        /// <param name="dispose">Determines whether the child should be disposed (added to delayed delete queue).</param>
        public virtual void RemoveChild(IControl child, bool dispose)
        {
            Children.Remove(child);
            OnChildRemoved(child);

            if (dispose)
                child.DelayedDelete();

            child.Parent = null;
        }

        /// <summary>
        /// Removes all children (and disposes them).
        /// </summary>
        public virtual void DeleteAllChildren()
        {
            // todo: probably shouldn't invalidate after each removal
            while (m_Children.Count > 0)
                RemoveChild(m_Children[0], true);
        }

        /// <summary>
        /// Handler invoked when a child is added.
        /// </summary>
        /// <param name="child">Child added.</param>
        protected virtual void OnChildAdded(IControl child)
        {
            Invalidate();
        }

        /// <summary>
        /// Handler invoked when a child is removed.
        /// </summary>
        /// <param name="child">Child removed.</param>
        protected virtual void OnChildRemoved(IControl child)
        {
            Invalidate();
        }

        /// <summary>
        /// Checks if the given control is a child of this instance.
        /// </summary>
        /// <param name="child">Control to examine.</param>
        /// <returns>True if the control is out child.</returns>
        public bool IsChild(IControl child)
        {
            return Children.Contains(child);
        }

        #endregion

        public override sealed void DoCacheRender(Skins.Skin skin, IControl master)
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

                if (Children.Count > 0)
                {
                    //Now render my kids
                    foreach (Control child in Children)
                    {
                        if (child.IsHidden)
                            continue;
                        child.DoCacheRender(skin, master);
                    }
                }
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

        /// <summary>
        /// Recursive rendering logic.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        /// <param name="clipRect">Clipping rectangle.</param>
        private void RenderRecursive(Skins.Skin skin, Rectangle clipRect)
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

            if (m_Children.Count > 0)
            {
                //Now render my kids
                foreach (IControl child in Children.Where(child => !child.IsHidden))
                    child.DoRender(skin);
            }

            render.ClipRegion = oldRegion;
            render.StartClip();
            RenderOver(skin);

            RenderFocus(skin);

            render.RenderOffset = oldRenderOffset;
        }

        public override void DoRender(Skins.Skin skin)
        {
            Renderers.Renderer render = skin.Renderer;

            if (render.CTT != null && ShouldCacheToTexture)
            {
                DoCacheRender(skin, this);
                return;
            }

            RenderRecursive(skin, Bounds);

            if (DrawDebugOutlines)
                skin.DrawDebugOutlines(this);
        }

        public override bool HandleAccelerator(String accelerator)
        {
            if (HandleAcceleratorAux(accelerator)) return true;

            return Children.Any(child => child.HandleAccelerator(accelerator));
        }

        public override void Redraw()
        {
            UpdateColors();
            CacheTextureDirty = true;
            var parent = LogicParent;
            if (parent != null)
                parent.Redraw();
        }

        public override void OnScaleChanged()
        {
            foreach (Control child in Children)
                child.OnScaleChanged();
        }

        /// <summary>
        /// Resizes the control to fit its children.
        /// </summary>
        /// <param name="width">Determines whether to change control's width.</param>
        /// <param name="height">Determines whether to change control's height.</param>
        /// <returns>True if bounds changed.</returns>
        public virtual bool SizeToChildren(bool width = true, bool height = true)
        {
            var size = GetChildrenSize();
            return SetSize(width ? size.X : Width, height ? size.Y : Height);
        }

        /// <summary>
        /// Returns the total width and height of all children.
        /// </summary>
        /// <remarks>Default implementation returns maximum size of children since the layout is unknown.
        /// Implement this in derived compound controls to properly return their size.</remarks>
        /// <returns></returns>
        public virtual Vector2i GetChildrenSize()
        {
            var size = new Vector2i(0, 0);

            foreach (Control child in Children)
            {
                if (child.IsHidden)
                    continue;

                size.X = Math.Max(size.X, child.Right);
                size.Y = Math.Max(size.Y, child.Bottom);
            }

            return size;
        }

        public override IControl GetControlAt(int x, int y)
        {
            if (IsHidden)
                return null;

            if (x < 0 || y < 0 || x >= Width || y >= Height)
                return null;

            // todo: convert to linq FindLast
            var rev = ((IList<IControl>)Children).Reverse(); // IList.Reverse creates new list, List.Reverse works in place.. go figure
            foreach (Control child in rev)
            {
                IControl found = child.GetControlAt(x - child.X, y - child.Y);
                if (found != null)
                    return found;
            }

            if (!MouseInputEnabled)
                return null;

            return this;
        }

        public override void Layout(Skins.Skin skin)
        {
            if (IsHidden)
                return;

            RecursiveLayoutSizes(skin);
            RecursiveLayoutAssign();

            //todo: from here on it's old code, not sure if it still serves any purpose
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
        }

        public abstract void LayoutSizes();

        public void RecursiveLayoutSizes(Skins.Skin skin)
        {
            foreach (var child in Children)
            {
                if(!(child is IPanel) && child.ShouldPreLayout)
                {
                    child.ShouldPreLayout = false;
                    child.PreLayout(skin);
                }
                if(child is IPanel)
                    ((IPanel)child).RecursiveLayoutSizes(skin);
            }
            if(NeedsLayout)
            {
                PreLayout(skin);
                if(IsFit)
                    LayoutSizes();
            }
        }

        /// <summary>
        /// Lays out this panel using the associated sizer.
        /// </summary>
        public void LayoutAssign()
        {
            if (Sizer == null)
                return;
            Sizer.RecalcSizes();
        }

        public void RecursiveLayoutAssign()
        {
            if(NeedsLayout)
            {
                LayoutAssign();
                NeedsLayout = false;
            }
            foreach (var child in Children.Where(child => child is IPanel))
            {
                ((IPanel)child).RecursiveLayoutAssign();
            }
        }

        public bool NeedsLayout { get; set; }
    }
}
