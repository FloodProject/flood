using System;
using GUI.Controls;
using GUI.DragDrop;
using GUI.Skins;

namespace Flood.Editor.Client.GUI.Controls
{
    class DockHelper : Control
    {
        private class DockButton : Control
        {
            private Func<Package, bool> dropAction;
            private string iconId;

            public DockButton(DockHelper parent,Func<Package, bool> dropAction, string icon) : base(parent)
            {
                this.dropAction = dropAction;
                SetSize(20, 20);
                iconId = icon;
            }

            public override bool DragAndDrop_CanAcceptPackage(Package p)
            {
                return true;
            }

            public override bool DragAndDrop_HandleDrop(Package p, int x, int y)
            {
                return dropAction.Invoke(p);
            }

            protected override void Render(Skin skin)
            {
                skin.Renderer.DrawColor = Color.Red;
                var testRect = new Rectangle(0, 0, 20, 20);
                skin.Renderer.DrawFilledRect(testRect);
                base.Render(skin);
            }
        }

        private DockButton addUp;
        private DockButton addLeft;
        private DockButton addDown;
        private DockButton addRigth;
        private DockButton addCenter;

        public DockHelper(PaneGroup parent) : base(parent)
        {
            var bSize = 30;

            addUp = new DockButton(this, AddUp, "up");
            addLeft = new DockButton(this, AddLeft, "left");
            addDown = new DockButton(this, AddDown, "down");
            addRigth = new DockButton(this, AddRigth, "rigth");
            addCenter = new DockButton(this, AddCenter, "center");

            addUp.SetPosition(bSize, 0);
            addLeft.SetPosition(0, bSize);
            addDown.SetPosition(bSize, 2 * bSize);
            addRigth.SetPosition(2 * bSize, bSize);
            addCenter.SetPosition(bSize, bSize);

            SizeToChildren();
        }

        private PaneGroup Parent
        {
            get { return base.Parent as PaneGroup; }
        }

        private PaneGroup GetControlPaneGroup(Control control)
        {
            var paneGroup = control as PaneGroup;
            if(paneGroup != null)
                return paneGroup;
            return GetControlPaneGroup(control.Parent);
        }

        private PaneGroup PaneGroup
        {
            get { return GetControlPaneGroup(Parent); }
        }

        private TabButton DraggedPane
        {
            get
            {
                var pane = DragAndDrop.SourceControl as TabButton;
                if (pane == null)
                    throw new Exception(
                        string.Format("DragAndDrop expects type {0} received invalid type {1}",
                                      typeof (TabButton),
                                      DragAndDrop.SourceControl.GetType()));
                return pane;
            }
        }
           

        private bool AddUp(Package p)
        {
            Console.WriteLine("AddUp");

            Parent.MovePaneVertically(DraggedPane, 0, true);
            return false;
        }

        private bool AddLeft(Package p)
        {
            Parent.MovePaneHorizontally(DraggedPane, 0, false);
            return false;
        }

        private bool AddDown(Package p)
        {
            Parent.MovePaneVertically(DraggedPane, 0, false);
            return false;
        }

        private bool AddRigth(Package p)
        {
            Parent.MovePaneHorizontally(DraggedPane, 0, true);
            return false;
        }

        private bool AddCenter(Package p)
        {
            Parent.MovePane(DraggedPane, PaneGroup);
            return false;
        }

        protected override void Layout(Skin skin)
        {
            base.Layout(skin);
            SetPosition(
                (Parent.Bounds.Width - Bounds.Width) / 2,
                Parent.Bounds.Height / 2 - Bounds.Height);
        }

        public override void Think()
        {
            if (DragAndDrop.CurrentPackage == null)
                IsHidden = true;
        }
    }
}
