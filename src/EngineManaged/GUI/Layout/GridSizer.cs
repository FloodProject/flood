using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Flood.GUI.Controls;
using Flood.GUI.Controls.Containers;

namespace Flood.GUI
{
    /// <summary>
    /// Grid type <see cref="Sizer"/>. Manages thew layout of of a <see cref="GridPanel"/>.
    /// </summary>
    public class GridSizer : Sizer
    {

        //todo: the grid sizer doesn't handle expand controls properly if they can't
        //todo: deal with the expansion flag internally or have minimal dimensions
        private class SzGrid
        {
            public List<SzRow> Rows = new List<SzRow>();
            public int TotalHeight { get; set; }
            public int TotalWidth { get; set; }
        }

        private class SzRow
        {
            public ExpansionFlags OverallExpansionType = ExpansionFlags.NotSet;
            public List<SzColumn> Columns = new List<SzColumn>();
            public int Width { get; set; }
            public int Height { get; set; }
        }

        private class SzColumn
        {
            public ExpansionFlags ExpansionType = ExpansionFlags.NotSet;
            public AlignmentFlags AlignmentType = AlignmentFlags.NotSet;
            public int X { get; set; }
            public int Y { get; set; }
            public int Width { get; set; }
            public int Height { get; set; }
            public int ControlWidth { get; set; }
            public int ControlHeight { get; set; }
            public Control Control { get; set; }
        }

        /// <summary>
        /// Gets or sets the associated <see cref="GridPanel"/>.
        /// </summary>
        public new GridPanel Panel
        {
            get { return (m_panel != null && m_panel.IsAlive) ? m_panel.Target as GridPanel : null; }
            set
            {
                m_panel = new WeakReference(value);
                Panel.Sizer = this;
            }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="GridSizer"/> class.
        /// </summary>
        /// <param name="panel">The panel.</param>
        public GridSizer(GridPanel panel) : base(panel)
        {
        }

        /// <summary>
        /// Main layout function, repositions and rescales panel's childen according to type of panel.
        /// </summary>
        public override void RecalcSizes()
        {

            var grid = GenGridLayout();
            Panel.Width = grid.TotalWidth;
            Panel.Height = grid.TotalHeight;
            var baseX = Panel.X;
            var colY = Panel.Y;

            foreach (var row in grid.Rows)
            {
                var rowX = baseX;
                foreach (var column in row.Columns)
                {
                    var ctrl = column.Control;
                    if (ctrl == null) continue;
                    var dim = column.ExpansionType == ExpansionFlags.Expand
                                  ? new Vector2i(column.Width, column.Height)
                                  : new Vector2i(column.ControlWidth, column.ControlHeight);
                    ctrl.SetDimension(new Vector2i(rowX + column.X, colY + column.Y), dim);
                    rowX += column.Width;
                }
                colY += row.Height;
            }
        }

        /// <summary>
        /// Gens the grid layout.
        /// </summary>
        /// <returns>Grid layout structure.</returns>
        private SzGrid GenGridLayout()
        {
            switch (Panel.Expansion)
            {
                case ExpansionFlags.Expand:
                    return GenGridLayoutExpand();
                case ExpansionFlags.Shaped:
                    return GenGridLayoutShaped();
                default:
                    return GenGridLayoutFit();
            }
        }

        /// <summary>
        /// Gens layout for a grid with expansion type fit.
        /// </summary>
        /// <returns></returns>
        private SzGrid GenGridLayoutFit()
        {
            SzGrid grid = new SzGrid();
            foreach (var row in Panel.Rows)
            {
                var szRow = new SzRow(){Width = 0, Height = 0};
                foreach (var column in row.Columns)
                {
                    var szColumn = new SzColumn()
                                       {
                                           Width = (column.Control != null) ? column.Control.Width + column.Control.Margin.Right + column.Control.Margin.Left : 0,
                                           Height = (column.Control != null) ? column.Control.Height + column.Control.Margin.Top + column.Control.Margin.Bottom : 0,
                                           ControlWidth = (column.Control != null) ? column.Control.Width + column.Control.Margin.Right + column.Control.Margin.Left : 0,
                                           ControlHeight = (column.Control != null) ? column.Control.Height + column.Control.Margin.Top + column.Control.Margin.Bottom : 0,
                                           ExpansionType = (column.Control != null) ? column.Control.Expansion : ExpansionFlags.NotSet,
                                           AlignmentType = (column.Control != null) ? column.Control.Alignment : AlignmentFlags.NotSet,
                                           Control = column.Control
                                       };
                    szRow.Width += szColumn.Width;
                    szRow.Height = Math.Max(szRow.Height, szColumn.Height);
                    szRow.Columns.Add(szColumn);
                }
                grid.TotalHeight += szRow.Height;
                grid.TotalWidth = Math.Max(szRow.Width, grid.TotalWidth);
                grid.Rows.Add(szRow);
            }

            FitCellsToAvailableSpace(grid);

            return grid;
        }

        /// <summary>
        /// Gens layout for a grid with expansion type expand.
        /// </summary>
        /// <returns></returns>
        /// <exception cref="NullParentException"></exception>
        private SzGrid GenGridLayoutExpand()
        {
            SzGrid grid = new SzGrid();

            if (Panel.Parent == null)
                throw new NullParentException();

            grid.TotalWidth = Panel.Parent.Width;
            grid.TotalHeight = Panel.Parent.Height;
            var overflowingRows = new List<SzRow>();
            var requiredHeight = GetRequiredGridDimensions(grid, ref overflowingRows);

            CorrectHeightOverflow(requiredHeight, grid);
            CorrectHeightUnderflow(requiredHeight, grid);
            CorrectWidthOverflow(overflowingRows, grid);

            FitCellsToAvailableSpace(grid);
            return grid;
        }

        /// <summary>
        /// Gens layout for a grid with expansion type shaped.
        /// </summary>
        /// <returns></returns>
        private SzGrid GenGridLayoutShaped()
        {
            SzGrid grid = new SzGrid();

            grid.TotalWidth = Panel.Width;
            grid.TotalHeight = Panel.Height;
            var overflowingRows = new List<SzRow>();
            var requiredHeight = GetRequiredGridDimensions(grid, ref overflowingRows);

            CorrectHeightOverflow(requiredHeight, grid);
            CorrectHeightUnderflow(requiredHeight, grid);
            CorrectWidthOverflow(overflowingRows, grid);

            FitCellsToAvailableSpace(grid);
            return grid;
        }

        /// <summary>
        /// Gets the dimensions required for the controls inside the grid.
        /// </summary>
        /// <param name="grid">The grid.</param>
        /// <param name="overflowingRows">Rows that overflow the grid width limit.</param>
        /// <returns></returns>
        private int GetRequiredGridDimensions(SzGrid grid, ref List<SzRow> overflowingRows)
        {
            var requiredHeight = 0;
            var requiredWidth = 0;
            foreach (var row in Panel.Rows)
            {
                var szRow = new SzRow() {Width = 0, Height = 0};
                var overallExpansionPossible = true;
                foreach (var column in row.Columns)
                {
                    var szColumn = new SzColumn()
                                       {
                                           Width = (column.Control != null) ? column.Control.Width + column.Control.Margin.Right + column.Control.Margin.Left : 0,
                                           Height = (column.Control != null) ? column.Control.Height + column.Control.Margin.Top + column.Control.Margin.Bottom : 0,
                                           ControlWidth = (column.Control != null) ? column.Control.Width + column.Control.Margin.Right + column.Control.Margin.Left : 0,
                                           ControlHeight = (column.Control != null) ? column.Control.Height + column.Control.Margin.Top + column.Control.Margin.Bottom : 0,
                                           ExpansionType =
                                               (column.Control != null)
                                                   ? column.Control.Expansion
                                                   : ExpansionFlags.NotSet,
                                           AlignmentType =
                                               (column.Control != null)
                                                   ? column.Control.Alignment
                                                   : AlignmentFlags.NotSet,
                                           Control = column.Control
                                       };
                    szRow.Width += szColumn.Width;
                    szRow.Height = Math.Max(szRow.Height, szColumn.Height);
                    szRow.Columns.Add(szColumn);
                    if (overallExpansionPossible)
                        overallExpansionPossible = GetOverallExpansionType(szColumn.ExpansionType,
                                                                           szRow.OverallExpansionType,
                                                                           out szRow.OverallExpansionType);
                }
                requiredHeight += szRow.Height;
                requiredWidth = Math.Max(szRow.Width, requiredWidth);
                if (requiredWidth > grid.TotalWidth)
                {
                    requiredWidth = grid.TotalWidth;
                    overflowingRows.Add(szRow);
                }
                grid.Rows.Add(szRow);
            }
            return requiredHeight;
        }

        /// <summary>
        /// Sets the row size to minimum possible.
        /// </summary>
        /// <param name="row">The row.</param>
        /// <returns>The resulting minimum size </returns>
        private Tuple<int, int> SetRowToMin(SzRow row)
        {
            row.Width = 0;
            row.Height = 0;
            foreach (var column in row.Columns)
            {
                column.X = 0;
                column.Y = 0;

                if(column.Control == null) continue;
                if(column.ExpansionType != ExpansionFlags.Shaped)
                {
                    var minSize = column.Control.GetMinSizeWithBorder();
                    column.Width = minSize.X;
                    column.Height = minSize.Y;
                    column.ControlWidth = minSize.X;
                    column.ControlHeight = minSize.Y;
                }

                row.Width += column.Width;
                row.Height = Math.Max(column.Height, row.Height);
            }
            return new Tuple<int, int>(row.Width, row.Height);
        }

        /// <summary>
        /// Sets the row  to the minimum height possible.
        /// </summary>
        /// <param name="row">The row.</param>
        /// <returns>The row with the minimum height possible</returns>
        private Tuple<int, int> SetRowToMinHeight(SzRow row)
        {
            row.Height = 0;
            foreach (var column in row.Columns)
            {
                column.Y = 0;

                if(column.Control == null) continue;
                if(column.ExpansionType != ExpansionFlags.Shaped)
                {
                    var minSize = column.Control.GetMinSizeWithBorder();
                    column.Height = minSize.Y;
                    column.ControlHeight = minSize.Y;
                }

                row.Height = Math.Max(column.Height, row.Height);
            }
            return new Tuple<int, int>(row.Width, row.Height);
        }

        /// <summary>
        /// Sets the row  to the minimum width possible.
        /// </summary>
        /// <param name="row">The row.</param>
        /// <returns>The row with the minimum width possible</returns>
        private Tuple<int, int> SetRowToMinWidth(SzRow row)
        {
            row.Width = 0;
            foreach (var column in row.Columns)
            {
                column.X = 0;

                if(column.Control == null) continue;
                if(column.ExpansionType != ExpansionFlags.Shaped)
                {
                    var minSize = column.Control.GetMinSizeWithBorder();
                    column.Width = minSize.X;
                    column.ControlWidth = minSize.X;
                }

                row.Width += column.Width;
            }
            return new Tuple<int, int>(row.Width, row.Height);
        }

        /// <summary>
        /// Fits the cells to available space.
        /// </summary>
        /// <param name="grid">The grid containing the cells.</param>
        private void FitCellsToAvailableSpace(SzGrid grid)
        {
            //adjust cell dimensions so that grid is even 
            foreach (var row in grid.Rows)
            {
                if (row.Width < grid.TotalWidth)
                    row.Width = grid.TotalWidth;
                var freeWidth = row.Width;
                var expandColumns = 0;
                var expandSize = 0;

                foreach (var column in row.Columns)
                {
                    if (0 != (column.ExpansionType & (ExpansionFlags.Shaped | ExpansionFlags.Fit | ExpansionFlags.NotSet)))
                    {
                        freeWidth -= column.Width;
                        if (0 !=
                            (column.AlignmentType &
                             (AlignmentFlags.Top | AlignmentFlags.CenterHorizontal | AlignmentFlags.NotSet)))
                            column.Y = 0;
                        else if (0 != (column.AlignmentType & AlignmentFlags.Bottom))
                            column.Y = row.Height - column.Height;
                        else
                            column.Y = (row.Height - column.Height) / 2;
                    }
                    if (column.ExpansionType == ExpansionFlags.Expand)
                    {
                        expandColumns++;
                        expandSize += column.Width;
                    }
                }
                if (expandColumns > 0) // allocate all free space to expand controls
                {
                    var width = (freeWidth - expandSize) / expandColumns;
                    foreach (var column in row.Columns)
                    {
                        if (column.ExpansionType == ExpansionFlags.Expand)
                        {
                            column.Width += Math.Min(width, freeWidth);
                            freeWidth -= Math.Min(width, freeWidth);
                        }
                        column.Height = row.Height;
                    }
                }
                else //allocate free space to shaped and fit controls and position them
                {
                    var width = freeWidth / row.Columns.Count;
                    foreach (var column in row.Columns)
                    {
                        var extraWidth = Math.Min(width, freeWidth);
                        if (0 !=
                            (column.AlignmentType &
                             (AlignmentFlags.Left | AlignmentFlags.CenterVertical | AlignmentFlags.NotSet)))
                            column.X = 0;
                        else if (0 != (column.AlignmentType & AlignmentFlags.Right))
                            column.X = extraWidth;
                        else
                            column.X = extraWidth / 2;

                        column.Width += extraWidth;
                        freeWidth -= extraWidth;
                        column.Height = row.Height;
                    }
                }
            }
        }

        /// <summary>
        /// Corrects the grid's width overflow if it exists.
        /// </summary>
        /// <param name="overflowingRows">The overflowing rows.</param>
        /// <param name="grid">The grid.</param>
        private void CorrectWidthOverflow(List<SzRow> overflowingRows, SzGrid grid)
        {
            if (overflowingRows.Count > 0)
            {
                foreach (var row in overflowingRows)
                {
                    if (row.Width <= grid.TotalWidth) continue;
                    SetRowToMinWidth(row);
                    if (row.Width <= grid.TotalWidth) continue;
                    double overflow = row.Width - grid.TotalWidth;
                    var expandColumns = row.Columns.Count(column => column.ExpansionType == ExpansionFlags.Expand);
                    foreach (var column in row.Columns)
                    {
                        if (column.ExpansionType != ExpansionFlags.Expand) continue;
                        var shave = Math.Min((int)Math.Ceiling(overflow / expandColumns), column.Width);
                        overflow -= shave;
                        expandColumns--;
                        column.Width -= shave;
                        row.Width -= shave;
                    }
                    if (row.Width <= grid.TotalWidth) continue;
                    for (int i = row.Columns.Count - 1; i >= 0; i--)
                    {
                        if (row.Width <= grid.TotalWidth) break;
                        var column = row.Columns[i];
                        if (row.Width - column.Width > grid.TotalWidth)
                        {
                            row.Width -= column.Width;
                            row.Columns[i].Control.Hide();
                            row.Columns[i].Control.Disable();
                            row.Columns.RemoveAt(i);
                            continue;
                        }
                        if (row.Width > grid.TotalWidth)
                        {
                            var shave = row.Width - grid.TotalWidth;
                            Debug.Assert(shave < column.Width);
                            column.Width -= shave;
                            row.Width -= shave;
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Corrects the grid's height overflow if it exists.
        /// </summary>
        /// <param name="requiredHeight">Height required.</param>
        /// <param name="grid">The grid.</param>
        private void CorrectHeightOverflow(int requiredHeight, SzGrid grid)
        {
            if (requiredHeight > grid.TotalHeight)
            {
                foreach (var row in grid.Rows)
                {
                    var oldSize = new Tuple<int, int>(row.Width, row.Height);
                    var newSize = SetRowToMinHeight(row);
                    requiredHeight -= oldSize.Item2 - newSize.Item2;
                }
                if (requiredHeight > grid.TotalHeight)
                {
                    double overflow = requiredHeight - grid.TotalHeight;
                    var expandRows = grid.Rows.Count(row => row.OverallExpansionType == ExpansionFlags.Expand);
                    foreach (var row in grid.Rows.Where(row => row.OverallExpansionType == ExpansionFlags.Expand))
                    {
                        var shave = Math.Min((int)Math.Ceiling(overflow / expandRows), row.Height);
                        overflow -= shave;
                        expandRows--;
                        row.Height -= shave;
                        requiredHeight -= shave;
                        foreach (var column in row.Columns)
                            column.Height = row.Height;
                    }
                }
                if (requiredHeight > grid.TotalHeight)
                {
                    for (int i = grid.Rows.Count - 1; i >= 0; i--)
                    {
                        if (requiredHeight < grid.TotalHeight) break;
                        var row = grid.Rows[i];
                        if (requiredHeight - row.Height > grid.TotalHeight)
                        {
                            requiredHeight -= row.Height;
                            foreach (var column in row.Columns)
                            {
                                column.Control.Hide();
                                column.Control.Disable();
                            }
                            grid.Rows.RemoveAt(i);
                            continue;
                        }
                        if (requiredHeight > grid.TotalHeight)
                        {
                            var shave = requiredHeight - grid.TotalHeight;
                            Debug.Assert(shave < row.Height);
                            row.Height -= shave;
                            requiredHeight -= shave;
                            foreach (var column in row.Columns)
                                column.Height = row.Height;
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Corrects the grid's height underflow if it exists.
        /// </summary>
        /// <param name="requiredHeight">Height required.</param>
        /// <param name="grid">The grid.</param>
        private void CorrectHeightUnderflow(int requiredHeight, SzGrid grid)
        {
            if (requiredHeight < grid.TotalHeight)
            {
                var freeRowHeight = (grid.TotalHeight - requiredHeight)/grid.Rows.Count;
                foreach (var row in grid.Rows)
                    row.Height += freeRowHeight;
                grid.Rows.Last().Height += (grid.TotalHeight - requiredHeight) - freeRowHeight*grid.Rows.Count;
            }
        }

        /// <summary>
        /// Gets the overall expansion type of a row if it exists.
        /// </summary>
        /// <param name="expansionType">Type of the current column's expansion.</param>
        /// <param name="previousExpansionType">Type of the overall expansion found so far.</param>
        /// <param name="overallExpansionType">Type of the overall expansion to return.</param>
        /// <returns>Whether overall expansion type is possible.</returns>
        private bool GetOverallExpansionType(ExpansionFlags expansionType, ExpansionFlags previousExpansionType, out ExpansionFlags overallExpansionType)
        {
            if (previousExpansionType == ExpansionFlags.NotSet)
            {
                overallExpansionType = expansionType;
                return true;
            }
            if (expansionType != ExpansionFlags.NotSet && expansionType != previousExpansionType)
            {
                overallExpansionType = ExpansionFlags.NotSet;
                return false;
            }
            overallExpansionType = previousExpansionType;
            return true;
        }

        private ExpansionFlags DetermineExpansion(ExpansionFlags newExpansion, ExpansionFlags oldExpansion)
        {
            if (oldExpansion == ExpansionFlags.NotSet)
                return newExpansion;
            if (newExpansion == ExpansionFlags.Shaped)
                return newExpansion;
            if (newExpansion == ExpansionFlags.Expand && oldExpansion != ExpansionFlags.Shaped)
                return newExpansion;

            return oldExpansion;
        }


        /// <summary>
        /// Calculates the minimum size of the associated panel.
        /// </summary>
        /// <returns>
        /// The calculated min size.
        /// </returns>
        public override Vector2i CalcMin()
        {
            foreach (var control in Panel.Children)
                control.ReduceToMinSize();
            RecalcSizes();
            return new Vector2i(Panel.Width, Panel.Height);

        }

        /// <summary>
        /// Informs this sizer regarding the size available in the minor direction 
        /// of the containing panel so it can redimension the associated panel.
        /// </summary>
        /// <param name="direction">The minor direction.</param>
        /// <param name="size">The size available in the minor panel direction.</param>
        /// <param name="availableOtherDir">The available size in the major panel dir.</param>
        /// <returns>
        /// Whether any size reduction was achieved.
        /// </returns>
        public override bool InformFirstDirection(BoxOrientation direction, int size, int availableOtherDir)
        {
            var PrevMin = CalcMin();
            foreach (var control in Children)
                control.InformFirstDirection(direction, size, availableOtherDir);
            var Min = CalcMin();
            return Min < PrevMin;
        }
    }
}
