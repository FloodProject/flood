using System;
using Flood.GUIv2;
using Flood.GUIv2.Controls;
using Flood.GUIv2.Panels;
using Flood.GUIv2.Panels.Layout;
using NUnit.Framework;

namespace Flood.Tests
{
    [TestFixture]
    class GUIv2Tests
    {
        private GUIv2 GUI;

        [TestFixtureSetUp]
        public void Setup()
        {
            GUI = new GUIv2();
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
            canvas.SetSize(64, 22);
            var control = new Button();
            canvas.AddChild(control);
            control.Text = "Button";

            GUI.Test(control, "button1");
        }

        [Test]
        public void TestButton2()
        {
            canvas.SetSize(304, 304);
            var control = new Button();
            canvas.AddChild(control);
            control.SetImage("moon.png", AlignmentFlags.Center);

            GUI.Test(control, "buttonImage");
        }

        [Test]
        public void TestButton3()
        {
            canvas.SetSize(356, 304);
            var control = new Button();
            canvas.AddChild(control);
            control.SetImage("moon.png", AlignmentFlags.Center);
            control.Text = "Moon";

            GUI.Test(control, "buttonImageText");
        }

        [Test]
        public void TestButton4()
        {
            canvas.SetSize(304, 326);
            var control = new Button();
            canvas.AddChild(control);
            control.SetImage("moon.png", AlignmentFlags.Center);
            control.Text = "Moon";
            control.ButtonOrientation = Orientation.Vertical;

            GUI.Test(control, "buttonImageTextVertical");
        }
        
        [Test]
        public void TestButton5()
        {
            canvas.SetSize(304, 326);
            var control = new Button();
            canvas.AddChild(control);
            control.SetImage("moon.png", AlignmentFlags.Center);
            control.Text = "Moon";
            control.ButtonOrientation = Orientation.Vertical;
            control.TextFirst = true;

            GUI.Test(control, "buttonImageTextVerticalSwapped");
        }
        
        [Test]
        public void TestButton6()
        {
            canvas.SetSize(304, 356);
            var control = new Button();
            canvas.AddChild(control);
            control.SetImage("moon.png");
            control.Text = "Moon";
            control.ImageAlignment = AlignmentFlags.Top | AlignmentFlags.Right;
            control.TextAlignment = AlignmentFlags.Bottom | AlignmentFlags.Left;
            control.TextFirst = true;

            GUI.Test(control, "buttonImageTextHorizontalSwappedAlignment");
        }
        
        [Test]
        public void TestButton7()
        {
            canvas.SetSize(600, 400);
            var control = new Button();
            canvas.AddChild(control);
            control.SetImage("moon.png");
            control.Text = "Moon";
            control.Shape(new Vector2i(600, 400));
            control.ImageAlignment = AlignmentFlags.Top | AlignmentFlags.Right;
            control.TextAlignment = AlignmentFlags.Bottom | AlignmentFlags.Left;
            control.TextFirst = true;

            GUI.Test(control, "buttonImageTextShapedAlignment");
        }

        [Test]
        public void TestButton8()
        {
            canvas.Shape(new Vector2i(300, 200));
            var dockPanel = new DockPanel{Expansion = ExpansionFlags.Expand};

            var control = new Button {Text = "Moon", Alignment = AlignmentFlags.Right | AlignmentFlags.Bottom };
            dockPanel.AddChild(control);
            canvas.AddChild(dockPanel);

            GUI.Test(control, "buttonAlignmentBottomLeft");
            control.Alignment = AlignmentFlags.Center;
            GUI.Test(control, "buttonAlignmentCenter");
            control.Alignment = AlignmentFlags.Right | AlignmentFlags.CenterVertical;
            GUI.Test(control, "buttonAlignmentCenterRight");
            control.Alignment = AlignmentFlags.CenterHorizontal | AlignmentFlags.Bottom;
            GUI.Test(control, "buttonAlignmentCenterBottom");
        }

        [Test]
        public void TestLabel()
        {
            var control = new Label { Text = "Label" };
            canvas.AddChild(control);

            GUI.Test(control, "labelSimpleFit");
        }
        
        [Test]
        public void TestLabel2()
        {
            var control = new Label { Text = "Label2" };
            control.Shape(new Vector2i(80, 30));
            control.TextAlignment = AlignmentFlags.Bottom | AlignmentFlags.Right;
            canvas.AddChild(control);

            GUI.Test(control, "labelShapedAlignment");
        }
        
        [Test]
        public void TestLabel3()
        {
            var control = new Label { Text = "Label3" };
            control.Shape(new Vector2i(80, 30)); 
            control.TextAlignment = AlignmentFlags.CenterHorizontal | AlignmentFlags.CenterVertical;
            canvas.AddChild(control);

            GUI.Test(control, "labelShapedAlignmentCenter");
        }
        
        [Test]
        public void TestLabel4()
        {
            canvas.Shape(new Vector2i(1000, 700)); 

            var control = new Label { Text = "Label4" };
            control.Expansion = ExpansionFlags.Expand;
            control.TextAlignment = AlignmentFlags.CenterHorizontal | AlignmentFlags.CenterVertical;
            canvas.AddChild(control);

            GUI.Test(control, "labelExpandAlignmentCenter");
        }

        [Test]
        public void TestDockPanel()
        {
            var labelTop = new Label { Text = "LabelTop", Alignment = AlignmentFlags.Top};
            var labelBottom = new Label { Text = "LabelBottom", Alignment = AlignmentFlags.Bottom};
            var control = new DockPanel(); 
            control.AddChild(labelTop);
            control.AddChild(labelBottom);
            canvas.AddChild(control);

            GUI.Test(control, "dockPanelVertical");
        }
        
        [Test]
        public void TestDockPanel2()
        {
            var labelLeft = new Label { Text = "LabelLeft", Alignment = AlignmentFlags.Left};
            var labelRight = new Label { Text = "LabelRight", Alignment = AlignmentFlags.Right};
            var control = new DockPanel(); 
            control.AddChild(labelLeft);
            control.AddChild(labelRight);
            canvas.AddChild(control);

            GUI.Test(control, "dockPanelHorizontal");
        }

        [Test]
        public void TestDockPanel3()
        {
            var labelCenter = new Label { Text = "LabelC", Alignment = AlignmentFlags.CenterHorizontal | AlignmentFlags.Top};
            var labelBottom = new Label { Text = "LabelBottom", Alignment = AlignmentFlags.Bottom };
            var control = new DockPanel();
            control.AddChild(labelCenter);
            control.AddChild(labelBottom);
            canvas.AddChild(control);

            GUI.Test(control, "dockPanelCenterHorizontal");
        }
        
        [Test]
        public void TestDockPanel4()
        {
            var labelTopLeft = new Label { Text = "LabelTopLeft", Alignment = AlignmentFlags.Left | AlignmentFlags.Top};
            var labelTopRight = new Label { Text = "LabelTopRight", Alignment = AlignmentFlags.Left | AlignmentFlags.Right};
            var labelBottomLeft = new Label { Text = "LabelBottomLeft", Alignment = AlignmentFlags.Bottom | AlignmentFlags.Left};
            var labelBottomRight = new Label { Text = "LabelBottomRight", Alignment = AlignmentFlags.Bottom | AlignmentFlags.Right };
            var control = new DockPanel();
            control.AddChild(labelTopLeft);
            control.AddChild(labelTopRight);
            control.AddChild(labelBottomLeft);
            control.AddChild(labelBottomRight);
            canvas.AddChild(control);

            GUI.Test(control, "dockPanel4corners");
        }

        [Test]
        public void TestDockPanel5()
        {
            var labelCenterTop = new Label { Text = "LabelCenterTop", Alignment = AlignmentFlags.CenterHorizontal | AlignmentFlags.Top};
            var labelTopRight = new Label { Text = "LabelTopRight", Alignment = AlignmentFlags.Left | AlignmentFlags.Right};
            var labelBottomLeft = new Label { Text = "LabelBottomLeft", Alignment = AlignmentFlags.Bottom | AlignmentFlags.Left};
            var labelBottomRight = new Label { Text = "LabelBottomRight", Alignment = AlignmentFlags.Bottom | AlignmentFlags.Right };
            var control = new DockPanel();
            control.AddChild(labelCenterTop);
            control.AddChild(labelTopRight);
            control.AddChild(labelBottomLeft);
            control.AddChild(labelBottomRight);
            canvas.AddChild(control);

            GUI.Test(control, "dockPanel3corners+center");
        }
        
        [Test]
        public void TestDockPanel6()
        {
            var labelTopLeft = new Label { Text = "LabelTopLeft", Alignment = AlignmentFlags.Left | AlignmentFlags.Top, Margin = Margin.Five};
            var labelTopRight = new Label { Text = "LabelTopRight", Alignment = AlignmentFlags.Left | AlignmentFlags.Right, Margin = Margin.Five };
            var labelBottomLeft = new Label { Text = "LabelBottomLeft", Alignment = AlignmentFlags.Bottom | AlignmentFlags.Left, Margin = Margin.Five };
            var labelBottomRight = new Label { Text = "LabelBottomRight", Alignment = AlignmentFlags.Bottom | AlignmentFlags.Right, Margin = Margin.Five };
            var control = new DockPanel();
            control.AddChild(labelTopLeft);
            control.AddChild(labelTopRight);
            control.AddChild(labelBottomLeft);
            control.AddChild(labelBottomRight);
            canvas.AddChild(control);

            GUI.Test(control, "dockPanel4cornersUniformMargins");
        }
        
        [Test]
        public void TestDockPanel7()
        {
            var labelTopLeft = new Label { Text = "LabelTopLeft", Alignment = AlignmentFlags.Left 
                | AlignmentFlags.Top, Margin = new Margin(10, 0)};
            var labelTopRight = new Label { Text = "LabelTopRight", Alignment = AlignmentFlags.Left 
                | AlignmentFlags.Right, Margin = new Margin(10, 0) };
            var labelBottomLeft = new Label { Text = "LabelBottomLeft", Alignment = AlignmentFlags.Bottom 
                | AlignmentFlags.Left, Margin = new Margin(10, 0) };
            var labelBottomRight = new Label { Text = "LabelBottomRight", Alignment = AlignmentFlags.Bottom 
                | AlignmentFlags.Right, Margin = new Margin(10, 0) };
            var control = new DockPanel();
            control.AddChild(labelTopLeft);
            control.AddChild(labelTopRight);
            control.AddChild(labelBottomLeft);
            control.AddChild(labelBottomRight);
            canvas.AddChild(control);

            GUI.Test(control, "dockPanel4cornersHorizontalMargins");
        }

        [Test]
        public void TestDockPanel8()
        {
            var labelTopLeft = new Label { Text = "LabelTopLeft", Alignment = AlignmentFlags.Left 
                | AlignmentFlags.Top, Margin = new Margin(0, 10)};
            var labelTopRight = new Label { Text = "LabelTopRight", Alignment = AlignmentFlags.Left 
                | AlignmentFlags.Right, Margin = new Margin(0, 10) };
            var labelBottomLeft = new Label { Text = "LabelBottomLeft", Alignment = AlignmentFlags.Bottom 
                | AlignmentFlags.Left, Margin = new Margin(0, 10) };
            var labelBottomRight = new Label { Text = "LabelBottomRight", Alignment = AlignmentFlags.Bottom 
                | AlignmentFlags.Right, Margin = new Margin(0, 10) };
            var control = new DockPanel();
            control.AddChild(labelTopLeft);
            control.AddChild(labelTopRight);
            control.AddChild(labelBottomLeft);
            control.AddChild(labelBottomRight);
            canvas.AddChild(control);

            GUI.Test(control, "dockPanel4cornersVerticalMargins");
        }

        [Test]
        public void TestDockPanel9()
        {
            var labelTopLeft = new Label { Text = "LabelTopLeft", Alignment = AlignmentFlags.Left 
                | AlignmentFlags.Top, Margin = new Margin(10, 15, 30, 45) };
            var labelTopRight = new Label { Text = "LabelTopRight", Alignment = AlignmentFlags.Left 
                | AlignmentFlags.Right, Margin = new Margin(4, 8, 3, 12) };
            var labelBottomLeft = new Label { Text = "LabelBottomLeft", Alignment = AlignmentFlags.Bottom 
                | AlignmentFlags.Left, Margin = new Margin(18, 11, 7, 23) };
            var labelBottomRight = new Label { Text = "LabelBottomRight", Alignment = AlignmentFlags.Bottom 
                | AlignmentFlags.Right, Margin = new Margin(11, 13, 43, 8) };
            var control = new DockPanel();
            control.AddChild(labelTopLeft);
            control.AddChild(labelTopRight);
            control.AddChild(labelBottomLeft);
            control.AddChild(labelBottomRight);
            canvas.AddChild(control);

            GUI.Test(control, "dockPanel4cornersAssymetricalMargins");
        }

        [Test]
        public void TestDockPanel10()
        {
            var labelTopLeft = new Label { Text = "LabelTopLeft", Alignment = AlignmentFlags.Left | AlignmentFlags.Top };
            var labelTopRight = new Label { Text = "LabelTopRight", Alignment = AlignmentFlags.Left | AlignmentFlags.Right };
            var labelBottomLeft = new Label { Text = "LabelBottomLeft", Alignment = AlignmentFlags.Bottom | AlignmentFlags.Left };
            var labelBottomRight = new Label { Text = "LabelBottomRight", Alignment = AlignmentFlags.Bottom | AlignmentFlags.Right };
            var control = new DockPanel();
            control.Shape(new Vector2i(150, 100)); 
            control.AddChild(labelTopLeft);
            control.AddChild(labelTopRight);
            control.AddChild(labelBottomLeft);
            control.AddChild(labelBottomRight);
            canvas.AddChild(control);

            GUI.Test(control, "dockPanel4cornersShapedOverflow");
        }
        [Test]
        public void TestDockPanel11()
        {
            var labelTopLeft = new Label { Text = "LabelTopLeft", Alignment = AlignmentFlags.Left | AlignmentFlags.Top };
            var labelTopRight = new Label { Text = "LabelTopRight", Alignment = AlignmentFlags.Left | AlignmentFlags.Right };
            var labelBottomLeft = new Label { Text = "LabelBottomLeft", Alignment = AlignmentFlags.Bottom | AlignmentFlags.Left };
            var labelBottomRight = new Label { Text = "LabelBottomRight", Alignment = AlignmentFlags.Bottom | AlignmentFlags.Right };
            var control = new DockPanel();
            control.Shape(new Vector2i(150, 100)); 
            control.AddChild(labelTopLeft);
            control.AddChild(labelTopRight);
            control.AddChild(labelBottomLeft);
            control.AddChild(labelBottomRight);
            canvas.AddChild(control);

            GUI.Test(control, "dockPanel4cornersShapedUnderflow");
        }

        [Test]
        public void TestDockPanel12()
        {
            var labelTopLeft = new Label { Text = "LabelTopLeft", Alignment = AlignmentFlags.Left 
                | AlignmentFlags.Top, Margin = Margin.Ten};
            var labelTopRight = new Label { Text = "LabelTopRight", Alignment = AlignmentFlags.Left 
                | AlignmentFlags.Right, Margin = Margin.Ten };
            var labelBottomLeft = new Label { Text = "LabelBottomLeft", Alignment = AlignmentFlags.Bottom 
                | AlignmentFlags.Left, Margin = Margin.Ten };
            var labelBottomRight = new Label { Text = "LabelBottomRight", Alignment = AlignmentFlags.Bottom 
                | AlignmentFlags.Right, Margin = Margin.Ten };
            var control = new DockPanel();
            control.Shape(new Vector2i(150, 100)); 
            control.AddChild(labelTopLeft);
            control.AddChild(labelTopRight);
            control.AddChild(labelBottomLeft);
            control.AddChild(labelBottomRight);
            canvas.AddChild(control);

            GUI.Test(control, "dockPanelShapedOverflowMargins");
        }

        [Test]
        public void TestDockPanel13()
        {
            var labelTopLeft = new Label { Text = "LabelTopLeft", Alignment = AlignmentFlags.Left 
                | AlignmentFlags.Top, Margin = Margin.Ten};
            var labelTopRight = new Label { Text = "LabelTopRight", Alignment = AlignmentFlags.Left 
                | AlignmentFlags.Right, Margin = Margin.Ten };
            var labelBottomLeft = new Label { Text = "LabelBottomLeft", Alignment = AlignmentFlags.Bottom 
                | AlignmentFlags.Left, Margin = Margin.Ten };
            var labelBottomRight = new Label { Text = "LabelBottomRight", Alignment = AlignmentFlags.Bottom 
                | AlignmentFlags.Right, Margin = Margin.Ten };
            var control = new DockPanel();
            control.Shape(new Vector2i(350, 100)); 
            control.AddChild(labelTopLeft);
            control.AddChild(labelTopRight);
            control.AddChild(labelBottomLeft);
            control.AddChild(labelBottomRight);
            canvas.AddChild(control);

            GUI.Test(control, "dockPanelShapedUnderflowMargins");
        }

        [Test]
        public void TestTextBox()
        {
            //var control = new TextBox(canvas);
            //control.Text = "TextBox";

            //GUI.Test(control, "TextBox1");
        }

        [Test]
        public void TestCheckBox()
        {
            //var control = new CheckBox(canvas);
            //GUI.Test(control, "CheckBox1");

            //control.Toggle();
            //GUI.Test(control, "CheckBox1_toggle");
        }

        [Test]
        public void TestCollapsibleList()
        {
            //var control = new CollapsibleList(canvas);
            //var cat1 = control.Add("Category1");
            //var cat2 = control.Add("Category2");
            //cat1.Add("but1-1");
            //cat1.Add("but1-2");
            //cat2.Add("but2-1");
            //cat2.Add("but2-2");
            //cat2.Add("but2-3");
            //cat2.Add("but2-4");
            //control.SizeToContents();

            //GUI.Test(control, "CollapsibleList1");
        }

        [Test]
        public void TestColorLerpBox()
        {
            //var control = new ColorLerpBox(canvas);
            //control.SetColor(Color.NavyBlue);

            //GUI.Test(control, "ColorLerpBox1");
        }

        [Test]
        public void TestColorPicker()
        {
            //var control = new ColorPicker(canvas);

            //GUI.Test(control, "ColorPicker1");
        }

        [Test]
        public void TestColorSlider()
        {
            //var control = new ColorSlider(canvas);

            //GUI.Test(control, "ColorSlider1");
        }

        [Test]
        public void TestComboBox()
        {
            //var control = new ComboBox(canvas);
            //control.AddItem("opt1");
            //control.AddItem("opt2");
            //control.AddItem("opt3");

            //GUI.Test(control, "ComboBox1");
        }

        [Test]
        public void TestCrossSplitter()
        {
            //var control = new CrossSplitter(canvas);

            //GUI.Test(control, "CrossSplitter1");
        }

        [Test]
        public void TestDockBase()
        {
            //var control = new DockBase(canvas);

            //GUI.Test(control, "DockBase1");
        }

        [Test]
        public void TestDockedTabControl()
        {
            //var control = new DockedTabControl(canvas);

            //GUI.Test(control, "DockedTabControl1");
        }

        [Test]
        public void TestGroupBox()
        {
            //var control = new GroupBox(canvas);
            //control.Text = "GroupBox";

            //GUI.Test(control, "GroupBox1");
        }

        [Test]
        public void TestHorizontalScrollBar()
        {
            //var control = new HorizontalScrollBar(canvas);

            //GUI.Test(control, "HorizontalScrollBar1");
        }

        [Test]
        public void TestHorizontalSlider()
        {
            //var control = new HorizontalSlider(canvas);
            //control.SetSize(128, 32);

            //GUI.Test(control, "HorizontalSlider1");
        }

        [Test]
        public void TestHorizontalSplitter()
        {
            //var control = new HorizontalSplitter(canvas);

            //GUI.Test(control, "HorizontalSplitter1");
        }

        [Test]
        public void TestHSVColorPicker()
        {
            //var control = new HSVColorPicker(canvas);
            //control.DefaultColor = Color.Red;

            //GUI.Test(control, "HSVColorPicker1");
        }

        //[Test]
        //public void TestImageBox()
        //{
        //    var control = new ImageBox(canvas);
        //    var img = Image.Create(Allocator.GetStack(), 100, 300, PixelFormat.B8G8R8A8);
        //    control.SetImage(img);

        //    GUI.Test(control, "ImageBox1");
        //}

        //[Test]
        //public void TestLabelClickable()
        //{
        //    var control = new LabelClickable(canvas);
        //    control.Text = "LabelClickable";

        //    GUI.Test(control, "LabelClickable1");
        //}

        //[Test]
        //public void TestLabeledCheckBox()
        //{
        //    var control = new LabeledCheckBox(canvas);
        //    control.Text = "LabeledCheckBox";

        //    GUI.Test(control, "LabeledCheckBox1");
        //}

        //[Test]
        //public void TestLabeledRadioButton()
        //{
        //    var control = new LabeledRadioButton(canvas);
        //    control.Text = "LabeledRadioButton";

        //    GUI.Test(control, "LabeledRadioButton1");
        //}

        //[Test]
        //public void TestListBox()
        //{
        //    var control = new ListBox(canvas);
        //    control.AddRow("row1");
        //    control.AddRow("row2");
        //    control.AddRow("row3");

        //    GUI.Test(control, "ListBox1");
        //}

        //[Test]
        //public void TestMenu()
        //{
        //    var control = new Flood.GUI.Controls.Menu(canvas);
        //    control.AddItem("item1");
        //    control.AddItem("item2");
        //    control.AddItem("item3");
        //    control.AddDivider();
        //    control.AddItem("item4");

        //    GUI.Test(control, "Menu1");
        //}

        //[Test]
        //public void TestMenuStrip()
        //{
        //    var control = new MenuStrip(canvas);
        //    control.AddItem("item1");
        //    control.AddItem("item2");
        //    control.AddItem("item3");
        //    control.AddDivider();
        //    control.AddItem("item4");

        //    GUI.Test(control, "MenuStrip1");
        //}

        //[Test]
        //public void TestMessageBox()
        //{
        //    var control = new MessageBox(canvas, "MessageBox", "caption");

        //    GUI.Test(control, "MessageBox1");
        //}

        //[Test]
        //public void TestNumericUpDown()
        //{
        //    var control = new NumericUpDown(canvas) { Min = 0, Max = 20, Value = 3 };

        //    GUI.Test(control, "NumericUpDown1");
        //}

        //[Test]
        //public void TestProgressBar()
        //{
        //    var control = new ProgressBar(canvas) { Value = 0.73f };

        //    GUI.Test(control, "ProgressBar1");
        //}

        //[Test]
        //public void TestProperties()
        //{
        //    var control = new Properties(canvas);
        //    control.SetSize(150, 150);
        //    control.Add("label1", "val1");
        //    control.Add("label1", "val2");
        //    control.Add("label2", "val1");
        //    control.Add("label2", "val2");
        //    control.Add("label2", "val3");

        //    GUI.Test(control, "Properties1");
        //}

        //[Test]
        //public void TestPropertyTree()
        //{
        //    var control = new PropertyTree(canvas);
        //    control.SetSize(150, 150);
        //    var label1 = control.Add("label1");
        //    label1.Add("item0");
        //    label1.Add("item1");
        //    control.Add("label2");
        //    control.Add("label3");
        //    control.ExpandAll();

        //    GUI.Test(control, "PropertyTree1");
        //}

        //[Test]
        //public void TestRadioButton()
        //{
        //    var control = new RadioButton(canvas);
        //    GUI.Test(control, "RadioButton1");

        //    control.Toggle();
        //    GUI.Test(control, "RadioButton1_toggle");
        //}

        //[Test]
        //public void TestRadioButtonGroup()
        //{
        //    var control = new RadioButtonGroup(canvas, "RadioButtonGroup");
        //    control.Text = "RadioButtonGroup2";
        //    control.AddOption("opt1");
        //    control.AddOption("opt2");
        //    control.AddOption("opt3");

        //    GUI.Test(control, "RadioButtonGroup1");
        //}

        //[Test]
        //public void TestResizableControl()
        //{
        //    var control = new ResizableControl(canvas);

        //    GUI.Test(control, "ResizableControl1");
        //}

        //[Test]
        //public void TestRichLabel()
        //{
        //    var control = new RichLabel(canvas);
        //    control.AddText("line1", Color.NavyBlue, new Flood.GUI.Font("Arial.ttf", 13));
        //    control.AddLineBreak();
        //    control.AddText("line2", Color.NavyBlue, new Flood.GUI.Font("Calibri.ttf", 13));
        //    control.AddLineBreak();
        //    control.AddText("line3", Color.NavyBlue, new Flood.GUI.Font("Verdana.ttf", 13));

        //    GUI.Test(control, "RichLabel1");
        //}

        //[Test]
        //public void TestScrollControl()
        //{
        //    var control = new ScrollControl(canvas);

        //    GUI.Test(control, "ScrollControl1");
        //}

        //[Test]
        //public void TestStatusBar()
        //{
        //    var control = new StatusBar(canvas) { Text = "StatusBar" };

        //    GUI.Test(control, "StatusBar1");
        //}

        //[Test]
        //public void TestTabButton()
        //{
        //    var control = new TabButton(canvas) { Text = "TabButton" };

        //    GUI.Test(control, "TabButton1");
        //}

        //[Test]
        //public void TestTabControl()
        //{
        //    var control = new TabControl(canvas);
        //    control.AddPage("pg1");
        //    control.AddPage("pg2");
        //    control.AddPage("pg3");
        //    control.AddPage("pg4");

        //    GUI.Test(control, "TabControl1");
        //}

        //[Test]
        //public void TestTabStrip()
        //{
        //    var control = new TabControl(canvas);
        //    var strip = new TabStrip(control);

        //    GUI.Test(control, "TabStrip1");
        //}

        //[Test]
        //public void TestTabTitleBar()
        //{
        //    var control = new TabControl(canvas);
        //    var title = new TabTitleBar(control) { Text = "TabTitleBar" };

        //    GUI.Test(control, "TabTitleBar1");
        //}

        //[Test]
        //public void TestTextBoxNumeric()
        //{
        //    var control = new TextBoxNumeric(canvas) { Value = 67.32f };

        //    GUI.Test(control, "TextBoxNumeric1");
        //}

        //[Test]
        //public void TestTextBoxPassword()
        //{
        //    var control = new TextBoxPassword(canvas) { Text = "TextBoxPassword" };

        //    GUI.Test(control, "TextBoxPassword1");
        //}

        //[Test]
        //public void TestTreeControl()
        //{
        //    var control = new TreeControl(canvas) { Text = "TreeControl" };
        //    var node = control.AddNode("node1");
        //    node.AddNode("node1-1");
        //    node.AddNode("node1-2");
        //    control.AddNode("node2");

        //    GUI.Test(control, "TreeControl1");
        //}

        //[Test]
        //public void TestVerticalScrollBar()
        //{
        //    var control = new VerticalScrollBar(canvas);

        //    GUI.Test(control, "VerticalScrollBar1");
        //}

        //[Test]
        //public void TestVerticalSlider()
        //{
        //    var control = new VerticalSlider(canvas);

        //    GUI.Test(control, "VerticalSlider1");
        //}

        //[Test]
        //public void TestVerticalSplitter()
        //{
        //    var control = new VerticalSplitter(canvas);

        //    GUI.Test(control, "VerticalSplitter1");
        //}

        //[Test]
        //public void TestWindowControl()
        //{
        //    var control = new WindowControl(canvas, "WindowControl");

        //    GUI.Test(control, "WindowControl1");
        //}

        [Test]
        public void TestBoxSizer1()
        {
            var boxPanel = new BoxPanel();
            for (int i = 1; i <= 5; ++i)
                boxPanel.AddChild(new Button { Text = "Button" + Math.Pow(10, i) });
            canvas.AddChild(boxPanel);

            GUI.Test(boxPanel, "BoxSizer1_AfterLayout");
            boxPanel.Orientation = Orientation.Horizontal;
            GUI.Test(boxPanel, "BoxSizer1_OrientationChanged");
        }

        [Test]
        public void TestBoxSizer2()
        {
            var boxPanel = new BoxPanel();
            for (int i = 1; i <= 5; ++i)
                boxPanel.AddChild(new Button() { Text = "Button" + Math.Pow(10, i) });

            canvas.AddChild(boxPanel);
            boxPanel.Orientation = Orientation.Horizontal;

            boxPanel.AddChild(new Button() { Text = "ExtraButton" });

            GUI.Test(boxPanel, "BoxSizer2");
        }

        [Test]
        public void TestBoxSizer3()
        {
            var boxPanel = new BoxPanel();
            for (int i = 1; i <= 5; ++i)
                boxPanel.AddChild(new Button() { Text = "Button" + Math.Pow(10, i) });

            canvas.AddChild(boxPanel);

            GUI.Test(boxPanel, "BoxSizer3_BeforeSwap");
            boxPanel.SwapChildren(1, 4);
            GUI.Test(boxPanel, "BoxSizer3_AfterSwap");
        }

        [Test]
        public void TestBoxSizer4()
        {
            var boxPanel = new BoxPanel();
            for (int i = 1; i <= 5; ++i)
                boxPanel.AddChild(new Button() { Text = "Button" + Math.Pow(10, i) });

            canvas.AddChild(boxPanel);

            GUI.Test(boxPanel, "BoxSizer4_BeforeRemoval");
            boxPanel.RemoveChild(2, true);
            GUI.Test(boxPanel, "BoxSizer4_AfterRemoval1");
            boxPanel.RemoveChild(2, false);
            GUI.Test(boxPanel, "BoxSizer4_AfterRemoval2");
        }

        [Test]
        public void TestGridSizer1()
        {
            var gridPanel = new GridPanel();
            gridPanel.SetRows(3);
            gridPanel.SetColumns(2);
            gridPanel.AddChild(new Button { Text = "grid_0-0" }, 0, 0);
            gridPanel.AddChild(new Button { Text = "grid_0-1" }, 0, 1);
            gridPanel.AddChild(new Button { Text = "grid_1-0" }, 1, 0);
            gridPanel.AddChild(new Button { Text = "grid_1-1" }, 1, 1);
            var boxPanel = new BoxPanel();
            for (int i = 1; i <= 5; ++i)
                boxPanel.AddChild(new Button { Text = "Button" + Math.Pow(10, i) });

            gridPanel.AddChild(boxPanel, 2, 0);
            gridPanel.AddChild(new Button { Text = "grid_2-1" }, 2, 1);

            canvas.AddChild(gridPanel); 
            GUI.Test(gridPanel, "GridSizer1");
        }

        [Test]
        public void TestGridSizer2()
        {
            var gridPanel = new GridPanel();
            gridPanel.SetRows(3);
            gridPanel.SetColumns(2);
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    var button = new Button { Text = "grid_" + i + "-" + j };
                    button.Alignment = (j % 2 == 0) ? AlignmentFlags.Center : AlignmentFlags.Right;
                    gridPanel.AddChild(button, i, j);
                }

            }

            var boxPanel = new BoxPanel();
            for (int i = 1; i <= 5; ++i)
                boxPanel.AddChild(new Button { Text = "Button" + Math.Pow(10, i) });


            gridPanel.AddChild(boxPanel, 2, 0);
            gridPanel.AddChild(new Button { Text = "grid_2-1", Alignment = AlignmentFlags.Center }, 2, 1);

            canvas.AddChild(gridPanel);
            GUI.Test(gridPanel, "GridSizer2_Alignment");
        }
        
        //[Test]
        //public void TestGridSizer3()
        //{
        //    var gridPanel = new GridPanel(canvas);
        //    gridPanel.SetRows(4);
        //    gridPanel.SetColumns(2);
        //    gridPanel.SetColumns(1, 3);
        //    for (int i = 0; i < 2;i++)
        //    {
        //        for (int j = 0; j < 2; j++)
        //        {
        //            var button = new Button(gridPanel) {Text = "grid_"+i+"-"+j};
        //            button.Alignment = (j % 2 == 0)? AlignmentFlags.Center : AlignmentFlags.Right;
        //            gridPanel.SetPosition(button, i, j);
        //        }
                
        //    }
            
        //    var boxPanel = new BoxPanel(gridPanel);
        //    for (int i = 1; i <= 5; ++i)
        //        new Button(boxPanel) { Text = "Button" + Math.Pow(10, i) };
        //    boxPanel.Redimension();
        //    boxPanel.Layout();

        //    gridPanel.SetPosition(boxPanel, 3,0);
        //    gridPanel.SetPosition(new Button(gridPanel) { Text = "grid_2-1", Alignment = AlignmentFlags.Center }, 2, 1);

        //    gridPanel.Layout();
        //    GUI.Test(gridPanel, "GridSizer3_Alignment");
        //}
        
        //[Test]
        //public void TestGridSizer4()
        //{
        //    canvas.Width = 400;
        //    canvas.Height = 600;
        //    var gridPanel = new GridPanel(canvas, ExpansionFlags.Expand);
        //    gridPanel.SetRows(2);
        //    gridPanel.SetColumns(2);
        //    for (int i = 0; i < 2;i++)
        //    {
        //        for (int j = 0; j < 2; j++)
        //        {
        //            var button = new Button(gridPanel) {Text = "grid_"+i+"-"+j};
        //            button.Alignment = (j % 2 == 0)? AlignmentFlags.Top : AlignmentFlags.Right;
        //            gridPanel.SetPosition(button, i, j);
        //        }
                
        //    }

        //    gridPanel.Layout();
        //    GUI.Test(gridPanel, "GridSizer4_Expand");
        //}

        //[Test]
        //public void TestGridSizer5()
        //{
        //    canvas.Width = 400;
        //    canvas.Height = 600;
        //    var gridPanel = new GridPanel(canvas, ExpansionFlags.Expand);
        //    gridPanel.SetRows(2);
        //    gridPanel.SetColumns(2);
        //    for (int i = 0; i < 2;i++)
        //    {
        //        for (int j = 0; j < 2; j++)
        //        {
        //            var button = new Button(gridPanel) {Text = "grid_"+i+"-"+j};
        //            button.Alignment = AlignmentFlags.Center;
        //            gridPanel.SetPosition(button, i, j);
        //        }
                
        //    }
   
        //    gridPanel.Layout();
        //    GUI.Test(gridPanel, "GridSizer5_Expand_Center");
        //}
        
        //[Test]
        //public void TestGridSizer6()
        //{
        //    canvas.Width = 400;
        //    canvas.Height = 600;
        //    var gridPanel = new GridPanel(canvas, ExpansionFlags.Expand);
        //    gridPanel.SetRows(2);
        //    gridPanel.SetColumns(10);
        //    for (int i = 0; i < 2;i++)
        //    {
        //        for (int j = 0; j < 10; j++)
        //        {
        //            var button = new Button(gridPanel) {Text = "grid_"+i+"-"+j};
        //            button.Alignment = AlignmentFlags.Center;
        //            gridPanel.SetPosition(button, i, j);
        //        }
                
        //    }
   
        //    gridPanel.Layout();
        //    GUI.Test(gridPanel, "GridSizer6_Expand_WidthOverflow");
        //}

        //[Test]
        //public void TestGridSizer7()
        //{
        //    canvas.Width = 400;
        //    canvas.Height = 600;
        //    var gridPanel = new GridPanel(canvas, ExpansionFlags.Expand);
        //    gridPanel.SetRows(30);
        //    gridPanel.SetColumns(2);
        //    for (int i = 0; i < 30;i++)
        //    {
        //        for (int j = 0; j < 2; j++)
        //        {
        //            var button = new Button(gridPanel) {Text = "grid_"+i+"-"+j};
        //            button.Alignment = AlignmentFlags.Center;
        //            gridPanel.SetPosition(button, i, j);
        //        }
                
        //    }
   
        //    gridPanel.Layout();
        //    GUI.Test(gridPanel, "GridSizer7_Expand_HeightOverflow");
        //}

        //[Test]
        //public void TestGridSizer8()
        //{
        //    canvas.Width = 400;
        //    canvas.Height = 600;
        //    var gridPanel = new GridPanel(canvas, ExpansionFlags.Expand);
        //    gridPanel.SetRows(30);
        //    gridPanel.SetColumns(20);
        //    for (int i = 0; i < 30;i++)
        //    {
        //        for (int j = 0; j < 20; j++)
        //        {
        //            var button = new Button(gridPanel) {Text = "grid_"+i+"-"+j};
        //            button.Alignment = AlignmentFlags.Center;
        //            gridPanel.SetPosition(button, i, j);
        //        }
                
        //    }
   
        //    gridPanel.Layout();
        //    GUI.Test(gridPanel, "GridSizer8_Expand_Width&HeightOverflow");
        //}
        
        //[Test]
        //public void TestGridSizer9()
        //{
        //    var gridPanel = new GridPanel(canvas);
        //    gridPanel.SetRows(3);
        //    gridPanel.SetColumns(2);
        //    var colorPicker = new ColorPicker(gridPanel);
        //    gridPanel.SetPosition(colorPicker, 0, 0);
        //    var rbg = new RadioButtonGroup(gridPanel, "RadioButtonGroup");
        //    rbg.Text = "RadioButtonGroup2";
        //    rbg.AddOption("opt1");
        //    rbg.AddOption("opt2");
        //    rbg.AddOption("opt3");
        //    gridPanel.SetPosition(rbg, 0, 1);

        //    var collapsibleList = new CollapsibleList(gridPanel);
        //    var cat1 = collapsibleList.Add("Category1");
        //    var cat2 = collapsibleList.Add("Category2");
        //    cat1.Add("but1-1");
        //    cat1.Add("but1-2");
        //    cat2.Add("but2-1");
        //    cat2.Add("but2-2");
        //    cat2.Add("but2-3");
        //    cat2.Add("but2-4");
        //    collapsibleList.SizeToContents();
        //    gridPanel.SetPosition(collapsibleList, 1, 0);
        //    var comboBox = new ComboBox(gridPanel);
        //    comboBox.AddItem("opt1");
        //    comboBox.AddItem("opt2");
        //    comboBox.AddItem("opt3");
        //    gridPanel.SetPosition(comboBox, 1, 1);
        //    var tabControl = new TabControl(gridPanel);
        //    tabControl.AddPage("pg1");
        //    tabControl.AddPage("pg2");
        //    tabControl.AddPage("pg3");
        //    tabControl.AddPage("pg4");
        //    gridPanel.SetPosition(tabControl, 2, 0);

        //    var listBox = new ListBox(gridPanel);
        //    listBox.AddRow("row1");
        //    listBox.AddRow("row2");
        //    listBox.AddRow("row3");
        //    gridPanel.SetPosition(listBox, 2, 1);

        //    gridPanel.Layout();
        //    GUI.Test(gridPanel, "GridSizer9_DifferentControls");
        //}

        //[Test]
        //public void TestGridSizer10()
        //{
        //    canvas.Width = 400;
        //    canvas.Height = 600;
        //    var gridPanel = new GridPanel(canvas, ExpansionFlags.Expand);
        //    gridPanel.SetRows(2);
        //    gridPanel.SetColumns(2);
        //    for (int i = 0; i < 2; i++)
        //    {
        //        for (int j = 0; j < 2; j++)
        //        {
        //            var button = new Button(gridPanel) { Text = "grid_" + i + "-" + j };
        //            button.Alignment = AlignmentFlags.Bottom | AlignmentFlags.Right;
        //            gridPanel.SetPosition(button, i, j);
        //        }
        //    }

        //    gridPanel.Layout();
        //    GUI.Test(gridPanel, "GridSizer10_AlignementBottomRight");
        //}
        //[Test]
        //public void TestGridSizer11()
        //{
        //    canvas.Width = 400;
        //    canvas.Height = 600;
        //    var gridPanel = new GridPanel(canvas, ExpansionFlags.Expand);
        //    gridPanel.SetRows(2);
        //    gridPanel.SetColumns(2);

        //    for (int i = 0; i < 1; i++)
        //    {
        //        for (int j = 0; j < 2; j++)
        //        {
        //            var button = new Button(gridPanel) { Text = "grid_" + i + "-" + j };
        //            gridPanel.SetPosition(button, i, j);
        //        }
        //    }
        //    var btn = new Button(gridPanel) { Text = "grid_1-0"};
        //    gridPanel.SetPosition(btn, 1, 0);

        //    var boxPanel = new BoxPanel(gridPanel);
        //    for (int i = 1; i <= 5; ++i)
        //        new Button(boxPanel) { Text = "Button" + Math.Pow(10, i) };
        //    boxPanel.Redimension();
        //    boxPanel.Layout();

        //    gridPanel.SetPosition(boxPanel, 1, 1);

        //    gridPanel.Layout();
        //    GUI.Test(gridPanel, "GridSizer11_BoxPanel");
        //}

        //[Test]
        //public void TestGridSizer12()
        //{
        //    var boxPanel = new BoxPanel(canvas){ Orientation = Orientation.Horizontal};

        //    var gridPanel1 = new GridPanel(boxPanel);
        //    gridPanel1.SetRows(2);
        //    gridPanel1.SetColumns(2);

        //    for (int i = 0; i < 2; i++)
        //    {
        //        for (int j = 0; j < 2; j++)
        //        {
        //            var button = new Button(gridPanel1) { Text = "grid_" + i + "-" + j };
        //            button.Alignment = AlignmentFlags.Bottom | AlignmentFlags.Right;
        //            gridPanel1.SetPosition(button, i, j);
        //        }
        //    }

        //    gridPanel1.Layout();

        //    var gridPanel2 = new GridPanel(boxPanel);
        //    gridPanel2.SetRows(2);
        //    gridPanel2.SetColumns(2);

        //    for (int i = 0; i < 2; i++)
        //    {
        //        for (int j = 0; j < 2; j++)
        //        {
        //            var button = new Button(gridPanel2) { Text = "grid_" + i + "-" + j };
        //            button.Alignment = AlignmentFlags.Bottom | AlignmentFlags.Right;
        //            gridPanel2.SetPosition(button, i, j);
        //        }
        //    }

        //    gridPanel2.Layout();

        //    boxPanel.Redimension();
        //    boxPanel.Layout();

        //    GUI.Test(gridPanel1, "GridSizer12_2GridPanels");
        //}

        //[Test]
        //public void TestRelativePositionSizer1()
        //{
        //    var relativePositionPanel = new RelativePanel(canvas);

        //    for (int i = 0; i < 2; i++)
        //    {
        //        for (int j = 0; j < 2; j++)
        //        {
        //            var button = new Button(relativePositionPanel) { Text = "relative_" + i + "-" + j };
        //            relativePositionPanel.SetPosition(button, 106 * i, 22 * j);
        //        }
        //    }

        //    relativePositionPanel.Layout();
        //    GUI.Test(relativePositionPanel, "relativePanel1");
        //}
        
        //[Test]
        //public void TestRelativePositionSizer2()
        //{
        //    var relativePositionPanel = new RelativePanel(canvas);

        //    for (int i = 0; i < 2; i++)
        //    {
        //        for (int j = 0; j < 2; j++)
        //        {
        //            var button = new Button(relativePositionPanel) { Text = "relative_" + i + "-" + j };
        //            relativePositionPanel.SetPosition(button, 126 * i, 32 * j);
        //        }
        //    }

        //    relativePositionPanel.Layout();
        //    GUI.Test(relativePositionPanel, "relativePanel2");
        //}
        
        //[Test]
        //public void TestRelativePositionSizer3()
        //{
        //    var relativePositionPanel = new RelativePanel(canvas, ExpansionFlags.Shaped);
        //    relativePositionPanel.Width = 200;
        //    relativePositionPanel.Height = 100;
        //    for (int i = 0; i < 2; i++)
        //    {
        //        for (int j = 0; j < 2; j++)
        //        {
        //            var button = new Button(relativePositionPanel) { Text = "relative_" + i + "-" + j };
        //            relativePositionPanel.SetPosition(button, 126 * i, 32 * j);
        //        }
        //    }

        //    relativePositionPanel.Layout();
        //    GUI.Test(relativePositionPanel, "relativePanel3_shaped");
        //}
        
        //[Test]
        //public void TestRelativePositionSizer4()
        //{
        //    var relativePositionPanel = new RelativePanel(canvas, ExpansionFlags.Shaped);
        //    relativePositionPanel.Width = 300;
        //    relativePositionPanel.Height = 50;
        //    for (int i = 0; i < 4; i++)
        //    {
        //        for (int j = 0; j < 3; j++)
        //        {
        //            var button = new Button(relativePositionPanel) { Text = "relative_" + i + "-" + j };
        //            relativePositionPanel.SetPosition(button, 106 * i, 22 * j);
        //        }
        //    }

        //    relativePositionPanel.Layout();
        //    GUI.Test(relativePositionPanel, "relativePanel4_shaped_overflow");
        //}
        
        //[Test]
        //public void TestRelativePositionSizer5()
        //{
        //    canvas.Width = 200;
        //    canvas.Height = 40;
        //    var relativePositionPanel = new RelativePanel(canvas, ExpansionFlags.Expand);

        //    for (int i = 0; i < 4; i++)
        //    {
        //        for (int j = 0; j < 3; j++)
        //        {
        //            var button = new Button(relativePositionPanel) { Text = "relative_" + i + "-" + j };
        //            relativePositionPanel.SetPosition(button, 106 * i, 22 * j);
        //        }
        //    }

        //    relativePositionPanel.Layout();
        //    GUI.Test(relativePositionPanel, "relativePanel5_expand_overflow");
        //}
    }

}
