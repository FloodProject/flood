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
        public void TestTextClipping()
        {
            canvas.SetSize(64, 22);
            var control = new Button();
            canvas.AddChild(control);
            control.Text = "gpalyq";

            GUI.Test(control, "Clipping", "textClipping1");
        }

        [Test]
        public void TestButton()
        {
            canvas.SetSize(64, 22);
            var control = new Button();
            canvas.AddChild(control);
            control.Text = "Button";

            GUI.Test(control, "Button", "button1");
        }

        [Test]
        public void TestButton2()
        {
            canvas.SetSize(304, 304);
            var control = new Button();
            canvas.AddChild(control);
            control.SetImage("moon.png", AlignmentFlags.Center);

            GUI.Test(control, "Button", "buttonImage");
        }

        [Test]
        public void TestButton3()
        {
            canvas.SetSize(356, 304);
            var control = new Button();
            canvas.AddChild(control);
            control.SetImage("moon.png", AlignmentFlags.Center);
            control.Text = "Moon";

            GUI.Test(control, "Button", "buttonImageText");
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

            GUI.Test(control, "Button", "buttonImageTextVertical");
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

            GUI.Test(control, "Button", "buttonImageTextVerticalSwapped");
        }
        
        [Test]
        public void TestButton6()
        {
            canvas.SetSize(356, 304);
            var control = new Button();
            canvas.AddChild(control);
            control.SetImage("moon.png");
            control.Text = "Moon";
            control.ImageAlignment = AlignmentFlags.Top | AlignmentFlags.Right;
            control.TextAlignment = AlignmentFlags.Bottom | AlignmentFlags.Left;
            control.TextFirst = true;

            GUI.Test(control, "Button", "buttonImageTextHorizontalSwappedAlignment");
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

            GUI.Test(control, "Button", "buttonImageTextShapedAlignment");
        }

        [Test]
        public void TestButton8()
        {
            canvas.SetSize(300, 200);
            var dockPanel = new DockPanel{Expansion = ExpansionFlags.Expand};

            var control = new Button {Text = "Moon", Alignment = AlignmentFlags.Right | AlignmentFlags.Bottom };
            dockPanel.AddChild(control);
            canvas.AddChild(dockPanel);

            GUI.Test(control, "Button", "buttonAlignmentBottomLeft");
            control.Alignment = AlignmentFlags.Center;
            GUI.Test(control, "Button", "buttonAlignmentCenter");
            control.Alignment = AlignmentFlags.Right | AlignmentFlags.CenterVertical;
            GUI.Test(control, "Button", "buttonAlignmentCenterRight");
            control.Alignment = AlignmentFlags.CenterHorizontal | AlignmentFlags.Bottom;
            GUI.Test(control, "Button", "buttonAlignmentCenterBottom");
        }

        [Test]
        public void TestLabel()
        {
            canvas.SetSize(45, 16);
            var control = new Label { Text = "Label" };
            canvas.AddChild(control);

            GUI.Test(control, "Label", "labelSimpleFit");
        }
        
        [Test]
        public void TestLabel2()
        {
            canvas.SetSize(80, 30);
            var control = new Label { Text = "Label2" };
            control.Shape(new Vector2i(80, 30));
            control.TextAlignment = AlignmentFlags.Bottom | AlignmentFlags.Right;
            canvas.AddChild(control);

            GUI.Test(control, "Label", "labelShapedAlignment");
        }
        
        [Test]
        public void TestLabel3()
        {
            canvas.SetSize(80, 30);
            var control = new Label { Text = "Label3" };
            control.Shape(new Vector2i(80, 30)); 
            control.TextAlignment = AlignmentFlags.CenterHorizontal | AlignmentFlags.CenterVertical;
            canvas.AddChild(control);

            GUI.Test(control, "Label", "labelShapedAlignmentCenter");
        }
        
        [Test]
        public void TestLabel4()
        {
            var control = new Label { Text = "Label4" };
            control.Expansion = ExpansionFlags.Expand;
            control.TextAlignment = AlignmentFlags.CenterHorizontal | AlignmentFlags.CenterVertical;
            canvas.AddChild(control);

            GUI.Test(control, "Label", "labelExpandAlignmentCenter");
        }

        [Test]
        public void TestDockPanel()
        {
            canvas.SetSize(107, 32);
            var labelTop = new Label { Text = "LabelTop", Alignment = AlignmentFlags.Top};
            var labelBottom = new Label { Text = "LabelBottom", Alignment = AlignmentFlags.Bottom};
            var control = new DockPanel(); 
            control.Shape(new Vector2i(107, 32));
            control.AddChild(labelTop);
            control.AddChild(labelBottom);
            canvas.AddChild(control);

            GUI.Test(control, "DockPanel", "dockPanelVertical");
        }
        
        [Test]
        public void TestDockPanel2()
        {
            canvas.SetSize(168, 16);
            var labelLeft = new Label { Text = "LabelLeft", Alignment = AlignmentFlags.Left};
            var labelRight = new Label { Text = "LabelRight", Alignment = AlignmentFlags.Right};
            var control = new DockPanel();
            control.Shape(new Vector2i(168, 16)); 
            control.AddChild(labelLeft);
            control.AddChild(labelRight);
            canvas.AddChild(control);

            GUI.Test(control, "DockPanel", "dockPanelHorizontal");
        }

        [Test]
        public void TestDockPanel3()
        {
            canvas.SetSize(107, 32);
            var labelCenter = new Label { Text = "LabelC", Alignment = AlignmentFlags.CenterHorizontal | AlignmentFlags.Top};
            var labelBottom = new Label { Text = "LabelBottom", Alignment = AlignmentFlags.Bottom };
            var control = new DockPanel();
            control.Shape(new Vector2i(107, 32));
            control.AddChild(labelCenter);
            control.AddChild(labelBottom);
            canvas.AddChild(control);

            GUI.Test(control, "DockPanel", "dockPanelCenterHorizontal");
        }
        
        [Test]
        public void TestDockPanel4()
        {
            canvas.SetSize(292, 32);
            var labelTopLeft = new Label { Text = "LabelTopLeft", Alignment = AlignmentFlags.Left | AlignmentFlags.Top};
            var labelTopRight = new Label { Text = "LabelTopRight", Alignment = AlignmentFlags.Left | AlignmentFlags.Right};
            var labelBottomLeft = new Label { Text = "LabelBottomLeft", Alignment = AlignmentFlags.Bottom | AlignmentFlags.Left};
            var labelBottomRight = new Label { Text = "LabelBottomRight", Alignment = AlignmentFlags.Bottom | AlignmentFlags.Right };
            var control = new DockPanel();
            control.Shape(new Vector2i(292, 32));
            control.AddChild(labelTopLeft);
            control.AddChild(labelTopRight);
            control.AddChild(labelBottomLeft);
            control.AddChild(labelBottomRight);
            canvas.AddChild(control);

            GUI.Test(control, "DockPanel", "dockPanel4corners");
        }

        [Test]
        public void TestDockPanel5()
        {
            canvas.SetSize(292, 32);
            var labelCenterTop = new Label { Text = "LabelCenterTop", Alignment = AlignmentFlags.CenterHorizontal | AlignmentFlags.Top};
            var labelTopRight = new Label { Text = "LabelTopRight", Alignment = AlignmentFlags.Left | AlignmentFlags.Right};
            var labelBottomLeft = new Label { Text = "LabelBottomLeft", Alignment = AlignmentFlags.Bottom | AlignmentFlags.Left};
            var labelBottomRight = new Label { Text = "LabelBottomRight", Alignment = AlignmentFlags.Bottom | AlignmentFlags.Right };
            var control = new DockPanel();
            control.Shape(new Vector2i(292, 32));
            control.AddChild(labelCenterTop);
            control.AddChild(labelTopRight);
            control.AddChild(labelBottomLeft);
            control.AddChild(labelBottomRight);
            canvas.AddChild(control);

            GUI.Test(control, "DockPanel", "dockPanel3corners+center");
        }
        
        [Test]
        public void TestDockPanel6()
        {
            canvas.SetSize(312, 52);
            var labelTopLeft = new Label { Text = "LabelTopLeft", Alignment = AlignmentFlags.Top | AlignmentFlags.Left, Margin = Margin.Five};
            var labelTopRight = new Label { Text = "LabelTopRight", Alignment = AlignmentFlags.Top | AlignmentFlags.Right, Margin = Margin.Five };
            var labelBottomLeft = new Label { Text = "LabelBottomLeft", Alignment = AlignmentFlags.Bottom | AlignmentFlags.Left, Margin = Margin.Five };
            var labelBottomRight = new Label { Text = "LabelBottomRight", Alignment = AlignmentFlags.Bottom | AlignmentFlags.Right, Margin = Margin.Five };
            var control = new DockPanel();
            control.Shape(new Vector2i(312, 52));
            control.AddChild(labelTopLeft);
            control.AddChild(labelTopRight);
            control.AddChild(labelBottomLeft);
            control.AddChild(labelBottomRight);
            canvas.AddChild(control);

            GUI.Test(control, "DockPanel", "dockPanel4cornersUniformMargins");
        }
        
        [Test]
        public void TestDockPanel7()
        {
            canvas.SetSize(332, 32);
            var labelTopLeft = new Label { Text = "LabelTopLeft", Alignment = AlignmentFlags.Top
                | AlignmentFlags.Left, Margin = new Margin(10, 0) };
            var labelTopRight = new Label { Text = "LabelTopRight", Alignment = AlignmentFlags.Top
                | AlignmentFlags.Right, Margin = new Margin(10, 0) };
            var labelBottomLeft = new Label { Text = "LabelBottomLeft", Alignment = AlignmentFlags.Bottom 
                | AlignmentFlags.Left, Margin = new Margin(10, 0) };
            var labelBottomRight = new Label { Text = "LabelBottomRight", Alignment = AlignmentFlags.Bottom 
                | AlignmentFlags.Right, Margin = new Margin(10, 0) };
            var control = new DockPanel();
            control.Shape(new Vector2i(332, 32));
            control.AddChild(labelTopLeft);
            control.AddChild(labelTopRight);
            control.AddChild(labelBottomLeft);
            control.AddChild(labelBottomRight);
            canvas.AddChild(control);

            GUI.Test(control, "DockPanel", "dockPanel4cornersHorizontalMargins");
        }

        [Test]
        public void TestDockPanel8()
        {
            canvas.SetSize(292, 72);
            var labelTopLeft = new Label { Text = "LabelTopLeft", Alignment = AlignmentFlags.Top
                | AlignmentFlags.Left, Margin = new Margin(0, 10) };
            var labelTopRight = new Label { Text = "LabelTopRight", Alignment = AlignmentFlags.Top
                | AlignmentFlags.Right, Margin = new Margin(0, 10) };
            var labelBottomLeft = new Label { Text = "LabelBottomLeft", Alignment = AlignmentFlags.Bottom 
                | AlignmentFlags.Left, Margin = new Margin(0, 10) };
            var labelBottomRight = new Label { Text = "LabelBottomRight", Alignment = AlignmentFlags.Bottom 
                | AlignmentFlags.Right, Margin = new Margin(0, 10) };
            var control = new DockPanel();
            control.Shape(new Vector2i(292, 72));
            control.AddChild(labelTopLeft);
            control.AddChild(labelTopRight);
            control.AddChild(labelBottomLeft);
            control.AddChild(labelBottomRight);
            canvas.AddChild(control);

            GUI.Test(control, "DockPanel", "dockPanel4cornersVerticalMargins");
        }

        [Test]
        public void TestDockPanel9()
        {
            canvas.SetSize(371, 126);
            var labelTopLeft = new Label { Text = "LabelTopLeft", Alignment = AlignmentFlags.Top
                | AlignmentFlags.Left, Margin = new Margin(10, 15, 30, 45) };
            var labelTopRight = new Label { Text = "LabelTopRight", Alignment = AlignmentFlags.Top
                | AlignmentFlags.Right, Margin = new Margin(4, 8, 3, 12) };
            var labelBottomLeft = new Label { Text = "LabelBottomLeft", Alignment = AlignmentFlags.Bottom 
                | AlignmentFlags.Left, Margin = new Margin(18, 11, 7, 23) };
            var labelBottomRight = new Label { Text = "LabelBottomRight", Alignment = AlignmentFlags.Bottom 
                | AlignmentFlags.Right, Margin = new Margin(11, 13, 43, 8) };
            var control = new DockPanel();
            control.Shape(new Vector2i(371, 126));
            control.AddChild(labelTopLeft);
            control.AddChild(labelTopRight);
            control.AddChild(labelBottomLeft);
            control.AddChild(labelBottomRight);
            canvas.AddChild(control);

            GUI.Test(control, "DockPanel", "dockPanel4cornersAssymetricalMargins");
        }

        [Test]
        public void TestDockPanel10()
        {
            canvas.SetSize(150, 100);
            var labelTopLeft = new Label { Text = "LabelTopLeft", Alignment = AlignmentFlags.Top | AlignmentFlags.Left };
            var labelTopRight = new Label { Text = "LabelTopRight", Alignment = AlignmentFlags.Top | AlignmentFlags.Right };
            var labelBottomLeft = new Label { Text = "LabelBottomLeft", Alignment = AlignmentFlags.Bottom | AlignmentFlags.Left };
            var labelBottomRight = new Label { Text = "LabelBottomRight", Alignment = AlignmentFlags.Bottom | AlignmentFlags.Right };
            var control = new DockPanel();
            control.Shape(new Vector2i(150, 100)); 
            control.AddChild(labelTopLeft);
            control.AddChild(labelTopRight);
            control.AddChild(labelBottomLeft);
            control.AddChild(labelBottomRight);
            canvas.AddChild(control);

            GUI.Test(control, "DockPanel", "dockPanel4cornersShapedOverflow");
        }

        [Test]
        public void TestDockPanel11()
        {
            canvas.SetSize(350, 100);
            var labelTopLeft = new Label { Text = "LabelTopLeft", Alignment = AlignmentFlags.Top | AlignmentFlags.Left };
            var labelTopRight = new Label { Text = "LabelTopRight", Alignment = AlignmentFlags.Top | AlignmentFlags.Right };
            var labelBottomLeft = new Label { Text = "LabelBottomLeft", Alignment = AlignmentFlags.Bottom | AlignmentFlags.Left };
            var labelBottomRight = new Label { Text = "LabelBottomRight", Alignment = AlignmentFlags.Bottom | AlignmentFlags.Right };
            var control = new DockPanel();
            control.Shape(new Vector2i(350, 100)); 
            control.AddChild(labelTopLeft);
            control.AddChild(labelTopRight);
            control.AddChild(labelBottomLeft);
            control.AddChild(labelBottomRight);
            canvas.AddChild(control);

            GUI.Test(control, "DockPanel", "dockPanel4cornersShapedUnderflow");
        }

        [Test]
        public void TestDockPanel12()
        {
            canvas.SetSize(150, 100);
            var labelTopLeft = new Label { Text = "LabelTopLeft", Alignment = AlignmentFlags.Top
                | AlignmentFlags.Left, Margin = Margin.Ten };
            var labelTopRight = new Label { Text = "LabelTopRight", Alignment = AlignmentFlags.Top
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

            GUI.Test(control, "DockPanel", "dockPanelShapedOverflowMargins");
        }

        [Test]
        public void TestDockPanel13()
        {
            canvas.SetSize(350, 100);
            var labelTopLeft = new Label { Text = "LabelTopLeft", Alignment = AlignmentFlags.Top
                | AlignmentFlags.Left, Margin = Margin.Ten };
            var labelTopRight = new Label { Text = "LabelTopRight", Alignment = AlignmentFlags.Top
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

            GUI.Test(control, "DockPanel", "dockPanelShapedUnderflowMargins");
        }

        [Test]
        public void TestTextBox()
        {
            //var control = new TextBox();
            //control.Text = "TextBox";

            //canvas.AddChild(control);
            //GUI.Test(control, "TextBox","TextBox1");
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
            canvas.SetSize(130, 110);
            var boxPanel = new BoxPanel();
            for (int i = 1; i <= 5; ++i)
                boxPanel.AddChild(new Button { Text = "Button" + Math.Pow(10, i) });
            canvas.AddChild(boxPanel);

            GUI.Test(boxPanel, "BoxPanel", "BoxSizer1_AfterLayout");
            canvas.SetSize(540, 22);
            boxPanel.Orientation = Orientation.Horizontal;
            GUI.Test(boxPanel, "BoxPanel", "BoxSizer1_OrientationChanged");
        }

        [Test]
        public void TestBoxSizer2()
        {
            canvas.SetSize(649, 22);
            var boxPanel = new BoxPanel();
            for (int i = 1; i <= 5; ++i)
                boxPanel.AddChild(new Button { Text = "Button" + Math.Pow(10, i) });

            canvas.AddChild(boxPanel);
            boxPanel.Orientation = Orientation.Horizontal;

            boxPanel.AddChild(new Button { Text = "ExtraButton" });

            GUI.Test(boxPanel, "BoxPanel", "BoxSizer2");
        }

        [Test]
        public void TestBoxSizer3()
        {
            canvas.SetSize(130, 110);
            var boxPanel = new BoxPanel();
            for (int i = 1; i <= 5; ++i)
                boxPanel.AddChild(new Button { Text = "Button" + Math.Pow(10, i) });

            canvas.AddChild(boxPanel);

            GUI.Test(boxPanel, "BoxPanel", "BoxSizer3_BeforeSwap");
            boxPanel.SwapChildren(1, 4);
            GUI.Test(boxPanel, "BoxPanel", "BoxSizer3_AfterSwap");
        }

        [Test]
        public void TestBoxSizer4()
        {
            canvas.SetSize(130, 110);
            var boxPanel = new BoxPanel();
            for (int i = 1; i <= 5; ++i)
                boxPanel.AddChild(new Button { Text = "Button" + Math.Pow(10, i) });

            canvas.AddChild(boxPanel);

            GUI.Test(boxPanel, "BoxPanel", "BoxSizer4_BeforeRemoval");
            boxPanel.RemoveChild(2, true);
            canvas.SetSize(130, 88);
            GUI.Test(boxPanel, "BoxPanel", "BoxSizer4_AfterRemoval1");
            boxPanel.RemoveChild(2, false);
            canvas.SetSize(130, 66);
            GUI.Test(boxPanel, "BoxPanel", "BoxSizer4_AfterRemoval2");
        }

        [Test]
        public void TestGridSizer1()
        {
            canvas.SetSize(206, 154);
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
            GUI.Test(gridPanel, "GridPanel", "GridSizer1");
        }

        [Test]
        public void TestGridSizer2()
        {
            canvas.SetSize(206, 154);
            var gridPanel = new GridPanel();
            gridPanel.SetRows(3);
            gridPanel.SetColumns(2);
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    var button = new Button
                                     {
                                         Text = "grid_" + i + "-" + j,
                                         Alignment = (j%2 == 0) ? AlignmentFlags.Center : AlignmentFlags.Right
                                     };
                    gridPanel.AddChild(button, i, j);
                }

            }

            var boxPanel = new BoxPanel();
            for (int i = 1; i <= 5; ++i)
                boxPanel.AddChild(new Button { Text = "Button" + Math.Pow(10, i) });


            gridPanel.AddChild(boxPanel, 2, 0);
            gridPanel.AddChild(new Button { Text = "grid_2-1", Alignment = AlignmentFlags.Center }, 2, 1);

            canvas.AddChild(gridPanel);
            GUI.Test(gridPanel, "GridPanel", "GridSizer2_Alignment");
        }

        [Test]
        public void TestGridSizer3()
        {
            canvas.SetSize(152, 176);
            var gridPanel = new GridPanel();
            gridPanel.SetRows(4);
            gridPanel.SetColumns(3);
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    var button = new Button
                                     {
                                         Text = "grid_" + i + "-" + j,
                                         Alignment = (j%2 == 0) ? AlignmentFlags.Center : AlignmentFlags.Right
                                     };
                    gridPanel.AddChild(button, i, j);
                }
            }

            var boxPanel = new BoxPanel();
            for (int i = 1; i <= 5; ++i)
                boxPanel.AddChild(new Button { Text = "Button" + Math.Pow(10, i) });
            
            gridPanel.AddChild(boxPanel, 3, 0, columnSpan:2);
            gridPanel.AddChild(new Button { Text = "grid_2-1", Alignment = AlignmentFlags.Center }, 2, 1);

            canvas.AddChild(gridPanel);
            GUI.Test(gridPanel, "GridPanel", "GridSizer3_Alignment");
        }

        [Test]
        public void TestGridSizer4()
        {
            canvas.SetSize(400, 600);
            var gridPanel = new GridPanel(ExpansionFlags.FitExpand);
            gridPanel.SetRows(2);
            gridPanel.SetColumns(2);
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    var alignmentHorizontal = (i % 2 == 0) ? "CtrHor" : "Right";
                    var alignmentVertical = (j % 2 == 0) ? "CtrVert" : "Bottom";
                    var button = new Button
                                     {
                                         Text = "grid_" + alignmentVertical + "-" + alignmentHorizontal,
                                         Alignment = (j%2 == 0) ? AlignmentFlags.CenterVertical : AlignmentFlags.Bottom
                                     };
                    button.Alignment |= (i % 2 == 0) ? AlignmentFlags.CenterHorizontal : AlignmentFlags.Right;
                    gridPanel.AddChild(button, i, j);
                }
            }

            canvas.AddChild(gridPanel);
            GUI.Test(gridPanel, "GridPanel", "GridSizer4_Expand");
        }

        [Test]
        public void TestGridSizer5()
        {
            canvas.SetSize(400, 600);
            var gridPanel = new GridPanel(ExpansionFlags.FitExpand);
            gridPanel.SetRows(2);
            gridPanel.SetColumns(2);
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    var button = new Button
                                     {
                                         Text = "grid_" + i + "-" + j, Alignment = AlignmentFlags.Center
                                     };
                    gridPanel.AddChild(button, i, j);
                }
            }

            canvas.AddChild(gridPanel);
            GUI.Test(gridPanel, "GridPanel", "GridSizer5_Expand_Center");
        }

        [Test]
        public void TestGridSizer6()
        {
            canvas.SetSize(400, 600);
            var gridPanel = new GridPanel(ExpansionFlags.FitExpand);
            gridPanel.SetRows(2);
            gridPanel.SetColumns(10);
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    var button = new Button
                                     {
                                         Text = "grid_" + i + "-" + j, Alignment = AlignmentFlags.Center
                                     };
                    gridPanel.AddChild(button, i, j);
                }
            }

            canvas.AddChild(gridPanel);
            GUI.Test(gridPanel, "GridPanel", "GridSizer6_Expand_WidthOverflow");
        }

        [Test]
        public void TestGridSizer7()
        {
            canvas.SetSize(400, 600);
            var gridPanel = new GridPanel(ExpansionFlags.FitExpand);
            gridPanel.SetRows(30);
            gridPanel.SetColumns(2);
            for (int i = 0; i < 30; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    var button = new Button
                                     {
                                         Text = "grid_" + i + "-" + j, Alignment = AlignmentFlags.Center
                                     };
                    gridPanel.AddChild(button, i, j);
                }
            }

            canvas.AddChild(gridPanel);
            GUI.Test(gridPanel, "GridPanel", "GridSizer7_Expand_HeightOverflow");
        }

        [Test]
        public void TestGridSizer8()
        {
            canvas.SetSize(400, 600);
            var gridPanel = new GridPanel(ExpansionFlags.FitExpand);
            gridPanel.SetRows(30);
            gridPanel.SetColumns(20);
            for (int i = 0; i < 30; i++)
            {
                for (int j = 0; j < 20; j++)
                {
                    var button = new Button
                                     {
                                         Text = "grid_" + i + "-" + j, Alignment = AlignmentFlags.Center
                                     };
                    gridPanel.AddChild(button, i, j);
                }
            }

            canvas.AddChild(gridPanel);
            GUI.Test(gridPanel, "GridPanel", "GridSizer8_Expand_Width&HeightOverflow");
        }

        //[Test]
        //public void TestGridSizer9()
        //{
        //    var gridPanel = new GridPanel();
        //    gridPanel.SetRows(3);
        //    gridPanel.SetColumns(2);
        //    var colorPicker = new ColorPicker();
        //    gridPanel.AddChild(colorPicker, 0, 0);
        //    var rbg = new RadioButtonGroup("RadioButtonGroup");
        //    rbg.Text = "RadioButtonGroup2";
        //    rbg.AddOption("opt1");
        //    rbg.AddOption("opt2");
        //    rbg.AddOption("opt3");
        //    gridPanel.AddChild(rbg, 0, 1);

        //    var collapsibleList = new CollapsibleList();
        //    var cat1 = collapsibleList.Add("Category1");
        //    var cat2 = collapsibleList.Add("Category2");
        //    cat1.Add("but1-1");
        //    cat1.Add("but1-2");
        //    cat2.Add("but2-1");
        //    cat2.Add("but2-2");
        //    cat2.Add("but2-3");
        //    cat2.Add("but2-4");
        //    collapsibleList.SizeToContents();
        //    gridPanel.AddChild(collapsibleList, 1, 0);
            
        //    var comboBox = new ComboBox();
        //    comboBox.AddItem("opt1");
        //    comboBox.AddItem("opt2");
        //    comboBox.AddItem("opt3");
        //    gridPanel.AddChild(comboBox, 1, 1);

        //    var tabControl = new TabControl(gridPanel);
        //    tabControl.AddPage("pg1");
        //    tabControl.AddPage("pg2");
        //    tabControl.AddPage("pg3");
        //    tabControl.AddPage("pg4");
        //    gridPanel.AddChild(tabControl, 2, 0);

        //    var listBox = new ListBox();
        //    listBox.AddRow("row1");
        //    listBox.AddRow("row2");
        //    listBox.AddRow("row3");
        //    gridPanel.AddChild(listBox, 2, 1);

        //    canvas.AddChild(gridPanel);
        //    GUI.Test(gridPanel, "GridPanel", "GridSizer9_DifferentControls");
        //}

        [Test]
        public void TestGridSizer10()
        {
            canvas.SetSize(400, 600);
            var gridPanel = new GridPanel(ExpansionFlags.FitExpand);
            gridPanel.SetRows(2);
            gridPanel.SetColumns(2);
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    var button = new Button
                                     {
                                         Text = "grid_" + i + "-" + j,
                                         Alignment = AlignmentFlags.Bottom | AlignmentFlags.Right
                                     };
                    gridPanel.AddChild(button, i, j);
                }
            }

            canvas.AddChild(gridPanel);
            GUI.Test(gridPanel, "GridPanel", "GridSizer10_AlignementBottomRight");
        }
        [Test]
        public void TestGridSizer11()
        {
            canvas.SetSize(400, 600);
            var gridPanel = new GridPanel(ExpansionFlags.FitExpand);
            gridPanel.SetRows(2);
            gridPanel.SetColumns(2);

            for (int i = 0; i < 1; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    var button = new Button { Text = "grid_" + i + "-" + j };
                    gridPanel.AddChild(button, i, j);
                }
            }
            var btn = new Button { Text = "grid_1-0" };
            gridPanel.AddChild(btn, 1, 0);

            var boxPanel = new BoxPanel();
            for (int i = 1; i <= 5; ++i)
                boxPanel.AddChild(new Button { Text = "Button" + Math.Pow(10, i) });
         
            gridPanel.AddChild(boxPanel, 1, 1);

            canvas.AddChild(gridPanel);
            GUI.Test(gridPanel, "GridPanel", "GridSizer11_BoxPanel");
        }

        [Test]
        public void TestGridSizer12()
        {
            canvas.SetSize(356, 52);
            var boxPanel = new BoxPanel { Orientation = Orientation.Horizontal };

            var gridPanel1 = new GridPanel();
            gridPanel1.SetRows(2);
            gridPanel1.SetColumns(2);

            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    var button = new Button
                                     {
                                         Text = "grid1_" + i + "-" + j,
                                         Alignment = AlignmentFlags.Bottom | AlignmentFlags.Right
                                     };
                    gridPanel1.AddChild(button, i, j);
                }
            }

            var gridPanel2 = new GridPanel();
            gridPanel2.SetRows(2);
            gridPanel2.SetColumns(2);

            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    var button = new Button
                                     {
                                         Text = "grid2_" + i + "-" + j,
                                         Margin = Margin.Two,
                                         Alignment = AlignmentFlags.Bottom | AlignmentFlags.Right
                                     };
                    gridPanel2.AddChild(button, i, j);
                }
            }

            boxPanel.AddChild(gridPanel1);
            boxPanel.AddChild(gridPanel2);
            canvas.AddChild(boxPanel);

            GUI.Test(gridPanel1, "GridPanel", "GridSizer12_2GridPanels");
        }

        [Test]
        public void TestRelativePositionSizer1()
        {
            canvas.SetSize(212, 44);
            var relativePositionPanel = new RelativePanel();

            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    var button = new Button { Text = "relative_" + i + "-" + j };
                    relativePositionPanel.AddChild(button, 106 * i, 22 * j);
                }
            }

            canvas.AddChild(relativePositionPanel);
            GUI.Test(relativePositionPanel, "RelativePanel", "relativePanel1");
        }

        [Test]
        public void TestRelativePositionSizer2()
        {
            canvas.SetSize(232, 54);
            var relativePositionPanel = new RelativePanel();

            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    var button = new Button { Text = "relative_" + i + "-" + j };
                    relativePositionPanel.AddChild(button, 126 * i, 32 * j);
                }
            }

            canvas.AddChild(relativePositionPanel);
            GUI.Test(relativePositionPanel, "RelativePanel", "relativePanel2");
        }

        [Test]
        public void TestRelativePositionSizer3()
        {
            canvas.SetSize(217, 49);
            var relativePositionPanel = new RelativePanel();
            relativePositionPanel.Shape(new Vector2i(200, 100));
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    var button = new Button { Text = "relative_" + i + "-" + j };
                    relativePositionPanel.AddChild(button, 106 * i, 22 * j);
                }
            }

            canvas.AddChild(relativePositionPanel);
            GUI.Test(relativePositionPanel, "RelativePanel", "relativePanel3_shaped");
        }

        [Test]
        public void TestRelativePositionSizer4()
        {
            canvas.SetSize(305, 55);
            var relativePositionPanel = new RelativePanel();
            relativePositionPanel.Shape(new Vector2i(300, 50));
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    var button = new Button { Text = "relative_" + i + "-" + j };
                    relativePositionPanel.AddChild(button, 106 * i, 22 * j);
                }
            }

            canvas.AddChild(relativePositionPanel);
            GUI.Test(relativePositionPanel, "RelativePanel", "relativePanel4_shaped_overflow");
        }

        [Test]
        public void TestRelativePositionSizer5()
        {
            canvas.SetSize(200, 40);
            var relativePositionPanel = new RelativePanel(ExpansionFlags.Expand);

            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    var button = new Button { Text = "relative_" + i + "-" + j };
                    relativePositionPanel.AddChild(button, 106 * i, 22 * j);
                }
            }

            canvas.AddChild(relativePositionPanel);
            GUI.Test(relativePositionPanel, "RelativePanel", "relativePanel5_expand_overflow");
        }
        
        [Test]
        public void TestRelativePositionSizer6()
        {
            canvas.SetSize(200, 40);
            var relativePositionPanel = new RelativePanel(ExpansionFlags.Expand);

            var button = new Button { Text = "Not_Visible" };
            relativePositionPanel.AddChild(button, 316, 10);

            canvas.AddChild(relativePositionPanel);
            GUI.Test(relativePositionPanel, "RelativePanel", "relativePanel6_outofcanvasbounds");
        }
        
        [Test]
        public void TestRelativePositionSizer7()
        {
            canvas.SetSize(305, 55);
            var relativePositionPanel = new RelativePanel();
            relativePositionPanel.Shape(new Vector2i(300, 50));
            var button = new Button { Text = "Not_Visible" };
            relativePositionPanel.AddChild(button, 316, 10);

            canvas.AddChild(relativePositionPanel);
            GUI.Test(relativePositionPanel, "RelativePanel", "relativePanel7_outofpanelbounds");
        }

        [Test]
        public void TestRelativePositionSizer8()
        {
            canvas.SetSize(156, 32);

            var relativePositionPanel = new RelativePanel();

            var button = new Button { Text = "relative_Margins", Margin = Margin.Five};
            relativePositionPanel.AddChild(button, 0, 0);

            canvas.AddChild(relativePositionPanel);
            GUI.Test(relativePositionPanel, "RelativePanel", "relativePanel8Margins");
        }
    }

}
