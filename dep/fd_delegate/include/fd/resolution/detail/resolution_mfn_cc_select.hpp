// FD.Resolution
// Do not include this header directly.

  /**
   * select() for member function bind with calling convention.
   */
  template<typename U>
  static UR (FD_DELEGATE_MF_CC U::*select(UR (FD_DELEGATE_MF_CC U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2)))(FD_DELEGATE_TEMPLATE_ARGS2)
  {
    return fxn;
  }

  template<typename U>
  static UR (FD_DELEGATE_MF_CC U::*select(UR (FD_DELEGATE_MF_CC U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2) const))(FD_DELEGATE_TEMPLATE_ARGS2) const
  {
    return fxn;
  }

#if FD_DELEGATE_NUM_ARGS > 0

  /**
   * select() for member function with calling convention.
   */
  template<typename U>
  static UR (FD_DELEGATE_MF_CC U::*select(UR (FD_DELEGATE_MF_CC U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2_SP)))(FD_DELEGATE_TEMPLATE_ARGS2_SP)
  {
    return fxn;
  }

  template<typename U>
  static UR (FD_DELEGATE_MF_CC U::*select(UR (FD_DELEGATE_MF_CC U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2_SP) const))(FD_DELEGATE_TEMPLATE_ARGS2_SP) const
  {
    return fxn;
  }

#endif // #if FD_DELEGATE_NUM_ARGS > 0
