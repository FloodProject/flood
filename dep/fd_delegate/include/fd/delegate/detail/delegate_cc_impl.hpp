// FD.Delegate
// Do not include this header directly.

protected:
  /**
   * function() implementation for free function with calling convention.
   */
  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2>
  self_type & FD_DELEGATE_CC_NAME(function)(FD_DELEGATE_ST UR (FD_DELEGATE_CC *fxn)(FD_DELEGATE_TEMPLATE_ARGS2), bool reset = true)
  {
    typedef struct
    {
      typedef FD_DELEGATE_ST UR (FD_DELEGATE_CC *TFxn)(FD_DELEGATE_TEMPLATE_ARGS2);

      typedef FD_DELEGATE_FUNCTION<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA TFxn> type;
    } select_stub;

    if( reset ) this->clear();
    select_stub::type::init( this->get_holder(), fxn );
    return *this;
  }
