using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace Flood.GUI.Controls.Containers
{
    public class GridPanel: Panel
    {
        internal Dictionary<Control, Tuple<int?, int?>> ChildrenPositions { get; private set; }
        internal List<Row> Rows { get; set; }

        internal class Row
        {
            internal List<Column> Columns { get; private set; }

            internal Row ()
            {
                Columns = new List<Column>();
            }

            internal Column AddColumn()
            {
                var column = new Column();
                Columns.Add(column);
                return column;
            }
        }

        internal class Column
        {
            internal Control Control { get; set; }
        }

        public GridPanel(Canvas parent, ExpansionFlags expansion = ExpansionFlags.Fit) : base(parent, expansion)
        {
            Init();
        }

        public GridPanel(Panel parent, ExpansionFlags expansion = ExpansionFlags.Fit) : base(parent, expansion)
        {
            Init();

        }

        private void Init()
        {
            Rows = new List<Row>();
            ChildrenPositions = new Dictionary<Control, Tuple<int?, int?>>();
            new GridSizer(this);

        }

        public void SetRows(int nRows)
        {
            if(nRows < 0)
                return;

            while(Rows.Count > nRows)
                Rows.RemoveAt(Rows.Count - 1);
            while (Rows.Count < nRows)
                Rows.Add(new Row());
        }

        public void SetColumns(int nColumns, int rowIndex)
        {
            if (nColumns < 0)
                return;

            if(rowIndex < 0 || rowIndex >= Rows.Count)
                return;

            var columns = Rows[rowIndex].Columns;

            while (columns.Count > nColumns)
                columns.RemoveAt(columns.Count - 1);
            while (columns.Count < nColumns)
                columns.Add(new Column());
        }
        
        public void SetColumns(int nColumns)
        {
            for(int i = 0; i < Rows.Count; i++)
                SetColumns(nColumns, i);
        }

        public override void AddChild(Control child)
        {
            AddChild(child, null, null);
        }

        public void AddChild(Control child, int? row, int? column)
        {
            if (row < 0 || column < 0)
                return;
            base.AddChild(child);
            ChildrenPositions[child] = new Tuple<int?, int?>(row, column);
            if (row != null && column != null && Rows.Count > row && Rows[(int)row].Columns.Count > column)
                Rows[(int)row].Columns[(int)column].Control = child;
        }

        public void SetPosition(Control child, int? row, int? column)
        {
            if (!ChildrenPositions.ContainsKey(child))
                return;
            ChildrenPositions[child] = new Tuple<int?, int?>(row, column);
            if (row != null && column != null && Rows.Count > row && Rows[(int)row].Columns.Count > column)
                Rows[(int) row].Columns[(int) column].Control = child;
        }

        public int? GetRow(Control child)
        {
            if (!ChildrenPositions.ContainsKey(child))
                return null;
            return ChildrenPositions[child].Item1;
        }

        public int? GetColumn(Control child)
        {
            if (!ChildrenPositions.ContainsKey(child))
                return null;
            return ChildrenPositions[child].Item2;
        }

        public override void RemoveChild(Control child, bool dispose)
        {

            if(!ChildrenPositions.ContainsKey(child)) return;

            var row = ChildrenPositions[child].Item1;
            var column = ChildrenPositions[child].Item2;
            if (row != null && column != null && Rows.Count > row && Rows[(int)row].Columns.Count > column)
                Rows[(int)row].Columns[(int)column].Control = null;

            ChildrenPositions.Remove(child);
            base.RemoveChild(child, dispose);
        }

        public List<Control> GetControlsAtRow(int row)
        {
            return Children.Where(control => GetRow(control) == row).ToList();
        }

        //because you can have a different number of columns by row the results may not be aligned
        public List<Control> GetControlsAtColumn(int column)
        {
            return Children.Where(control => GetColumn(control) == column).ToList();
        }

        public List<Control> GetControlsAtPosition(int row, int column)
        {
            return Children.Where(control => GetRow(control) == row && GetColumn(control) == column).ToList();
        }
    }
}
