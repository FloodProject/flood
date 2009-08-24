// FD.Delegate
// Do not include this header directly.

  /**
   * Copy constructor for member function bind with calling convention.
   */
  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2, typename U, typename T>
  delegate(UR (FD_DELEGATE_MF_CC U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2), T t)
    : base_type( fxn, t )
  {
  }

  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2, typename U, typename T>
  delegate(UR (FD_DELEGATE_MF_CC U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2) const, T t)
    : base_type( fxn, t )
  {
  }
