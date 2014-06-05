using System;
using System.Linq;

namespace Flood.GUIv2.Panels.Layout
{

    /// <summary>
    /// Grid type <see cref="Sizer"/>. Manages thew layout of of a <see cref="GridPanel"/>.
    /// </summary>
    public class GridSizer : Sizer
    {

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

        //1- get rows y
        //2 - if final row y > height
        //2.1 - get row with biggest height that has any best sized control
        //2.1.1 if no control at best size continue;
        //2.2 - set that rows best sized control with biggest height to min size
        //2.3 - update y of subsequent rows
        //2.4 - goto 2

        //3- get columns x
        //4 - if final column x > width
        //4.1 - get column with biggest width that has any best sized control
        //4.1.1 if no control at best size continue;
        //4.2 - set that rows best sized control with biggest width to min size
        //4.3 - update x of subsequent columns
        //4.4 - goto 4

        //5 - while final x < width && expandControlNotAtMaxWidth.Count > 0
        //5.1 - columnSpace = get free space / expandControlNotAtMaxWidth.Count
        //5.2 - add columnSpace to not maxed out expand columns
        //6 - if final x < width divide remaining width evenly for all columns

        //7 - while final y < height && expandControlNotAtMaxHeight.Count > 0
        //8.1 - rowSpace = get free space / expandControlNotAtMaxHeight.Count
        //8.2 - add rowSpace to not maxed out expand rows
        //9 - if final y < height divide remaining height evenly for all rows

        //10 - get width of columns and height of rows from x and y
        //11 - give controls their positions and boxes from the rows and columns they occupy
        // note: the algorith operates on width and height independently
        // a control "A" whose height is effectively reduced in the height pass does not
        // transmit that knowledge to the width pass, and as such the width pass may reduce another 
        // control "B"'s width unnecessarily when the control "A"'s possible consequent width reduction 
        // in the first pass already stopped the overflow
        // this happens because controls are abstracted from this proccess and the algorithm has no way to 
        // relate rows and columns, establishing a relation between them given the fact that a control can span
        // multiple rows and columns and rows and columns may contain an undeterminable number of controls would cripple 
        // this algorithm's performance critically as far as I can tell

        /// <summary>
        /// Main layout function, repositions and rescales panel's childen according to type of panel.
        /// </summary>
        public override void RecalcSizes()
        {
            var width = Panel.Width;
            var height = Panel.Height;

            var grid = Panel.WorkGrid;
            //layout rows w/o considering expand
            while(grid.Rows.Last().Y > height && grid.SortedRows.Count > 0)
            {
                //get largest row and remove it from the sorted list
                var tallestRow = grid.SortedRows.Last().Value;
                grid.SortedRows.RemoveAt(grid.SortedRows.Count - 1);
                
                int shift;
                //reduce tallest row's height if lower best heights available, min it if not
                if (tallestRow.PreferredYs.Count > 0 
                    && tallestRow.PreferredYs.Last() > tallestRow.MinHeight)
                {
                    var newY = tallestRow.PreferredYs.Last(); //select next biggest height
                    tallestRow.PreferredYs.Remove(newY); //and remove it from future considerations
                    shift = newY - tallestRow.Y;
                    tallestRow.Y = newY;

                    //reinsert modified row in rows sorted list
                    grid.SortedRows.Add(tallestRow.Height, tallestRow);
                }
                else
                {
                    var min = tallestRow.MinHeight;
                    shift = min - tallestRow.Y;
                    tallestRow.Y = (tallestRow.PreviousRow == null)
                        ? min : tallestRow.PreviousRow.Y + min;
                }

                //update subsequent rows
                UpdateYs(tallestRow.Index + 1, shift);
            }

            //layout columns w/o considering expand
            while (grid.Columns.Last().X > width && grid.SortedColumns.Count > 0)
            {
                //get largest column and remove it from the sorted list
                var widestColumn = grid.SortedColumns.Last().Value;
                grid.SortedColumns.RemoveAt(grid.SortedColumns.Count - 1);

                int shift;
                //reduce widest column's width if lower best widths available, min it if not
                if (widestColumn.PreferredXs.Count > 0 
                    && widestColumn.PreferredXs.Last() > widestColumn.MinWidth)
                {
                    var newX = widestColumn.PreferredXs.Last(); //select next largest width
                    widestColumn.PreferredXs.Remove(newX); //and remove it from future considerations
                    shift = newX - widestColumn.X;
                    widestColumn.X = newX;

                    //reinsert modified column in columns' sorted list
                    grid.SortedColumns.Add(widestColumn.Width, widestColumn);
                }
                else
                {
                    var min = widestColumn.MinWidth;
                    shift = min - widestColumn.X;
                    widestColumn.X = (widestColumn.PreviousColumn == null)
                        ? min : widestColumn.PreviousColumn.X + min;
                }

                //update subsequent columns
                UpdateXs(widestColumn.Index + 1, shift);
            }

            //expand vertically
            var expandableRows = grid.Rows.Where(row => row.ExpandVertical && row.Y < row.Max).ToList();
            while (grid.Rows.Last().Y < height && expandableRows.Count > 0)
            {
                var freeSpace = (grid.Rows.Last().Y - height)/expandableRows.Count;
                if (freeSpace == 0)
                    break;
                foreach (var expandableRow in expandableRows)
                {
                    var allotedSpace = Math.Min(freeSpace, expandableRow.Max);
                    expandableRow.Y += allotedSpace;
                    UpdateYs(expandableRow.Index + 1, allotedSpace);
                }
                expandableRows = grid.Rows.Where(row => row.ExpandVertical && row.Y < row.Max).ToList();
            }

            //assign remaining free height if any exists
            if(grid.Rows.Last().Y < height)
            {
                var remaining = height - grid.Rows.Last().Y;
                var count = grid.Rows.Count;
                var allocatableHeight = remaining/count;
                //distribute remaining space evenly allong aall controls
                for(int i = 0; i < count; i++)
                    grid.Rows[i].Y += i*allocatableHeight;

                //if any left over add it to last control
                grid.Rows[count - 1].Y += remaining - allocatableHeight*count;
            }

            //expand horizontally
            var expandableColumns = grid.Columns.Where(column => column.ExpandHorizontal && column.X < column.Max).ToList();
            while (grid.Columns.Last().X < width && expandableColumns.Count > 0)
            {
                var freeSpace = (grid.Columns.Last().X - width)/expandableColumns.Count;
                if (freeSpace == 0)
                    break;
                foreach (var expandableColumn in expandableColumns)
                {
                    var allotedSpace = Math.Min(freeSpace, expandableColumn.Max);
                    expandableColumn.X += allotedSpace;
                    UpdateXs(expandableColumn.Index + 1, allotedSpace);
                }
                expandableColumns = grid.Columns.Where(column => column.ExpandHorizontal && column.X < column.Max).ToList();
            }

            //assign remaining free width if any exists
            if(grid.Columns.Last().X < width)
            {
                var remaining = width - grid.Columns.Last().X;
                var count = grid.Columns.Count;
                var allocatableWidth = remaining/count;
                //distribute remaining space evenly along all controls
                for(int i = 0; i < count; i++)
                    grid.Columns[i].X += i*allocatableWidth;

                //if any left over add it to last control
                grid.Columns[count - 1].X += remaining - allocatableWidth*count;
            }

            //finally get position and boxes of controls and place them
            foreach (var control in Children)
            {
                if (control.IsHidden)
                    continue;
                var data = Panel.ControlData[control];
                var x = data.Column == 0 ? 0 : grid.Columns[data.Column - 1].X;
                var y = data.Row == 0 ? 0 : grid.Rows[data.Row - 1].Y;
                var w = grid.Columns[data.ColumnSpan - 1 + data.Column].X  - x;
                var h = grid.Rows[data.RowSpan - 1 + data.Row].Y - y;

                PositionAndAlign(new Vector2i(x, y), new Vector2i(w, h), control);
            }
        }

        private void UpdateYs(int start, int shift)
        {
            var rows = Panel.WorkGrid.Rows;
            for (int i = start; i < rows.Count; i++)
            {
                rows[i].Y += shift;
                rows[i].MinY += shift;
                rows[i].BestY += shift;
                rows[i].MaxY += shift;
            }
        }
        
        private void UpdateXs(int start, int shift)
        {
            var columns = Panel.WorkGrid.Columns;
            for (int i = start; i < columns.Count; i++)
            {
                columns[i].X += shift;
                columns[i].MinX += shift;
                columns[i].BestX += shift;
                columns[i].MaxX += shift;
            }
        }
    }
}
