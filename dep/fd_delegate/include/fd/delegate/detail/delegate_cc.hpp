// FD.Delegate
// Do not include this header directly.

  /**
   * assign_to() for free function with calling convention.
   */
  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2>
  FD_DELEGATE_DELEGATE & assign_to(FD_DELEGATE_ST UR (FD_DELEGATE_CC *fxn)(FD_DELEGATE_TEMPLATE_ARGS2), fd::detail::delegate::pointer_tag, bool reset = false)
  {
    this->FD_DELEGATE_CC_NAME(function)( fxn, reset );
    return *this;
  }
