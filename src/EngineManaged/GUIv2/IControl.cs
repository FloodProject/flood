using System;
using Flood.GUIv2.Controls;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2
{
    public interface IControl
    {
        /// <summary>
        /// Invoked when mouse pointer enters the control.
        /// </summary>
        event Control.GwenEventHandler HoverEnter;

        /// <summary>
        /// Invoked when mouse pointer leaves the control.
        /// </summary>
        event Control.GwenEventHandler HoverLeave;

        /// <summary>
        /// Invoked when control's bounds have been changed.
        /// </summary>
        event Control.GwenEventHandler BoundsChanged;

        /// <summary>
        /// Invoked when control's parent has changed.
        /// </summary>
        event Control.ParentChangedEventHandler ParentChanged;

        void OnCanvasChanged(Canvas canvas);

        bool ShouldPreLayout { get; set; }

        /// <summary>
        /// The logical parent. It's usually what you expect, the control you've parented it to.
        /// </summary>
        IControl Parent { get; set; }

        /// <summary>
        /// Current skin.
        /// </summary>
        Skins.Skin Skin { get; }

        //todo: since this is no longer a child of this it currently doesn't render or anything else
        /// <summary>
        /// Current tooltip.
        /// </summary>
        IControl ToolTip { get; set; }

        /// <summary>
        /// Indicates whether this control is a menu component.
        /// </summary>
        bool IsMenuComponent { get; }

        /// <summary>
        /// Determines whether the control should be clipped to its bounds while rendering.
        /// </summary>
        bool ShouldClip { get; }

        /// <summary>
        /// Current margin - outer spacing.
        /// </summary>
        Margin Margin { get; set; }

        /// <summary>
        /// Indicates whether the control is on top of its parent's children.
        /// </summary>
        bool IsOnTop { get; }

        /// <summary>
        /// User data associated with the control.
        /// </summary>
        object UserData { get; set; }

        /// <summary>
        /// Indicates whether the control is hovered by mouse pointer.
        /// </summary>
        bool IsHovered { get; }

        /// <summary>
        /// Indicates whether the control has focus.
        /// </summary>
        bool HasFocus { get; }

        /// <summary>
        /// Indicates whether the control is disabled.
        /// </summary>
        bool IsDisabled { get; set; }

        /// <summary>
        /// Indicates whether the control is hidden.
        /// </summary>
        bool IsHidden { get; set; }

        /// <summary>
        /// Determines whether the control's position should be restricted to parent's bounds.
        /// </summary>
        bool RestrictToParent { get; set; }

        /// <summary>
        /// Determines whether the control receives mouse input events.
        /// </summary>
        bool MouseInputEnabled { get; set; }

        /// <summary>
        /// Determines whether the control receives keyboard input events.
        /// </summary>
        bool KeyboardInputEnabled { get; set; }

        /// <summary>
        /// Gets or sets the mouse cursor when the cursor is hovering the control.
        /// </summary>
        Cursor Cursor { get; set; }

        /// <summary>
        /// Indicates whether the control is tabable (can be focused by pressing Tab).
        /// </summary>
        bool IsTabable { get; set; }

        /// <summary>
        /// Indicates whether control's background should be drawn during rendering.
        /// </summary>
        bool ShouldDrawBackground { get; set; }

        /// <summary>
        /// Indicates whether the renderer should cache drawing to a texture to improve performance (at the cost of memory).
        /// </summary>
        bool ShouldCacheToTexture { get; set; }

        /// <summary>
        /// Gets or sets the control's internal name.
        /// </summary>
        String Name { get; set; }

        /// <summary>
        /// Control's size and position relative to the parent.
        /// </summary>
        Rectangle Bounds { get; }

        /// <summary>
        /// Bounds for the renderer.
        /// </summary>
        Rectangle RenderBounds { get; }

        /// <summary>
        /// Size restriction.
        /// </summary>
        Vector2i MinimumSize { get; set; }

        /// <summary>
        /// Best size.
        /// </summary>
        Vector2i BestSize { get; set; }

        /// <summary>
        /// Size restriction.
        /// </summary>
        Vector2i MaximumSize { get; set; }

        /// <summary>
        /// Shapes the control to the specified size.
        /// </summary>
        /// <param name="size">The size.</param>
        void Shape(Vector2i size);

        /// <summary>
        /// Determines whether hover should be drawn during rendering.
        /// </summary>
        bool ShouldDrawHover { get; }

        bool AccelOnlyFocus { get; }
        bool NeedsInputChars { get; }

        /// <summary>
        /// Indicates whether the control and its parents are visible.
        /// </summary>
        bool IsVisible { get; }

        /// <summary>
        /// Leftmost coordinate of the control.
        /// </summary>
        int X { get; set; }

        /// <summary>
        /// Topmost coordinate of the control.
        /// </summary>
        int Y { get; set; }

        int Width { get; set; }
        int Height { get; set; }
        int Bottom { get; }
        int Right { get; }

        /// <summary>
        /// Height plus vertical margins.
        /// </summary>
        int RenderHeight { get; }

        /// <summary>
        /// Width plus horizontal margins.
        /// </summary>
        int RenderWidth { get; }
        
        /// <summary>
        /// Vertical margins.
        /// </summary>
        int VerticalMargins { get; }

        /// <summary>
        /// Horizontal margins.
        /// </summary>
        int HorizontalMargins { get; }

        Vector2i MarginSizes { get; }

        /// <summary>
        /// Determines whether margin, padding and bounds outlines for the control will be drawn. Applied recursively to all children.
        /// </summary>
        bool DrawDebugOutlines { get; set; }

        Color PaddingOutlineColor { get; set; }
        Color MarginOutlineColor { get; set; }
        Color BoundsOutlineColor { get; set; }
        MarginFlags MarginFlags { get; }

        int Proportion { get; set; }
        ExpansionFlags Expansion { get; }
        AlignmentFlags Alignment { get; set; }

        /// <summary>
        /// Detaches the control from canvas and adds to the deletion queue (processed in Canvas.DoThink).
        /// </summary>
        void DelayedDelete();

        string ToString();

        /// <summary>
        /// Gets the canvas (root parent) of the control.
        /// </summary>
        /// <returns></returns>
        Canvas GetCanvas();

        /// <summary>
        /// Closes all menus recursively.
        /// </summary>
        void CloseMenus();

        /// <summary>
        /// Copies Bounds to RenderBounds.
        /// </summary>
        void UpdateRenderBounds();

        /// <summary>
        /// Sets the control bounds.
        /// </summary>
        /// <param name="bounds">New bounds.</param>
        /// <returns>True if bounds changed.</returns>
        bool SetBounds(Rectangle bounds);

        /// <summary>
        /// Sets the control bounds.
        /// </summary>
        /// <param name="x">X.</param>
        /// <param name="y">Y.</param>
        /// <param name="width">Width.</param>
        /// <param name="height">Height.</param>
        /// <returns>
        /// True if bounds changed.
        /// </returns>
        bool SetBounds(float x, float y, float width, float height);

        /// <summary>
        /// Sets the control bounds.
        /// </summary>
        /// <param name="x">X position.</param>
        /// <param name="y">Y position.</param>
        /// <param name="width">Width.</param>
        /// <param name="height">Height.</param>
        /// <returns>
        /// True if bounds changed.
        /// </returns>
        bool SetBounds(int x, int y, int width, int height);

        /// <summary>
        /// Handler invoked when control's bounds change.
        /// </summary>
        /// <param name="oldBounds">Old bounds.</param>
        void OnBoundsChanged(Rectangle oldBounds);

        /// <summary>
        /// Handler invoked when control's scale changes.
        /// </summary>
        void OnScaleChanged();

        /// <summary>
        /// Handler invoked when control children's bounds change.
        /// </summary>
        void OnChildBoundsChanged(Rectangle oldChildBounds, IControl child);

        /// <summary>
        /// Sets the control size.
        /// </summary>
        /// <param name="width">New width.</param>
        /// <param name="height">New height.</param>
        /// <returns>True if bounds changed.</returns>
        bool SetSize(int width, int height);

        /// <summary>
        /// Moves the control by a specific amount.
        /// </summary>
        /// <param name="x">X-axis movement.</param>
        /// <param name="y">Y-axis movement.</param>
        void MoveBy(int x, int y);

        /// <summary>
        /// Moves the control to a specific point.
        /// </summary>
        /// <param name="x">Target x coordinate.</param>
        /// <param name="y">Target y coordinate.</param>
        void MoveTo(float x, float y);

        /// <summary>
        /// Moves the control to a specific point, clamping on paren't bounds if RestrictToParent is set.
        /// </summary>
        /// <param name="x">Target x coordinate.</param>
        /// <param name="y">Target y coordinate.</param>
        void MoveTo(int x, int y);

        /// <summary>
        /// Sets the control position.
        /// </summary>
        /// <param name="x">Target x coordinate.</param>
        /// <param name="y">Target y coordinate.</param>
        void SetPosition(float x, float y);

        /// <summary>
        /// Sets the control position.
        /// </summary>
        /// <param name="x">Target x coordinate.</param>
        /// <param name="y">Target y coordinate.</param>
        void SetPosition(int x, int y);

        /// <summary>
        /// Gets a child by its coordinates.
        /// </summary>
        /// <param name="x">Child X.</param>
        /// <param name="y">Child Y.</param>
        /// <returns>Control or null if not found.</returns>
        IControl GetControlAt(int x, int y);

        /// <summary>
        /// Converts local coordinates to canvas coordinates.
        /// </summary>
        /// <param name="pnt">Local coordinates.</param>
        /// <returns>Canvas coordinates.</returns>
        Vector2i LocalPosToCanvas(Vector2i pnt);

        /// <summary>
        /// Converts canvas coordinates to local coordinates.
        /// </summary>
        /// <param name="pnt">Canvas coordinates.</param>
        /// <returns>Local coordinates.</returns>
        Vector2i CanvasPosToLocal(Vector2i pnt);

        /// <summary>
        /// Lays out the control's interior according to alignment, padding, dock etc.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        void PreLayout(Skins.Skin skin = null);

        /// <summary>
        /// Lays out the control's interior according to alignment, margin, padding, dock etc.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        void Layout(Skins.Skin skin);

        Vector2i LayoutMinSize { get; }

        Vector2i LayoutBestSize { get; }

        Vector2i LayoutMaxSize { get; }

        /// <summary>
        /// Defines if the control tries to take the maximum vertical space available or not.
        /// </summary>
        bool IsExpandVertical { get; set; }

        /// <summary>
        /// Defines if the control tries to take the maximum horizontal space available or not.
        /// </summary>
        bool IsExpandHorizontal { get; set; }

        /// <summary>
        /// Defines if the control tries to expand to take only the 
        /// width of its parent
        /// </summary>
        bool IsFillHorizontal { get; set; }

        /// <summary>
        /// Defines if the control tries to expand to take only the 
        /// height of its parent
        /// </summary>
        bool IsFillVertical { get; set; }

        /// <summary>
        /// Defines if control sizes itself in function of its children
        /// </summary>
        bool IsFit { get; set; }

        /// <summary>
        /// Function invoked after layout.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        void PostLayout(Skins.Skin skin);

        /// <summary>
        /// Invalidates control's parent.
        /// </summary>
        void InvalidateParent();

        void SetDimension(Vector2i pos, Vector2i size);
        Vector2i GetMinSizeWithBorder();
        Vector2i GetSizeWithBorder();
        void ReduceToMinSize();

        ///// <summary>
        ///// Informs the control regarding the size available in the minor direction of
        ///// an enveloping BoxPanel so it can call its sizer to redimension itself.
        ///// </summary>
        ///// <param name="direction">The minor direction.</param>
        ///// <param name="size">The size available in the minor boxPanel direction.</param>
        ///// <param name="availableOtherDir">The available size in the major boxPanel dir.</param>
        ///// <returns>Whether any size reduction was achieved.</returns>
        //bool InformFirstDirection(Orientation direction, int size, int availableOtherDir);
        
        Vector2i GetMaxSizeWithBorder();

        /// <summary>
        /// Sets mouse cursor to current cursor.
        /// </summary>
        void UpdateCursor();

        /// <summary>
        /// Enables the control.
        /// </summary>
        void Enable();

        /// <summary>
        /// Disables the control.
        /// </summary>
        void Disable();

        /// <summary>
        /// Hides the control.
        /// </summary>
        void Hide();

        /// <summary>
        /// Shows the control.
        /// </summary>
        void Show();

        /// <summary>
        /// Focuses the control.
        /// </summary>
        void Focus();

        /// <summary>
        /// Unfocuses the control.
        /// </summary>
        void Blur();

        /// <summary>
        /// Handler for gaining keyboard focus.
        /// </summary>
        void OnKeyboardFocus();

        /// <summary>
        /// Handler for losing keyboard focus.
        /// </summary>
        void OnLostKeyboardFocus();

        /// <summary>
        /// Sends the control to the bottom of parent' visibility stack.
        /// </summary>
        void SendToBack();

        /// <summary>
        /// Brings the control to the top of paren't visibility stack.
        /// </summary>
        void BringToFront();

        void BringNextToControl(IControl child, bool behind);

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        void Render(Skins.Skin skin);

        /// <summary>
        /// Renders the control to a cache using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        /// <param name="master">Root parent.</param>
        void DoCacheRender(Skins.Skin skin, IControl master);

        /// <summary>
        /// Rendering logic implementation.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        void DoRender(Skins.Skin skin);

        /// <summary>
        /// Re-renders the control, invalidates cached texture.
        /// </summary>
        void Redraw();

        /// <summary>
        /// Updates control colors.
        /// </summary>
        /// <remarks>
        /// Used in composite controls like lists to differentiate row colors etc.
        /// </remarks>
        void UpdateColors();

        /// <summary>
        /// Handles keyboard accelerator.
        /// </summary>
        /// <param name="accelerator">Accelerator text.</param>
        /// <returns>True if handled.</returns>
        bool HandleAccelerator(String accelerator);

        /// <summary>
        /// Adds keyboard accelerator.
        /// </summary>
        /// <param name="accelerator">Accelerator text.</param>
        /// <param name="handler">Handler.</param>
        void AddAccelerator(String accelerator, Control.GwenEventHandler handler);

        /// <summary>
        /// Adds keyboard accelerator with a default handler.
        /// </summary>
        /// <param name="accelerator">Accelerator text.</param>
        void AddAccelerator(String accelerator);

        /// <summary>
        /// Creates a tooltip for the control.
        /// </summary>
        /// <param name="text">Tooltip text.</param>
        void SetToolTipText(String text);

        DragDrop.Package DragAndDrop_GetPackage(int x, int y);
        bool DragAndDrop_Draggable();
        void DragAndDrop_SetPackage(bool draggable, String name = "", object userData = null);
        bool DragAndDrop_ShouldStartDrag();
        void DragAndDrop_StartDragging(DragDrop.Package package, int x, int y);
        void DragAndDrop_EndDragging(bool success, int x, int y);
        bool DragAndDrop_HandleDrop(DragDrop.Package p, int x, int y);
        void DragAndDrop_HoverEnter(DragDrop.Package p, int x, int y);
        void DragAndDrop_HoverLeave(DragDrop.Package p);
        void DragAndDrop_Hover(DragDrop.Package p, int x, int y);
        bool DragAndDrop_CanAcceptPackage(DragDrop.Package p);

        /// <summary>
        /// Handler for Paste event.
        /// </summary>
        /// <param name="from">Source control.</param>
        void OnPaste(IControl from);

        /// <summary>
        /// Handler for Copy event.
        /// </summary>
        /// <param name="from">Source control.</param>
        void OnCopy(IControl from);

        /// <summary>
        /// Handler for Cut event.
        /// </summary>
        /// <param name="from">Source control.</param>
        void OnCut(IControl from);

        /// <summary>
        /// Handler for Select All event.
        /// </summary>
        /// <param name="from">Source control.</param>
        void OnSelectAll(IControl from);

        void InputCopy(IControl from);

        void InputPaste(IControl from);

        void InputCut(IControl from);

        void InputSelectAll(IControl from);

        /// <summary>
        /// Handler for character input event.
        /// </summary>
        /// <param name="chr">Character typed.</param>
        /// <returns>True if handled.</returns>
        bool OnChar(Char chr);

        bool InputChar(Char chr);

        #region Keyboard events

        /// <summary>
        /// Handler for keyboard events.
        /// </summary>
        /// <param name="key">Key pressed.</param>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>True if handled.</returns>
        bool OnKeyPressed(Keys key, bool down = true);

        /// <summary>
        /// Invokes key press event (used by input system).
        /// </summary>
        bool InputKeyPressed(Keys key, bool down = true);

        /// <summary>
        /// Handler for Tab keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>True if handled.</returns>
        bool OnKeyTab(bool down);

        /// <summary>
        /// Handler for Space keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>True if handled.</returns>
        bool OnKeySpace(bool down);

        /// <summary>
        /// Handler for Return keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>True if handled.</returns>
        bool OnKeyReturn(bool down);

        /// <summary>
        /// Handler for Backspace keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>True if handled.</returns>
        bool OnKeyBackspace(bool down);

        /// <summary>
        /// Handler for Delete keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>True if handled.</returns>
        bool OnKeyDelete(bool down);

        /// <summary>
        /// Handler for Right Arrow keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>True if handled.</returns>
        bool OnKeyRight(bool down);

        /// <summary>
        /// Handler for Left Arrow keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>True if handled.</returns>
        bool OnKeyLeft(bool down);

        /// <summary>
        /// Handler for Home keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>True if handled.</returns>
        bool OnKeyHome(bool down);

        /// <summary>
        /// Handler for End keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>True if handled.</returns>
        bool OnKeyEnd(bool down);

        /// <summary>
        /// Handler for Up Arrow keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>True if handled.</returns>
        bool OnKeyUp(bool down);

        /// <summary>
        /// Handler for Down Arrow keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>True if handled.</returns>
        bool OnKeyDown(bool down);

        /// <summary>
        /// Handler for Escape keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>True if handled.</returns>
        bool OnKeyEscape(bool down);

        #endregion

        #region Mouse events

        /// <summary>
        /// Handler invoked on mouse wheel event.
        /// </summary>
        /// <param name="delta">Scroll delta.</param>
        bool OnMouseWheeled(int delta);

        /// <summary>
        /// Invokes mouse wheeled event (used by input system).
        /// </summary>
        bool InputMouseWheeled(int delta);

        /// <summary>
        /// Handler invoked on mouse moved event.
        /// </summary>
        /// <param name="x">X coordinate.</param>
        /// <param name="y">Y coordinate.</param>
        /// <param name="dx">X change.</param>
        /// <param name="dy">Y change.</param>
        void OnMouseMoved(int x, int y, int dx, int dy);

        /// <summary>
        /// Invokes mouse moved event (used by input system).
        /// </summary>
        void InputMouseMoved(int x, int y, int dx, int dy);

        /// <summary>
        /// Handler invoked on mouse click (left) event.
        /// </summary>
        /// <param name="x">X coordinate.</param>
        /// <param name="y">Y coordinate.</param>
        /// <param name="down">If set to <c>true</c> mouse button is down.</param>
        void OnMouseClickedLeft(int x, int y, bool down);

        /// <summary>
        /// Invokes left mouse click event (used by input system).
        /// </summary>
        void InputMouseClickedLeft(int x, int y, bool down);

        /// <summary>
        /// Handler invoked on mouse click (right) event.
        /// </summary>
        /// <param name="x">X coordinate.</param>
        /// <param name="y">Y coordinate.</param>
        /// <param name="down">If set to <c>true</c> mouse button is down.</param>
        void OnMouseClickedRight(int x, int y, bool down);

        /// <summary>
        /// Invokes right mouse click event (used by input system).
        /// </summary>
        void InputMouseClickedRight(int x, int y, bool down);

        /// <summary>
        /// Handler invoked on mouse double click (left) event.
        /// </summary>
        /// <param name="x">X coordinate.</param>
        /// <param name="y">Y coordinate.</param>
        void OnMouseDoubleClickedLeft(int x, int y);

        /// <summary>
        /// Invokes left double mouse click event (used by input system).
        /// </summary>
        void InputMouseDoubleClickedLeft(int x, int y);

        /// <summary>
        /// Handler invoked on mouse double click (right) event.
        /// </summary>
        /// <param name="x">X coordinate.</param>
        /// <param name="y">Y coordinate.</param>
        void OnMouseDoubleClickedRight(int x, int y);

        /// <summary>
        /// Invokes right double mouse click event (used by input system).
        /// </summary>
        void InputMouseDoubleClickedRight(int x, int y);

        /// <summary>
        /// Handler invoked on mouse cursor entering control's bounds.
        /// </summary>
        void OnMouseEntered();

        /// <summary>
        /// Invokes mouse enter event (used by input system).
        /// </summary>
        void InputMouseEntered();

        /// <summary>
        /// Handler invoked on mouse cursor leaving control's bounds.
        /// </summary>
        void OnMouseLeft();

        /// <summary>
        /// Invokes mouse leave event (used by input system).
        /// </summary>
        void InputMouseLeft();

        /// <summary>
        /// Control has been clicked - invoked by input system. Windows use it to propagate activation.
        /// </summary>
        void Touch();

        void OnChildTouched(IControl control);

        #endregion

        #region Render events

        /// <summary>
        /// Renders the focus overlay.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        void RenderFocus(Skins.Skin skin);

        /// <summary>
        /// Renders under the actual control (shadows etc).
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        void RenderUnder(Skins.Skin skin);

        /// <summary>
        /// Renders over the actual control (overlays).
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        void RenderOver(Skins.Skin skin);

        #endregion

        void Anim_WidthIn(float length, float delay = 0.0f, float ease = 1.0f);
        void Anim_HeightIn(float length, float delay, float ease);
        void Anim_WidthOut(float length, bool hide, float delay, float ease);
        void Anim_HeightOut(float length, bool hide, float delay, float ease);
    }
}