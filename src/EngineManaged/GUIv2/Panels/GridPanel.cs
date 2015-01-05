using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.Panels
{
    public class GridPanel: Panel
    {
        public class GridControlData
        {
            private GridPanel Panel;
            public GridControlData(GridPanel panel, IControl control)
            {
                Control = control;
                Panel = panel;
            }

            public GridControlData(GridPanel panel, IControl control, Vector2i position, Vector2i span) 
                : this(panel, control, position.X, position.Y, span.X, span.Y)
            {
            }

            public GridControlData(GridPanel panel, IControl control, int row, int column, int rowSpan, int columnSpan)
                : this(panel, control)
            {
                Row = row;
                Column = column;
                RowSpan = rowSpan;
                ColumnSpan = columnSpan;
            }

            public readonly IControl Control; 
            public int Row { get; set; }
            public int Column { get; set; }
            public int RowSpan { get; set; }
            public int EffectiveRowSpan 
            { 
                get
                {
                    return  ( Row + RowSpan  > Panel.Rows  ) ? Panel.Rows  - Row  : RowSpan;
                }
            }
            public int ColumnSpan { get; set; }
            public int EffectiveColumnSpan
            {
                get
                {
                    return (Column + ColumnSpan > Panel.Columns) ? Panel.Columns - Column : ColumnSpan;
                }
            }
            public bool IsValid()
            {
                return Row >= 0 && Row < Panel.Rows && Column >= 0 && Column < Panel.Columns;
            }
        }

        internal Dictionary<IControl, GridControlData> ControlData { get; private set; }

        public int Rows { get; private set; }
        public int Columns { get; private set; }
        
        public GridPanel(ExpansionFlags expansion = ExpansionFlags.Fit) 
            : base(expansion)
        {
            ControlData = new Dictionary<IControl, GridControlData>();
            new GridSizer(this);

        }

        public void SetRows(int nRows)
        {
            if(nRows < 0)
                return;
            Rows = nRows;
        }

        public void SetColumns(int nColumns)
        {
            if (nColumns < 0)
                return;
            Columns = nColumns;
        }

        public override void AddChild(IControl child)
        {
            AddChild(child, new Vector2i(0,0));
        }

        public void AddChild(IControl child, Vector2i position, Vector2i span)
        {
            if (position.X < 0 || position.Y < 0)
                throw new ArgumentException("Position cannot be negative.");
            if (span.X < 1 || span.Y < 1)
                throw new ArgumentException("Span must at least be size 1.");
            base.AddChild(child);
            ControlData[child] = new GridControlData(this, child, position, span);
        }

        public void AddChild(IControl child, Vector2i position)
        {
                AddChild(child, position, new Vector2i(1,1));
        }

        public void AddChild(IControl child, int row, int column, int rowSpan = 1, int columnSpan = 1)
        {
                AddChild(child, new Vector2i(row, column), new Vector2i(rowSpan,columnSpan));
        }


        public void SetPosition(IControl child, int row, int column)
        {
            if (!ControlData.ContainsKey(child))
                return;
            ControlData[child].Row = row;
            ControlData[child].Column = column;
        }

        public void SetPosition(IControl child, Vector2i position)
        {
            SetPosition(child, position.X, position.Y);
        }

        public void SetSpan(IControl child, int rowSpan, int columnSpan)
        {
            if (!ControlData.ContainsKey(child))
                return;
            ControlData[child].RowSpan = rowSpan;
            ControlData[child].ColumnSpan = columnSpan;
        }

        public void SetSpan(IControl child, Vector2i span)
        {
            SetSpan(child, span.X, span.Y);
        }

        public bool TryGetRow(IControl child, out int row)
        {
            row = 0;
            if (!ControlData.ContainsKey(child))
                return false;
            row = ControlData[child].Row;
            return true;
        }

        public bool TryGetColumn(IControl child, out int column)
        {
            column = 0;
            if (!ControlData.ContainsKey(child))
                return false;
            column = ControlData[child].Column;
            return true;
        }
        public bool TryGetRowSpan(IControl child, out int rowSpan)
        {
            rowSpan = 0;
            if (!ControlData.ContainsKey(child))
                return false;
            rowSpan = ControlData[child].RowSpan;
            return true;
        }

        public bool TryGetColumnSpan(IControl child, out int columnSpan)
        {
            columnSpan = 0;
            if (!ControlData.ContainsKey(child))
                return false;
            columnSpan = ControlData[child].ColumnSpan;
            return true;
        }
        
        public List<int> GetRows(IControl child)
        {
            if (!ControlData.ContainsKey(child))
                return new List<int>();
            return Enumerable.Range(ControlData[child].Row, ControlData[child].Row + ControlData[child].RowSpan).ToList();
        }

        public List<int> GetColumns(IControl child)
        {
            if (!ControlData.ContainsKey(child))
                return new List<int>();
            return Enumerable.Range(ControlData[child].Column, ControlData[child].Column + ControlData[child].ColumnSpan).ToList();
        }

        public override void RemoveChild(IControl child, bool dispose)
        {
            if(!ControlData.ContainsKey(child)) return;
            ControlData.Remove(child);
            base.RemoveChild(child, dispose);
        }

        private int ClampAdd(int current, int @new)
        {
            if (current == MaxNotSet || @new == MaxNotSet)
                return MaxNotSet;
            return current + @new;
        }

        internal class SizerRow
        {
            internal SizerRow PreviousRow { get; private set; }

            internal int Index { get; private set; }
            internal int Max { get; set; }
            #region min, best, max

            internal SortedSet<int> PreferredYs = new SortedSet<int>();

            internal int MinHeight
            {
                get { return (PreviousRow == null) ? MinY: MinY - PreviousRow.MinY; }
            }

            internal int MinY { get; set; }

            internal int BestHeight
            {
                get { return (PreviousRow == null) ? BestY: BestY - PreviousRow.BestY; }
            }

            internal int BestY { get; set; }

            internal int MaxHeight
            {
                get { return (PreviousRow == null) ? MaxY: MaxY - PreviousRow.MaxY; }
            }

            internal int MaxY { get; set; }

            #endregion

            internal bool ExpandVertical { get; set; }

            internal int Height
            {
                get { return (PreviousRow == null) ? Y : Y - PreviousRow.Y; }
            }

            internal int Y { get; set; }

            internal SizerRow(int index, SizerRow previous = null)
            {
                PreviousRow = previous;
                Index = index;
            }
        }

        internal class SizerColumn
        {
            internal SizerColumn PreviousColumn { get; private set; }

            internal int Index { get; private set; }
            internal int Max { get; set; }
            #region min, best, max

            internal SortedSet<int> PreferredXs = new SortedSet<int>();
            internal int MinWidth
            {
                get { return (PreviousColumn == null) ? MinX : MinX - PreviousColumn.MinX; }
            }

            internal int MinX { get; set; }

            internal int BestWidth
            {
                get { return (PreviousColumn == null) ? BestX : BestX - PreviousColumn.BestX; }
            }

            internal int BestX { get; set; }

            internal int MaxWidth
            {
                get { return (PreviousColumn == null) ? MaxX : MaxX - PreviousColumn.MaxX; }
            }

            internal int MaxX { get; set; }


            #endregion

            internal bool ExpandHorizontal { get; set; }

            internal int Width
            {
                get { return (PreviousColumn == null) ? X : X - PreviousColumn.X; }
            }

            internal int X { get; set; }

            internal SizerColumn(int index, SizerColumn previous = null)
            {
                PreviousColumn = previous;
                Index = index;
            }
        }


        private class RowComparer : IComparer<SizerRow>
        {
            public int Compare(SizerRow x, SizerRow y)
            {
                // add null checking, demo purposes only
                return x.Height.CompareTo(y.Height);
            }
        }
        private class ColumnComparer : IComparer<SizerColumn>
        {
            public int Compare(SizerColumn x, SizerColumn y)
            {
                // add null checking, demo purposes only
                return x.Width.CompareTo(y.Width);
            }
        }

        internal class SizerGrid
        {
            internal List<SizerRow> Rows = new List<SizerRow>();
            internal List<SizerColumn> Columns = new List<SizerColumn>();
            internal SortedSet<SizerRow> SortedRows = new SortedSet<SizerRow>(new RowComparer());
            internal SortedSet<SizerColumn> SortedColumns = new SortedSet<SizerColumn>(new ColumnComparer());
            internal SizerRow NewRow()
            {
                var row = new SizerRow(Rows.Count, Rows.LastOrDefault());
                Rows.Add(row);
                return row;
            }
            internal SizerColumn NewColumn()
            {
                var column = new SizerColumn(Columns.Count, Columns.LastOrDefault());
                Columns.Add(column);
                return column;
            }
        }

        internal SizerGrid WorkGrid { get; private set; }

        public override void LayoutSizes()
        {
            WorkGrid = new SizerGrid();

            var lastRows = new Dictionary<int, List<IControl>>();
            var lastColumns = new Dictionary<int, List<IControl>>();
            foreach (var control in Children)
            {
                if (control.IsHidden || !ControlData[control].IsValid()) continue;
                if (control.IsExpandHorizontal) InheritedExpandHorizontal = true;
                if (control.IsExpandVertical) InheritedExpandVertical = true; 
                var row = ControlData[control].Row + ControlData[control].EffectiveRowSpan - 1;
                if(!lastRows.ContainsKey(row))
                    lastRows[row] = new List<IControl>();
                lastRows[row].Add(control);
                var column = ControlData[control].Column + ControlData[control].EffectiveColumnSpan - 1;
                if (!lastColumns.ContainsKey(column))
                    lastColumns[column] = new List<IControl>();
                lastColumns[column].Add(control);
            }

            var minRows = new int[Rows];
            var bestRows = new int[Rows];
            var maxRows = new int[Rows];
            for (var i = 0; i < Rows; i++)
            {
                var row = WorkGrid.NewRow();
                if (!lastRows.ContainsKey(i))
                {
                    row.MinY = minRows[i] = (i == 0) ? 0 : minRows[i - 1];
                    row.BestY = bestRows[i] = (i == 0) ? 0 : bestRows[i - 1];
                    row.MaxY = maxRows[i] = (i == 0) ? 0 : maxRows[i - 1];
                    row.Y = row.BestY;
                    WorkGrid.SortedRows.Add(row);
                    row.PreferredYs.Add(row.BestY);
                    continue;
                }

                foreach (var control in lastRows[i])
                {
                    minRows[i] = UpdateRowY(control, minRows, control.LayoutMinSize, minRows[i]);
                    bestRows[i] = UpdateRowY(control, bestRows, control.LayoutBestSize, bestRows[i], row.PreferredYs);
                    maxRows[i] = UpdateRowY(control, maxRows, control.LayoutMaxSize, maxRows[i]);
                    if (control.IsExpandVertical)
                    {
                        row.ExpandVertical = true;
                        row.Max = Math.Max(control.LayoutMaxSize.Y, row.Max);
                    }
                }
                //set layout data for use in sizer. not used in this algorithm
                row.MinY = minRows[i];
                row.BestY = bestRows[i];
                row.MaxY = maxRows[i];
                row.Y = row.BestY;
                WorkGrid.SortedRows.Add(row);
                Debug.Assert(row.PreferredYs.Count > 0 && row.BestY == row.PreferredYs.Last());
                row.PreferredYs.Remove(row.PreferredYs.Last());

            }

            var minColumns = new int[Columns];
            var bestColumns = new int[Columns];
            var maxColumns = new int[Columns];
            for (var i = 0; i < Columns; i++)
            {
                var column = WorkGrid.NewColumn();
                if (!lastColumns.ContainsKey(i))
                {
                    column.MinX = minColumns[i] = (i == 0) ? 0 : minColumns[i - 1];
                    column.BestX = bestColumns[i] = (i == 0) ? 0 : bestColumns[i - 1];
                    column.MaxX = maxColumns[i] = (i == 0) ? 0 : maxColumns[i - 1];
                    column.X = column.BestX;
                    WorkGrid.SortedColumns.Add(column);
                    column.PreferredXs.Add(column.BestX);
                    continue;
                }

                foreach (var control in lastColumns[i])
                {
                    minColumns[i]= UpdateColumnX(control, minColumns, control.LayoutMinSize, minColumns[i]);
                    bestColumns[i] = UpdateColumnX(control, bestColumns, control.LayoutBestSize, bestColumns[i], column.PreferredXs);
                    maxColumns[i] = UpdateColumnX(control, maxColumns, control.LayoutMaxSize, maxColumns[i]);
                    if (control.IsExpandHorizontal)
                    {
                        column.ExpandHorizontal = true;
                        column.Max = Math.Max(control.LayoutMaxSize.X, column.Max);
                    }
                 }
                //set layout data for use in sizer. not used in this algorithm
                column.MinX = minColumns[i];
                column.BestX = bestColumns[i];
                column.MaxX = maxColumns[i];
                column.X = column.BestX;
                WorkGrid.SortedColumns.Add(column);
                Debug.Assert(column.PreferredXs.Count > 0 && column.BestX == column.PreferredXs.Last());
                column.PreferredXs.Remove(column.PreferredXs.Last());
            }

            layoutMinSize = new Vector2i(minColumns[Columns-1], minRows[Rows-1]);
            layoutBestSize = new Vector2i(bestColumns[Columns-1], bestRows[Rows-1]);
            layoutMaxSize = new Vector2i(maxColumns[Columns-1], maxRows[Rows-1]);
        }

        private int UpdateColumnX(IControl control, int[] columnsX, Vector2i controlSize, int currentX, SortedSet<int> preferred = null)
        {
            var startX = ControlData[control].Column == 0 ? 0 : columnsX[ControlData[control].Column - 1];
            var newX = ClampAdd(controlSize.X, startX);
            if (preferred != null) preferred.Add(newX);
            return Math.Max(newX, currentX);
        }

        private int UpdateRowY(IControl control, int[] rowsY, Vector2i controlSize, int currentY, SortedSet<int> preferred = null)
        {
            var startY = ControlData[control].Row == 0 ? 0 : rowsY[ControlData[control].Row - 1];
            var newY = ClampAdd(controlSize.Y, startY);
            if (preferred != null) preferred.Add(newY);
            return Math.Max(newY, currentY);
        }

        public int EffectiveRowSpan(int row, int rowSpan)
        {
            return (row + rowSpan > Columns) ? Rows - row : rowSpan;
        }

        public int EffectiveColumnSpan(int column, int columnSpan)
        {
            return (column + columnSpan > Columns) ? Columns - column : columnSpan;
        }

        public List<IControl> GetControlsAtRow(int row)
        {
            return Children.Where(control => GetRows(control).Contains(row)).ToList();
        }

        public List<IControl> GetControlsAtColumn(int column)
        {
            return Children.Where(control => GetColumns(control).Contains(column)).ToList();
        }

        public List<IControl> GetControlsAtPosition(int row, int column)
        {
            return Children.Where(control => GetRows(control).Contains(row) && GetColumns(control).Contains(column)).ToList();
        }
    }
}
