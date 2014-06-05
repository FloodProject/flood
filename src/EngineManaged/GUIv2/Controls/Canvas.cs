using Flood.GUIv2.Anim;
using Flood.GUIv2.DragDrop;
using Flood.GUIv2.Input;
using System;
using System.Collections.Generic;
using Flood.GUIv2.Panels;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.Controls
{
    /// <summary>
    /// Canvas control. It should be the root parent for all other controls.
    /// </summary>
    public class Canvas : CompositeControl
    {
        private bool m_NeedsRedraw;
        private float m_Scale;
        private Color m_BackgroundColor;
        protected Skins.Skin m_Skin;

        public override Skins.Skin Skin
        {
            get { return m_Skin; }
        }

        // [omeg] these are not created by us, so no disposing
        internal Control FirstTab;
        internal Control NextTab;

        private readonly List<IDisposable> m_DisposeQueue; // dictionary for faster access?

        /// <summary>
        /// Scale for rendering.
        /// </summary>
        public float Scale
        {
            get { return m_Scale; }
            set
            {
                if (m_Scale == value)
                    return;

                m_Scale = value;

                if (Skin != null && Skin.Renderer != null)
                    Skin.Renderer.Scale = m_Scale;

                OnScaleChanged();
                Redraw();
            }
        }

        /// <summary>
        /// Background color.
        /// </summary>
        public Color BackgroundColor { get { return m_BackgroundColor; } set { m_BackgroundColor = value; } }

        ///// <summary>
        ///// In most situations you will be rendering the canvas every frame. 
        ///// But in some situations you will only want to render when there have been changes. 
        ///// You can do this by checking NeedsRedraw.
        ///// </summary>
        //public bool NeedsRedraw { get { return m_NeedsRedraw; } set { m_NeedsRedraw = value; } }

        /// <summary>
        /// Initializes a new instance of the <see cref="Canvas"/> class.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public Canvas(Skins.Skin skin, Renderers.Renderer renderer, ResourceHandle<Image> imageHandle) 
            : base(new RelativePanel())
        {
            //SetBounds(0, 0, 10000, 10000);
            m_Skin = skin;
            Scale = 1.0f;
            BackgroundColor = Color.White;
            ShouldDrawBackground = false;

            m_DisposeQueue = new List<IDisposable>();
        }

        public override void Dispose()
        {
            ProcessDelayedDeletes();
            base.Dispose();
        }

        ///// <summary>
        ///// Re-renders the control, invalidates cached texture.
        ///// </summary>
        //public override void Redraw()
        //{
        //    NeedsRedraw = true;
        //    base.Redraw();
        //}
        
        // Children call parent.GetCanvas() until they get to 
        // this top level function.
        public override Canvas GetCanvas()
        {
            return this;
        }

        /// <summary>
        /// Additional initialization (which is sometimes not appropriate in the constructor)
        /// </summary>
        protected void Initialize()
        {

        }

        /// <summary>
        /// Renders the canvas. Call in your rendering loop.
        /// </summary>
        public void RenderCanvas()
        {
            Update();

            Renderers.Renderer render = Skin.Renderer;

            render.Begin();

            Layout(Skin);

            render.ClipRegion = Bounds;
            render.RenderOffset = new Vector2i(0,0);
            render.Scale = Scale;

            if (ShouldDrawBackground)
            {
                render.DrawColor = m_BackgroundColor;
                render.DrawFilledRect(RenderBounds);
            }

            DoRender(Skin);

            DragAndDrop.RenderOverlay(this, Skin);

            GUIv2.ToolTip.RenderToolTip(Skin);

            render.EndClip();

            render.End();
        }

        ///// <summary>
        ///// Renders the control using specified skin.
        ///// </summary>
        ///// <param name="skin">Skin to use.</param>
        //public override void Render(Skins.Skin skin)
        //{
        //    //skin.Renderer.rnd = new Random(1);
        //    base.Render(skin);
        //    m_NeedsRedraw = false;
        //}

        /// <summary>
        /// Handler invoked when control's bounds change.
        /// </summary>
        /// <param name="oldBounds">Old bounds.</param>
        public override void OnBoundsChanged(Rectangle oldBounds)
        {
            base.OnBoundsChanged(oldBounds);
            InvalidateChildren(true);
        }

        /// <summary>
        /// Processes input and layout. Also purges delayed delete queue.
        /// </summary>
        public void Update()
        {
            if (IsHidden)
                return;

            Animation.GlobalThink();

            // Reset tabbing
            NextTab = null;
            FirstTab = null;
            
            ProcessDelayedDeletes();

            // Check has focus etc..
            Layout(Skin);

            // If we didn't have a next tab, cycle to the start.
            if (NextTab == null)
                NextTab = FirstTab;

            InputHandler.OnCanvasThink(this);
        }

        public void GetRenderBounds(out int width, out int height)
        {
            //width = 0;
            //height = 0;
            //foreach (var control in Panel.Children)
            //{
            //    width = Math.Max(width, control.Right);
            //    height = Math.Max(height, control.Bottom);
            //}
            width = Panel.RenderWidth;
            height = Panel.RenderHeight;
        }

        public void AddChild(Control child, int x, int y)
        {
            ((RelativePanel)Panel).AddChild(child, x, y);
        }

        public void AddChild(IControl child)
        {
            Panel.AddChild(child);
        }

        /// <summary>
        /// Adds given control to the delete queue and detaches it from canvas. Don't call from Dispose, it modifies child list.
        /// </summary>
        /// <param name="control">Control to delete.</param>
        public void AddDelayedDelete(IControl control)
        {
            if (!m_DisposeQueue.Contains((IDisposable)control))
            {
                m_DisposeQueue.Add((IDisposable)control);
                Panel.RemoveChild(control, false);
            }
#if DEBUG
            else
                throw new InvalidOperationException("Control deleted twice");
#endif
        }

        private void ProcessDelayedDeletes()
        {
            //if (m_DisposeQueue.Count > 0)
            //    System.Diagnostics.Debug.Print("Canvas.ProcessDelayedDeletes: {0} items", m_DisposeQueue.Count);
            for (int i = 0; i < m_DisposeQueue.Count; i++)
            {
                m_DisposeQueue[i].Dispose();
            }
            m_DisposeQueue.Clear();
        }

        #region Input Handlers

        /// <summary>
        /// Handles mouse movement events. Called from Input subsystems.
        /// </summary>
        /// <returns>True if handled.</returns>
        public bool Input_MouseMoved(int x, int y, int dx, int dy)
        {
            if (IsHidden)
                return false;

            // Todo: Handle scaling here..
            //float fScale = 1.0f / Scale();

            InputHandler.OnMouseMoved(this, x, y, dx, dy);

            if (InputHandler.HoveredControl == null) return false;
            if (InputHandler.HoveredControl == this) return false;
            if (InputHandler.HoveredControl.GetCanvas() != this) return false;

            InputHandler.HoveredControl.InputMouseMoved(x, y, dx, dy);
            InputHandler.HoveredControl.UpdateCursor();

            DragAndDrop.OnMouseMoved(InputHandler.HoveredControl, x, y);
            return true;
        }

        /// <summary>
        /// Handles mouse button events. Called from Input subsystems.
        /// </summary>
        /// <returns>True if handled.</returns>
        public bool Input_MouseButton(int button, bool down)
        {
            if (IsHidden) return false;

            return InputHandler.OnMouseClicked(this, button, down);
        }

        /// <summary>
        /// Handles keyboard events. Called from Input subsystems.
        /// </summary>
        /// <returns>True if handled.</returns>
        public bool Input_Key(Keys key, bool down)
        {
            if (IsHidden) return false;

            return InputHandler.OnKeyEvent(this, key, down);
        }

        /// <summary>
        /// Handles keyboard events. Called from Input subsystems.
        /// </summary>
        /// <returns>True if handled.</returns>
        public bool Input_Character(char chr)
        {
            if (IsHidden) return false;
            if (char.IsControl(chr)) return false;

            //Handle Accelerators
            if (InputHandler.HandleAccelerator(this, chr))
                return true;

            //Handle characters
            if (InputHandler.KeyboardFocus == null) return false;
            if (InputHandler.KeyboardFocus.GetCanvas() != this) return false;
            if (!InputHandler.KeyboardFocus.IsVisible) return false;
            if (InputHandler.IsControlDown) return false;

            return InputHandler.KeyboardFocus.InputChar(chr);
        }

        /// <summary>
        /// Handles the mouse wheel events. Called from Input subsystems.
        /// </summary>
        /// <returns>True if handled.</returns>
        public bool Input_MouseWheel(int val)
        {
            if (IsHidden) return false;
            if (InputHandler.HoveredControl == null) return false;
            if (InputHandler.HoveredControl == this) return false;
            if (InputHandler.HoveredControl.GetCanvas() != this) return false;

            return InputHandler.HoveredControl.InputMouseWheeled(val);
        }

        #endregion
    }
}
