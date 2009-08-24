// FD.Resolution
// Do not include this header directly.

  /**
   * select() for free function with calling convention.
   */
  static FD_DELEGATE_ST UR (FD_DELEGATE_CC *select(FD_DELEGATE_ST UR (FD_DELEGATE_CC *fxn)(FD_DELEGATE_TEMPLATE_ARGS2)))(FD_DELEGATE_TEMPLATE_ARGS2)
  {
    return fxn;
  }
