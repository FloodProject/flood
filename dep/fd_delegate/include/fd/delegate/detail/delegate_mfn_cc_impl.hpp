// FD.Delegate
// Do not include this header directly.

protected:
  /**
   * mfn_bind() for member function bind with calling convention.
   */
  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2, typename U, typename T>
  self_type & FD_DELEGATE_CC_NAME(mfn_bind)(UR (FD_DELEGATE_MF_CC U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2), T t, bool reset = true)
  {
    typedef struct
    {
      typedef UR (FD_DELEGATE_MF_CC U::*TFxn)(FD_DELEGATE_TEMPLATE_ARGS2);

      typedef
        FD_DELEGATE_MFN_BIND_ON_PTR<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA BOOST_DEDUCED_TYPENAME boost::remove_pointer<T>::type, TFxn>
        ptr_type;

      typedef
        FD_DELEGATE_MFN_BIND_ON_REF<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA T, TFxn>
        ref_type;

      typedef
        FD_DELEGATE_MFN_BIND_ON_OBJ<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA T, TFxn, Allocator>
        obj_type;

      typedef
        FD_DELEGATE_MFN_BIND_ON_CUSTOM<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA T, TFxn, Allocator>
        custom_type;

      typedef typename boost::ct_if<
        (::boost::is_pointer<T>::value),
        ptr_type,
        boost::ct_if<
          (::boost::is_reference_wrapper<T>::value),
          ref_type,
          boost::ct_if<
            (::fd::detail::delegate::is_<T *>::template pointer_of_<U>::value),
            obj_type,
            custom_type
          >
        >
      >::type type_1;

      typedef typename type_1::type type_2;
      typedef typename type_2::type type;

    } select_stub;

    if( reset ) this->clear();
    select_stub::type::init( this->get_holder(), t, fxn );
    return *this;
  }

  /**
   * mfn_bind_const() for member function bind with calling convention.
   */
  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2, typename U, typename T>
  self_type & FD_DELEGATE_CC_NAME(mfn_bind_const)(UR (FD_DELEGATE_MF_CC U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2) const, T t, bool reset = true)
  {
    typedef struct
    {
      typedef UR (FD_DELEGATE_MF_CC U::*TFxn)(FD_DELEGATE_TEMPLATE_ARGS2) const;

      typedef
        FD_DELEGATE_MFN_BIND_ON_PTR<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA BOOST_DEDUCED_TYPENAME boost::remove_pointer<T>::type, TFxn>
        ptr_type;

      typedef
        FD_DELEGATE_MFN_BIND_ON_REF<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA T, TFxn>
        ref_type;

      typedef
        FD_DELEGATE_MFN_BIND_ON_OBJ<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA T, TFxn, Allocator>
        obj_type;

      typedef
        FD_DELEGATE_MFN_BIND_ON_CUSTOM<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA T, TFxn, Allocator>
        custom_type;

      typedef typename boost::ct_if<
        (::boost::is_pointer<T>::value),
        ptr_type,
        boost::ct_if<
          (::boost::is_reference_wrapper<T>::value),
          ref_type,
          boost::ct_if<
            (::fd::detail::delegate::is_<T *>::template pointer_of_<U>::value),
            obj_type,
            custom_type
          >
        >
      >::type type_1;

      typedef typename type_1::type type_2;
      typedef typename type_2::type type;

    } select_stub;

    if( reset ) this->clear();
    select_stub::type::init( this->get_holder(), t, fxn );
    return *this;
  }

private:
  /**
   * 
   */
  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2, typename U, typename T>
  self_type & fast_mfn_bind(UR (FD_DELEGATE_MF_CC U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2), T t, bool reset)
  {
    this->FD_DELEGATE_CC_NAME(mfn_bind)( fxn, t, reset );
    return *this;
  }

  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2, typename U, typename T>
  self_type & fast_mfn_bind(UR (FD_DELEGATE_MF_CC U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2) const, T t, bool reset)
  {
    this->FD_DELEGATE_CC_NAME(mfn_bind_const)( fxn, t, reset );
    return *this;
  }

#if FD_DELEGATE_NUM_ARGS > 0

protected:
  /**
   * mfn() for member function bind with calling convention.
   */
  template<typename UR FD_DELEGATE_COMMA_SP FD_DELEGATE_TEMPLATE_PARMS2_SP, typename U>
  self_type & FD_DELEGATE_CC_NAME(mfn)(UR (FD_DELEGATE_MF_CC U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2_SP), bool reset = true)
  {
    typedef struct
    {
      typedef UR (FD_DELEGATE_MF_CC U::*TFxn)(FD_DELEGATE_TEMPLATE_ARGS2_SP);

      typedef
        FD_DELEGATE_MFN_ON_PTR<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA TFxn>
        ptr_type;

      typedef
        FD_DELEGATE_MFN_ON_OBJ<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA TFxn>
        obj_type;

      typedef
        FD_DELEGATE_MFN_ON_REF<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA TFxn>
        ref_type;

      typedef
        FD_DELEGATE_MFN_ON_CUSTOM<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA TFxn>
        custom_type;

      typedef typename boost::ct_if<
        (::fd::detail::delegate::is_<T0>::template pointer_of_<U>::value),
        ptr_type,
        boost::ct_if<
          (::boost::is_same<T0, U>::value),
          obj_type,
          boost::ct_if<
            (::fd::detail::delegate::is_<T0>::template reference_of_<U>::value),
            ref_type,
            custom_type
          >
        >
      >::type type_1;

      typedef typename type_1::type type_2;
      typedef typename type_2::type type;

    } select_stub;

    if( reset ) this->clear();
    select_stub::type::init( this->get_holder(), fxn );
    return *this;
  }

  /**
   * mfn_const() for member function bind with calling convention.
   */
  template<typename UR FD_DELEGATE_COMMA_SP FD_DELEGATE_TEMPLATE_PARMS2_SP, typename U>
  self_type & FD_DELEGATE_CC_NAME(mfn_const)(UR (FD_DELEGATE_MF_CC U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2_SP) const, bool reset = true)
  {
    typedef struct
    {
      typedef UR (FD_DELEGATE_MF_CC U::*TFxn)(FD_DELEGATE_TEMPLATE_ARGS2_SP) const;

      typedef
        FD_DELEGATE_MFN_ON_PTR<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA TFxn>
        ptr_type;

      typedef
        FD_DELEGATE_MFN_ON_OBJ<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA TFxn>
        obj_type;

      typedef
        FD_DELEGATE_MFN_ON_REF<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA TFxn>
        ref_type;

      typedef
        FD_DELEGATE_MFN_ON_CUSTOM<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA TFxn>
        custom_type;

      typedef typename boost::ct_if<
        (::fd::detail::delegate::is_<T0>::template pointer_of_<U>::value),
        ptr_type,
        boost::ct_if<
          (::boost::type_traits::ice_or<(::boost::is_same<T0, U>::value), (::boost::is_same<T0, U const>::value)>::value),
          obj_type,
          boost::ct_if<
            (::fd::detail::delegate::is_<T0>::template reference_of_<U>::value),
            ref_type,
            custom_type
          >
        >
      >::type type_1;

      typedef typename type_1::type type_2;
      typedef typename type_2::type type;

    } select_stub;

    if( reset ) this->clear();
    select_stub::type::init( this->get_holder(), fxn );
    return *this;
  }

private:
  /**
   * 
   */
  template<typename UR FD_DELEGATE_COMMA_SP FD_DELEGATE_TEMPLATE_PARMS2_SP, typename U>
  self_type & fast_mfn_assign_to(UR (FD_DELEGATE_MF_CC U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2_SP), bool reset)
  {
    this->FD_DELEGATE_CC_NAME(mfn)( fxn, reset );
    return *this;
  }

  template<typename UR FD_DELEGATE_COMMA_SP FD_DELEGATE_TEMPLATE_PARMS2_SP, typename U>
  self_type & fast_mfn_assign_to(UR (FD_DELEGATE_MF_CC U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2_SP) const, bool reset)
  {
    this->FD_DELEGATE_CC_NAME(mfn_const)( fxn, reset );
    return *this;
  }

#endif // #if FD_DELEGATE_NUM_ARGS > 0
