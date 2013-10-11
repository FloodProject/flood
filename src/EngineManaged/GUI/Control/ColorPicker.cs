using Flood.GUI.ControlInternal;
using System;

namespace Flood.GUI.Controls
{
    /// <summary>
    /// RGBA color picker.
    /// </summary>
    public class ColorPicker : Control, IColorPicker
    {
        private Color m_Color;

        /// <summary>
        /// Selected color.
        /// </summary>
        public Color SelectedColor { get { return m_Color; } set { m_Color = value; UpdateControls(); } }

        /// <summary>
        /// Red value of the selected color.
        /// </summary>
        public byte R { get { return m_Color.R; } set { m_Color = new Color(value, m_Color.G, m_Color.B, m_Color.A); } }

        /// <summary>
        /// Green value of the selected color.
        /// </summary>
        public byte G { get { return m_Color.G; } set { m_Color = new Color(m_Color.R, value, m_Color.B, m_Color.A); } }

        /// <summary>
        /// Blue value of the selected color.
        /// </summary>
        public byte B { get { return m_Color.B; } set { m_Color = new Color(m_Color.R, m_Color.G, value, m_Color.A); } }

        /// <summary>
        /// Alpha value of the selected color.
        /// </summary>
        public byte A { get { return m_Color.A; } set { m_Color = new Color(m_Color.R, m_Color.G, m_Color.B, value); } }

        /// <summary>
        /// Invoked when the selected color has been changed.
        /// </summary>
        public event GwenEventHandler ColorChanged;

        /// <summary>
        /// Initializes a new instance of the <see cref="ColorPicker"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public ColorPicker(Control parent) : base(parent)
        {
            MouseInputEnabled = true;

            SetSize(256, 150);
            CreateControls();
            SelectedColor = new Color(50, 60, 70, 255);
        }

        private void CreateColorControl(String name, int y)
        {
            const int colorSize = 12;

            GroupBox colorGroup = new GroupBox(this);
            colorGroup.SetPosition(10, y);
            colorGroup.SetText(name);
            colorGroup.SetSize(160, 35);
            colorGroup.Name = name + "groupbox";

            ColorDisplay disp = new ColorDisplay(colorGroup);
            disp.Name = name;
            disp.SetBounds(0, 10, colorSize, colorSize);

            TextBoxNumeric numeric = new TextBoxNumeric(colorGroup);
            numeric.Name = name + "Box";
            numeric.SetPosition(105, 7);
            numeric.SetSize(26, 16);
            numeric.SelectAllOnFocus = true;
            numeric.TextChanged += NumericTyped;

            HorizontalSlider slider = new HorizontalSlider(colorGroup);
            slider.SetPosition(colorSize + 5, 10);
            slider.SetRange(0, 255);
            slider.SetSize(80, colorSize);
            slider.Name = name + "Slider";
            slider.ValueChanged += SlidersMoved;
        }

        private void NumericTyped(Control control)
        {
            TextBoxNumeric box = control as TextBoxNumeric;
            if (null == box)
                return;

            if (box.Text == string.Empty)
                return;

            var textValue = (byte) box.Value;
            if (textValue > 255) textValue = 255;

            if (box.Name.Contains("Red"))
                R = textValue;

            if (box.Name.Contains("Green"))
                G = textValue;

            if (box.Name.Contains("Blue"))
                B = textValue;

            if (box.Name.Contains("Alpha"))
                A = textValue;

            UpdateControls();
        }

        private void CreateControls()
        {
            const int startY = 5;
            const int height = 35;

            CreateColorControl("Red", startY);
            CreateColorControl("Green", startY + height);
            CreateColorControl("Blue", startY + height*2);
            CreateColorControl("Alpha", startY + height*3);

            GroupBox finalGroup = new GroupBox(this);
            finalGroup.SetPosition(180, 40);
            finalGroup.SetSize(60, 60);
            finalGroup.SetText("Result");
            finalGroup.Name = "ResultGroupBox";
            
            ColorDisplay disp = new ColorDisplay(finalGroup);
            disp.Name = "Result";
            disp.SetBounds(0, 10, 32, 32);
            //disp.DrawCheckers = true;

            //UpdateControls();
        }

        private void UpdateColorControls(String name, Color col, int sliderVal)
        {
            ColorDisplay disp = FindChildByName(name, true) as ColorDisplay;
            disp.Color = col;

            HorizontalSlider slider = FindChildByName(name + "Slider", true) as HorizontalSlider;
            slider.Value = sliderVal;

            TextBoxNumeric box = FindChildByName(name + "Box", true) as TextBoxNumeric;
            box.Value = sliderVal;
        }

        private void UpdateControls()
        {	//This is a little weird, but whatever for now
            UpdateColorControls("Red", new Color(SelectedColor.R, 0, 0, 255), SelectedColor.R);
            UpdateColorControls("Green", new Color(0, SelectedColor.G, 0, 255), SelectedColor.G);
            UpdateColorControls("Blue", new Color(0, 0, SelectedColor.B, 255), SelectedColor.B);
            UpdateColorControls("Alpha", new Color(255, 255, 255, SelectedColor.A), SelectedColor.A);

            ColorDisplay disp = FindChildByName("Result", true) as ColorDisplay;
            disp.Color = SelectedColor;

            if (ColorChanged != null)
                ColorChanged.Invoke(this);
        }

        private void SlidersMoved(Control control)
        {
            /*
            HorizontalSlider* redSlider		= gwen_cast<HorizontalSlider>(	FindChildByName( "RedSlider",   true ) );
            HorizontalSlider* greenSlider	= gwen_cast<HorizontalSlider>(	FindChildByName( "GreenSlider", true ) );
            HorizontalSlider* blueSlider	= gwen_cast<HorizontalSlider>(	FindChildByName( "BlueSlider",  true ) );
            HorizontalSlider* alphaSlider	= gwen_cast<HorizontalSlider>(	FindChildByName( "AlphaSlider", true ) );
            */

            HorizontalSlider slider = control as HorizontalSlider;
            if (slider != null)
                SetColorByName(GetColorFromName(slider.Name), (byte)slider.Value);

            UpdateControls();
            //SetColor( EngineManaged::GUI::Color( redSlider->GetValue(), greenSlider->GetValue(), blueSlider->GetValue(), alphaSlider->GetValue() ) );
        }

        /// <summary>
        /// Lays out the control's interior according to alignment, padding, dock etc.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Layout(Skins.Skin skin)
        {
            base.Layout(skin);

            SizeToChildren(false, true);
            SetSize(Width, Height + 5);

            GroupBox groupBox = FindChildByName("ResultGroupBox", true) as GroupBox;
            if (groupBox != null)
                groupBox.SetPosition(groupBox.X, Height * 0.5f - groupBox.Height * 0.5f);

            //UpdateControls(); // this spams events continuously every tick
        }

        private int GetColorByName(String colorName)
        {
            if (colorName == "Red")
                return SelectedColor.R;
            if (colorName == "Green")
                return SelectedColor.G;
            if (colorName == "Blue")
                return SelectedColor.B;
            if (colorName == "Alpha")
                return SelectedColor.A;
            return 0;
        }

        private static String GetColorFromName(String name)
        {
            if (name.Contains("Red"))
                return "Red";
            if (name.Contains("Green"))
                return "Green";
            if (name.Contains("Blue"))
                return "Blue";
            if (name.Contains("Alpha"))
                return "Alpha";
            return String.Empty;
        }

        private void SetColorByName(String colorName, byte colorValue)
        {
            if (colorName == "Red")
                R = colorValue;
            else if (colorName == "Green")
                G = colorValue;
            else if (colorName == "Blue")
                B = colorValue;
            else if (colorName == "Alpha")
                A = colorValue;
        }

        /// <summary>
        /// Determines whether the Alpha control is visible.
        /// </summary>
        public bool AlphaVisible
        {
            get
            {
                GroupBox gb = FindChildByName("Alphagroupbox", true) as GroupBox;
                return !gb.IsHidden;
            }
            set
            {
                GroupBox gb = FindChildByName("Alphagroupbox", true) as GroupBox;
                gb.IsHidden = !value;
                Invalidate();
            }
        }
    }
}
