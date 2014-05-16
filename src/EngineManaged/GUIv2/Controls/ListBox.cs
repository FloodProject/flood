using Flood.GUIv2.Controls.Layout;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.Controls
{
    /// <summary>
    /// ListBox control.
    /// </summary>
    public class ListBox : ScrollControl
    {
        private readonly Table m_Table;
        private readonly List<TableRow> m_SelectedRows;

        private bool m_MultiSelect;
        private bool m_IsToggle;
        private bool m_SizeToContents;
        private ExpansionFlags m_OldExpansion; // used while autosizing
        private AlignmentFlags m_OldAlignment; // used while autosizing

        /// <summary>
        /// Determines whether multiple rows can be selected at once.
        /// </summary>
        public bool AllowMultiSelect
        {
            get { return m_MultiSelect; }
            set
            {
                m_MultiSelect = value; 
                if (value)
                    IsToggle = true;
            }
        }

        /// <summary>
        /// Determines whether rows can be unselected by clicking on them again.
        /// </summary>
        public bool IsToggle { get { return m_IsToggle; } set { m_IsToggle = value; } }

        /// <summary>
        /// Number of rows in the list box.
        /// </summary>
        public int RowCount { get { return m_Table.RowCount; } }

        /// <summary>
        /// Returns specific row of the ListBox.
        /// </summary>
        /// <param name="index">Row index.</param>
        /// <returns>Row at the specified index.</returns>
        public ListBoxRow this[int index] { get { return m_Table[index] as ListBoxRow; } }

        /// <summary>
        /// List of selected rows.
        /// </summary>
        public IEnumerable<TableRow> SelectedRows { get { return m_SelectedRows; } }

        /// <summary>
        /// First selected row (and only if list is not multiselectable).
        /// </summary>
        public TableRow SelectedRow
        {
            get
            {
                if (m_SelectedRows.Count == 0) 
                    return null;
                return m_SelectedRows[0];
            }
        }

        /// <summary>
        /// Gets the selected row number.
        /// </summary>
        public int SelectedRowIndex
        {
            get
            {
                var selected = SelectedRow;
                if (selected == null)
                    return -1;
                return m_Table.GetRowIndex(selected);
            }
        }

        /// <summary>
        /// Column count of table rows.
        /// </summary>
        public int ColumnCount { get { return m_Table.ColumnCount; } set { m_Table.ColumnCount = value; Invalidate(); } }

        /// <summary>
        /// Invoked when a row has been selected.
        /// </summary>
        public event GwenEventHandler RowSelected;

        /// <summary>
        /// Invoked whan a row has beed unselected.
        /// </summary>
        public event GwenEventHandler RowUnselected;

        /// <summary>
        /// Initializes a new instance of the <see cref="ListBox"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public ListBox(Control parent)
            : base(parent)
        {
            SetSize(100, 200);
            m_SelectedRows = new List<TableRow>();

            EnableScroll(false, true);
            AutoHideBars = true;
            Margin = Margin.One;

            m_Table = new Table(this);
            m_Table.Expansion = ExpansionFlags.Expand;
            m_Table.ColumnCount = 1;
            m_Table.BoundsChanged += TableResized;

            m_MultiSelect = false;
            m_IsToggle = false;
        }

        /// <summary>
        /// Selects the specified row by index.
        /// </summary>
        /// <param name="index">Row to select.</param>
        /// <param name="clearOthers">Determines whether to deselect previously selected rows.</param>
        public void SelectRow(int index, bool clearOthers = false)
        {
            throw new NotImplementedException();

            //if (index < 0 || index >= m_Table.RowCount)
            //    return;

            //SelectRow(m_Table.Children[index], clearOthers);
        }

        /// <summary>
        /// Selects the specified row(s) by text.
        /// </summary>
        /// <param name="rowText">Text to search for (exact match).</param>
        /// <param name="clearOthers">Determines whether to deselect previously selected rows.</param>
        public void SelectRows(String rowText, bool clearOthers = false)
        {
            throw new NotImplementedException();

            //var rows = m_Table.Children.OfType<ListBoxRow>().Where(x => x.Text == rowText);
            //foreach (ListBoxRow row in rows)
            //{
            //    SelectRow(row, clearOthers);
            //}
        }

        /// <summary>
        /// Selects the specified row(s) by regex text search.
        /// </summary>
        /// <param name="pattern">Regex pattern to search for.</param>
        /// <param name="regexOptions">Regex options.</param>
        /// <param name="clearOthers">Determines whether to deselect previously selected rows.</param>
        public void SelectRowsByRegex(String pattern, RegexOptions regexOptions = RegexOptions.None, bool clearOthers = false)
        {
            throw new NotImplementedException();

            //var rows = m_Table.Children.OfType<ListBoxRow>().Where(x => Regex.IsMatch(x.Text, pattern) );
            //foreach (ListBoxRow row in rows)
            //{
            //    SelectRow(row, clearOthers);
            //}
        }

        /// <summary>
        /// Slelects the specified row.
        /// </summary>
        /// <param name="control">Row to select.</param>
        /// <param name="clearOthers">Determines whether to deselect previously selected rows.</param>
        public void SelectRow(IControl control, bool clearOthers = false)
        {
            if (!AllowMultiSelect || clearOthers)
                UnselectAll();

            ListBoxRow row = control as ListBoxRow;
            if (row == null)
                return;

            // TODO: make sure this is one of our rows!
            row.IsSelected = true;
            m_SelectedRows.Add(row);
            if (RowSelected != null)
                RowSelected.Invoke(this);
        }

        /// <summary>
        /// Removes the specified row by index.
        /// </summary>
        /// <param name="idx">Row index.</param>
        public void RemoveRow(int idx)
        {
            m_Table.RemoveRow(idx); // this calls Dispose()
        }

        /// <summary>
        /// Adds a new row.
        /// </summary>
        /// <param name="label">Row text.</param>
        /// <returns>Newly created control.</returns>
        public TableRow AddRow(String label)
        {
            return AddRow(label, String.Empty);
        }

        /// <summary>
        /// Adds a new row.
        /// </summary>
        /// <param name="label">Row text.</param>
        /// <param name="name">Internal control name.</param>
        /// <returns>Newly created control.</returns>
        public TableRow AddRow(String label, String name)
        {
            ListBoxRow row = new ListBoxRow(this);
            m_Table.AddRow(row);

            row.SetCellText(0, label);
            row.Name = name;

            row.Selected += OnRowSelected;

            m_Table.SizeToContents(Width);

            return row;
        }


        /// <summary>
        /// Sets the column width (in pixels).
        /// </summary>
        /// <param name="column">Column index.</param>
        /// <param name="width">Column width.</param>
        public void SetColumnWidth(int column, int width)
        {
            m_Table.SetColumnWidth(column, width);
            Invalidate();
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public override void Render(Skins.Skin skin)
        {
            skin.DrawListBox(this);
        }

        /// <summary>
        /// Deselects all rows.
        /// </summary>
        public virtual void UnselectAll()
        {
            foreach (ListBoxRow row in m_SelectedRows)
            {
                row.IsSelected = false;
                if (RowUnselected != null)
                    RowUnselected.Invoke(this);
            }
            m_SelectedRows.Clear();
        }

        /// <summary>
        /// Unselects the specified row.
        /// </summary>
        /// <param name="row">Row to unselect.</param>
        public void UnselectRow(ListBoxRow row)
        {
            row.IsSelected = false;
            m_SelectedRows.Remove(row);

            if (RowUnselected != null)
                RowUnselected.Invoke(this);
        }

        /// <summary>
        /// Handler for the row selection event.
        /// </summary>
        /// <param name="control">Event source.</param>
        protected virtual void OnRowSelected(IControl control)
        {
            // [omeg] changed default behavior
            bool clear = false;// !InputHandler.InputHandler.IsShiftDown;
            ListBoxRow row = control as ListBoxRow;
            if (row == null)
                return;
            
            if (row.IsSelected)
            {
                if (IsToggle)
                    UnselectRow(row);
            }
            else
            {
                SelectRow(control, clear);
            }
        }

        /// <summary>
        /// Removes all rows.
        /// </summary>
        public virtual void Clear()
        {
            UnselectAll();
            m_Table.RemoveAll();
        }
        
        public void SizeToContents()
        {
            m_SizeToContents = true;
            // docking interferes with autosizing so we disable it until sizing is done
            m_OldAlignment = m_Table.Alignment;
            m_OldExpansion = m_Table.Expansion;
            m_Table.Alignment = AlignmentFlags.NotSet;
            m_Table.Expansion = ExpansionFlags.NotSet;
            m_Table.SizeToContents(0); // autosize without constraints
        }

        private void TableResized(IControl control)
        {
            if (m_SizeToContents)
            {
                SetSize(m_Table.Width, m_Table.Height);
                m_SizeToContents = false;
                m_Table.Expansion = m_OldExpansion;
                m_Table.Alignment = m_OldAlignment;
                Invalidate();
            }
        }
    }
}
