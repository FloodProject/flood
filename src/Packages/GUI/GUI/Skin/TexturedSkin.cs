﻿using Flood;
using GUI.Skins.Texturing;

namespace GUI.Skins
{
    #region UI element textures
    public struct SkinTextures
    {
        public Bordered StatusBar;
        public Bordered Selection;
        public Bordered Shadow;
        public Bordered Tooltip;

        public struct _Panel
        {
            public Bordered Normal;
            public Bordered Bright;
            public Bordered Dark;
            public Bordered Highlight;
        }

        public struct _Window
        {
            public Bordered Normal;
            public Bordered Inactive;
            public Texturing.Single Close;
            public Texturing.Single Close_Hover;
            public Texturing.Single Close_Down;
            public Texturing.Single Close_Disabled;
        }

        public struct _CheckBox
        {
            public struct _Active
            {
                public Texturing.Single Normal;
                public Texturing.Single Checked;
            }
            public struct _Disabled
            {
                public Texturing.Single Normal;
                public Texturing.Single Checked;
            }

            public _Active Active;
            public _Disabled Disabled;
        }

        public struct _RadioButton
        {
            public struct _Active
            {
                public Texturing.Single Normal;
                public Texturing.Single Checked;
            }
            public struct _Disabled
            {
                public Texturing.Single Normal;
                public Texturing.Single Checked;
            }

            public _Active Active;
            public _Disabled Disabled;
        }

        public struct _TextBox
        {
            public Bordered Normal;
            public Bordered Focus;
            public Bordered Disabled;
        }

        public struct _Tree
        {
            public Bordered Background;
            public Texturing.Single Minus;
            public Texturing.Single Plus;
        }

        public struct _ProgressBar
        {
            public Bordered Back;
            public Bordered Front;
        }

        public struct _Scroller
        {
            public Bordered TrackV;
            public Bordered TrackH;
            public Bordered ButtonV_Normal;
            public Bordered ButtonV_Hover;
            public Bordered ButtonV_Down;
            public Bordered ButtonV_Disabled;
            public Bordered ButtonH_Normal;
            public Bordered ButtonH_Hover;
            public Bordered ButtonH_Down;
            public Bordered ButtonH_Disabled;

            public struct _Button
            {
                public Bordered[] Normal;
                public Bordered[] Hover;
                public Bordered[] Down;
                public Bordered[] Disabled;
            }

            public _Button Button;
        }

        public struct _Menu
        {
            public Texturing.Single RightArrow;
            public Texturing.Single Check;

            public Bordered Strip;
            public Bordered Background;
            public Bordered BackgroundWithMargin;
            public Bordered Hover;
        }

        public struct _Input
        {
            public struct _Button
            {
                public Bordered Normal;
                public Bordered Hovered;
                public Bordered Disabled;
                public Bordered Pressed;
            }

            public struct _ComboBox
            {
                public Bordered Normal;
                public Bordered Hover;
                public Bordered Down;
                public Bordered Disabled;

                public struct _Button
                {
                    public Texturing.Single Normal;
                    public Texturing.Single Hover;
                    public Texturing.Single Down;
                    public Texturing.Single Disabled;
                }

                public _Button Button;
            }

            public struct _Slider
            {
                public struct _H
                {
                    public Texturing.Single Normal;
                    public Texturing.Single Hover;
                    public Texturing.Single Down;
                    public Texturing.Single Disabled;
                }

                public struct _V
                {
                    public Texturing.Single Normal;
                    public Texturing.Single Hover;
                    public Texturing.Single Down;
                    public Texturing.Single Disabled;
                }

                public _H H;
                public _V V;
            }

            public struct _ListBox
            {
                public Bordered Background;
                public Bordered Hovered;
                public Bordered EvenLine;
                public Bordered OddLine;
                public Bordered EvenLineSelected;
                public Bordered OddLineSelected;
            }

            public struct _UpDown
            {
                public struct _Up
                {
                    public Texturing.Single Normal;
                    public Texturing.Single Hover;
                    public Texturing.Single Down;
                    public Texturing.Single Disabled;
                }

                public struct _Down
                {
                    public Texturing.Single Normal;
                    public Texturing.Single Hover;
                    public Texturing.Single Down;
                    public Texturing.Single Disabled;
                }

                public _Up Up;
                public _Down Down;
            }

            public _Button Button;
            public _ComboBox ComboBox;
            public _Slider Slider;
            public _ListBox ListBox;
            public _UpDown UpDown;
        }

        public struct _Tab
        {
            public struct _Bottom
            {
                public Bordered Inactive;
                public Bordered Active;
            }

            public struct _Top
            {
                public Bordered Inactive;
                public Bordered Active;
            }

            public struct _Left
            {
                public Bordered Inactive;
                public Bordered Active;
            }

            public struct _Right
            {
                public Bordered Inactive;
                public Bordered Active;
            }

            public _Bottom Bottom;
            public _Top Top;
            public _Left Left;
            public _Right Right;

            public Bordered Control;
            public Bordered HeaderBar;
        }

        public struct _CategoryList
        {
            public Bordered Outer;
            public Bordered Inner;
            public Bordered Header;
        }

        public _Panel Panel;
        public _Window Window;
        public _CheckBox CheckBox;
        public _RadioButton RadioButton;
        public _TextBox TextBox;
        public _Tree Tree;
        public _ProgressBar ProgressBar;
        public _Scroller Scroller;
        public _Menu Menu;
        public _Input Input;
        public _Tab Tab;
        public _CategoryList CategoryList;
    }
    #endregion

    /// <summary>
    /// Base textured skin.
    /// </summary>
    public class TexturedSkin : Skin
    {
        protected SkinTextures Textures;

        private readonly ResourceHandle<Image> imageHandle;

        /// <summary>
        /// Initializes a new instance of the <see cref="TexturedSkin"/> class.
        /// </summary>
        /// <param name="renderer">Renderer to use.</param>
        /// <param name="imageHandle">Name of the skin texture map.</param>
        /// <param name="defaultFont"> </param>
        public TexturedSkin(Renderers.Renderer renderer, ResourceHandle<Image> imageHandle, Font defaultFont) 
            : base(renderer, defaultFont)
        {
            this.imageHandle = imageHandle;

            InitializeColors();
            InitializeTextures();
        }

        /// <summary>
        /// Performs application-defined tasks associated with freeing, releasing, or resetting unmanaged resources.
        /// </summary>
        public override void Dispose()
        {
            //imageHandle.Dispose();
            base.Dispose();
        }

        #region Initialization
        private void InitializeColors()
        {
            Colors.Window.TitleActive   = Renderer.PixelColor(imageHandle, 4 + 8*0, 508, Color.Red);
            Colors.Window.TitleInactive = Renderer.PixelColor(imageHandle, 4 + 8*1, 508, Color.Yellow);

            Colors.Button.Normal   = Renderer.PixelColor(imageHandle, 4 + 8*2, 508, Color.Yellow);
            Colors.Button.Hover    = Renderer.PixelColor(imageHandle, 4 + 8*3, 508, Color.Yellow);
            Colors.Button.Down     = Renderer.PixelColor(imageHandle, 4 + 8*2, 500, Color.Yellow);
            Colors.Button.Disabled = Renderer.PixelColor(imageHandle, 4 + 8*3, 500, Color.Yellow);

            Colors.Tab.Active.Normal     = Renderer.PixelColor(imageHandle, 4 + 8*4, 508, Color.Yellow);
            Colors.Tab.Active.Hover      = Renderer.PixelColor(imageHandle, 4 + 8*5, 508, Color.Yellow);
            Colors.Tab.Active.Down       = Renderer.PixelColor(imageHandle, 4 + 8*4, 500, Color.Yellow);
            Colors.Tab.Active.Disabled   = Renderer.PixelColor(imageHandle, 4 + 8*5, 500, Color.Yellow);
            Colors.Tab.Inactive.Normal   = Renderer.PixelColor(imageHandle, 4 + 8*6, 508, Color.Yellow);
            Colors.Tab.Inactive.Hover    = Renderer.PixelColor(imageHandle, 4 + 8*7, 508, Color.Yellow);
            Colors.Tab.Inactive.Down     = Renderer.PixelColor(imageHandle, 4 + 8*6, 500, Color.Yellow);
            Colors.Tab.Inactive.Disabled = Renderer.PixelColor(imageHandle, 4 + 8*7, 500, Color.Yellow);

            Colors.Label.Default   = Renderer.PixelColor(imageHandle, 4 + 8*8, 508, Color.Yellow);
            Colors.Label.Bright    = Renderer.PixelColor(imageHandle, 4 + 8*9, 508, Color.Yellow);
            Colors.Label.Dark      = Renderer.PixelColor(imageHandle, 4 + 8*8, 500, Color.Yellow);
            Colors.Label.Highlight = Renderer.PixelColor(imageHandle, 4 + 8*9, 500, Color.Yellow);

            Colors.Tree.Lines    = Renderer.PixelColor(imageHandle, 4 + 8*10, 508, Color.Yellow);
            Colors.Tree.Normal   = Renderer.PixelColor(imageHandle, 4 + 8*11, 508, Color.Yellow);
            Colors.Tree.Hover    = Renderer.PixelColor(imageHandle, 4 + 8*10, 500, Color.Yellow);
            Colors.Tree.Selected = Renderer.PixelColor(imageHandle, 4 + 8*11, 500, Color.Yellow);

            Colors.Properties.Line_Normal     = Renderer.PixelColor(imageHandle, 4 + 8*12, 508, Color.Yellow);
            Colors.Properties.Line_Selected   = Renderer.PixelColor(imageHandle, 4 + 8*13, 508, Color.Yellow);
            Colors.Properties.Line_Hover      = Renderer.PixelColor(imageHandle, 4 + 8*12, 500, Color.Yellow);
            Colors.Properties.Title           = Renderer.PixelColor(imageHandle, 4 + 8*13, 500, Color.Yellow);
            Colors.Properties.Column_Normal   = Renderer.PixelColor(imageHandle, 4 + 8*14, 508, Color.Yellow);
            Colors.Properties.Column_Selected = Renderer.PixelColor(imageHandle, 4 + 8*15, 508, Color.Yellow);
            Colors.Properties.Column_Hover    = Renderer.PixelColor(imageHandle, 4 + 8*14, 500, Color.Yellow);
            Colors.Properties.Border          = Renderer.PixelColor(imageHandle, 4 + 8*15, 500, Color.Yellow);
            Colors.Properties.Label_Normal    = Renderer.PixelColor(imageHandle, 4 + 8*16, 508, Color.Yellow);
            Colors.Properties.Label_Selected  = Renderer.PixelColor(imageHandle, 4 + 8*17, 508, Color.Yellow);
            Colors.Properties.Label_Hover     = Renderer.PixelColor(imageHandle, 4 + 8*16, 500, Color.Yellow);

            Colors.ModalBackground = Renderer.PixelColor(imageHandle, 4 + 8*18, 508, Color.Yellow);
            
            Colors.TooltipText = Renderer.PixelColor(imageHandle, 4 + 8*19, 508, Color.Yellow);

            Colors.Category.Header                  = Renderer.PixelColor(imageHandle, 4 + 8*18, 500, Color.Yellow);
            Colors.Category.Header_Closed           = Renderer.PixelColor(imageHandle, 4 + 8*19, 500, Color.Yellow);
            Colors.Category.Line.Text               = Renderer.PixelColor(imageHandle, 4 + 8*20, 508, Color.Yellow);
            Colors.Category.Line.Text_Hover         = Renderer.PixelColor(imageHandle, 4 + 8*21, 508, Color.Yellow);
            Colors.Category.Line.Text_Selected      = Renderer.PixelColor(imageHandle, 4 + 8*20, 500, Color.Yellow);
            Colors.Category.Line.Button             = Renderer.PixelColor(imageHandle, 4 + 8*21, 500, Color.Yellow);
            Colors.Category.Line.Button_Hover       = Renderer.PixelColor(imageHandle, 4 + 8*22, 508, Color.Yellow);
            Colors.Category.Line.Button_Selected    = Renderer.PixelColor(imageHandle, 4 + 8*23, 508, Color.Yellow);
            Colors.Category.LineAlt.Text            = Renderer.PixelColor(imageHandle, 4 + 8*22, 500, Color.Yellow);
            Colors.Category.LineAlt.Text_Hover      = Renderer.PixelColor(imageHandle, 4 + 8*23, 500, Color.Yellow);
            Colors.Category.LineAlt.Text_Selected   = Renderer.PixelColor(imageHandle, 4 + 8*24, 508, Color.Yellow);
            Colors.Category.LineAlt.Button          = Renderer.PixelColor(imageHandle, 4 + 8*25, 508, Color.Yellow);
            Colors.Category.LineAlt.Button_Hover    = Renderer.PixelColor(imageHandle, 4 + 8*24, 500, Color.Yellow);
            Colors.Category.LineAlt.Button_Selected = Renderer.PixelColor(imageHandle, 4 + 8*25, 500, Color.Yellow);
        }

        private void InitializeTextures()
        {
            Textures.Shadow    = new Bordered(imageHandle, 448, 0, 31, 31, Margin.Eight);
            Textures.Tooltip   = new Bordered(imageHandle, 128, 320, 127, 31, Margin.Eight);
            Textures.StatusBar = new Bordered(imageHandle, 128, 288, 127, 31, Margin.Eight);
            Textures.Selection = new Bordered(imageHandle, 384, 32, 31, 31, Margin.Four);

            Textures.Panel.Normal    = new Bordered(imageHandle, 256, 0, 63, 63, new Margin(16, 16, 16, 16));
            Textures.Panel.Bright    = new Bordered(imageHandle, 256 + 64, 0, 63, 63, new Margin(16, 16, 16, 16));
            Textures.Panel.Dark      = new Bordered(imageHandle, 256, 64, 63, 63, new Margin(16, 16, 16, 16));
            Textures.Panel.Highlight = new Bordered(imageHandle, 256 + 64, 64, 63, 63, new Margin(16, 16, 16, 16));

            Textures.Window.Normal   = new Bordered(imageHandle, 0, 0, 127, 127, new Margin(8, 32, 8, 8));
            Textures.Window.Inactive = new Bordered(imageHandle, 128, 0, 127, 127, new Margin(8, 32, 8, 8));

            Textures.CheckBox.Active.Checked  = new Texturing.Single(imageHandle, 448, 32, 15, 15);
            Textures.CheckBox.Active.Normal   = new Texturing.Single(imageHandle, 464, 32, 15, 15);
            Textures.CheckBox.Disabled.Normal = new Texturing.Single(imageHandle, 448, 48, 15, 15);
            Textures.CheckBox.Disabled.Normal = new Texturing.Single(imageHandle, 464, 48, 15, 15);

            Textures.RadioButton.Active.Checked  = new Texturing.Single(imageHandle, 448, 64, 15, 15);
            Textures.RadioButton.Active.Normal   = new Texturing.Single(imageHandle, 464, 64, 15, 15);
            Textures.RadioButton.Disabled.Normal = new Texturing.Single(imageHandle, 448, 80, 15, 15);
            Textures.RadioButton.Disabled.Normal = new Texturing.Single(imageHandle, 464, 80, 15, 15);

            Textures.TextBox.Normal   = new Bordered(imageHandle, 0, 150, 127, 21, Margin.Four);
            Textures.TextBox.Focus    = new Bordered(imageHandle, 0, 172, 127, 21, Margin.Four);
            Textures.TextBox.Disabled = new Bordered(imageHandle, 0, 193, 127, 21, Margin.Four);

            Textures.Menu.Strip                = new Bordered(imageHandle, 0, 128, 127, 21, Margin.One);
            Textures.Menu.BackgroundWithMargin = new Bordered(imageHandle, 128, 128, 127, 63, new Margin(24, 8, 8, 8));
            Textures.Menu.Background           = new Bordered(imageHandle, 128, 192, 127, 63, Margin.Eight);
            Textures.Menu.Hover                = new Bordered(imageHandle, 128, 256, 127, 31, Margin.Eight);
            Textures.Menu.RightArrow           = new Texturing.Single(imageHandle, 464, 112, 15, 15);
            Textures.Menu.Check                = new Texturing.Single(imageHandle, 448, 112, 15, 15);

            Textures.Tab.Control         = new Bordered(imageHandle, 0, 256, 127, 127, Margin.Eight);
            Textures.Tab.Bottom.Active   = new Bordered(imageHandle, 0, 416, 63, 31, Margin.Eight);
            Textures.Tab.Bottom.Inactive = new Bordered(imageHandle, 0 + 128, 416, 63, 31, Margin.Eight);
            Textures.Tab.Top.Active      = new Bordered(imageHandle, 0, 384, 63, 31, Margin.Eight);
            Textures.Tab.Top.Inactive    = new Bordered(imageHandle, 0 + 128, 384, 63, 31, Margin.Eight);
            Textures.Tab.Left.Active     = new Bordered(imageHandle, 64, 384, 31, 63, Margin.Eight);
            Textures.Tab.Left.Inactive   = new Bordered(imageHandle, 64 + 128, 384, 31, 63, Margin.Eight);
            Textures.Tab.Right.Active    = new Bordered(imageHandle, 96, 384, 31, 63, Margin.Eight);
            Textures.Tab.Right.Inactive  = new Bordered(imageHandle, 96 + 128, 384, 31, 63, Margin.Eight);
            Textures.Tab.HeaderBar       = new Bordered(imageHandle, 128, 352, 127, 31, Margin.Four);

            Textures.Window.Close       = new Texturing.Single(imageHandle, 0, 224, 24, 24);
            Textures.Window.Close_Hover = new Texturing.Single(imageHandle, 32, 224, 24, 24);
            Textures.Window.Close_Hover = new Texturing.Single(imageHandle, 64, 224, 24, 24);
            Textures.Window.Close_Hover = new Texturing.Single(imageHandle, 96, 224, 24, 24);

            Textures.Scroller.TrackV           = new Bordered(imageHandle, 384, 208, 15, 127, Margin.Four);
            Textures.Scroller.ButtonV_Normal   = new Bordered(imageHandle, 384 + 16, 208, 15, 127, Margin.Four);
            Textures.Scroller.ButtonV_Hover    = new Bordered(imageHandle, 384 + 32, 208, 15, 127, Margin.Four);
            Textures.Scroller.ButtonV_Down     = new Bordered(imageHandle, 384 + 48, 208, 15, 127, Margin.Four);
            Textures.Scroller.ButtonV_Disabled = new Bordered(imageHandle, 384 + 64, 208, 15, 127, Margin.Four);
            Textures.Scroller.TrackH           = new Bordered(imageHandle, 384, 128, 127, 15, Margin.Four);
            Textures.Scroller.ButtonH_Normal   = new Bordered(imageHandle, 384, 128 + 16, 127, 15, Margin.Four);
            Textures.Scroller.ButtonH_Hover    = new Bordered(imageHandle, 384, 128 + 32, 127, 15, Margin.Four);
            Textures.Scroller.ButtonH_Down     = new Bordered(imageHandle, 384, 128 + 48, 127, 15, Margin.Four);
            Textures.Scroller.ButtonH_Disabled = new Bordered(imageHandle, 384, 128 + 64, 127, 15, Margin.Four);

            Textures.Scroller.Button.Normal   = new Bordered[4];
            Textures.Scroller.Button.Disabled = new Bordered[4];
            Textures.Scroller.Button.Hover    = new Bordered[4];
            Textures.Scroller.Button.Down     = new Bordered[4];

            Textures.Tree.Background = new Bordered(imageHandle, 256, 128, 127, 127, new Margin(16, 16, 16, 16));
            Textures.Tree.Plus       = new Texturing.Single(imageHandle, 448, 96, 15, 15);
            Textures.Tree.Minus      = new Texturing.Single(imageHandle, 464, 96, 15, 15);

            Textures.Input.Button.Normal   = new Bordered(imageHandle, 480, 0, 31, 31, Margin.Eight);
            Textures.Input.Button.Hovered  = new Bordered(imageHandle, 480, 32, 31, 31, Margin.Eight);
            Textures.Input.Button.Disabled = new Bordered(imageHandle, 480, 64, 31, 31, Margin.Eight);
            Textures.Input.Button.Pressed  = new Bordered(imageHandle, 480, 96, 31, 31, Margin.Eight);

            for (int i = 0; i < 4; i++)
            {
                Textures.Scroller.Button.Normal[i]   = new Bordered(imageHandle, 464 + 0, 208 + i * 16, 15, 15, Margin.Two);
                Textures.Scroller.Button.Hover[i]    = new Bordered(imageHandle, 480, 208 + i * 16, 15, 15, Margin.Two);
                Textures.Scroller.Button.Down[i]     = new Bordered(imageHandle, 464, 272 + i * 16, 15, 15, Margin.Two);
                Textures.Scroller.Button.Disabled[i] = new Bordered(imageHandle, 480 + 48, 272 + i * 16, 15, 15, Margin.Two);
            }

            Textures.Input.ListBox.Background       = new Bordered(imageHandle, 256, 256, 63, 127, Margin.Eight);
            Textures.Input.ListBox.Hovered          = new Bordered(imageHandle, 320, 320, 31, 31, Margin.Eight);
            Textures.Input.ListBox.EvenLine         = new Bordered(imageHandle, 352, 256, 31, 31, Margin.Eight);
            Textures.Input.ListBox.OddLine          = new Bordered(imageHandle, 352, 288, 31, 31, Margin.Eight);
            Textures.Input.ListBox.EvenLineSelected = new Bordered(imageHandle, 320, 270, 31, 31, Margin.Eight);
            Textures.Input.ListBox.OddLineSelected  = new Bordered(imageHandle, 320, 288, 31, 31, Margin.Eight);

            Textures.Input.ComboBox.Normal   = new Bordered(imageHandle, 384, 336, 127, 31, new Margin(8, 8, 32, 8));
            Textures.Input.ComboBox.Hover    = new Bordered(imageHandle, 384, 336 + 32, 127, 31, new Margin(8, 8, 32, 8));
            Textures.Input.ComboBox.Down     = new Bordered(imageHandle, 384, 336 + 64, 127, 31, new Margin(8, 8, 32, 8));
            Textures.Input.ComboBox.Disabled = new Bordered(imageHandle, 384, 336 + 96, 127, 31, new Margin(8, 8, 32, 8));

            Textures.Input.ComboBox.Button.Normal   = new Texturing.Single(imageHandle, 496, 272, 15, 15);
            Textures.Input.ComboBox.Button.Hover    = new Texturing.Single(imageHandle, 496, 272 + 16, 15, 15);
            Textures.Input.ComboBox.Button.Down     = new Texturing.Single(imageHandle, 496, 272 + 32, 15, 15);
            Textures.Input.ComboBox.Button.Disabled = new Texturing.Single(imageHandle, 496, 272 + 48, 15, 15);

            Textures.Input.UpDown.Up.Normal     = new Texturing.Single(imageHandle, 384, 112, 7, 7);
            Textures.Input.UpDown.Up.Hover      = new Texturing.Single(imageHandle, 384 + 8, 112, 7, 7);
            Textures.Input.UpDown.Up.Down       = new Texturing.Single(imageHandle, 384 + 16, 112, 7, 7);
            Textures.Input.UpDown.Up.Disabled   = new Texturing.Single(imageHandle, 384 + 24, 112, 7, 7);
            Textures.Input.UpDown.Down.Normal   = new Texturing.Single(imageHandle, 384, 120, 7, 7);
            Textures.Input.UpDown.Down.Hover    = new Texturing.Single(imageHandle, 384 + 8, 120, 7, 7);
            Textures.Input.UpDown.Down.Down     = new Texturing.Single(imageHandle, 384 + 16, 120, 7, 7);
            Textures.Input.UpDown.Down.Disabled = new Texturing.Single(imageHandle, 384 + 24, 120, 7, 7);

            Textures.ProgressBar.Back  = new Bordered(imageHandle, 384, 0, 31, 31, Margin.Eight);
            Textures.ProgressBar.Front = new Bordered(imageHandle, 384 + 32, 0, 31, 31, Margin.Eight);

            Textures.Input.Slider.H.Normal   = new Texturing.Single(imageHandle, 416, 32, 15, 15);
            Textures.Input.Slider.H.Hover    = new Texturing.Single(imageHandle, 416, 32 + 16, 15, 15);
            Textures.Input.Slider.H.Down     = new Texturing.Single(imageHandle, 416, 32 + 32, 15, 15);
            Textures.Input.Slider.H.Disabled = new Texturing.Single(imageHandle, 416, 32 + 48, 15, 15);

            Textures.Input.Slider.V.Normal   = new Texturing.Single(imageHandle, 416 + 16, 32, 15, 15);
            Textures.Input.Slider.V.Hover    = new Texturing.Single(imageHandle, 416 + 16, 32 + 16, 15, 15);
            Textures.Input.Slider.V.Down     = new Texturing.Single(imageHandle, 416 + 16, 32 + 32, 15, 15);
            Textures.Input.Slider.V.Disabled = new Texturing.Single(imageHandle, 416 + 16, 32 + 48, 15, 15);

            Textures.CategoryList.Outer  = new Bordered(imageHandle, 256, 384, 63, 63, Margin.Eight);
            Textures.CategoryList.Inner  = new Bordered(imageHandle, 256 + 64, 384, 63, 63, new Margin(8, 21, 8, 8));
            Textures.CategoryList.Header = new Bordered(imageHandle, 320, 352, 63, 31, Margin.Eight);
        }
        #endregion

        #region UI elements
        public override void DrawButton(Controls.Control control, bool depressed, bool hovered, bool disabled)
        {
            if (disabled)
            {
                Textures.Input.Button.Disabled.Draw(Renderer, control.RenderBounds);
                return;
            }
            if (depressed)
            {
                Textures.Input.Button.Pressed.Draw(Renderer, control.RenderBounds);
                return;
            }
            if (hovered)
            {
                Textures.Input.Button.Hovered.Draw(Renderer, control.RenderBounds);
                return;
            }
            Textures.Input.Button.Normal.Draw(Renderer, control.RenderBounds);
        }

        public override void DrawMenuRightArrow(Controls.Control control)
        {
            Textures.Menu.RightArrow.Draw(Renderer, control.RenderBounds);
        }

        public override void DrawMenuItem(Controls.Control control, bool submenuOpen, bool isChecked)
        {
            if (submenuOpen || control.IsHovered)
                Textures.Menu.Hover.Draw(Renderer, control.RenderBounds);

            if (isChecked)
                Textures.Menu.Check.Draw(Renderer, new Rectangle(control.RenderBounds.X + 4, control.RenderBounds.Y + 3, 15, 15));
        }

        public override void DrawMenuStrip(Controls.Control control)
        {
            Textures.Menu.Strip.Draw(Renderer, control.RenderBounds);
        }

        public override void DrawMenu(Controls.Control control, bool paddingDisabled)
        {
            if (!paddingDisabled)
            {
                Textures.Menu.BackgroundWithMargin.Draw(Renderer, control.RenderBounds);
                return;
            }

            Textures.Menu.Background.Draw(Renderer, control.RenderBounds);
        }

        public override void DrawShadow(Controls.Control control)
        {
            Rectangle r = control.RenderBounds;
            r.X -= 4;
            r.Y -= 4;
            r.Width += 10;
            r.Height += 10;
            Textures.Shadow.Draw(Renderer, r);
        }

        public override void DrawRadioButton(Controls.Control control, bool selected, bool depressed)
        {
            if (selected)
            {
                if (control.IsDisabled)
                    Textures.RadioButton.Disabled.Checked.Draw(Renderer, control.RenderBounds);
                else
                    Textures.RadioButton.Active.Checked.Draw(Renderer, control.RenderBounds);
            }
            else
            {
                if (control.IsDisabled)
                    Textures.RadioButton.Disabled.Normal.Draw(Renderer, control.RenderBounds);
                else
                    Textures.RadioButton.Active.Normal.Draw(Renderer, control.RenderBounds);
            }
        }

        public override void DrawCheckBox(Controls.Control control, bool selected, bool depressed)
        {
            if (selected)
            {
                if (control.IsDisabled)
                    Textures.CheckBox.Disabled.Checked.Draw(Renderer, control.RenderBounds);
                else
                    Textures.CheckBox.Active.Checked.Draw(Renderer, control.RenderBounds);
            }
            else
            {
                if (control.IsDisabled)
                    Textures.CheckBox.Disabled.Normal.Draw(Renderer, control.RenderBounds);
                else
                    Textures.CheckBox.Active.Normal.Draw(Renderer, control.RenderBounds);
            }
        }

        public override void DrawGroupBox(Controls.Control control, int textStart, int textHeight, int textWidth)
        {
            Rectangle rect = control.RenderBounds;

            rect.Y += (int)(textHeight * 0.5f);
            rect.Height -= (int)(textHeight * 0.5f);

            Color m_colDarker = new Color(0, 50, 60, 50);
            Color m_colLighter = new Color(255, 255, 255, 150);

            Renderer.DrawColor = m_colLighter;

            Renderer.DrawFilledRect(new Rectangle(rect.X + 1, rect.Y + 1, textStart - 3, 1));
            Renderer.DrawFilledRect(new Rectangle(rect.X + 1 + textStart + textWidth, rect.Y + 1, rect.Width - textStart + textWidth - 2, 1));
            Renderer.DrawFilledRect(new Rectangle(rect.X + 1, (rect.Y + rect.Height) - 1, rect.X + rect.Width - 2, 1));

            Renderer.DrawFilledRect(new Rectangle(rect.X + 1, rect.Y + 1, 1, rect.Height));
            Renderer.DrawFilledRect(new Rectangle((rect.X + rect.Width) - 2, rect.Y + 1, 1, rect.Height - 1));

            Renderer.DrawColor = m_colDarker;

            Renderer.DrawFilledRect(new Rectangle(rect.X + 1, rect.Y, textStart - 3, 1));
            Renderer.DrawFilledRect(new Rectangle(rect.X + 1 + textStart + textWidth, rect.Y, rect.Width - textStart - textWidth - 2, 1));
            Renderer.DrawFilledRect(new Rectangle(rect.X + 1, (rect.Y + rect.Height) - 1, rect.X + rect.Width - 2, 1));

            Renderer.DrawFilledRect(new Rectangle(rect.X, rect.Y + 1, 1, rect.Height - 1));
            Renderer.DrawFilledRect(new Rectangle((rect.X + rect.Width) - 1, rect.Y + 1, 1, rect.Height - 1));
        }

        public override void DrawTextBox(Controls.Control control)
        {
            if (control.IsDisabled)
            {
                Textures.TextBox.Disabled.Draw(Renderer, control.RenderBounds);
                return;
            }

            if (control.HasFocus)
                Textures.TextBox.Focus.Draw(Renderer, control.RenderBounds);
            else
                Textures.TextBox.Normal.Draw(Renderer, control.RenderBounds);
        }

        public override void DrawTabButton(Controls.Control control, bool active, Pos dir)
        {
            if (active)
            {
                DrawActiveTabButton(control, dir);
                return;
            }

            if (dir == Pos.Top)
            {
                Textures.Tab.Top.Inactive.Draw(Renderer, control.RenderBounds);
                return;
            }
            if (dir == Pos.Left)
            {
                Textures.Tab.Left.Inactive.Draw(Renderer, control.RenderBounds);
                return;
            }
            if (dir == Pos.Bottom)
            {
                Textures.Tab.Bottom.Inactive.Draw(Renderer, control.RenderBounds);
                return;
            }
            if (dir == Pos.Right)
            {
                Textures.Tab.Right.Inactive.Draw(Renderer, control.RenderBounds);
                return;
            }
        }

        private void DrawActiveTabButton(Controls.Control control, Pos dir)
        {
            if (dir == Pos.Top)
            {
                Textures.Tab.Top.Active.Draw(Renderer, control.RenderBounds.Add(new Rectangle(0, 0, 0, 8)));
                return;
            }
            if (dir == Pos.Left)
            {
                Textures.Tab.Left.Active.Draw(Renderer, control.RenderBounds.Add(new Rectangle(0, 0, 8, 0)));
                return;
            }
            if (dir == Pos.Bottom)
            {
                Textures.Tab.Bottom.Active.Draw(Renderer, control.RenderBounds.Add(new Rectangle(0, -8, 0, 8)));
                return;
            }
            if (dir == Pos.Right)
            {
                Textures.Tab.Right.Active.Draw(Renderer, control.RenderBounds.Add(new Rectangle(-8, 0, 8, 0)));
                return;
            }
        }

        public override void DrawTabControl(Controls.Control control)
        {
            Textures.Tab.Control.Draw(Renderer, control.RenderBounds);
        }

        public override void DrawTabTitleBar(Controls.Control control)
        {
            Textures.Tab.HeaderBar.Draw(Renderer, control.RenderBounds);
        }

        public override void DrawWindow(Controls.Control control, int topHeight, bool inFocus)
        {
            if (inFocus) 
               Textures.Window.Normal.Draw(Renderer, control.RenderBounds);
            else 
                Textures.Window.Inactive.Draw(Renderer, control.RenderBounds);
        }

        public override void DrawHighlight(Controls.Control control)
        {
            Rectangle rect = control.RenderBounds;
            Renderer.DrawColor = new Color(255, 100, 255, 255);
            Renderer.DrawFilledRect(rect);
        }

        public override void DrawScrollBar(Controls.Control control, bool horizontal, bool depressed)
        {
            if (horizontal)
                Textures.Scroller.TrackH.Draw(Renderer, control.RenderBounds);
            else
                Textures.Scroller.TrackV.Draw(Renderer, control.RenderBounds);
        }

        public override void DrawScrollBarBar(Controls.Control control, bool depressed, bool hovered, bool horizontal)
        {
            if (!horizontal)
            {
                if (control.IsDisabled)
                {
                    Textures.Scroller.ButtonV_Disabled.Draw(Renderer, control.RenderBounds);
                    return;
                }

                if (depressed)
                {
                    Textures.Scroller.ButtonV_Down.Draw(Renderer, control.RenderBounds);
                    return;
                }

                if (hovered)
                {
                    Textures.Scroller.ButtonV_Hover.Draw(Renderer, control.RenderBounds);
                    return;
                }

                Textures.Scroller.ButtonV_Normal.Draw(Renderer, control.RenderBounds);
                return;
            }

            if (control.IsDisabled)
            {
                Textures.Scroller.ButtonH_Disabled.Draw(Renderer, control.RenderBounds);
                return;
            }

            if (depressed)
            {
                Textures.Scroller.ButtonH_Down.Draw(Renderer, control.RenderBounds);
                return;
            }

            if (hovered)
            {
                Textures.Scroller.ButtonH_Hover.Draw(Renderer, control.RenderBounds);
                return;
            }

            Textures.Scroller.ButtonH_Normal.Draw(Renderer, control.RenderBounds);
        }

        public override void DrawProgressBar(Controls.Control control, bool horizontal, float progress)
        {
            Rectangle rect = control.RenderBounds;

            if (horizontal)
            {
                Textures.ProgressBar.Back.Draw(Renderer, rect);
                rect.Width = (int) (rect.Width*progress);
                Textures.ProgressBar.Front.Draw(Renderer, rect);
            }
            else
            {
                Textures.ProgressBar.Back.Draw(Renderer, rect);
                rect.Y = (int) (rect.Y + rect.Height*(1 - progress));
                rect.Height = (int)(rect.Height * progress);
                Textures.ProgressBar.Front.Draw(Renderer, rect);
            }
        }

        public override void DrawListBox(Controls.Control control)
        {
            Textures.Input.ListBox.Background.Draw(Renderer, control.RenderBounds);
        }

        public override void DrawListBoxLine(Controls.Control control, bool selected, bool even)
        {
            if (selected)
            {
                if (even)
                {
                    Textures.Input.ListBox.EvenLineSelected.Draw(Renderer, control.RenderBounds);
                    return;
                }
                Textures.Input.ListBox.OddLineSelected.Draw(Renderer, control.RenderBounds);
                return;
            }
            
            if (control.IsHovered)
            {
                Textures.Input.ListBox.Hovered.Draw(Renderer, control.RenderBounds);
                return;
            }

            if (even)
            {
                Textures.Input.ListBox.EvenLine.Draw(Renderer, control.RenderBounds);
                return;
            }

            Textures.Input.ListBox.OddLine.Draw(Renderer, control.RenderBounds);
        }

        public void DrawSliderNotchesH(Rectangle rect, int numNotches, float dist)
        {
            if (numNotches == 0) return;

            float iSpacing = rect.Width / (float)numNotches;
            for (int i = 0; i < numNotches + 1; i++)
                Renderer.DrawFilledRect(Util.FloatRect(rect.X + iSpacing * i, rect.Y + dist - 2, 1, 5));
        }

        public void DrawSliderNotchesV(Rectangle rect, int numNotches, float dist)
        {
            if (numNotches == 0) return;

            float iSpacing = rect.Height / (float)numNotches;
            for (int i = 0; i < numNotches + 1; i++)
                Renderer.DrawFilledRect(Util.FloatRect(rect.X + dist - 2, rect.Y + iSpacing * i, 5, 1));
        }

        public override void DrawSlider(Controls.Control control, bool horizontal, int numNotches, int barSize)
        {
            Rectangle rect = control.RenderBounds;
            Renderer.DrawColor = new Color(0, 0, 0, 100);

            if (horizontal)
            {
                rect.X += (int) (barSize*0.5);
                rect.Width -= barSize;
                rect.Y += (int)(rect.Height * 0.5 - 1);
                rect.Height = 1;
                DrawSliderNotchesH(rect, numNotches, barSize*0.5f);
                Renderer.DrawFilledRect(rect);
                return;
            }

            rect.Y += (int)(barSize * 0.5);
            rect.Height -= barSize;
            rect.X += (int)(rect.Width * 0.5 - 1);
            rect.Width = 1;
            DrawSliderNotchesV(rect, numNotches, barSize * 0.4f);
            Renderer.DrawFilledRect(rect);
        }

        public override void DrawComboBox(Controls.Control control, bool down, bool open)
        {
            if (control.IsDisabled)
            {
                Textures.Input.ComboBox.Disabled.Draw(Renderer, control.RenderBounds);
                return;
            }

            if (down || open)
            {
                Textures.Input.ComboBox.Down.Draw(Renderer, control.RenderBounds);
                return;
            }

            if (control.IsHovered)
            {
                Textures.Input.ComboBox.Down.Draw(Renderer, control.RenderBounds);
                return;
            }

            Textures.Input.ComboBox.Normal.Draw(Renderer, control.RenderBounds);
        }

        public override void DrawKeyboardHighlight(Controls.Control control, Rectangle r, int offset)
        {
            Rectangle rect = r;

            rect.X += offset;
            rect.Y += offset;
            rect.Width -= offset * 2;
            rect.Height -= offset * 2;

            //draw the top and bottom
            bool skip = true;
            for (int i = 0; i < rect.Width * 0.5; i++)
            {
                m_Renderer.DrawColor = Color.Black;
                if (!skip)
                {
                    Renderer.DrawPixel(rect.X + (i * 2), rect.Y);
                    Renderer.DrawPixel(rect.X + (i * 2), rect.Y + rect.Height - 1);
                }
                else
                    skip = false;
            }

            for (int i = 0; i < rect.Height * 0.5; i++)
            {
                Renderer.DrawColor = Color.Black;
                Renderer.DrawPixel(rect.X, rect.Y + i * 2);
                Renderer.DrawPixel(rect.X + rect.Width - 1, rect.Y + i * 2);
            }
        }

        public override void DrawToolTip(Controls.Control control)
        {
            Textures.Tooltip.Draw(Renderer, control.RenderBounds);
        }

        public override void DrawScrollButton(Controls.Control control, Pos direction, bool depressed, bool hovered, bool disabled)
        {
            int i = 0;
            if (direction == Pos.Top) i = 1;
            if (direction == Pos.Right) i = 2;
            if (direction == Pos.Bottom) i = 3;

            if (disabled)
            {
                Textures.Scroller.Button.Disabled[i].Draw(Renderer, control.RenderBounds);
                return;
            }

            if (depressed)
            {
                Textures.Scroller.Button.Down[i].Draw(Renderer, control.RenderBounds);
                return;
            }

            if (hovered)
            {
                Textures.Scroller.Button.Hover[i].Draw(Renderer, control.RenderBounds);
                return;
            }

            Textures.Scroller.Button.Normal[i].Draw(Renderer, control.RenderBounds);
        }

        public override void DrawComboBoxArrow(Controls.Control control, bool hovered, bool down, bool open, bool disabled)
        {
            if (disabled)
            {
                Textures.Input.ComboBox.Button.Disabled.Draw(Renderer, control.RenderBounds);
                return;
            }

            if (down || open)
            {
                Textures.Input.ComboBox.Button.Down.Draw(Renderer, control.RenderBounds);
                return;
            }

            if (hovered)
            {
                Textures.Input.ComboBox.Button.Hover.Draw(Renderer, control.RenderBounds);
                return;
            }

            Textures.Input.ComboBox.Button.Normal.Draw(Renderer, control.RenderBounds);
        }

        public override void DrawNumericUpDownButton(Controls.Control control, bool depressed, bool up)
        {
            if (up)
            {
                if (control.IsDisabled)
                {
                    Textures.Input.UpDown.Up.Disabled.DrawCenter(Renderer, control.RenderBounds);
                    return;
                }

                if (depressed)
                {
                    Textures.Input.UpDown.Up.Down.DrawCenter(Renderer, control.RenderBounds);
                    return;
                }

                if (control.IsHovered)
                {
                    Textures.Input.UpDown.Up.Hover.DrawCenter(Renderer, control.RenderBounds);
                    return;
                }

                Textures.Input.UpDown.Up.Normal.DrawCenter(Renderer, control.RenderBounds);
                return;
            }

            if (control.IsDisabled)
            {
                Textures.Input.UpDown.Down.Disabled.DrawCenter(Renderer, control.RenderBounds);
                return;
            }

            if (depressed)
            {
                Textures.Input.UpDown.Down.Down.DrawCenter(Renderer, control.RenderBounds);
                return;
            }

            if (control.IsHovered)
            {
                Textures.Input.UpDown.Down.Hover.DrawCenter(Renderer, control.RenderBounds);
                return;
            }

            Textures.Input.UpDown.Down.Normal.DrawCenter(Renderer, control.RenderBounds);
        }

        public override void DrawStatusBar(Controls.Control control)
        {
            Textures.StatusBar.Draw(Renderer, control.RenderBounds);
        }

        public override void DrawTreeButton(Controls.Control control, bool open)
        {
            Rectangle rect = control.RenderBounds;

            if (open)
                Textures.Tree.Minus.Draw(Renderer, rect);
            else
                Textures.Tree.Plus.Draw(Renderer, rect);
        }

        public override void DrawTreeControl(Controls.Control control)
        {
            Textures.Tree.Background.Draw(Renderer, control.RenderBounds);
        }

        public override void DrawTreeNode(Controls.Control ctrl, bool open, bool selected, int labelHeight, int labelWidth, int halfWay, int lastBranch, bool isRoot)
        {
            if (selected)
            {
                Textures.Selection.Draw(Renderer, new Rectangle(17, 0, labelWidth + 2, labelHeight - 1));
            }

            base.DrawTreeNode(ctrl, open, selected, labelHeight, labelWidth, halfWay, lastBranch, isRoot);
        }

        public override void DrawColorDisplay(Controls.Control control, Color color)
        {
            Rectangle rect = control.RenderBounds;

            if (color.A != 255)
            {
                Renderer.DrawColor = new Color(255, 255, 255, 255);
                Renderer.DrawFilledRect(rect);

                Renderer.DrawColor = new Color(128, 128, 128, 128);

                Renderer.DrawFilledRect(Util.FloatRect(0, 0, rect.Width * 0.5f, rect.Height * 0.5f));
                Renderer.DrawFilledRect(Util.FloatRect(rect.Width * 0.5f, rect.Height * 0.5f, rect.Width * 0.5f, rect.Height * 0.5f));
            }

            Renderer.DrawColor = color;
            Renderer.DrawFilledRect(rect);

            Renderer.DrawColor = Color.Black;
            Renderer.DrawLinedRect(rect);
        }

        public override void DrawModalControl(Controls.Control control)
        {
            if (!control.ShouldDrawBackground)
                return;
            Rectangle rect = control.RenderBounds;
            Renderer.DrawColor = Colors.ModalBackground;
            Renderer.DrawFilledRect(rect);
        }

        public override void DrawMenuDivider(Controls.Control control)
        {
            Rectangle rect = control.RenderBounds;
            Renderer.DrawColor = new Color(0, 0, 0, 100);
            Renderer.DrawFilledRect(rect);
        }

        public override void DrawWindowCloseButton(Controls.Control control, bool depressed, bool hovered, bool disabled)
        {

            if (disabled)
            {
                Textures.Window.Close_Disabled.Draw(Renderer, control.RenderBounds);
                return;
            }

            if (depressed)
            {
                Textures.Window.Close_Down.Draw(Renderer, control.RenderBounds);
                return;
            }

            if (hovered)
            {
                Textures.Window.Close_Hover.Draw(Renderer, control.RenderBounds);
                return;
            }

            Textures.Window.Close.Draw(Renderer, control.RenderBounds);
        }

        public override void DrawSliderButton(Controls.Control control, bool depressed, bool horizontal)
        {
            if (!horizontal)
            {
                if (control.IsDisabled)
                {
                    Textures.Input.Slider.V.Disabled.DrawCenter(Renderer, control.RenderBounds);
                    return;
                }
                
                if (depressed)
                {
                    Textures.Input.Slider.V.Down.DrawCenter(Renderer, control.RenderBounds);
                    return;
                }
                
                if (control.IsHovered)
                {
                    Textures.Input.Slider.V.Hover.DrawCenter(Renderer, control.RenderBounds);
                    return;
                }

                Textures.Input.Slider.V.Normal.DrawCenter(Renderer, control.RenderBounds);
                return;
            }

            if (control.IsDisabled)
            {
                Textures.Input.Slider.H.Disabled.DrawCenter(Renderer, control.RenderBounds);
                return;
            }

            if (depressed)
            {
                Textures.Input.Slider.H.Down.DrawCenter(Renderer, control.RenderBounds);
                return;
            }

            if (control.IsHovered)
            {
                Textures.Input.Slider.H.Hover.DrawCenter(Renderer, control.RenderBounds);
                return;
            }

            Textures.Input.Slider.H.Normal.DrawCenter(Renderer, control.RenderBounds);
        }

        public override void DrawCategoryHolder(Controls.Control control)
        {
            Textures.CategoryList.Outer.Draw(Renderer, control.RenderBounds);
        }

        public override void DrawCategoryInner(Controls.Control control, bool collapsed)
        {
            if (collapsed)
                Textures.CategoryList.Header.Draw(Renderer, control.RenderBounds);
            else
                Textures.CategoryList.Inner.Draw(Renderer, control.RenderBounds);
        }
        #endregion
    }
}
