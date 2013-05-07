
namespace TextAddin
{
    public static class FontWeights
  {
    /// <summary>
    /// Specifies a "Thin" font weight.
    /// </summary>
    /// 
    /// <returns>
    /// A value that represents a "Thin" font weight.
    /// </returns>
    public static FontWeight Thin
    {
      get
      {
        return new FontWeight(100);
      }
    }

    /// <summary>
    /// Specifies an "Extra-light" font weight.
    /// </summary>
    /// 
    /// <returns>
    /// A value that represents an "Extra-light" font weight.
    /// </returns>
    public static FontWeight ExtraLight
    {
      get
      {
        return new FontWeight(200);
      }
    }

    /// <summary>
    /// Specifies an "Ultra-light" font weight.
    /// </summary>
    /// 
    /// <returns>
    /// A value that represents an "Ultra-light" font weight.
    /// </returns>
    public static FontWeight UltraLight
    {
      get
      {
        return new FontWeight(200);
      }
    }

    /// <summary>
    /// Specifies a "Light" font weight.
    /// </summary>
    /// 
    /// <returns>
    /// A value that represents a "Light" font weight.
    /// </returns>
    public static FontWeight Light
    {
      get
      {
        return new FontWeight(300);
      }
    }

    /// <summary>
    /// Specifies a "Normal" font weight.
    /// </summary>
    /// 
    /// <returns>
    /// A value that represents a "Normal" font weight.
    /// </returns>
    public static FontWeight Normal
    {
      get
      {
        return new FontWeight(400);
      }
    }

    /// <summary>
    /// Specifies a "Regular" font weight.
    /// </summary>
    /// 
    /// <returns>
    /// A value that represents a "Regular" font weight.
    /// </returns>
    public static FontWeight Regular
    {
      get
      {
        return new FontWeight(400);
      }
    }

    /// <summary>
    /// Specifies a "Medium" font weight.
    /// </summary>
    /// 
    /// <returns>
    /// A value that represents a "Medium" font weight.
    /// </returns>
    public static FontWeight Medium
    {
      get
      {
        return new FontWeight(500);
      }
    }

    /// <summary>
    /// Specifies a "Demi-bold" font weight.
    /// </summary>
    /// 
    /// <returns>
    /// A value that represents a "Demi-bold" font weight.
    /// </returns>
    public static FontWeight DemiBold
    {
      get
      {
        return new FontWeight(600);
      }
    }

    /// <summary>
    /// Specifies a "Semi-bold" font weight.
    /// </summary>
    /// 
    /// <returns>
    /// A value that represents a "Semi-bold" font weight.
    /// </returns>
    public static FontWeight SemiBold
    {
      get
      {
        return new FontWeight(600);
      }
    }

    /// <summary>
    /// Specifies a "Bold" font weight.
    /// </summary>
    /// 
    /// <returns>
    /// A value that represents a "Bold" font weight.
    /// </returns>
    public static FontWeight Bold
    {
      get
      {
        return new FontWeight(700);
      }
    }

    /// <summary>
    /// Specifies an "Extra-bold" font weight.
    /// </summary>
    /// 
    /// <returns>
    /// A value that represents an "Extra-bold" font weight.
    /// </returns>
    public static FontWeight ExtraBold
    {
      get
      {
        return new FontWeight(800);
      }
    }

    /// <summary>
    /// Specifies an "Ultra-bold" font weight.
    /// </summary>
    /// 
    /// <returns>
    /// A value that represents an "Ultra-bold" font weight.
    /// </returns>
    public static FontWeight UltraBold
    {
      get
      {
        return new FontWeight(800);
      }
    }

    /// <summary>
    /// Specifies a "Black" font weight.
    /// </summary>
    /// 
    /// <returns>
    /// A value that represents a "Black" font weight.
    /// </returns>
    public static FontWeight Black
    {
      get
      {
        return new FontWeight(900);
      }
    }

    /// <summary>
    /// Specifies a "Heavy" font weight.
    /// </summary>
    /// 
    /// <returns>
    /// A value that represents a "Heavy" font weight.
    /// </returns>
    public static FontWeight Heavy
    {
      get
      {
        return new FontWeight(900);
      }
    }

    /// <summary>
    /// Specifies an "Extra-black" font weight.
    /// </summary>
    /// 
    /// <returns>
    /// A value that represents an "Extra-black" font weight.
    /// </returns>
    public static FontWeight ExtraBlack
    {
      get
      {
        return new FontWeight(950);
      }
    }

    /// <summary>
    /// Specifies an "Ultra-black" font weight.
    /// </summary>
    /// 
    /// <returns>
    /// A value that represents an "Ultra-black" font weight.
    /// </returns>
    public static FontWeight UltraBlack
    {
      get
      {
        return new FontWeight(950);
      }
    }

    internal static bool FontWeightToString(int weight, out string convertedValue)
    {
      switch (weight)
      {
        case 800:
          convertedValue = "ExtraBold";
          return true;
        case 900:
          convertedValue = "Black";
          return true;
        case 950:
          convertedValue = "ExtraBlack";
          return true;
        case 600:
          convertedValue = "SemiBold";
          return true;
        case 700:
          convertedValue = "Bold";
          return true;
        case 300:
          convertedValue = "Light";
          return true;
        case 400:
          convertedValue = "Normal";
          return true;
        case 500:
          convertedValue = "Medium";
          return true;
        case 100:
          convertedValue = "Thin";
          return true;
        case 200:
          convertedValue = "ExtraLight";
          return true;
        default:
          convertedValue = (string) null;
          return false;
      }
    }
  }
}
