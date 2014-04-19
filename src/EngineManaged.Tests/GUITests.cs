using System;
using Flood.GUI;
using Flood.GUI.Controls;
using Flood.GUI.Controls.Containers;
using NUnit.Framework;

namespace Flood.Tests
{
    [TestFixture]
    class GUITests
    {
        private GUI GUI;

        [TestFixtureSetUp]
        public void Setup()
        {
            GUI = new GUI();
        }

        [TestFixtureTearDown]
        public void TearDown()
        {
            GUI.Dispose();
        }

        Canvas canvas;

        [SetUp]
        public void TestSetup()
        {
            GUI.ResetRenderable();
            canvas = GUI.guiRenderable.Canvas;
            canvas.Children.Clear();
        }

        [Test]
        public void TestButton()
        {
            var control = new Button(canvas);
            control.Text = "Button";

            GUI.Test(control, "button1");
        }

        [Test]
        public void TestLabel()
        {
            var control = new Label(canvas);
            control.Text = "Label";

            GUI.Test(control, "label1");
        }

        [Test]
        public void TestTextBox()
        {
            var control = new TextBox(canvas);
            control.Text = "TextBox";

            GUI.Test(control, "TextBox1");
        }

        [Test]
        public void TestCheckBox()
        {
            var control = new CheckBox(canvas);
            GUI.Test(control, "CheckBox1");

            control.Toggle();
            GUI.Test(control, "CheckBox1_toggle");
        }

        [Test]
        public void TestCollapsibleList()
        {
            var control = new CollapsibleList(canvas);
            var cat1 = control.Add("Category1");
            var cat2 = control.Add("Category2");
            cat1.Add("but1-1");
            cat1.Add("but1-2");
            cat2.Add("but2-1");
            cat2.Add("but2-2");
            cat2.Add("but2-3");
            cat2.Add("but2-4");
            control.SizeToContents();

            GUI.Test(control, "CollapsibleList1");
        }

        [Test]
        public void TestColorLerpBox()
        {
            var control = new ColorLerpBox(canvas);
            control.SetColor(Color.NavyBlue);

            GUI.Test(control, "ColorLerpBox1");
        }

        [Test]
        public void TestColorPicker()
        {
            var control = new ColorPicker(canvas);

            GUI.Test(control, "ColorPicker1");
        }

        [Test]
        public void TestColorSlider()
        {
            var control = new ColorSlider(canvas);

            GUI.Test(control, "ColorSlider1");
        }

        [Test]
        public void TestComboBox()
        {
            var control = new ComboBox(canvas);
            control.AddItem("opt1");
            control.AddItem("opt2");
            control.AddItem("opt3");

            GUI.Test(control, "ComboBox1");
        }

        [Test]
        public void TestCrossSplitter()
        {
            var control = new CrossSplitter(canvas);

            GUI.Test(control, "CrossSplitter1");
        }

        [Test]
        public void TestDockBase()
        {
            var control = new DockBase(canvas);

            GUI.Test(control, "DockBase1");
        }

        [Test]
        public void TestDockedTabControl()
        {
            var control = new DockedTabControl(canvas);

            GUI.Test(control, "DockedTabControl1");
        }

        [Test]
        public void TestGroupBox()
        {
            var control = new GroupBox(canvas);
            control.Text = "GroupBox";

            GUI.Test(control, "GroupBox1");
        }

        [Test]
        public void TestHorizontalScrollBar()
        {
            var control = new HorizontalScrollBar(canvas);

            GUI.Test(control, "HorizontalScrollBar1");
        }

        [Test]
        public void TestHorizontalSlider()
        {
            var control = new HorizontalSlider(canvas);
            control.SetSize(128, 32);

            GUI.Test(control, "HorizontalSlider1");
        }

        [Test]
        public void TestHorizontalSplitter()
        {
            var control = new HorizontalSplitter(canvas);

            GUI.Test(control, "HorizontalSplitter1");
        }

        [Test]
        public void TestHSVColorPicker()
        {
            var control = new HSVColorPicker(canvas);
            control.DefaultColor = Color.Red;

            GUI.Test(control, "HSVColorPicker1");
        }

        [Test]
        public void TestImagePanel()
        {
            var control = new ImagePanel(canvas);
            var img = Image.Create(Allocator.GetStack(), 100, 300, PixelFormat.B8G8R8A8);
            control.SetImage(img);

            GUI.Test(control, "ImagePanel1");
        }

        [Test]
        public void TestLabelClickable()
        {
            var control = new LabelClickable(canvas);
            control.Text = "LabelClickable";

            GUI.Test(control, "LabelClickable1");
        }

        [Test]
        public void TestLabeledCheckBox()
        {
            var control = new LabeledCheckBox(canvas);
            control.Text = "LabeledCheckBox";

            GUI.Test(control, "LabeledCheckBox1");
        }

        [Test]
        public void TestLabeledRadioButton()
        {
            var control = new LabeledRadioButton(canvas);
            control.Text = "LabeledRadioButton";

            GUI.Test(control, "LabeledRadioButton1");
        }

        [Test]
        public void TestListBox()
        {
            var control = new ListBox(canvas);
            control.AddRow("row1");
            control.AddRow("row2");
            control.AddRow("row3");

            GUI.Test(control, "ListBox1");
        }

        [Test]
        public void TestMenu()
        {
            var control = new Flood.GUI.Controls.Menu(canvas);
            control.AddItem("item1");
            control.AddItem("item2");
            control.AddItem("item3");
            control.AddDivider();
            control.AddItem("item4");

            GUI.Test(control, "Menu1");
        }

        [Test]
        public void TestMenuStrip()
        {
            var control = new MenuStrip(canvas);
            control.AddItem("item1");
            control.AddItem("item2");
            control.AddItem("item3");
            control.AddDivider();
            control.AddItem("item4");

            GUI.Test(control, "MenuStrip1");
        }

        [Test]
        public void TestMessageBox()
        {
            var control = new MessageBox(canvas, "MessageBox", "caption");

            GUI.Test(control, "MessageBox1");
        }

        [Test]
        public void TestNumericUpDown()
        {
            var control = new NumericUpDown(canvas) { Min = 0, Max = 20, Value = 3 };

            GUI.Test(control, "NumericUpDown1");
        }

        [Test]
        public void TestProgressBar()
        {
            var control = new ProgressBar(canvas) { Value = 0.73f };

            GUI.Test(control, "ProgressBar1");
        }

        [Test]
        public void TestProperties()
        {
            var control = new Properties(canvas);
            control.SetSize(150, 150);
            control.Add("label1", "val1");
            control.Add("label1", "val2");
            control.Add("label2", "val1");
            control.Add("label2", "val2");
            control.Add("label2", "val3");

            GUI.Test(control, "Properties1");
        }

        [Test]
        public void TestPropertyTree()
        {
            var control = new PropertyTree(canvas);
            control.SetSize(150, 150);
            var label1 = control.Add("label1");
            label1.Add("item0");
            label1.Add("item1");
            control.Add("label2");
            control.Add("label3");
            control.ExpandAll();

            GUI.Test(control, "PropertyTree1");
        }

        [Test]
        public void TestRadioButton()
        {
            var control = new RadioButton(canvas);
            GUI.Test(control, "RadioButton1");

            control.Toggle();
            GUI.Test(control, "RadioButton1_toggle");
        }

        [Test]
        public void TestRadioButtonGroup()
        {
            var control = new RadioButtonGroup(canvas, "RadioButtonGroup");
            control.Text = "RadioButtonGroup2";
            control.AddOption("opt1");
            control.AddOption("opt2");
            control.AddOption("opt3");

            GUI.Test(control, "RadioButtonGroup1");
        }

        [Test]
        public void TestResizableControl()
        {
            var control = new ResizableControl(canvas);

            GUI.Test(control, "ResizableControl1");
        }

        [Test]
        public void TestRichLabel()
        {
            var control = new RichLabel(canvas);
            control.AddText("line1", Color.NavyBlue, new Flood.GUI.Font("Arial.ttf", 13));
            control.AddLineBreak();
            control.AddText("line2", Color.NavyBlue, new Flood.GUI.Font("Calibri.ttf", 13));
            control.AddLineBreak();
            control.AddText("line3", Color.NavyBlue, new Flood.GUI.Font("Verdana.ttf", 13));

            GUI.Test(control, "RichLabel1");
        }

        [Test]
        public void TestScrollControl()
        {
            var control = new ScrollControl(canvas);

            GUI.Test(control, "ScrollControl1");
        }

        [Test]
        public void TestStatusBar()
        {
            var control = new StatusBar(canvas) { Text = "StatusBar" };

            GUI.Test(control, "StatusBar1");
        }

        [Test]
        public void TestTabButton()
        {
            var control = new TabButton(canvas) { Text = "TabButton" };

            GUI.Test(control, "TabButton1");
        }

        [Test]
        public void TestTabControl()
        {
            var control = new TabControl(canvas);
            control.AddPage("pg1");
            control.AddPage("pg2");
            control.AddPage("pg3");
            control.AddPage("pg4");

            GUI.Test(control, "TabControl1");
        }

        [Test]
        public void TestTabStrip()
        {
            var control = new TabControl(canvas);
            var strip = new TabStrip(control);

            GUI.Test(control, "TabStrip1");
        }

        [Test]
        public void TestTabTitleBar()
        {
            var control = new TabControl(canvas);
            var title = new TabTitleBar(control) { Text = "TabTitleBar" };

            GUI.Test(control, "TabTitleBar1");
        }

        [Test]
        public void TestTextBoxNumeric()
        {
            var control = new TextBoxNumeric(canvas) { Value = 67.32f };

            GUI.Test(control, "TextBoxNumeric1");
        }

        [Test]
        public void TestTextBoxPassword()
        {
            var control = new TextBoxPassword(canvas) { Text = "TextBoxPassword" };

            GUI.Test(control, "TextBoxPassword1");
        }

        [Test]
        public void TestTreeControl()
        {
            var control = new TreeControl(canvas) { Text = "TreeControl" };
            var node = control.AddNode("node1");
            node.AddNode("node1-1");
            node.AddNode("node1-2");
            control.AddNode("node2");

            GUI.Test(control, "TreeControl1");
        }

        [Test]
        public void TestVerticalScrollBar()
        {
            var control = new VerticalScrollBar(canvas);

            GUI.Test(control, "VerticalScrollBar1");
        }

        [Test]
        public void TestVerticalSlider()
        {
            var control = new VerticalSlider(canvas);

            GUI.Test(control, "VerticalSlider1");
        }

        [Test]
        public void TestVerticalSplitter()
        {
            var control = new VerticalSplitter(canvas);

            GUI.Test(control, "VerticalSplitter1");
        }

        [Test]
        public void TestWindowControl()
        {
            var control = new WindowControl(canvas, "WindowControl");

            GUI.Test(control, "WindowControl1");
        }
        
        [Test]
        public void TestBoxSizer1()
        {
            var boxPanel = new BoxPanel(canvas);
            for(int i = 1; i <= 5; ++i)
                 new Button(boxPanel) {Text = "Button" + Math.Pow(10, i)};

            boxPanel.Redimension();

            GUI.Test(boxPanel, "BoxSizer1_BeforeLayout");
            boxPanel.Layout();
            GUI.Test(boxPanel, "BoxSizer1_AfterLayout");
            boxPanel.Orientation = BoxOrientation.Horizontal;
            boxPanel.Layout();
            GUI.Test(boxPanel, "BoxSizer1_OrientationChanged");
        }

        [Test]
        public void TestBoxSizer2()
        {
            var boxPanel = new BoxPanel(canvas);
            for(int i = 1; i <= 5; ++i)
                 new Button(boxPanel) {Text = "Button" + Math.Pow(10, i)};

            boxPanel.Redimension();

            boxPanel.Orientation = BoxOrientation.Horizontal;
            boxPanel.Layout();
            
            new Button(boxPanel) { Text = "ExtraButton" };

            GUI.Test(boxPanel, "BoxSizer2_BeforeRedimension");
            
            boxPanel.Redimension();

            GUI.Test(boxPanel, "BoxSizer2_BeforeLayout");
            boxPanel.Layout();
            GUI.Test(boxPanel, "BoxSizer2_AfterLayout");
        }
        
        [Test]
        public void TestBoxSizer3()
        {
            var boxPanel = new BoxPanel(canvas);
            for(int i = 1; i <= 5; ++i)
                 new Button(boxPanel) {Text = "Button" + Math.Pow(10, i)};

            boxPanel.Redimension();
            boxPanel.Layout();

            GUI.Test(boxPanel, "BoxSizer3_BeforeSwap");
            boxPanel.SwapChildren(1, 4);
            GUI.Test(boxPanel, "BoxSizer3_AfterSwap");
        }
        
        [Test]
        public void TestBoxSizer4()
        {
            var boxPanel = new BoxPanel(canvas);
            for(int i = 1; i <= 5; ++i)
                 new Button(boxPanel) {Text = "Button" + Math.Pow(10, i)};

            boxPanel.Redimension();
            boxPanel.Layout();


            GUI.Test(boxPanel, "BoxSizer4_BeforeRemoval");
            boxPanel.RemoveChild(2, true);
            GUI.Test(boxPanel, "BoxSizer4_AfterRemoval1");
            boxPanel.RemoveChild(2, false);
            GUI.Test(boxPanel, "BoxSizer4_AfterRemoval2");
        }
    }

}
