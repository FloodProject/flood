// FD.Delegate
// Do not include this header directly.

public:
  /**
   * Copy constructor for member function bind with calling convention.
   */
  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2, typename U, typename T>
  FD_DELEGATE_DELEGATE(UR (FD_DELEGATE_MF_CC U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2), T t)
  {
    struct select_stub
    {
      typedef UR (FD_DELEGATE_MF_CC U::*TFxn)(FD_DELEGATE_TEMPLATE_ARGS2);

      typedef typename boost::ct_if<( sizeof( TFxn ) <= detail::delegate::any_fxn_size ),
        BOOST_DEDUCED_TYPENAME impl_class::fast_mfn_delegate,
        BOOST_DEDUCED_TYPENAME impl_class::normal_mfn_delegate
      >::type type;
    };

    select_stub::type::bind( *this, fxn, t, boost::type<U>() );
  }

  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2, typename U, typename T>
  FD_DELEGATE_DELEGATE(UR (FD_DELEGATE_MF_CC U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2) const, T t)
  {
    struct select_stub
    {
      typedef UR (FD_DELEGATE_MF_CC U::*TFxn)(FD_DELEGATE_TEMPLATE_ARGS2) const;

      typedef typename boost::ct_if<( sizeof( TFxn ) <= detail::delegate::any_fxn_size ),
        BOOST_DEDUCED_TYPENAME impl_class::fast_mfn_delegate,
        BOOST_DEDUCED_TYPENAME impl_class::normal_mfn_delegate
      >::type type;
    };

    select_stub::type::bind( *this, fxn, t, boost::type<U>() );
  }

  /**
   * bind() for member function bind with calling convention.
   */
  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2, typename U, typename T>
  FD_DELEGATE_DELEGATE & bind(UR (FD_DELEGATE_MF_CC U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2), T t)
  {
    struct select_stub
    {
      typedef UR (FD_DELEGATE_MF_CC U::*TFxn)(FD_DELEGATE_TEMPLATE_ARGS2);

      typedef typename boost::ct_if<( sizeof( TFxn ) <= detail::delegate::any_fxn_size ),
        BOOST_DEDUCED_TYPENAME impl_class::fast_mfn_delegate,
        BOOST_DEDUCED_TYPENAME impl_class::normal_mfn_delegate
      >::type type;
    };

    select_stub::type::bind( *this, fxn, t, boost::type<U>(), true );
    return *this;
  }

  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2, typename U, typename T>
  FD_DELEGATE_DELEGATE & bind(UR (FD_DELEGATE_MF_CC U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2) const, T t)
  {
    struct select_stub
    {
      typedef UR (FD_DELEGATE_MF_CC U::*TFxn)(FD_DELEGATE_TEMPLATE_ARGS2) const;

      typedef typename boost::ct_if<( sizeof( TFxn ) <= detail::delegate::any_fxn_size ),
        BOOST_DEDUCED_TYPENAME impl_class::fast_mfn_delegate,
        BOOST_DEDUCED_TYPENAME impl_class::normal_mfn_delegate
      >::type type;
    };

    select_stub::type::bind( *this, fxn, t, boost::type<U>(), true );
    return *this;
  }

#if FD_DELEGATE_NUM_ARGS > 0

  /**
   * assgin_to() for member function with calling convention.
   */
  template<typename UR FD_DELEGATE_COMMA_SP FD_DELEGATE_TEMPLATE_PARMS2_SP, typename U>
  FD_DELEGATE_DELEGATE & assign_to(UR (FD_DELEGATE_MF_CC U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2_SP), fd::detail::delegate::mfn_ptr_tag, bool reset = false)
  {
    struct select_stub
    {
      typedef UR (FD_DELEGATE_MF_CC U::*TFxn)(FD_DELEGATE_TEMPLATE_ARGS2_SP);

      typedef typename boost::ct_if<( sizeof( TFxn ) <= detail::delegate::any_fxn_size ),
        BOOST_DEDUCED_TYPENAME impl_class::fast_mfn_delegate,
        BOOST_DEDUCED_TYPENAME impl_class::normal_mfn_delegate
      >::type type;
    };

    select_stub::type::assign_to( *this, fxn, reset );
    return *this;
  }

  template<typename UR FD_DELEGATE_COMMA_SP FD_DELEGATE_TEMPLATE_PARMS2_SP, typename U>
  FD_DELEGATE_DELEGATE & assign_to(UR (FD_DELEGATE_MF_CC U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2_SP) const, fd::detail::delegate::mfn_ptr_tag, bool reset = false)
  {
    struct select_stub
    {
      typedef UR (FD_DELEGATE_MF_CC U::*TFxn)(FD_DELEGATE_TEMPLATE_ARGS2_SP) const;

      typedef typename boost::ct_if<( sizeof( TFxn ) <= detail::delegate::any_fxn_size ),
        BOOST_DEDUCED_TYPENAME impl_class::fast_mfn_delegate,
        BOOST_DEDUCED_TYPENAME impl_class::normal_mfn_delegate
      >::type type;
    };

    select_stub::type::assign_to( *this, fxn, reset );
    return *this;
  }

#endif // #if FD_DELEGATE_NUM_ARGS > 0
