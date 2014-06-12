using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;

/*
 * Ported from wxWidgets BoxSizerOld.
 */
namespace Flood.GUIv2.Panels.Layout
{
    /// <summary>
    /// Whether box is orineted horizontally or vertically.
    /// </summary>
    [Flags]
    public enum Orientation
    {
        Vertical = 1,
        Horizontal = 1 << 1,
        Both = Vertical | Horizontal
    }

    /// <summary>
    /// Box type <see cref="Sizer"/>. Manages thew layout of of a <see cref="BoxPanel"/>.
    /// </summary>
    public class BoxSizerOld : Sizer
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="BoxSizerOld"/> class.
        /// </summary>
        /// <param name="panel">The panel.</param>
        public BoxSizerOld(BoxPanelOld panel)
            : base(panel)
        {
            TotalProportion = 0;
            Debug.Assert(Orientation == Orientation.Vertical 
                        || Orientation == Orientation.Horizontal
                        , "Invalid value for BoxSizerOld orientation");
        }

        /// <summary>
        /// Determines whether the sizer's alignment is vertical.
        /// </summary>
        public bool IsVertical()
        {
            return Orientation == Layout.Orientation.Vertical;
        }

        ///// <summary>
        ///// Calculates the minimum size of the associated panel.
        ///// </summary>
        ///// <returns>
        ///// The calculated min size.
        ///// </returns>
        //public override Vector2i CalcMin()
        //{
        //    TotalProportion = 0;
        //    MinSize = new Vector2i(0,0);

        //    // The minimal size for the sizer should be big enough to allocate its
        //    // element at least its minimal size but also, and this is the non trivial
        //    // part, to respect the children proportion. To satisfy the latter
        //    // condition we must find the greatest min-size-to-proportion ratio for all
        //    // elements with non-zero proportion.
        //    float maxMinSizeToProp = 0;
        //    Vector2i minSize = MinSize;

        //    foreach (var ctrl in Children)
        //    {
        //        if (ctrl.IsHidden)
        //            continue;

        //        Vector2i sizeMinThis = ctrl.GetMinSizeWithBorder();
        //        int propThis = ctrl.Proportion;
        //        if (propThis > 0)
        //        {
        //            float minSizeToProp = GetVectorInMajorDir(sizeMinThis);
        //            minSizeToProp /= propThis;

        //            if (minSizeToProp > maxMinSizeToProp)
        //                maxMinSizeToProp = minSizeToProp;
        //            TotalProportion += propThis;
        //        }
        //        else // fixed size item
        //            // Just account for its size directly
        //            AddVectorInMajorDir(ref minSize, GetVectorInMajorDir(sizeMinThis));

        //        // In the transversal direction we just need to find the maximum.
        //        if (GetVectorInMinorDir(sizeMinThis) > GetVectorInMinorDir(MinSize))
        //            SetVectorInMinorDir(ref minSize, GetVectorInMinorDir(sizeMinThis));
        //    }

        //    // Using the max ratio ensures that the min size is big enough for all
        //    // items to have their min size and satisfy the proportions among them.
        //    AddVectorInMajorDir(ref minSize, (int)(maxMinSizeToProp*TotalProportion));
        //    MinSize = minSize;
        //    return MinSize;
        //}

        /// <summary>
        /// Informs this sizer regarding the size available in the minor direction 
        /// of the containing box panel so it can redimension the associated box panel.
        /// </summary>
        /// <param name="direction">The minor direction.</param>
        /// <param name="size">The size available in the minor panel direction.</param>
        /// <param name="availableOtherDir">The available size in the major panel dir.</param>
        /// <returns>Whether any size reduction was achieved.</returns>
        public bool InformFirstDirection(Orientation direction, int size, int availableOtherDir)
        {
            throw new NotImplementedException();
            //var PrevMin = CalcMin();
            //foreach (var control in Children)
            //    control.InformFirstDirection(direction, size, availableOtherDir);
            //var Min = CalcMin();
            //return Min < PrevMin;
        }

        /// <summary>
        /// Main layout function, repositions and 
        /// rescales panel's childen in sequential order, either horizontally or vertically.
        /// </summary>
        public override void RecalcSizes()
        {
            throw new NotImplementedException();
            //if(Children.Count == 0)
            //    return;

            //int totalMinorSize = GetVectorInMinorDir(Size);
            //int totalMajorSize = GetVectorInMajorDir(Size);

            //// the amount of free space which we should redistribute among the
            //// stretchable items (i.e. those with non zero proportion)
            //int delta = totalMajorSize - GetVectorInMajorDir(MinSize);
            //Vector2i minSize = MinSize;

            //// First, inform each control about the available size in minor direction as this
            //// can change their size in the major direction. Also compute the number of
            //// visible/renderable items and sum of their min sizes in major direction.
            //int minMajorSize = 0;
            //foreach(var ctrl in Children)
            //{
            //    if (ctrl.IsHidden)
            //        continue;

            //    Vector2i PrevMinSize = ctrl.GetMinSizeWithBorder();
            //    ctrl.InformFirstDirection(Orientation ^ Layout.Orientation.Both, totalMinorSize, delta);
            //    Vector2i CurMinSize = ctrl.GetMinSizeWithBorder();
            //    int deltaChange = GetVectorInMajorDir(CurMinSize - PrevMinSize);

            //    // Since we passed available space along to the item, it should not
            //    // take too much, so delta should not become negative.
            //    if (deltaChange != 0)
            //        delta -= deltaChange;

            //    minMajorSize += GetVectorInMajorDir(ctrl.GetMinSizeWithBorder());
            //}

            //// update our min size have changed
            //SetVectorInMajorDir(ref minSize, minMajorSize);
            //MinSize = minSize;

            //// space and sum of proportions for the remaining items, both may change
            //// below
            //int remaining = totalMajorSize;
            //int totalProportion = TotalProportion;

            //// size of the (visible) items in major direction, -1 means "not fixed yet"
            //List<int> majorSizes = Enumerable.Repeat(DefaultSize, Children.Count).ToList();

            //// Check for the degenerated case when we don't have enough space for even
            //// the min sizes of all the items: in this case we really can't do much
            //// more than to allocate the min size to as many of fixed size items as
            //// possible (on the assumption that variable size items such as text zones
            //// or list boxes may use scrollbars to show their content even if their
            //// size is less than min size but that fixed size items such as buttons
            //// will suffer even more if we don't give them their min size)
            //if(totalMajorSize < minMajorSize)
            //{

            //    // Second degenerated case pass: allocate min size to all fixed size
            //    // items.
            //    int n = -1;
            //    foreach (var ctrl in Children)
            //    {
            //        n++;
            //        if (ctrl.IsHidden)
            //            continue;

            //        // deal with fixed size items only during this pass
            //        if (ctrl.Proportion != 0)
            //            continue;

            //        majorSizes[n] = GetMinOrRemainingSize(ctrl, ref remaining);
            //    }

            //    // Third degenerated case pass: allocate min size to all the remaining,
            //    // i.e. non-fixed size, items.
            //    n = -1;
            //    foreach (var ctrl in Children)
            //    {
            //        n++;
            //        if (ctrl.IsHidden)
            //            continue;

            //        // we've already dealt with fixed size items above
            //        if (ctrl.Proportion == 0)
            //            continue;

            //        majorSizes[n] = GetMinOrRemainingSize(ctrl, ref remaining);
            //    }
            //}
            //else // we do have enough space to give at least min sizes to all items
            //{
            //    // Second and maybe more passes in the non-degenerated case: deal with
            //    // fixed size items and items whose min size is greater than what we
            //    // would allocate to them taking their proportion into account. For
            //    // both of them, we will just use their min size, but for the latter we
            //    // also need to reexamine all the items as the items which fitted
            //    // before we adjusted their size upwards might not fit any more. This
            //    // does make for a quadratic algorithm but it's not obvious how to
            //    // avoid it and hopefully it's not a huge problem in practice as the
            //    // sizers don't have many items usually (and, of course, the algorithm
            //    // still reduces into a linear one if there is enough space for all the
            //    // min sizes).
            //    bool nonFixedSpaceChanged = false;
            //    for(int n = 0 ;; ++n)
            //    {
            //        if (nonFixedSpaceChanged)
            //        {
            //            n = 0;
            //            nonFixedSpaceChanged = false;
            //        }

            //        // check for the end of the loop only after the check above as
            //        // otherwise we wouldn't do another pass if the last child resulted
            //        // in non fixed space reduction
            //        if(n >= Children.Count )
            //            break;

            //        var ctrl = Children[n];
            //        if (ctrl.IsHidden)
            //            continue;

            //        // don't check the item which we had already dealt with during a
            //        // previous pass (this is more than an optimization, the code
            //        // wouldn't work correctly if we kept adjusting for the same item
            //        // over and over again)
            //        if (majorSizes[n] != DefaultSize)
            //            continue;

            //        int minMajor = GetVectorInMajorDir(ctrl.GetMinSizeWithBorder());
                    
            //        if (minMajor < 0)
            //            minMajor = 0;

            //        int propCtrl = ctrl.Proportion;
            //        if(propCtrl != 0)
            //        {

            //            // is the desired size of this item big enough?
            //            if ((remaining * propCtrl) / totalProportion >= minMajor)
            //                // yes, it is, we'll determine the real size of this
            //                // item later, for now just leave it as wxDefaultCoord
            //                continue;

            //            // the proportion of this item won't count, it has
            //            // effectively become fixed
            //            totalProportion -= propCtrl;
            //        }

            //        // we can already allocate space for this item
            //        majorSizes[n] = minMajor;

            //        // change the amount of the space remaining to the other items,
            //        // as this can result in not being able to satisfy their
            //        // proportions any more we will need to redo another loop
            //        // iteration
            //        remaining -= minMajor;
            //        nonFixedSpaceChanged = true;
            //    }

            //    // Similar to the previous loop, but dealing with items whose max size
            //    // is less than what we would allocate to them taking their proportion
            //    // into account.
            //    for(int n = 0 ;; ++n)
            //    {
            //        if (nonFixedSpaceChanged)
            //        {
            //            n = 0;
            //            nonFixedSpaceChanged = false;
            //        }

            //        // check for the end of the loop only after the check above as
            //        // otherwise we wouldn't do another pass if the last child resulted
            //        // in non fixed space reduction
            //        if(n >= Children.Count )
            //            break;

            //        var ctrl = Children[n];
            //        if (ctrl.IsHidden)
            //            continue;

            //        // don't check the item which we had already dealt with during a
            //        // previous pass (this is more than an optimization, the code
            //        // wouldn't work correctly if we kept adjusting for the same item
            //        // over and over again)
            //        if (majorSizes[n] != DefaultSize)
            //            continue;

            //        int maxMajor = GetVectorInMajorDir(ctrl.GetMaxSizeWithBorder());

            //        // must be nonzero, fixed-size items were dealt with in previous loop
            //        int propCtrl = ctrl.Proportion;

            //        // is the desired size of this item small enough?
            //        if (maxMajor < 0 || (remaining * propCtrl) / totalProportion >= maxMajor)
            //            // yes, it is, we'll determine the real size of this
            //            // item later, for now just leave it as wxDefaultCoord
            //            continue;

            //        // the proportion of this item won't count, it has
            //        // effectively become fixed
            //        totalProportion -= propCtrl;

            //        // we can already allocate space for this item
            //        majorSizes[n] = maxMajor;

            //        // change the amount of the space remaining to the other items,
            //        // as this can result in not being able to satisfy their
            //        // proportions any more we will need to redo another loop
            //        // iteration
            //        remaining -= maxMajor;
            //        nonFixedSpaceChanged = true;

            //    }

            //    // Last by one pass: distribute the remaining space among the non-fixed
            //    // items whose size weren't fixed yet according to their proportions.
            //    for(int n = 0; n < Children.Count; ++n)
            //    {
            //        var ctrl = Children[n];
            //        if (ctrl.IsHidden)
            //            continue;
            //        if (majorSizes[n] == DefaultSize)
            //        {
            //            int propCtrl = ctrl.Proportion;
            //            majorSizes[n] = (remaining*propCtrl)/totalProportion;
            //            remaining -= majorSizes[n];
            //            totalProportion -= propCtrl;
            //        }
            //    }
            //}

            //// the position at which we put the next child
            //////Vector2i pt = new Vector2i(Position.X, Position.Y);
            //Vector2i pt = new Vector2i(0, 0);

            //// Final pass: finally do position the items correctly using their sizes as
            //// determined above.
            //for(int n = 0; n < Children.Count; ++n)
            //{
            //    var ctrl = Children[n];
            //    if (ctrl.IsHidden)
            //        continue;
                
            //    int majorSize = majorSizes[n];
            //    Vector2i sizeThis = ctrl.GetMinSizeWithBorder();

            //    // apply the alignment in the minor direction
            //    Vector2i posChild = new Vector2i(pt.X, pt.Y);
                
            //    var majorMargin = IsVertical() ? ctrl.Margin.Top : ctrl.Margin.Left;
            //    var minorMargin = IsVertical() ? ctrl.Margin.Left : ctrl.Margin.Top;

            //    int minorSize = GetVectorInMinorDir(sizeThis);
            //    if(((ctrl.Expansion & (ExpansionFlags.Expand | ExpansionFlags.Shaped)) != 0) 
            //        || minorSize > totalMinorSize)
            //    {
            //        // do not allow the size in the minor direction to grow beyond the max
            //        // size of the item in the minor direction
            //        int maxMinorSize = GetVectorInMinorDir(ctrl.GetMaxSizeWithBorder());
            //        minorSize = (maxMinorSize >= 0 && totalMinorSize > maxMinorSize)?
            //            maxMinorSize : totalMinorSize;
            //    }

            //    if((ctrl.Alignment & (IsVertical()? AlignmentFlags.Right : AlignmentFlags.Bottom)) != 0)
            //    {
            //        AddVectorInMinorDir(ref posChild, totalMinorSize - minorSize + minorMargin);
            //    }
            //    else if((ctrl.Alignment & (AlignmentFlags.Center | (IsVertical()?
            //        AlignmentFlags.CenterHorizontal : AlignmentFlags.CenterVertical))) != 0)
            //    {
            //        AddVectorInMinorDir(ref posChild, Math.Max((totalMinorSize - minorSize)/2, minorMargin));
            //    }

            //    AddVectorInMajorDir(ref posChild, majorMargin);

            //    // finally set size of this child and advance to the next one
            //    ctrl.SetDimension(posChild, SizeFromMajorMinor(majorSize, minorSize, ctrl));
            //    AddVectorInMajorDir(ref pt, majorSize);
            //}
        }

        /// <summary>
        /// Gets or sets whether the orientation is vertical or horizontal.
        /// </summary>
        public Orientation Orientation
        {
            get
            {
                if (Panel != null)
                    return ((BoxPanelOld)Panel).Orientation;
                return Orientation.Both;
            }
        }

        /// <summary>
        /// Gets or sets the minimum size.
        /// </summary>
        /// <remarks> 
        /// The minimal size needed for this sizer as calculated by the last call to
        /// CalcMin().
        /// </remarks>
        protected Vector2i MinSize { get; set; }

        /// <summary>
        /// Gets the sum of proportion of all child elements.
        /// </summary>
        /// <remarks>
        /// CalcMin() needs to be called first to set the value.
        /// </remarks>
        protected int TotalProportion { get; set; }

        /// <summary>
        /// Gets the component of the vector in the direction set as
        /// the current <see cref="Orientation"/>.
        /// </summary>
        /// <remarks>
        /// The direction matching the <see cref="Orientation"/> is referred to as the major direction.
        /// The other one as the minor direction.
        /// </remarks>
        /// <param name="vector">Vector to get major dir component from.</param>
        /// <returns>Major component of vector.</returns>
        private int GetVectorInMajorDir(Vector2i vector)
        {
            return (Orientation == Orientation.Horizontal)? vector.X : vector.Y;
        }

        /// <summary>
        /// Gets the component of the vector in the direction perpendicular to
        /// the current <see cref="Orientation"/>.
        /// </summary>
        /// <remarks>
        /// The direction matching the <see cref="Orientation"/> is referred to as the major direction.
        /// The other one as the minor direction.
        /// </remarks>
        /// <param name="vector">Vector to get minor dir component from.</param>
        /// <returns>Minor component of vector.</returns>
        private int GetVectorInMinorDir(Vector2i vector)
        {
            return (Orientation == Orientation.Horizontal)? vector.Y : vector.X;
        }

        /// <summary>
        /// Sets the component of the vector in the direction set as
        /// the current <see cref="Orientation"/>.
        /// </summary>
        /// <remarks>
        /// The direction matching the <see cref="Orientation"/> is referred to as the major direction.
        /// The other one as the minor direction.
        /// </remarks>
        /// <param name="vector">Vector to set major dir component.</param>
        /// <param name="value">Value to assign.</param>
        private void SetVectorInMajorDir(ref Vector2i vector, int value)
        {
            if(Orientation == Orientation.Horizontal)
                vector.X = value;
            else 
                vector.Y = value;
        }

        /// <summary>
        /// Sets the component of the vector in the direction perpendicular to
        /// the current <see cref="Orientation"/>.
        /// </summary>
        /// <remarks>
        /// The direction matching the <see cref="Orientation"/> is referred to as the major direction.
        /// The other one as the minor direction.
        /// </remarks>
        /// <param name="vector">Vector to set minor dir component.</param>
        /// <param name="value">Value to assign.</param>
        private void SetVectorInMinorDir(ref Vector2i vector, int value)
        {
            if (Orientation == Orientation.Horizontal)
                vector.Y = value;
            else
                vector.X = value;
        }

        /// <summary>
        /// Increments the vector in in the direction set as
        /// the current <see cref="Orientation"/>.
        /// </summary>
        /// <remarks>
        /// The direction matching the <see cref="Orientation"/> is referred to as the major direction.
        /// The other one as the minor direction.
        /// </remarks>
        /// <param name="vector">The vector to increment in the major dir.</param>
        /// <param name="value">The value to increment by.</param>
        private void AddVectorInMajorDir(ref Vector2i vector, int value)
        {
            if(Orientation == Orientation.Horizontal)
                vector.X += value;
            else 
                vector.Y += value;
        }


        /// <summary>
        /// Increments the vector in in the direction perpendicular to
        /// the current <see cref="Orientation"/>.
        /// </summary>
        /// <remarks>
        /// The direction matching the <see cref="Orientation"/> is referred to as the major direction.
        /// The other one as the minor direction.
        /// </remarks>
        /// <param name="vector">The vector to increment in the minor dir.</param>
        /// <param name="value">The value to increment by.</param>
        private void AddVectorInMinorDir(ref Vector2i vector, int value)
        {
            if (Orientation == Orientation.Horizontal)
                vector.Y += value;
            else
                vector.X += value;
        }

        /// <summary>
        /// Returns the minimum size the control will take in the major direction
        /// or the remaining available space if it exceeds it.
        /// </summary>
        /// <param name="control">The control to get the minimum size from.</param>
        /// <param name="remainingSpace">The remaining space.</param>
        /// <returns>Size to be assigned to the control.</returns>
        private int GetMinOrRemainingSize(IControl control, ref int remainingSpace)
        {
            int size = 0;
            if(remainingSpace <= 0)
                return size;

            Vector2i sizeMin = control.GetMinSizeWithBorder();
            size = (Orientation == Orientation.Horizontal) ? sizeMin.X : sizeMin.Y;
            
            if (size >= remainingSpace)
                size = remainingSpace;
            
            remainingSpace -= size;

            return size;
        }

        /// <summary>
        /// Convert major, minor values to x, y vector.
        /// </summary>
        /// <param name="major">The value in the major direction.</param>
        /// <param name="minor">The value in the minor direction.</param>
        /// <returns>A size vector according to major, minor values.</returns>
        private Vector2i SizeFromMajorMinor(int major, int minor, IControl control)
        {
            if ( Orientation == Orientation.Horizontal )
            {
                return new Vector2i(major - control.HorizontalMargins, minor - control.VerticalMargins);
            }
            else // Orientation.Vertical
            {
                return new Vector2i(minor - control.HorizontalMargins, major - control.VerticalMargins);
            }
    }

        /// <summary>
        /// The default size when none has been assigned.
        /// </summary>
        private const int DefaultSize = -1;

    }
}
