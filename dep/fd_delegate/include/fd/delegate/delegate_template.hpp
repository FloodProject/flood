#include "./detail/prolouge.hpp"

// typename T0, typename T1, ..., typename Tn-1
#define FD_DELEGATE_TEMPLATE_PARMS BOOST_PP_ENUM_PARAMS(FD_DELEGATE_NUM_ARGS, typename T)

// typename U0, typename U1, ..., typename Un-1
#define FD_DELEGATE_TEMPLATE_PARMS2 BOOST_PP_ENUM_PARAMS(FD_DELEGATE_NUM_ARGS, typename U)

// typename U1, typename U2, ..., typename Un-1
#define FD_DELEGATE_TEMPLATE_PARM2_SP(J,I,D) typename BOOST_PP_CAT(U, BOOST_PP_INC(I))
#define FD_DELEGATE_TEMPLATE_PARMS2_SP BOOST_PP_ENUM(BOOST_PP_DEC(FD_DELEGATE_NUM_ARGS),FD_DELEGATE_TEMPLATE_PARM2_SP,BOOST_PP_EMPTY)

// T0, T1, ..., Tn-1
#define FD_DELEGATE_TEMPLATE_ARGS BOOST_PP_ENUM_PARAMS(FD_DELEGATE_NUM_ARGS, T)

// U0, U1, ..., Un-1
#define FD_DELEGATE_TEMPLATE_ARGS2 BOOST_PP_ENUM_PARAMS(FD_DELEGATE_NUM_ARGS, U)

// U1, U2, ..., Un-1
#define FD_DELEGATE_TEMPLATE_ARG2_SP(J,I,D) BOOST_PP_CAT(U, BOOST_PP_INC(I))
#define FD_DELEGATE_TEMPLATE_ARGS2_SP BOOST_PP_ENUM(BOOST_PP_DEC(FD_DELEGATE_NUM_ARGS),FD_DELEGATE_TEMPLATE_ARG2_SP,BOOST_PP_EMPTY)

// T0 a0, T1 a1, ..., Tn-1 an-1
#define FD_DELEGATE_PARM(J,I,D) BOOST_PP_CAT(T,I) BOOST_PP_CAT(a,I)
#define FD_DELEGATE_PARMS BOOST_PP_ENUM(FD_DELEGATE_NUM_ARGS,FD_DELEGATE_PARM,BOOST_PP_EMPTY)

// T0 a0; T1 a1; ...; Tn-1 an-1;
#define FD_DELEGATE_ARG_AS_MEMBER(J,I,D) BOOST_PP_CAT(T,I) BOOST_PP_CAT(a,I);
#define FD_DELEGATE_ARGS_AS_MEMBER BOOST_PP_REPEAT(FD_DELEGATE_NUM_ARGS,FD_DELEGATE_ARG_AS_MEMBER,BOOST_PP_EMPTY)

// args.a0, args.a1, ..., args.an-1
#define FD_DELEGATE_BOUND_ARG(J,I,D) args. BOOST_PP_CAT(a,I)
#define FD_DELEGATE_BOUND_ARGS BOOST_PP_ENUM(FD_DELEGATE_NUM_ARGS,FD_DELEGATE_BOUND_ARG,BOOST_PP_EMPTY)

// a0, a1, ..., an-1
#define FD_DELEGATE_ARGS BOOST_PP_ENUM_PARAMS(FD_DELEGATE_NUM_ARGS, a)

// a1, a2, ..., an-1
#define FD_DELEGATE_ARG_SP(J,I,D) BOOST_PP_CAT(a, BOOST_PP_INC(I))
#define FD_DELEGATE_ARGS_SP BOOST_PP_ENUM(BOOST_PP_DEC(FD_DELEGATE_NUM_ARGS),FD_DELEGATE_ARG_SP,BOOST_PP_EMPTY)

// typename boost::call_traits<T0>::param_type a0, typename boost::call_traits<T1>::param_type a1, ..., typename boost::call_traits<Tn-1>::param_type an-1
#define FD_DELEGATE_CALL_TRAITS_PARM(J,I,D) \
  typename boost::call_traits<BOOST_PP_CAT(T,I)>::param_type BOOST_PP_CAT(a,I)
#define FD_DELEGATE_CALL_TRAITS_PARMS BOOST_PP_ENUM(FD_DELEGATE_NUM_ARGS,FD_DELEGATE_CALL_TRAITS_PARM,BOOST_PP_EMPTY)

// typedef T0 arg1_type; typedef T1 arg2_type; ...; typedef Tn-1 argn_type;
#define FD_DELEGATE_ARG_TYPE(J,I,D) \
  typedef BOOST_PP_CAT(T,I) BOOST_PP_CAT(arg, BOOST_PP_CAT(BOOST_PP_INC(I),_type));
#define FD_DELEGATE_ARG_TYPES BOOST_PP_REPEAT(FD_DELEGATE_NUM_ARGS,FD_DELEGATE_ARG_TYPE,BOOST_PP_EMPTY)

// Type of the default allocator
#ifndef FD_DELEGATE_NO_STD_ALLOCATOR
#  if defined(__sgi) && defined(_COMPILER_VERSION) && _COMPILER_VERSION <= 730 && !defined(BOOST_STRICT_CONFIG)
// The library shipping with MIPSpro 7.3.1.3m has a broken allocator<void>
#    define FD_DELEGATE_DEFAULT_ALLOCATOR std::allocator<delegate_base>
#  else
#    define FD_DELEGATE_DEFAULT_ALLOCATOR std::allocator<void>
#  endif
#else
#  define FD_DELEGATE_DEFAULT_ALLOCATOR int
#endif // FD_DELEGATE_NO_STD_ALLOCATOR

// Comma if nonzero number of arguments
#if FD_DELEGATE_NUM_ARGS == 0
#  define FD_DELEGATE_COMMA
#else
#  define FD_DELEGATE_COMMA ,
#endif // FD_DELEGATE_NUM_ARGS > 0

#if FD_DELEGATE_NUM_ARGS == 0 || FD_DELEGATE_NUM_ARGS == 1
#  define FD_DELEGATE_COMMA_SP
#else
#  define FD_DELEGATE_COMMA_SP ,
#endif // FD_DELEGATE_NUM_ARGS > 1

// Class names used in this version of the code
#define FD_DELEGATE_FXN_TABLE           BOOST_JOIN(fxn_table,           FD_DELEGATE_NUM_ARGS)
#define FD_DELEGATE_STUB_BASE           BOOST_JOIN(stub_base,           FD_DELEGATE_NUM_ARGS)
#define FD_DELEGATE_NULL_TYPE           BOOST_JOIN(null_type,           FD_DELEGATE_NUM_ARGS)
#define FD_DELEGATE_INVOKER             BOOST_JOIN(invoker,             FD_DELEGATE_NUM_ARGS)
#define FD_DELEGATE_BIND                BOOST_JOIN(bind,                FD_DELEGATE_NUM_ARGS)
#define FD_DELEGATE_FUNCTOR_PTR         BOOST_JOIN(functor_ptr,         FD_DELEGATE_NUM_ARGS)
#define FD_DELEGATE_FUNCTOR_REF         BOOST_JOIN(functor_ref,         FD_DELEGATE_NUM_ARGS)
#define FD_DELEGATE_FUNCTOR_OBJ         BOOST_JOIN(functor_obj,         FD_DELEGATE_NUM_ARGS)
#define FD_DELEGATE_FUNCTOR_STATELESS   BOOST_JOIN(functor_stateless,   FD_DELEGATE_NUM_ARGS)
#define FD_DELEGATE_MFN_BIND_ON_PTR     BOOST_JOIN(mfn_bind_on_ptr,     FD_DELEGATE_NUM_ARGS)
#define FD_DELEGATE_MFN_BIND_ON_REF     BOOST_JOIN(mfn_bind_on_ref,     FD_DELEGATE_NUM_ARGS)
#define FD_DELEGATE_MFN_BIND_ON_OBJ     BOOST_JOIN(mfn_bind_on_obj,     FD_DELEGATE_NUM_ARGS)
#define FD_DELEGATE_MFN_BIND_ON_CUSTOM  BOOST_JOIN(mfn_bind_on_custom,  FD_DELEGATE_NUM_ARGS)
#define FD_DELEGATE_MFN_ON_PTR          BOOST_JOIN(mfn_on_ptr,          FD_DELEGATE_NUM_ARGS)
#define FD_DELEGATE_MFN_ON_REF          BOOST_JOIN(mfn_on_ref,          FD_DELEGATE_NUM_ARGS)
#define FD_DELEGATE_MFN_ON_OBJ          BOOST_JOIN(mfn_on_obj,          FD_DELEGATE_NUM_ARGS)
#define FD_DELEGATE_MFN_ON_CUSTOM       BOOST_JOIN(mfn_on_custom,       FD_DELEGATE_NUM_ARGS)
#define FD_DELEGATE_FUNCTION            BOOST_JOIN(function,            FD_DELEGATE_NUM_ARGS)
#define FD_DELEGATE_MULTICAST_TYPE      BOOST_JOIN(multicast_type,      FD_DELEGATE_NUM_ARGS)
#define FD_DELEGATE_MULTICAST           BOOST_JOIN(multicast,           FD_DELEGATE_NUM_ARGS)
#define FD_DELEGATE_DELEGATE_IMPL       BOOST_JOIN(delegate_impl,       FD_DELEGATE_NUM_ARGS)
#define FD_DELEGATE_DELEGATE            BOOST_JOIN(delegate,            FD_DELEGATE_NUM_ARGS)
#define FD_DELEGATE_ARGS_STRUCT         BOOST_JOIN(arg,                 FD_DELEGATE_NUM_ARGS)
#define FD_DELEGATE_CALL_BOUND          BOOST_JOIN(call_bound,          FD_DELEGATE_NUM_ARGS)

namespace fd
{

namespace detail { namespace delegate
{

/**
 * Function pointers table.
 */
template<typename R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS>
struct FD_DELEGATE_FXN_TABLE
  : public fxn_table_base
{
  /**
   * invoke() is dependent on the function signature thus it is separated from fxn_table_base.
   */
  R (FD_DELEGATE_FASTCALL *invoke)(delegate_holder const & FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS);

  /**
   * Constructor.
   *
   *  @param fxn_table_base_  [in] Specifies the base function pointers table.
   *  @param invoke_          [in] Specifies the function pointer to invoke().
   */
  FD_DELEGATE_FXN_TABLE(fxn_table_base const & fxn_table_base_,
    R (FD_DELEGATE_FASTCALL *invoke_)(delegate_holder const & FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS) )
    : fxn_table_base( fxn_table_base_ ), invoke( invoke_ )
  {
  }
};  // struct FD_DELEGATE_FXN_TABLE

// ----------------------------------------------------------------------------------------------------

template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename Dummy = int>
struct FD_DELEGATE_ARGS_STRUCT
{
  // T0 a0; T1 a1; ...; Tn-1 an-1;
  FD_DELEGATE_ARGS_AS_MEMBER;
};

// ----------------------------------------------------------------------------------------------------

/**
 * Composites policies to create stub for a specific callable entity.
 * Tstb specifies general policy (behaviors which are dependent on function signature).
 * Tman specifies object manager policy.
 * Tmtc specifies multicast policy.
 */
template<
typename R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS,
typename Tstb, typename Tman, typename Tmtc>
struct FD_DELEGATE_STUB_BASE
{
  typedef FD_DELEGATE_FXN_TABLE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS> fxn_table_type;

  // Gets static function reference table.
  static fxn_table_type const * get_table()
  {
    // Initialized the base function reference table.
    static fxn_table_base const static_ftable_base =
    {
      &Tstb::size_of_fxn, &Tstb::type_of_fxn, &Tstb::compare_equal_same_type, &Tstb::memcmp_delegate, &Tstb::is_functor, &Tstb::is_empty,
      &Tman::copy_obj, &Tman::delete_obj,
      &Tmtc::is_multicast, &Tmtc::add_delegates, &Tmtc::remove_delegates, &Tmtc::find_delegate,
    };

    // Static function reference table.
    static fxn_table_type const static_ftable( static_ftable_base, &Tstb::invoke );
    return &static_ftable;
  }
};  // struct FD_DELEGATE_STUB_BASE

// ----------------------------------------------------------------------------------------------------

/**
 * Primary class template stub implementing invoke() static member function template for
 * non-void returning.
 *
 * This helper base class template to give some relief to those outdated compilers which do
 * not support void returning or template partial specialization.
 */
template<typename R>
struct FD_DELEGATE_INVOKER
{
  /**
   * Base stub for function object invocation which is stored as pointer to the object.
   */
  template
    <
    FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA
    typename F, typename UR FD_DELEGATE_COMMA  FD_DELEGATE_TEMPLATE_PARMS2
    >
  struct functor_ptr
  {
    static R FD_DELEGATE_FASTCALL
      invoke(delegate_holder const & dh FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS)
    {
#if FD_DELEGATE_NUM_ARGS != 0
      typedef FD_DELEGATE_ARGS_STRUCT<FD_DELEGATE_TEMPLATE_ARGS2> args_type;
      args_type const args = { FD_DELEGATE_ARGS };
#endif
      F & f = *static_cast<F *>( const_cast<typename select_void_pointer<F>::type>( dh.obj_ptr ) );
      return static_cast<UR>( f( FD_DELEGATE_BOUND_ARGS ) );
    }
  };  // struct functor_ptr

  /**
   * Base stub for function object invocation which is stored as an a copy of the object.
   */
  template<
    FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA
    typename F, typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2>
  struct functor_obj
  {
    static R FD_DELEGATE_FASTCALL
      invoke(delegate_holder const & dh FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS)
    {
#if FD_DELEGATE_NUM_ARGS != 0
      typedef FD_DELEGATE_ARGS_STRUCT<FD_DELEGATE_TEMPLATE_ARGS2> args_type;
      args_type const args = { FD_DELEGATE_ARGS };
#endif
      return static_cast<UR>( ( *static_cast<F *>( const_cast<typename select_void_pointer<F>::type>( dh.obj_ptr ) ) ) ( FD_DELEGATE_BOUND_ARGS ) );
    }
  };  // struct functor_obj

  /**
   * Base stub for the stateless function object invocation.
   */
  template<
    FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA
    typename F, typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2>
  struct functor_stateless
  {
    static R FD_DELEGATE_FASTCALL
      invoke(delegate_holder const & dh FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS)
    {
#if FD_DELEGATE_NUM_ARGS != 0
      typedef FD_DELEGATE_ARGS_STRUCT<FD_DELEGATE_TEMPLATE_ARGS2> args_type;
      args_type const args = { FD_DELEGATE_ARGS };
#endif
      return static_cast<UR>( F()( FD_DELEGATE_BOUND_ARGS ) );
    }
  };  // struct functor_stateless

  /**
   * Base stub for member function invocation which is called on the pointer to the bound object.
   */
  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename T, typename TFxn>
  struct mfn_bind_on_ptr
  {
    static R FD_DELEGATE_FASTCALL
      invoke(delegate_holder const & dh FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS)
    {
      T * obj = static_cast<T *>( const_cast<typename select_void_pointer<T>::type>( dh.obj_ptr ) );
      return ( static_cast<T *>( const_cast<typename select_void_pointer<T>::type>( dh.obj_ptr ) )->*simplify_mfn<TFxn>::restore( dh.any.mfn_ptr ) )( FD_DELEGATE_ARGS );
    }
  };  // struct mfn_bind_on_ptr

  /**
   * Base stub for member function invocation which is called on the copy of the bound object.
   */
  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename T, typename TFxn>
  struct mfn_bind_on_obj
  {
    static R FD_DELEGATE_FASTCALL
      invoke(delegate_holder const & dh FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS)
    {
      return ( static_cast<T *>( const_cast<typename select_void_pointer<T>::type>( dh.obj_ptr ) )->*simplify_mfn<TFxn>::restore( dh.any.mfn_ptr ) )( FD_DELEGATE_ARGS );
    }
  };  // struct mfn_bind_on_obj

  /**
   * Base stub for member function invocation which is called on the copy of the custom object.
   */
  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename T, typename TFxn>
  struct mfn_bind_on_custom
  {
    static R FD_DELEGATE_FASTCALL
      invoke(delegate_holder const & dh FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS)
    {
      return ( fd::detail::delegate::addressof( *get_pointer( *static_cast<T const *>( dh.obj_ptr ) ) )->*simplify_mfn<TFxn>::restore( dh.any.mfn_ptr ) )( FD_DELEGATE_ARGS );
    }
  };  // struct mfn_bind_on_custom

#if FD_DELEGATE_NUM_ARGS > 0

  /**
   * Base stub for member function invocation which is called on the pointer to the object
   * that is provided as the first argument of the function call.
   */
  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename TFxn>
  struct mfn_on_ptr
  {
    static R FD_DELEGATE_FASTCALL
      invoke(delegate_holder const & dh FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS)
    {
      return ( a0->*simplify_mfn<TFxn>::restore( dh.any.mfn_ptr ) )( FD_DELEGATE_ARGS_SP );
    }
  };  // struct mfn_on_ptr

  /**
   * Base stub for member function invocation which is called on the reference to the object
   * that is provided as the first argument of the function call.
   */
  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename TFxn>
  struct mfn_on_ref
  {
    static R FD_DELEGATE_FASTCALL
      invoke(delegate_holder const & dh FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS)
    {
      return ( a0.*simplify_mfn<TFxn>::restore( dh.any.mfn_ptr ) )( FD_DELEGATE_ARGS_SP );
    }
  };  // struct mfn_on_ref

  /**
   * Base stub for member function invocation which is called on the copy of the object
   * that is provided as the first argument of the function call.
   */
  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename TFxn>
  struct mfn_on_obj
  {
    static R FD_DELEGATE_FASTCALL
      invoke(delegate_holder const & dh FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS)
    {
      return ( T0( a0 ).*simplify_mfn<TFxn>::restore( dh.any.mfn_ptr ) )( FD_DELEGATE_ARGS_SP );
    }
  };  // struct mfn_on_obj

  /**
   * Base stub for member function invocation which is called on the copy of the custom object
   * which provides get_pointer() overload.
   */
  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename TFxn>
  struct mfn_on_custom
  {
    static R FD_DELEGATE_FASTCALL
      invoke(delegate_holder const & dh FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS)
    {
      return ( fd::detail::delegate::addressof( *get_pointer( a0 ) )->*simplify_mfn<TFxn>::restore( dh.any.mfn_ptr ) )( FD_DELEGATE_ARGS_SP );
    }
  };  // struct mfn_on_custom

#endif  // #if FD_DELEGATE_NUM_ARGS > 0

  /**
   * Base stub for free function or static member function invocation.
   */
  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename TFxn>
  struct function
  {
    static R FD_DELEGATE_FASTCALL invoke(delegate_holder const & dh FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS)
    {
      return ( *reinterpret_cast<TFxn>( dh.any.fxn_ptr ) )( FD_DELEGATE_ARGS );
    }
  };  // struct function

  /**
   * Base stub for multicast invocation.
   */
  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename Allocator>
  struct multicast
  {
    typedef typename multicast_info<Allocator>::watch_list_type notification_type;

    static R FD_DELEGATE_FASTCALL
      invoke(delegate_holder const & dh FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS)
    {
      typedef FD_DELEGATE_FXN_TABLE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS> fxn_table_type;

      notification_type notification = static_cast<multicast_info<Allocator> const *>( dh.obj_ptr )->get_watch_list();

      typename notification_type::const_iterator it = notification.begin(), it_e = notification.end();

      R result = default_value<R>::get();
      for(it = std::find_if( it, it_e, is_callable() ); it != it_e; it = std::find_if( ++it, it_e, is_callable() ) )
      {
        delegate_shared_ptr dg_sptr = it->lock();

        delegate_holder const & dh_target = dg_sptr->get_holder();
        result = static_cast<fxn_table_type const *>( dh_target.tbl_ptr )->invoke( dh_target FD_DELEGATE_COMMA FD_DELEGATE_ARGS );
      }
      return result; // Returns the result of the invocation of the last delegate in the list.
    }
  };  // struct multicast

};  // struct FD_DELEGATE_INVOKER

// ----------------------------------------------------------------------------------------------------

/**
* Specialization class template stub implementing invoke() static member function template for
* void returning.
*/
template<>
struct FD_DELEGATE_INVOKER<FD_DELEGATE_VOID_RETURN_TYPE>
{
  /**
   * Base stub for function object invocation which is stored as pointer to the object.
   * void return.
   */
  template<
    FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA
    typename F, typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2>
  struct functor_ptr
  {
    static FD_DELEGATE_VOID_RETURN_TYPE FD_DELEGATE_FASTCALL
      invoke(delegate_holder const & dh FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS)
    {
#if FD_DELEGATE_NUM_ARGS != 0
      typedef FD_DELEGATE_ARGS_STRUCT<FD_DELEGATE_TEMPLATE_ARGS2> args_type;
      args_type const args = { FD_DELEGATE_ARGS };
#endif
      F & f = *static_cast<F *>( const_cast<typename select_void_pointer<F>::type>( dh.obj_ptr ) );
      FD_DELEGATE_RETURN( static_cast<UR>( f( FD_DELEGATE_BOUND_ARGS ) ) );
    }
  };  // struct functor_ptr

  /**
   * Base stub for function object invocation which is stored as an a copy of the object.
   * void return.
   */
  template<
    FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA
    typename F, typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2>
  struct functor_obj
  {
    static FD_DELEGATE_VOID_RETURN_TYPE FD_DELEGATE_FASTCALL
      invoke(delegate_holder const & dh FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS)
    {
#if FD_DELEGATE_NUM_ARGS != 0
      typedef FD_DELEGATE_ARGS_STRUCT<FD_DELEGATE_TEMPLATE_ARGS2> args_type;
      args_type const args = { FD_DELEGATE_ARGS };
#endif

      FD_DELEGATE_RETURN( static_cast<UR>( ( *static_cast<F *>( const_cast<typename select_void_pointer<F>::type>( dh.obj_ptr ) ) )( FD_DELEGATE_BOUND_ARGS ) ) );
    }
  };  // struct functor_obj

  /**
   * Base stub for the stateless function object invocation.
   * void return.
   */
  template<
    FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA
    typename F, typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2>
  struct functor_stateless
  {
    static FD_DELEGATE_VOID_RETURN_TYPE FD_DELEGATE_FASTCALL
      invoke(delegate_holder const & dh FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS)
    {
#if FD_DELEGATE_NUM_ARGS != 0
      typedef FD_DELEGATE_ARGS_STRUCT<FD_DELEGATE_TEMPLATE_ARGS2> args_type;
      args_type const args = { FD_DELEGATE_ARGS };
#endif
      FD_DELEGATE_RETURN( static_cast<UR>( F()( FD_DELEGATE_BOUND_ARGS ) ) );
    }
  };  // struct functor_stateless

  /**
   * Base stub for member function invocation which is called on the pointer to the bound object.
   * void return.
   */
  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename T, typename TFxn>
  struct mfn_bind_on_ptr
  {
    static FD_DELEGATE_VOID_RETURN_TYPE FD_DELEGATE_FASTCALL
      invoke(delegate_holder const & dh FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS)
    {
      FD_DELEGATE_RETURN( ( static_cast<T *>( const_cast<typename select_void_pointer<T>::type>( dh.obj_ptr ) )->*simplify_mfn<TFxn>::restore( dh.any.mfn_ptr ) )( FD_DELEGATE_ARGS ) );
    }
  };  // struct mfn_bind_on_ptr

  /**
   * Base stub for member function invocation which is invoked on the copy of the bound object.
   * void return.
   */
  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename T, typename TFxn>
  struct mfn_bind_on_obj
  {
    static FD_DELEGATE_VOID_RETURN_TYPE FD_DELEGATE_FASTCALL
      invoke(delegate_holder const & dh FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS)
    {
      FD_DELEGATE_RETURN( ( static_cast<T *>( const_cast<typename select_void_pointer<T>::type>( dh.obj_ptr ) )->*simplify_mfn<TFxn>::restore( dh.any.mfn_ptr ) )( FD_DELEGATE_ARGS ) );
    }
  };  // struct mfn_bind_on_obj

  /**
   * Base stub for member function invocation which is invoked on the copy of the custom object.
   * void return.
   */
  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename T, typename TFxn>
  struct mfn_bind_on_custom
  {
    static FD_DELEGATE_VOID_RETURN_TYPE FD_DELEGATE_FASTCALL
      invoke(delegate_holder const & dh FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS)
    {
      FD_DELEGATE_RETURN( ( fd::detail::delegate::addressof( *get_pointer( *static_cast<T const *>( dh.obj_ptr ) ) )->*simplify_mfn<TFxn>::restore( dh.any.mfn_ptr ) )( FD_DELEGATE_ARGS ) );
    }
  };  // struct mfn_bind_on_custom

#if FD_DELEGATE_NUM_ARGS > 0

  /**
   * Base stub for member function invocation which is called on the pointer to the object
   * that is provided as the first argument of the function call.
   * void return.
   */
  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename TFxn>
  struct mfn_on_ptr
  {
    static FD_DELEGATE_VOID_RETURN_TYPE FD_DELEGATE_FASTCALL
      invoke(delegate_holder const & dh FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS)
    {
      FD_DELEGATE_RETURN( ( a0->*simplify_mfn<TFxn>::restore( dh.any.mfn_ptr ) )( FD_DELEGATE_ARGS_SP ) );
    }
  };  // struct mfn_on_ptr

  /**
   * Base stub for member function invocation which is called either on the reference to the object
   * that is provided as the first argument of the function call.
   * void return.
   */
  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename TFxn>
  struct mfn_on_ref
  {
    static FD_DELEGATE_VOID_RETURN_TYPE FD_DELEGATE_FASTCALL
      invoke(delegate_holder const & dh FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS)
    {
      FD_DELEGATE_RETURN( ( a0.*simplify_mfn<TFxn>::restore( dh.any.mfn_ptr ) )( FD_DELEGATE_ARGS_SP ) );
    }
  };  // struct mfn_on_ref

  /**
   * Base stub for member function invocation which is called on the copy of the object
   * that is provided as the first argument of the function call.
   * void return.
   */
  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename TFxn>
  struct mfn_on_obj
  {
    static FD_DELEGATE_VOID_RETURN_TYPE FD_DELEGATE_FASTCALL
      invoke(delegate_holder const & dh FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS)
    {
      FD_DELEGATE_RETURN( ( T0( a0 ).*simplify_mfn<TFxn>::restore( dh.any.mfn_ptr ) )( FD_DELEGATE_ARGS_SP ) );
    }
  };  // struct mfn_on_obj

  /**
   * Base stub for member function invocation which is called on the copy of the custom object
   * which provides get_pointer() overload.
   * void return.
   */
  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename TFxn>
  struct mfn_on_custom
  {
    static FD_DELEGATE_VOID_RETURN_TYPE FD_DELEGATE_FASTCALL
      invoke(delegate_holder const & dh FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS)
    {
      FD_DELEGATE_RETURN( ( fd::detail::delegate::addressof( *get_pointer( a0 ) )->*simplify_mfn<TFxn>::restore( dh.any.mfn_ptr ) )( FD_DELEGATE_ARGS_SP ) );
    }
  };  // struct mfn_on_custom

#endif  // #if FD_DELEGATE_NUM_ARGS > 0

  /**
   * Base stub for free function or static member function invocation.
   * void return.
   */
  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename TFxn>
  struct function
  {
    static FD_DELEGATE_VOID_RETURN_TYPE FD_DELEGATE_FASTCALL
      invoke(delegate_holder const & dh FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS)
    {
      FD_DELEGATE_RETURN( ( *reinterpret_cast<TFxn>( dh.any.fxn_ptr ) )( FD_DELEGATE_ARGS ) );
    }
  };  // struct function

  /**
   * Base stub for multicast invocation.
   * void return.
   */
  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename Allocator>
  struct multicast
  {
    typedef typename multicast_info<Allocator>::watch_list_type notification_type;

    static FD_DELEGATE_VOID_RETURN_TYPE FD_DELEGATE_FASTCALL
      invoke(delegate_holder const & dh FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS)
    {
      typedef FD_DELEGATE_FXN_TABLE<FD_DELEGATE_VOID_RETURN_TYPE FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS> fxn_table_type;

      notification_type notification = static_cast<multicast_info<Allocator> const *>( dh.obj_ptr )->get_watch_list();

      typename notification_type::const_iterator it = notification.begin(), it_e = notification.end();

      for(it = std::find_if( it, it_e, is_callable() ); it != it_e; it = std::find_if( ++it, it_e, is_callable() ) )
      {
        delegate_shared_ptr dg_sptr = it->lock();

        delegate_holder const & dh_target = dg_sptr->get_holder();
        static_cast<fxn_table_type const *>( dh_target.tbl_ptr )->invoke( dh_target FD_DELEGATE_COMMA FD_DELEGATE_ARGS);
      }
      FD_DELEGATE_RETURN( unusable() );
    }
  };  // struct multicast

};  // struct FD_DELEGATE_INVOKER<FD_DELEGATE_VOID_RETURN_TYPE>

// ----------------------------------------------------------------------------------------------------

template<typename R>
struct FD_DELEGATE_BIND
{
  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename T, typename TFxn>
  struct bind_ptr
  {
    typedef bind_ptr type;

    TFxn fxn_; T t_;
    bind_ptr(TFxn fxn, T t) : fxn_( fxn ), t_( t ) { }
    R operator ()(FD_DELEGATE_PARMS) const
    {
      return ( t_->*fxn_ )( FD_DELEGATE_ARGS );
    }
  };

  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename T, typename TFxn>
  struct bind_ref : public bind_ptr<FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA BOOST_DEDUCED_TYPENAME T::type, TFxn>
  {
    typedef bind_ref type;

    bind_ref(TFxn fxn, T t)
     : bind_ptr<FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA BOOST_DEDUCED_TYPENAME T::type, TFxn>( fxn, t.get_pointer() )
    {
    }
  };

  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename T, typename TFxn>
  struct bind_obj
  {
    typedef bind_obj type;

    TFxn fxn_; T t_;
    bind_obj(TFxn fxn, T t) : fxn_( fxn ), t_( t ) { }
    R operator ()(FD_DELEGATE_PARMS) const
    {
      return ( t_.*fxn_ )( FD_DELEGATE_ARGS );
    }
  };

  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename T, typename TFxn>
  struct bind_custom
  {
    typedef bind_custom type;

    TFxn fxn_; T t_;
    bind_custom(TFxn fxn, T t) : fxn_( fxn ), t_( t ) { }
    R operator ()(FD_DELEGATE_PARMS) const
    {
      return ( fd::detail::delegate::addressof( *get_pointer( t_ ) )->*fxn_ )( FD_DELEGATE_ARGS );
    }
  };
};  // struct FD_DELEGATE_BIND

template<>
struct FD_DELEGATE_BIND<FD_DELEGATE_VOID_RETURN_TYPE>
{
  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename T, typename TFxn>
  struct bind_ptr
  {
    typedef bind_ptr type;

    TFxn fxn_; T t_;
    bind_ptr(TFxn fxn, T t) : fxn_( fxn ), t_( t ) { }
    FD_DELEGATE_VOID_RETURN_TYPE operator ()(FD_DELEGATE_PARMS) const
    {
      FD_DELEGATE_RETURN( ( t_->*fxn_ )( FD_DELEGATE_ARGS ) );
    }
  };

  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename T, typename TFxn>
  struct bind_ref : public bind_ptr<FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA BOOST_DEDUCED_TYPENAME T::type, TFxn>
  {
    typedef bind_ref type;

    bind_ref(TFxn fxn, T t)
     : bind_ptr<FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA BOOST_DEDUCED_TYPENAME T::type, TFxn>( fxn, t.get_pointer() )
    {
    }
  };

  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename T, typename TFxn>
  struct bind_obj
  {
    typedef bind_obj type;

    TFxn fxn_; T t_;
    bind_obj(TFxn fxn, T t) : fxn_( fxn ), t_( t ) { }
    FD_DELEGATE_VOID_RETURN_TYPE operator ()(FD_DELEGATE_PARMS) const
    {
      FD_DELEGATE_RETURN( ( t_.*fxn_ )( FD_DELEGATE_ARGS ) );
    }
  };

  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename T, typename TFxn>
  struct bind_custom
  {
    typedef bind_custom type;

    TFxn fxn_; T t_;
    bind_custom(TFxn fxn, T t) : fxn_( fxn ), t_( t ) { }
    FD_DELEGATE_VOID_RETURN_TYPE operator ()(FD_DELEGATE_PARMS) const
    {
      FD_DELEGATE_RETURN( ( fd::detail::delegate::addressof( *get_pointer( t_ ) )->*fxn_ )( FD_DELEGATE_ARGS ) );
    }
  };
};  // struct FD_DELEGATE_BIND<FD_DELEGATE_VOID_RETURN_TYPE>

// ----------------------------------------------------------------------------------------------------

template<typename R>
struct FD_DELEGATE_CALL_BOUND
{
  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename Dummy = int>
  struct caller
  {
    typedef FD_DELEGATE_FXN_TABLE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS> fxn_table_type;
    typedef FD_DELEGATE_ARGS_STRUCT<FD_DELEGATE_TEMPLATE_ARGS> args_type;

    args_type const * args_ptr_;

    typedef R result_type;

    explicit caller(args_type const & args_in) : args_ptr_( &args_in ) { }

    R FD_DELEGATE_FASTCALL operator ()(delegate_weak_ptr const & dg_wptr) const
    {
      delegate_shared_ptr dg_sptr = dg_wptr.lock();

      delegate_holder const & dh = dg_sptr->get_holder();
      BOOST_ASSERT( !static_cast<fxn_table_type const *>( dh.tbl_ptr )->is_empty() );
      args_type const & args = *args_ptr_;
      return static_cast<fxn_table_type const *>( dh.tbl_ptr )->invoke( dh FD_DELEGATE_COMMA FD_DELEGATE_BOUND_ARGS);
    }
  };
};  // struct FD_DELEGATE_CALL_BOUND

template<>
struct FD_DELEGATE_CALL_BOUND<fd::detail::delegate::unusable>
{
  template<FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename Dummy = int>
  struct caller
  {
    typedef FD_DELEGATE_FXN_TABLE<FD_DELEGATE_VOID_RETURN_TYPE FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS> fxn_table_type;
    typedef FD_DELEGATE_ARGS_STRUCT<FD_DELEGATE_TEMPLATE_ARGS> args_type;

    args_type const * args_ptr_;

    typedef fd::detail::delegate::unusable result_type;

    explicit caller(args_type const & args_in) : args_ptr_( &args_in ) { }

    fd::detail::delegate::unusable FD_DELEGATE_FASTCALL operator ()(delegate_weak_ptr const & dg_wptr) const
    {
      delegate_shared_ptr dg_sptr = dg_wptr.lock();

      delegate_holder const & dh = dg_sptr->get_holder();
      BOOST_ASSERT( !static_cast<fxn_table_type const *>( dh.tbl_ptr )->is_empty() );
      args_type const & args = *args_ptr_;
      static_cast<fxn_table_type const *>( dh.tbl_ptr )->invoke( dh FD_DELEGATE_COMMA FD_DELEGATE_BOUND_ARGS);
      return fd::detail::delegate::unusable();
    }
  };
};  // struct FD_DELEGATE_CALL_BOUND<FD_DELEGATE_VOID_RETURN_TYPE>

// ----------------------------------------------------------------------------------------------------

/**
 * Null stub to indicate the delegate is empty.
 *
 * By providing null_typeN, we can avoid NULL check when delegate is being invoked and,
 * most of all, invoke() member function of non empty delegates do not contain the exception
 * (bad_functio_call) handling code.
 */
template<typename R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS>
struct FD_DELEGATE_NULL_TYPE
  : public null_type, public ptr_manager, public single_cast_stub
{
  typedef FD_DELEGATE_FXN_TABLE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS> fxn_table_type;

  static R FD_DELEGATE_FASTCALL invoke(delegate_holder const & FD_DELEGATE_COMMA FD_DELEGATE_CALL_TRAITS_PARMS)
  {
    // Calling an empty delegate.
    boost::throw_exception( bad_function_call() );
    return default_value<R>::get();
  }

  // Gets static function reference table.
  static fxn_table_type const * get_table()
  {
    // Static function reference table.
    static fxn_table_type const static_ftable( *null_type::get_table(), &invoke );
    return &static_ftable;
  }
};  // struct FD_DELEGATE_NULL_TYPE

// ----------------------------------------------------------------------------------------------------

/**
 * Stub for function object which is stored as pointer to the object.
 */
template<
typename R, FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA
typename F, typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2>
struct FD_DELEGATE_FUNCTOR_PTR
  : public FD_DELEGATE_INVOKER<R>::template functor_ptr<FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA F, UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS2>
  , public FD_DELEGATE_STUB_BASE<
  R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS,
  FD_DELEGATE_FUNCTOR_PTR<R, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA F, UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS2>,
  ptr_manager,
  single_cast_stub>
{
  typedef FD_DELEGATE_FUNCTOR_PTR type;

  static void init(delegate_holder & dh, F * f)
  {
    ptr_manager::copy_obj( dh, f );
    dh.tbl_ptr = FD_DELEGATE_FUNCTOR_PTR::get_table();
  }

  static unsigned size_of_fxn(void const *)
  {
    return sizeof( F );
  }

  static std::type_info const & type_of_fxn(void const *)
  {
    return typeid( F );
  }

  static bool compare_equal_same_type(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs)
  {
    return ( dh_lhs.obj_ptr == dh_rhs.obj_ptr ) ||
      compare_equal( *static_cast<F const *>( dh_lhs.obj_ptr ), *static_cast<F const *>( dh_rhs.obj_ptr ) );
  }

  static int memcmp_delegate(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs)
  {
    return dh_util::compare_memory( dh_lhs, dh_rhs );
  }

  static bool is_functor()
  {
    return true;
  }

  static bool is_empty()
  {
    return false;
  }
};  // struct FD_DELEGATE_FUNCTOR_PTR

/**
 * Stub for function object which is stored as reference to the object.
 * Derived from FD_DELEGATE_FUNCTOR_PTR.
 */
template<
typename R, FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA
typename F, typename UR FD_DELEGATE_COMMA  FD_DELEGATE_TEMPLATE_PARMS2>
struct FD_DELEGATE_FUNCTOR_REF
  : public FD_DELEGATE_FUNCTOR_PTR<
  R, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA
  typename F::type, UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS2>
{
  typedef FD_DELEGATE_FUNCTOR_REF type;

  static void init(delegate_holder & dh, F f)
  {
    typedef
      FD_DELEGATE_FUNCTOR_PTR<R, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA BOOST_DEDUCED_TYPENAME F::type, UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS2>
      base_type;

    if( !has_empty_target( f.get_pointer() ) )
    {
      base_type::init( dh, f.get_pointer() );
    }
  }
};  // struct FD_DELEGATE_FUNCTOR_REF

/**
 * Stub for function object which is stored as a copy of the object.
 */
template<
typename R, FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA
typename F, typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2,
typename Allocator>
struct FD_DELEGATE_FUNCTOR_OBJ
  : public FD_DELEGATE_INVOKER<R>::template functor_obj<FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA F, UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS2>
  , public FD_DELEGATE_STUB_BASE<
  R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS,
  FD_DELEGATE_FUNCTOR_OBJ<R, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA F, UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS2, Allocator>,
  obj_manager<F, Allocator>,
  single_cast_stub>
{
  typedef FD_DELEGATE_FUNCTOR_OBJ type;

  static void init(delegate_holder & dh, F f)
  {
    if( !has_empty_target( fd::detail::delegate::addressof( f ) ) )
    {
      obj_manager<F, Allocator>::copy_obj( dh, fd::detail::delegate::addressof( f ) );
      dh.tbl_ptr = FD_DELEGATE_FUNCTOR_OBJ::get_table();
    }
  }

  static unsigned size_of_fxn(void const * obj_ptr)
  {
    return sizeof( *static_cast<F const *>( obj_ptr ) );
  }

  static std::type_info const & type_of_fxn(void const * obj_ptr)
  {
    return typeid( *static_cast<F const *>( obj_ptr ) );
  }

  static bool compare_equal_same_type(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs)
  {
    return ( static_cast<F const *>( dh_lhs.obj_ptr ) == static_cast<F const *>( dh_rhs.obj_ptr ) ) ||
      compare_equal( *static_cast<F const *>( dh_lhs.obj_ptr ) , *static_cast<F const *>( dh_rhs.obj_ptr ) );
  }

  static int memcmp_delegate(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs)
  {
    return dh_util::compare_memory( dh_lhs, dh_rhs );
  }

  static bool is_functor()
  {
    return true;
  }

  static bool is_empty()
  {
    return false;
  }
};  // struct FD_DELEGATE_FUNCTOR_OBJ

/**
 *
 */
template<
typename R, FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA
typename F, typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2>
struct FD_DELEGATE_FUNCTOR_STATELESS
  : public FD_DELEGATE_INVOKER<R>::template functor_stateless<FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA F, UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS2>
  , public FD_DELEGATE_STUB_BASE<
  R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS,
  FD_DELEGATE_FUNCTOR_STATELESS<R, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA F, UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS2>,
  ptr_manager,
  single_cast_stub>
{
  typedef FD_DELEGATE_FUNCTOR_STATELESS type;

  static void init(delegate_holder & dh, F)
  {
    static F stateless_functor;
    ptr_manager::copy_obj( dh, &stateless_functor );
    dh.tbl_ptr = FD_DELEGATE_FUNCTOR_STATELESS::get_table();
  }

  static unsigned size_of_fxn(void const *)
  {
    return sizeof( F );
  }

  static std::type_info const & type_of_fxn(void const *)
  {
    return typeid( F );
  }

  static bool compare_equal_same_type(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs)
  {
    // Compares stateless function objects with same type.
    return true;
  }

  static int memcmp_delegate(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs)
  {
    return dh_util::compare_memory( dh_lhs, dh_rhs );
  }

  static bool is_functor()
  {
    return true;
  }

  static bool is_empty()
  {
    return false;
  }
};  // struct FD_DELEGATE_FUNCTOR_STATELESS

/**
 * Stub for member function which is invoked on the pointer to the bound object.
 */
template<typename R, FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename T, typename TFxn>
struct FD_DELEGATE_MFN_BIND_ON_PTR
  : public FD_DELEGATE_INVOKER<R>::template mfn_bind_on_ptr<FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA T, TFxn>
  , public FD_DELEGATE_STUB_BASE<
  R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS,
  FD_DELEGATE_MFN_BIND_ON_PTR<R, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA T, TFxn>,
  ptr_manager,
  single_cast_stub>
{
  typedef FD_DELEGATE_MFN_BIND_ON_PTR type;

  static void init(delegate_holder & dh, T * t, TFxn fxn)
  {
    ptr_manager::copy_obj( dh, t );
    dh.any.mfn_ptr = simplify_mfn<TFxn>::convert( fxn );
    dh.tbl_ptr = FD_DELEGATE_MFN_BIND_ON_PTR::get_table();
  }

  static unsigned size_of_fxn(void const *)
  {
    return sizeof( TFxn );
  }

  static std::type_info const & type_of_fxn(void const *)
  {
    return typeid( TFxn );
  }

  static bool compare_equal_same_type(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs)
  {
    return ( simplify_mfn<TFxn>::restore( dh_lhs.any.mfn_ptr ) == simplify_mfn<TFxn>::restore( dh_rhs.any.mfn_ptr ) ) &&
      ( ( dh_lhs.obj_ptr == dh_rhs.obj_ptr ) ||
      compare_equal( *static_cast<T const *>( dh_lhs.obj_ptr ), *static_cast<T const *>( dh_rhs.obj_ptr ) ) );
  }

  static int memcmp_delegate(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs)
  {
    return dh_util::compare_memory( dh_lhs, dh_rhs );
  }

  static bool is_functor()
  {
    return false;
  }

  static bool is_empty()
  {
    return false;
  }
};  // struct FD_DELEGATE_MFN_BIND_ON_PTR

/**
 * Stub for member function which is invoked on the reference to the bound object.
 * Derived from FD_DELEGATE_MFN_BIND_ON_PTR.
 */
template<typename R, FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename T, typename TFxn>
struct FD_DELEGATE_MFN_BIND_ON_REF
  : public FD_DELEGATE_MFN_BIND_ON_PTR<
  R, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA typename T::type, TFxn>
{
  typedef FD_DELEGATE_MFN_BIND_ON_REF type;

  static void init(delegate_holder & dh, T t, TFxn fxn)
  {
    typedef
      FD_DELEGATE_MFN_BIND_ON_PTR<R, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA BOOST_DEDUCED_TYPENAME T::type, TFxn>
      base_type;

    base_type::init( dh, t.get_pointer(), fxn );
  }
};  // struct FD_DELEGATE_MFN_BIND_ON_REF

/**
 * Stub for member function which is invoked on the copy of the bound object.
 */
template<
typename R, FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA
typename T, typename TFxn, typename Allocator>
struct FD_DELEGATE_MFN_BIND_ON_OBJ
  : public FD_DELEGATE_INVOKER<R>::template mfn_bind_on_obj<FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA T, TFxn>
  , public FD_DELEGATE_STUB_BASE<
  R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS,
  FD_DELEGATE_MFN_BIND_ON_OBJ<R, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA T, TFxn, Allocator>,
  obj_manager<T, Allocator>,
  single_cast_stub>
{
  typedef FD_DELEGATE_MFN_BIND_ON_OBJ type;

  static void init(delegate_holder & dh, T t, TFxn fxn)
  {
    obj_manager<T, Allocator>::copy_obj( dh, fd::detail::delegate::addressof( t ) );
    dh.any.mfn_ptr = simplify_mfn<TFxn>::convert( fxn );
    dh.tbl_ptr = FD_DELEGATE_MFN_BIND_ON_OBJ::get_table();
  }

  static unsigned size_of_fxn(void const *)
  {
    return sizeof( TFxn );
  }

  static std::type_info const & type_of_fxn(void const *)
  {
    return typeid( TFxn );
  }

  static bool compare_equal_same_type(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs)
  {
    return ( simplify_mfn<TFxn>::restore( dh_lhs.any.mfn_ptr ) == simplify_mfn<TFxn>::restore( dh_rhs.any.mfn_ptr ) ) &&
      ( ( fd::detail::delegate::addressof( *static_cast<T const *>( dh_lhs.obj_ptr ) ) ==
      fd::detail::delegate::addressof( *static_cast<T const *>( dh_rhs.obj_ptr ) ) ) ||
      compare_equal( *static_cast<T const *>( dh_lhs.obj_ptr ), *static_cast<T const *>( dh_rhs.obj_ptr ) ) );
  }

  static int memcmp_delegate(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs)
  {
    return dh_util::compare_memory( dh_lhs, dh_rhs );
  }

  static bool is_functor()
  {
    return false;
  }

  static bool is_empty()
  {
    return false;
  }
};  // struct FD_DELEGATE_MFN_BIND_ON_OBJ

/**
 * Stub for member function which is invoked on the copy of the bound object.
 */
template<
typename R, FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA
typename T, typename TFxn, typename Allocator>
struct FD_DELEGATE_MFN_BIND_ON_CUSTOM
  : public FD_DELEGATE_INVOKER<R>::template mfn_bind_on_custom<FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA T, TFxn>
  , public FD_DELEGATE_STUB_BASE<
  R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS,
  FD_DELEGATE_MFN_BIND_ON_CUSTOM<R, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA T, TFxn, Allocator>,
  obj_manager<T, Allocator>,
  single_cast_stub>
{
  typedef FD_DELEGATE_MFN_BIND_ON_CUSTOM type;

  static void init(delegate_holder & dh, T t, TFxn fxn)
  {
    obj_manager<T, Allocator>::copy_obj( dh, fd::detail::delegate::addressof( t ) );
    dh.any.mfn_ptr = simplify_mfn<TFxn>::convert( fxn );
    dh.tbl_ptr = FD_DELEGATE_MFN_BIND_ON_CUSTOM::get_table();
  }

  static unsigned size_of_fxn(void const *)
  {
    return sizeof( TFxn );
  }

  static std::type_info const & type_of_fxn(void const *)
  {
    return typeid( TFxn );
  }

  static bool compare_equal_same_type(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs)
  {
    return ( simplify_mfn<TFxn>::restore( dh_lhs.any.mfn_ptr ) == simplify_mfn<TFxn>::restore( dh_rhs.any.mfn_ptr ) ) &&
      ( ( fd::detail::delegate::addressof( *get_pointer( *static_cast<T const *>( dh_lhs.obj_ptr ) ) ) ==
      fd::detail::delegate::addressof( *get_pointer( *static_cast<T const *>( dh_rhs.obj_ptr ) ) ) ) ||
      compare_equal( *get_pointer( *static_cast<T const *>( dh_lhs.obj_ptr ) ), *get_pointer( *static_cast<T const *>( dh_rhs.obj_ptr ) ) ) );
  }

  static int memcmp_delegate(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs)
  {
    // Creates temporaries to hold real target objects.
    delegate_holder dh_lhs_obj( dh_lhs ), dh_rhs_obj( dh_rhs );

    dh_lhs_obj.obj_ptr = fd::detail::delegate::addressof( *get_pointer( *static_cast<T const *>( dh_lhs.obj_ptr ) ) );
    dh_rhs_obj.obj_ptr = fd::detail::delegate::addressof( *get_pointer( *static_cast<T const *>( dh_rhs.obj_ptr ) ) );

    return dh_util::compare_memory( dh_lhs_obj, dh_rhs_obj );
  }

  static bool is_functor()
  {
    return false;
  }

  static bool is_empty()
  {
    return false;
  }
};  // struct FD_DELEGATE_MFN_BIND_ON_OBJ

#if FD_DELEGATE_NUM_ARGS > 0

/**
 * Stub for member function which is called on the pointer to the object that is
 * provided as the first argument of the function call.
 */
template<typename R, FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename TFxn>
struct FD_DELEGATE_MFN_ON_PTR
  : public FD_DELEGATE_INVOKER<R>::template mfn_on_ptr<FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA TFxn>
  , public FD_DELEGATE_STUB_BASE<
  R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS,
  FD_DELEGATE_MFN_ON_PTR<R, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA TFxn>,
  ptr_manager,
  single_cast_stub>
{
  typedef FD_DELEGATE_MFN_ON_PTR type;

  static void init(delegate_holder & dh, TFxn fxn)
  {
    dh.any.mfn_ptr = simplify_mfn<TFxn>::convert( fxn );
    dh.tbl_ptr = FD_DELEGATE_MFN_ON_PTR::get_table();
  }

  static unsigned size_of_fxn(void const *)
  {
    return sizeof( TFxn );
  }

  static std::type_info const & type_of_fxn(void const *)
  {
    return typeid( TFxn );
  }

  static bool compare_equal_same_type(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs)
  {
    return simplify_mfn<TFxn>::restore( dh_lhs.any.mfn_ptr ) == simplify_mfn<TFxn>::restore( dh_rhs.any.mfn_ptr );
  }

  static int memcmp_delegate(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs)
  {
    return dh_util::compare_memory( dh_lhs, dh_rhs );
  }

  static bool is_functor()
  {
    return false;
  }

  static bool is_empty()
  {
    return false;
  }
};  // struct FD_DELEGATE_MFN_ON_PTR

/**
 * Stub for member function which is called on the reference to the object that is
 * provided as the first argument of the function call.
 */
template<typename R, FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename TFxn>
struct FD_DELEGATE_MFN_ON_REF
  : public FD_DELEGATE_INVOKER<R>::template mfn_on_ref<FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA TFxn>
  , public FD_DELEGATE_STUB_BASE<
  R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS,
  FD_DELEGATE_MFN_ON_REF<R, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA TFxn>,
  ptr_manager,
  single_cast_stub>
{
  typedef FD_DELEGATE_MFN_ON_REF type;

  static void init(delegate_holder & dh, TFxn fxn)
  {
    dh.any.mfn_ptr = simplify_mfn<TFxn>::convert( fxn );
    dh.tbl_ptr = FD_DELEGATE_MFN_ON_REF::get_table();
  }

  static unsigned size_of_fxn(void const *)
  {
    return sizeof( TFxn );
  }

  static std::type_info const & type_of_fxn(void const *)
  {
    return typeid( TFxn );
  }

  static bool compare_equal_same_type(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs)
  {
    return simplify_mfn<TFxn>::restore( dh_lhs.any.mfn_ptr ) == simplify_mfn<TFxn>::restore( dh_rhs.any.mfn_ptr );
  }

  static int memcmp_delegate(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs)
  {
    return dh_util::compare_memory( dh_lhs, dh_rhs );
  }

  static bool is_functor()
  {
    return false;
  }

  static bool is_empty()
  {
    return false;
  }
};  // struct FD_DELEGATE_MFN_ON_REF

/**
 * Stub for member function which is called either on the copy of the object
 * that is provided as the first argument of the function call.
 */
template<typename R, FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename TFxn>
struct FD_DELEGATE_MFN_ON_OBJ
  : public FD_DELEGATE_INVOKER<R>::template mfn_on_obj<FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA TFxn>
  , public FD_DELEGATE_STUB_BASE<
  R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS,
  FD_DELEGATE_MFN_ON_OBJ<R, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA TFxn>,
  ptr_manager,
  single_cast_stub>
{
  typedef FD_DELEGATE_MFN_ON_OBJ type;

  static void init(delegate_holder & dh, TFxn fxn)
  {
    dh.any.mfn_ptr = simplify_mfn<TFxn>::convert( fxn );
    dh.tbl_ptr = FD_DELEGATE_MFN_ON_OBJ::get_table();
  }

  static unsigned size_of_fxn(void const *)
  {
    return sizeof( TFxn );
  }

  static std::type_info const & type_of_fxn(void const *)
  {
    return typeid( TFxn );
  }

  static bool compare_equal_same_type(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs)
  {
    return simplify_mfn<TFxn>::restore( dh_lhs.any.mfn_ptr ) == simplify_mfn<TFxn>::restore( dh_rhs.any.mfn_ptr );
  }

  static int memcmp_delegate(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs)
  {
    return dh_util::compare_memory( dh_lhs, dh_rhs );
  }

  static bool is_functor()
  {
    return false;
  }

  static bool is_empty()
  {
    return false;
  }
};  // struct FD_DELEGATE_MFN_ON_OBJ

/**
 * Stub for member function which is called either on the copy of the custom object
 * which provides get_pointer() overload.
 */
template<typename R, FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename TFxn>
struct FD_DELEGATE_MFN_ON_CUSTOM
  : public FD_DELEGATE_INVOKER<R>::template mfn_on_custom<FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA TFxn>
  , public FD_DELEGATE_STUB_BASE<
  R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS,
  FD_DELEGATE_MFN_ON_CUSTOM<R, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA TFxn>,
  ptr_manager,
  single_cast_stub>
{
  typedef FD_DELEGATE_MFN_ON_CUSTOM type;

  static void init(delegate_holder & dh, TFxn fxn)
  {
    dh.any.mfn_ptr = simplify_mfn<TFxn>::convert( fxn );
    dh.tbl_ptr = FD_DELEGATE_MFN_ON_CUSTOM::get_table();
  }

  static unsigned size_of_fxn(void const *)
  {
    return sizeof( TFxn );
  }

  static std::type_info const & type_of_fxn(void const *)
  {
    return typeid( TFxn );
  }

  static bool compare_equal_same_type(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs)
  {
    return simplify_mfn<TFxn>::restore( dh_lhs.any.mfn_ptr ) == simplify_mfn<TFxn>::restore( dh_rhs.any.mfn_ptr );
  }

  static int memcmp_delegate(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs)
  {
    return dh_util::compare_memory( dh_lhs, dh_rhs );
  }

  static bool is_functor()
  {
    return false;
  }

  static bool is_empty()
  {
    return false;
  }
};  // struct FD_DELEGATE_MFN_ON_CUSTOM

#endif // #if FD_DELEGATE_NUM_ARGS > 0

/**
 * Stub for free function or static member function.
 */
template<typename R, FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename TFxn>
struct FD_DELEGATE_FUNCTION
  : public FD_DELEGATE_INVOKER<R>::template function<FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA TFxn>
  , public FD_DELEGATE_STUB_BASE<
  R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS,
  FD_DELEGATE_FUNCTION<R, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA TFxn>,
  ptr_manager,
  single_cast_stub>
{
  typedef FD_DELEGATE_FUNCTION type;

  static void init(delegate_holder & dh, TFxn fxn)
  {
    dh.any.fxn_ptr = reinterpret_cast<generic_fxn *>( fxn );
    dh.tbl_ptr = FD_DELEGATE_FUNCTION::get_table();
  }

  static unsigned size_of_fxn(void const *)
  {
    return sizeof( TFxn );
  }

  static std::type_info const & type_of_fxn(void const *)
  {
    return typeid( TFxn );
  }

  static bool compare_equal_same_type(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs)
  {
    return reinterpret_cast<TFxn>( dh_lhs.any.fxn_ptr ) == reinterpret_cast<TFxn>( dh_rhs.any.fxn_ptr );
  }

  static int memcmp_delegate(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs)
  {
    return dh_util::compare_memory( dh_lhs, dh_rhs );
  }

  static bool is_functor()
  {
    return false;
  }

  static bool is_empty()
  {
    return false;
  }
};  // struct FD_DELEGATE_FUNCTION

/**
 * Helper class template to aid returning multicast type without Allocator template parameter.
 */
template<typename R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS>
struct FD_DELEGATE_MULTICAST_TYPE { };

/**
 * Stub for multicast.
 */
template<typename R, FD_DELEGATE_TEMPLATE_PARMS FD_DELEGATE_COMMA typename Allocator>
struct FD_DELEGATE_MULTICAST
  : public FD_DELEGATE_INVOKER<R>::template multicast<FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA Allocator>
  , public FD_DELEGATE_STUB_BASE<
  R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS,
  FD_DELEGATE_MULTICAST<R, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA Allocator>,
  multicast_shared_list_manager<Allocator>,
  multicast_stub<Allocator> >
{
  typedef FD_DELEGATE_MULTICAST type;

  typedef typename multicast_info<Allocator>::delegate_list_type delegate_list_type;

  typedef FD_DELEGATE_FXN_TABLE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS> fxn_table_type;

  static void init(delegate_holder & dh)
  {

#ifndef FD_DELEGATE_NO_STD_ALLOCATOR
    typedef typename Allocator::template rebind<multicast_info<Allocator> >::other allocator_type;
    typedef typename allocator_type::pointer pointer_type;
#else
    typedef multicast_info<Allocator> * pointer_type;
#endif // FD_DELEGATE_NO_STD_ALLOCATOR

#  ifndef FD_DELEGATE_NO_STD_ALLOCATOR
    allocator_type allocator;
#  endif // FD_DELEGATE_NO_STD_ALLOCATOR

#  ifndef FD_DELEGATE_NO_STD_ALLOCATOR
    pointer_type minfo_ptr = allocator.allocate( 1 );
    allocator.construct( minfo_ptr, multicast_info<Allocator>() );
#  else
    pointer_type minfo_ptr = new multicast_info<Allocator>();
#  endif // FD_DELEGATE_NO_STD_ALLOCATOR

    dh.obj_ptr = minfo_ptr;
    dh.tbl_ptr = FD_DELEGATE_MULTICAST::get_table();
  }

  /**
   * @return Count of delegates in the list.
   */
  static unsigned size_of_fxn(void const * obj_ptr)
  {
    delegate_list_type const & dg_list =
      static_cast<multicast_info<Allocator> *>( const_cast<void *>( obj_ptr ) )->delegate_list();

    return static_cast<unsigned>( dg_list.size() );
  }

  static std::type_info const & type_of_fxn(void const *)
  {
    // Allocator is not taken into consideration as type.
    return typeid( FD_DELEGATE_MULTICAST_TYPE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS> );
  }

  static bool compare_equal_same_type(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs)
  {
    delegate_list_type const & dg_list_lhs =
      static_cast<multicast_info<Allocator> *>( const_cast<void *>( dh_lhs.obj_ptr ) )->delegate_list();
    delegate_list_type const & dg_list_rhs =
      static_cast<multicast_info<Allocator> *>( const_cast<void *>( dh_rhs.obj_ptr ) )->delegate_list();

    typename delegate_list_type::const_iterator it_lhs = dg_list_lhs.begin(), it_lhs_e = dg_list_lhs.end();
    typename delegate_list_type::const_iterator it_rhs = dg_list_rhs.begin(), it_rhs_e = dg_list_rhs.end();

    for( ; it_lhs != it_lhs_e && it_rhs != it_rhs_e; ++it_lhs, ++it_rhs )
    {
      if( !static_cast<fxn_table_type const *>( (*it_lhs)->get_holder().tbl_ptr )->compare_equal_same_type( (*it_lhs)->get_holder(), (*it_rhs)->get_holder() ) )
        return false;
    }

    return (it_lhs == it_lhs_e && it_rhs == it_rhs_e);
  }

  static int memcmp_delegate(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs)
  {
    if( static_cast<fxn_table_type const *>( dh_rhs.tbl_ptr )->is_multicast() )
    {
      delegate_list_type const & dg_list_lhs =
        static_cast<multicast_info<Allocator> *>( const_cast<void *>( dh_lhs.obj_ptr ) )->delegate_list();
      delegate_list_type const & dg_list_rhs =
        static_cast<multicast_info<Allocator> *>( const_cast<void *>( dh_rhs.obj_ptr ) )->delegate_list();

      typename delegate_list_type::const_iterator it_lhs = dg_list_lhs.begin(), it_lhs_e = dg_list_lhs.end();
      typename delegate_list_type::const_iterator it_rhs = dg_list_rhs.begin(), it_rhs_e = dg_list_rhs.end();

      for( ; it_lhs != it_lhs_e && it_rhs != it_rhs_e; ++it_lhs, ++it_rhs )
      {
        int compare_result =
          static_cast<fxn_table_type const *>( (*it_lhs)->get_holder().tbl_ptr )->memcmp_delegate( (*it_lhs)->get_holder(), (*it_rhs)->get_holder() );
        if( 0 !=  compare_result )
          return compare_result;
      }

      if( it_lhs == it_lhs_e && it_rhs == it_rhs_e )
      {
        return 0;
      }
      else
      {
        return static_cast<int>( std::distance( it_lhs, it_lhs_e ) - std::distance( it_rhs, it_rhs_e ) );
      }
    }
    else
    {
      delegate_list_type const & dg_list_lhs =
        static_cast<multicast_info<Allocator> *>( const_cast<void *>( dh_lhs.obj_ptr ) )->delegate_list();

      delegate_holder const & dh_lhs = ( *( dg_list_lhs.begin() ) )->get_holder();

      return  static_cast<fxn_table_type const *>( dh_lhs.tbl_ptr )->memcmp_delegate( dh_lhs, dh_rhs );
    }
  }

  static bool is_functor()
  {
    return false;
  }

  static bool is_empty()
  {
    return false;
  }
};  // struct FD_DELEGATE_MULTICAST

// ----------------------------------------------------------------------------------------------------

/**
 * Delegate implementation class template.
 * Implements all the base functionality details of a delegate.
 */
template<typename R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS, typename Allocator>
struct FD_DELEGATE_DELEGATE_IMPL
  : public delegate_base
{
protected:
  typedef FD_DELEGATE_DELEGATE_IMPL<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, Allocator>  self_type;

#ifndef FD_DELEGATE_NO_VOID_RETURNS
  typedef R result_type;
#else
  typedef typename delegate_return_type<R>::type  result_type;
#endif // FD_DELEGATE_NO_VOID_RETURNS

  typedef FD_DELEGATE_FXN_TABLE<result_type FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS>  fxn_table_type;
  typedef FD_DELEGATE_NULL_TYPE<result_type FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS>  null_type_stub;

  struct clear_type { };  // Assigning or comparing to 0

  FD_DELEGATE_DELEGATE_IMPL()
    : delegate_base( null_type_stub::get_table() )
  {
  }

  FD_DELEGATE_DELEGATE_IMPL(FD_DELEGATE_DELEGATE_IMPL const & other)
    : delegate_base( static_cast<delegate_base const &>( other ) )
  {
  }

public:
  void clear()
  {
    self_type().swap( *this );
  }

  /**
   * 'safe bool' operator.
   */
#if (defined __SUNPRO_CC) && (__SUNPRO_CC <= 0x530) && !(defined BOOST_NO_COMPILER_CONFIG)
  // Sun C++ 5.3 can't handle the safe_bool idiom, so don't use it
  operator bool () const
  {
    return !this->empty();
  }
#else
protected:
  struct dummy
  {
    void nonnull() {};
  };

  typedef void (dummy::*safe_bool)();

public:
  operator safe_bool () const
  {
    return ( this->empty() ) ? 0 : &dummy::nonnull;
  }

  bool operator!() const
  {
    return this->empty();
  }
#endif

  unsigned count() const
  {
    delegate_holder const & dh = this->get_holder();

    // Returns the count of delegate in the list.
    if( static_cast<fxn_table_type const *>( dh.tbl_ptr )->is_multicast() )
    {
      // size_of_fxn() of the multicast stub returns the count of delegate in the list.
      return static_cast<fxn_table_type const *>( dh.tbl_ptr )->size_of_fxn( dh.obj_ptr );
    }
    else
    {
      return this->empty() ? 0 : 1;
    }
  }

  bool equal_to(self_type const & other) const
  {
    return dh_util::compare_holder( this->get_holder(), other.get_holder() );
  }

protected:
  /**
   * Adds a new (single cast or multicast) delegate.
   *
   * @param other [in]  Specifies the delegate to add. It can add either a single delegate
   *                    or a multicast delegate.
   * @return The count of delegates in the delegate.
   */
  multicast::token add(self_type const & other)
  {
    delegate_holder & dh = this->get_holder();

    if( this == &other || other.empty() )
    { // Adding itself.
      return multicast::token( 0, other, std::deque<delegate_weak_ptr>(), null_type_stub::get_table() );
    }
    else
    {
      std::deque<delegate_weak_ptr> dg_wlist_added;

      if( !static_cast<fxn_table_type const *>( this->get_holder().tbl_ptr )->is_multicast() )
      { // It is not a multicast so turns it into multicast delegate first.

        typedef
          FD_DELEGATE_MULTICAST<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA Allocator>
          multicast_type;

        self_type dg_x;
        delegate_holder & dh_x = dg_x.get_holder();
        multicast_type::init( dh_x );

        if( !this->empty() )
        { // If the single delegate was not empty, add it into newly created multicast.

          if( !static_cast<fxn_table_type const *>( dh_x.tbl_ptr )->add_delegates( dh_x.obj_ptr, dh, &dg_wlist_added ) )
            return multicast::token( 0, other, std::deque<delegate_weak_ptr>(), null_type_stub::get_table() );
        }

        this->swap( dg_x ); // Thread-safe~
      }

      // Single cast delegate has been successfully turned into multicast delegate
      // so adds the other delegate(s) into this multicast delegate.
      static_cast<fxn_table_type const *>( dh.tbl_ptr )->add_delegates( dh.obj_ptr, other.get_holder(), &dg_wlist_added );

      return multicast::token( this, other, dg_wlist_added, null_type_stub::get_table() );
    }
  }

  /**
   * Remove a (single cast or multicast) delegate.
   *
   * @param other [in] Specifies the delegate to remove. It can remove either a single delegate
   *                    or multicast delegate.
   * @return The count of delegates in the delegate.
   */
  unsigned remove(self_type const & other)
  {
    delegate_holder & dh = this->get_holder();

    if( this == &other )
    { // Removing itself.

      this->clear();
      return 0;
    }
    else if( other.empty() )
    {
      return count();
    }
    else
    {
      delegate_holder const & dh_other = other.get_holder();

      if( static_cast<fxn_table_type const *>( dh.tbl_ptr )->is_multicast() )
      { // This is a multicast delegate.

        int delegate_cnt = static_cast<fxn_table_type const *>( dh.tbl_ptr )->remove_delegates( dh.obj_ptr, dh_other );

        if( 0 == delegate_cnt )
        { // If delegate become empty after removal then clear it to null delegate.

          this->clear();
        }

        return delegate_cnt;
      }
      else
      { // This is not a multicast delegate.

        if( static_cast<fxn_table_type const *>( dh_other.tbl_ptr )->is_multicast() )
        {
          if( static_cast<fxn_table_type const *>( dh_other.tbl_ptr )->find_delegate( dh_other.obj_ptr, dh ) )
          { // Removing a multicast which contains this delegate as one of the element in its delegate list.

            this->clear();
            return 0;
          }
        }

        return this->empty() ? 0 : 1;
      }
    }
  }

protected:
  /**
   * Helper member function template to store a function object into delegate.
   *
   * @param f [in] Specifies function object to store.
   * @return Delegate which can invoke the specified function object.
   */
  template<typename F>
  self_type & functor(F f, bool reset = true)
  {
    /**
     * If F is a pointer type then select FD_DELEGATE_FUNCTOR_PTR<> stub,
     * else if F is a referecne_wrapper<> type then select FD_DELEGATE_FUNCTOR_REF<> stub,
     * otherwise select FD_DELEGATE_FUNCTOR_OBJ<> stub
     */
    struct select_stub
    {
      typedef
        FD_DELEGATE_FUNCTOR_PTR<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA BOOST_DEDUCED_TYPENAME boost::remove_pointer<F>::type, R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS>
        ptr_type;

      typedef
        FD_DELEGATE_FUNCTOR_REF<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA F, R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS>
        ref_type;

      typedef
        FD_DELEGATE_FUNCTOR_STATELESS<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA F, R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS>
        stateless_type;

      typedef
        FD_DELEGATE_FUNCTOR_OBJ<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA F, R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, Allocator>
        obj_type;

      typedef typename boost::ct_if<
        (::boost::is_pointer<F>::value),
        ptr_type,
        boost::ct_if<
        (::boost::is_reference_wrapper<F>::value),
        ref_type,
        boost::ct_if<
        (::boost::is_stateless<F>::value),
        stateless_type,
        obj_type
        >
        >
      >::type type_1;

      typedef typename type_1::type type_2;
      typedef typename type_2::type type;
    };

    if( reset ) this->clear();
    select_stub::type::init( this->get_holder(), f );
    return *this;
  }

  /**
   * Helper class template for overload resolution for function object.
   * This class allows user to be able to specify a function object of the relaxed function
   * signature when the return type and all individual input arguments can be trivially
   * convertible.
   */
  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2>
  struct resolution
  {
    template<typename F>
    static self_type functor(F f)
    {
      struct select_stub
      {
        typedef
          FD_DELEGATE_FUNCTOR_PTR<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA BOOST_DEDUCED_TYPENAME boost::remove_pointer<F>::type, UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS2>
          ptr_type;

        typedef
          FD_DELEGATE_FUNCTOR_REF<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA F, UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS2>
          ref_type;

        typedef
          FD_DELEGATE_FUNCTOR_OBJ<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA F, UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS2, Allocator>
          obj_type;

        typedef typename boost::ct_if<
          (::boost::is_pointer<F>::value),
          ptr_type,
          boost::ct_if<
          (::boost::is_reference_wrapper<F>::value),
          ref_type,
          obj_type
          >
        >::type type_1;

        typedef typename type_1::type type;
      };

      self_type dg;
      select_stub::type::init( dg.get_holder(), f );
      return dg;
    }
  };  // struct resolution

public:
  /**
   *
   */
  struct fast_mfn_delegate
  {
    template<typename TFxn, typename T, typename U>
    static void bind(self_type & dg, TFxn fxn, T t, boost::type<U>, bool reset = false)
    {
      dg.fast_mfn_bind( fxn, t, reset );
    }

#if FD_DELEGATE_NUM_ARGS > 0
    template<typename TFxn>
    static void assign_to(self_type & dg, TFxn fxn, bool reset = false)
    {
      dg.fast_mfn_assign_to( fxn, reset );
    }
#endif
  };
  friend struct fast_mfn_delegate;

  /**
  *
  */
  struct normal_mfn_delegate
  {
    template<typename TFxn, typename T, typename U>
    static void bind(self_type & dg, TFxn fxn, T t, boost::type<U>, bool reset = false)
    {
      struct select_stub
      {
        typedef typename FD_DELEGATE_BIND<result_type>::template bind_ptr<FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA T, TFxn>     ptr_type;
        typedef typename FD_DELEGATE_BIND<result_type>::template bind_ref<FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA T, TFxn>     ref_type;
        typedef typename FD_DELEGATE_BIND<result_type>::template bind_obj<FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA T, TFxn>     obj_type;
        typedef typename FD_DELEGATE_BIND<result_type>::template bind_custom<FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA T, TFxn>  custom_type;

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
      };

      dg.functor( select_stub::type( fxn, t ) );
    }

#if FD_DELEGATE_NUM_ARGS > 0
    template<typename TFxn>
    static void assign_to(self_type & dg, TFxn fxn, bool reset = false)
    {
      dg.functor( boost::mem_fn( fxn ), reset );
    }
#endif
  };
  friend struct normal_mfn_delegate;


protected:
  /**
   * Helper member function template to store a non-const member function and its bound object
   * into delegate.
   *
   * @param fxn [in] Specifies the non-const member function to store.
   * @param t   [in] Specifies the bound object on which the specified member function is called.
   * @return  Delegate which can invoke the specified non-const member function on the specified
   *          bound object.
   */
  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2, typename U, typename T>
  self_type & mfn_bind(UR (U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2), T t, bool reset = true)
  {
    struct select_stub
    {
      typedef UR (U::*TFxn)(FD_DELEGATE_TEMPLATE_ARGS2);

      typedef
        FD_DELEGATE_MFN_BIND_ON_PTR<
        result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA BOOST_DEDUCED_TYPENAME boost::remove_pointer<T>::type, TFxn>
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
    };

    if( reset ) this->clear();
    select_stub::type::init( this->get_holder(), t, fxn );
    return *this;
  }

  /**
   * Helper member function template to store a const member function and its bound object
   * into delegate.
   *
   * @param fxn [in] Specifies the const member function to store.
   * @param t   [in] Specifies the bound object on which the specified member function is called.
   * @return  Delegate which can invoke the specified const member function on the specified
   *          bound object.
   */
  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2, typename U, typename T>
  self_type & mfn_bind_const(UR (U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2) const, T t, bool reset = true)
  {
    struct select_stub
    {
      typedef UR (U::*TFxn)(FD_DELEGATE_TEMPLATE_ARGS2) const;

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
    };

    if( reset ) this->clear();
    select_stub::type::init( this->get_holder(), t, fxn );
    return *this;
  }

private:
  /**
   *
   */
  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2, typename U, typename T>
  self_type & fast_mfn_bind(UR (U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2), T t, bool reset)
  {
    this->mfn_bind( fxn, t, reset );
    return *this;
  }

  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2, typename U, typename T>
  self_type & fast_mfn_bind(UR (U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2) const, T t, bool reset)
  {
    this->mfn_bind_const( fxn, t, reset );
    return *this;
  }

#if FD_DELEGATE_NUM_ARGS > 0

protected:
  /**
   * Helper member function template to store a non-const member function into delegate.
   *
   * @param fxn [in] Specifies the non-const member function to store.
   * @return  Delegate which can invoke the specified non-const member function.
   */
  template<typename UR FD_DELEGATE_COMMA_SP FD_DELEGATE_TEMPLATE_PARMS2_SP, typename U>
  self_type & mfn(UR (U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2_SP), bool reset = true)
  {
    /**
     * If T0 is a pointer type of underlying type U then select FD_DELEGATE_MFN_ON_PTR<> stub,
     * else if T0 is a reference type of undeylying type U then select FD_DELEGATE_MFN_ON_REF<> stub,
     * otherwise select FD_DELEGATE_MFN_ON_OBJ<> stub
     */
    struct select_stub
    {
      typedef UR (U::*TFxn)(FD_DELEGATE_TEMPLATE_ARGS2_SP);

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
    };

    if( reset ) this->clear();
    select_stub::type::init( this->get_holder(), fxn );
    return *this;
  }

  /**
   * Helper member function template to store a const member function into delegate.
   *
   * @param fxn [in] Specifies the const member function to store.
   * @return  Delegate which can invoke the specified const member function.
   */
  template<typename UR FD_DELEGATE_COMMA_SP FD_DELEGATE_TEMPLATE_PARMS2_SP, typename U>
  self_type & mfn_const(UR (U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2_SP) const, bool reset = true)
  {
    /**
     * If T0 is a pointer type of underlying type U then select FD_DELEGATE_MFN_ON_PTR<> stub,
     * else if T0 is a reference type of undeylying type U then select FD_DELEGATE_MFN_ON_REF<> stub,
     * otherwise select FD_DELEGATE_MFN_ON_OBJ<> stub
     */
    struct select_stub
    {
      typedef UR (U::*TFxn)(FD_DELEGATE_TEMPLATE_ARGS2_SP) const;

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
    };

    if( reset ) this->clear();
    select_stub::type::init( this->get_holder(), fxn );
    return *this;
  }

private:
  /**
  *
  */
  template<typename UR FD_DELEGATE_COMMA_SP FD_DELEGATE_TEMPLATE_PARMS2_SP, typename U>
  self_type & fast_mfn_assign_to(UR (U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2_SP), bool reset)
  {
    this->mfn( fxn, reset );
    return *this;
  }

  template<typename UR FD_DELEGATE_COMMA_SP FD_DELEGATE_TEMPLATE_PARMS2_SP, typename U>
  self_type & fast_mfn_assign_to(UR (U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2_SP) const, bool reset)
  {
    this->mfn_const( fxn, reset );
    return *this;
  }

#endif // #if FD_DELEGATE_NUM_ARGS > 0

#if defined(FD_MEM_FN_ENABLE_CDECL) || defined(BOOST_MEM_FN_ENABLE_CDECL)

#define FD_DELEGATE_MF_CC       __cdecl
#define FD_DELEGATE_CC_NAME(X)  X##_cdecl
#  include "./detail/delegate_mfn_cc_impl.hpp"
#undef  FD_DELEGATE_MF_CC
#undef  FD_DELEGATE_CC_NAME

#endif  // #if defined(FD_MEM_FN_ENABLE_CDECL) || defined(BOOST_MEM_FN_ENABLE_CDECL)

#if defined(FD_MEM_FN_ENABLE_STDCALL) || defined(BOOST_MEM_FN_ENABLE_STDCALL)

#define FD_DELEGATE_MF_CC       __stdcall
#define FD_DELEGATE_CC_NAME(X)  X##_stdcall
#  include "./detail/delegate_mfn_cc_impl.hpp"
#undef  FD_DELEGATE_MF_CC
#undef  FD_DELEGATE_CC_NAME

#endif  // #if defined(FD_MEM_FN_ENABLE_STDCALL) || defined(BOOST_MEM_FN_ENABLE_STDCALL)

#if defined(FD_MEM_FN_ENABLE_FASTCALL) || defined(BOOST_MEM_FN_ENABLE_FASTCALL)

#define FD_DELEGATE_MF_CC       __fastcall
#define FD_DELEGATE_CC_NAME(X)  X##_fastcall
#  include "./detail/delegate_mfn_cc_impl.hpp"
#undef  FD_DELEGATE_MF_CC
#undef  FD_DELEGATE_CC_NAME

#endif  // #if defined(FD_MEM_FN_ENABLE_FASTCALL) || defined(BOOST_MEM_FN_ENABLE_FASTCALL)

protected:
  /**
   * Helper member function template to store a free function or static member function
   * into delegate.
   *
   * @param fxn [in] Specifies the free function or static member function to store.
   * @return  Delegate which can invoke the specified free function or static member function.
   */
  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2>
  self_type & function(UR (*fxn)(FD_DELEGATE_TEMPLATE_ARGS2), bool reset = true)
  {
    struct select_stub
    {
      typedef UR (*TFxn)(FD_DELEGATE_TEMPLATE_ARGS2);

      typedef FD_DELEGATE_FUNCTION<result_type, FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA TFxn> type;
    };

    if( reset ) this->clear();
    select_stub::type::init( this->get_holder(), fxn );
    return *this;
  }

#if defined(FD_FUNCTION_ENABLE_STDCALL) || defined(BOOST_BIND_ENABLE_STDCALL)

#define FD_DELEGATE_ST
#define FD_DELEGATE_CC          __stdcall
#define FD_DELEGATE_CC_NAME(X)  X##_stdcall
#  include "./detail/delegate_cc_impl.hpp"
#undef  FD_DELEGATE_ST
#undef  FD_DELEGATE_CC
#undef  FD_DELEGATE_CC_NAME

#endif  // #if defined(FD_FUNCTION_ENABLE_STDCALL) || defined(BOOST_BIND_ENABLE_STDCALL)

#if defined(FD_FUNCTION_ENABLE_FASTCALL) || defined(BOOST_BIND_ENABLE_FASTCALL)

#define FD_DELEGATE_ST
#define FD_DELEGATE_CC          __fastcall
#define FD_DELEGATE_CC_NAME(X)  X##_fastcall
#  include "./detail/delegate_cc_impl.hpp"
#undef  FD_DELEGATE_ST
#undef  FD_DELEGATE_CC
#undef  FD_DELEGATE_CC_NAME

#endif  // #if defined(FD_FUNCTION_ENABLE_FASTCALL) || defined(BOOST_BIND_ENABLE_FASTCALL)

};  // struct FD_DELEGATE_DELEGATE_IMPL

// ----------------------------------------------------------------------------------------------------

} } // namespace detail::delegate

template<
  typename R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS,
  typename Allocator = FD_DELEGATE_DEFAULT_ALLOCATOR>
class FD_DELEGATE_DELEGATE
  : public fd::detail::delegate::FD_DELEGATE_DELEGATE_IMPL<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, Allocator>
{
private:
  typedef FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, Allocator>  self_type;
  typedef fd::detail::delegate::FD_DELEGATE_DELEGATE_IMPL<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, Allocator> impl_class;

  // impl_class impl_;

protected:
  typedef typename impl_class::clear_type clear_type;
  typedef typename impl_class::safe_bool  safe_bool;

public:
  BOOST_STATIC_CONSTANT(int, args = FD_DELEGATE_NUM_ARGS);

  typedef typename impl_class::result_type result_type;

  // Adds signature for boost::lambda
  template<typename Args>
  struct sig
  {
    typedef result_type type;
  };

#if FD_DELEGATE_NUM_ARGS == 1
  // std::unary_function support.
  typedef T0 argument_type;
#elif FD_DELEGATE_NUM_ARGS == 2
  // std::binary_function support.
  typedef T0 first_argument_type;
  typedef T1 second_argument_type;
#endif

  BOOST_STATIC_CONSTANT(int, arity = FD_DELEGATE_NUM_ARGS);

  FD_DELEGATE_ARG_TYPES

    typedef Allocator allocator_type;

  /**
   * Default constructor.
   */
  FD_DELEGATE_DELEGATE()
    : impl_class()
  {
  }

  // MSVC chokes if the following two constructors are collapsed into
  // one with a default parameter.
  template<typename F>
  FD_DELEGATE_DELEGATE(F FD_DELEGATE_TARGET_FIX(const &) f
#ifndef BOOST_NO_SFINAE
    ,typename boost::enable_if_c
    <
    (::boost::type_traits::ice_not<(::boost::is_integral<F>::value)>::value),
    int
    >::type = 0
#endif // BOOST_NO_SFINAE
    )
    : impl_class()
  {
    this->assign_to( f, false );
  }

#ifndef BOOST_NO_SFINAE
  FD_DELEGATE_DELEGATE(clear_type *) : impl_class() {}
#else
  FD_DELEGATE_DELEGATE(int zero) : impl_class()
  {
    BOOST_ASSERT(zero == 0);
  }
#endif

  FD_DELEGATE_DELEGATE(FD_DELEGATE_DELEGATE const & other)
    : impl_class( static_cast<impl_class const &>( other ) )
  {
  }

  // The distinction between when to use BOOST_FUNCTION_FUNCTION and
  // when to use self_type is obnoxious. MSVC cannot handle self_type as
  // the return type of these assignment operators, but Borland C++ cannot
  // handle BOOST_FUNCTION_FUNCTION as the type of the temporary to
  // construct.
  template<typename F>
#ifndef BOOST_NO_SFINAE
  typename boost::enable_if_c
    <
    (::boost::type_traits::ice_not<(::boost::is_integral<F>::value)>::value),
    FD_DELEGATE_DELEGATE &
    >::type
#else
  FD_DELEGATE_DELEGATE &
#endif
    operator =(F FD_DELEGATE_TARGET_FIX(const &) f)
  {
    self_type( f ).swap( *this );
    return *this;
  }

#ifndef BOOST_NO_SFINAE
  FD_DELEGATE_DELEGATE & operator =(clear_type *)
  {
    this->clear();
    return *this;
  }
#else
  FD_DELEGATE_DELEGATE & operator =(int zero)
  {
    BOOST_ASSERT(zero == 0);
    this->clear();
    return *this;
  }
#endif

  // Assignment from another FD_DELEGATE_DELEGATE
  FD_DELEGATE_DELEGATE & operator =(FD_DELEGATE_DELEGATE const & other)
  {
    if( this != &other )
      self_type( other ).swap( *this );
    return *this;
  }

  void swap(FD_DELEGATE_DELEGATE & other)
  {
    if( this != &other )
      impl_class::swap( other );
  }

  /**
   *
   */
  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2, typename U, typename T>
  FD_DELEGATE_DELEGATE(UR (U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2), T t)
  {
    struct select_stub
    {
      typedef UR (U::*TFxn)(FD_DELEGATE_TEMPLATE_ARGS2);

      typedef typename boost::ct_if<( sizeof( TFxn ) <= detail::delegate::any_fxn_size ),
        BOOST_DEDUCED_TYPENAME impl_class::fast_mfn_delegate,
        BOOST_DEDUCED_TYPENAME impl_class::normal_mfn_delegate
      >::type type;
    };

    select_stub::type::bind( *this, fxn, t, boost::type<U>() );
  }

  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2, typename U, typename T>
  FD_DELEGATE_DELEGATE(UR (U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2) const, T t)
  {
    struct select_stub
    {
      typedef UR (U::*TFxn)(FD_DELEGATE_TEMPLATE_ARGS2) const;

      typedef typename boost::ct_if<( sizeof( TFxn ) <= detail::delegate::any_fxn_size ),
        BOOST_DEDUCED_TYPENAME impl_class::fast_mfn_delegate,
        BOOST_DEDUCED_TYPENAME impl_class::normal_mfn_delegate
      >::type type;
    };

    select_stub::type::bind( *this, fxn, t, boost::type<U>() );
  }

  /**
   *
   */
  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2, typename U, typename T>
  FD_DELEGATE_DELEGATE & bind(UR (U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2), T t)
  {
    struct select_stub
    {
      typedef UR (U::*TFxn)(FD_DELEGATE_TEMPLATE_ARGS2);

      typedef typename boost::ct_if<( sizeof( TFxn ) <= detail::delegate::any_fxn_size ),
        BOOST_DEDUCED_TYPENAME impl_class::fast_mfn_delegate,
        BOOST_DEDUCED_TYPENAME impl_class::normal_mfn_delegate
      >::type type;
    };

    select_stub::type::bind( *this, fxn, t, boost::type<U>(), true );
    return *this;
  }

  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2, typename U, typename T>
  FD_DELEGATE_DELEGATE & bind(UR (U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2) const, T t)
  {
    struct select_stub
    {
      typedef UR (U::*TFxn)(FD_DELEGATE_TEMPLATE_ARGS2) const;

      typedef typename boost::ct_if<( sizeof( TFxn ) <= detail::delegate::any_fxn_size ),
        BOOST_DEDUCED_TYPENAME impl_class::fast_mfn_delegate,
        BOOST_DEDUCED_TYPENAME impl_class::normal_mfn_delegate
      >::type type;
    };

    select_stub::type::bind( *this, fxn, t, boost::type<U>(), true );
    return *this;
  }

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1200)
  /**
   * Function call operator to invoke delegate.
   */
  result_type operator ()(FD_DELEGATE_PARMS) const
  {
    typename impl_class::fxn_table_type const * ftable =
      static_cast<typename impl_class::fxn_table_type const *>( this->get_holder().tbl_ptr );

    return ftable->invoke( this->get_holder() FD_DELEGATE_COMMA FD_DELEGATE_ARGS);
  }

  /**
   * Function call operator to invoke delegate with combiner interface.
   */
  template<typename Combiner>
  typename Combiner::result_type operator ()(FD_DELEGATE_PARMS FD_DELEGATE_COMMA Combiner combine) const
  {
    typedef fd::detail::delegate::FD_DELEGATE_ARGS_STRUCT<FD_DELEGATE_TEMPLATE_ARGS> args_type;
#if FD_DELEGATE_NUM_ARGS == 0
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
    args_type const args;
#else
    args_type const args = { };
#endif
#else
    args_type const args = { FD_DELEGATE_ARGS };
#endif

    typedef typename fd::detail::delegate::FD_DELEGATE_CALL_BOUND<BOOST_DEDUCED_TYPENAME detail::delegate::delegate_return_type<R>::type>::template caller<FD_DELEGATE_TEMPLATE_ARGS> call_bound_args;
    call_bound_args f( args );

    typedef fd::detail::delegate::multicast_info<Allocator> multicast_info_type;
    multicast_info_type minfo( this->get_holder() );

    typedef typename multicast_info_type::watch_list_type notification_type;
    notification_type notification = minfo.get_watch_list();

    typedef fd::detail::delegate::multicast_call_iterator<call_bound_args, BOOST_DEDUCED_TYPENAME notification_type::const_iterator> multicast_call_iterator;
    return combine( multicast_call_iterator(notification.begin(), notification.end(), f),
      multicast_call_iterator(notification.end(), notification.end(), f) );
  }

#else
  result_type operator ()(FD_DELEGATE_PARMS) const;

  template<typename Combiner>
  typename Combiner::result_type operator ()(FD_DELEGATE_PARMS FD_DELEGATE_COMMA Combiner combine) const;
#endif

  /**
   * Multicast add operators.
   */
  FD_DELEGATE_DELEGATE & operator +=(FD_DELEGATE_DELEGATE const & other)
  {
    impl_class::add( static_cast<impl_class const &>( other ) );
    return *this;
  }

  template<typename F>
  FD_DELEGATE_DELEGATE & operator +=(boost::reference_wrapper<F> ref)
  {
    //impl_class::add( static_cast<impl_class const &>( FD_DELEGATE_DELEGATE( ref.get_pointer() ) ) );
    impl_class::add( FD_DELEGATE_DELEGATE( ref ) );
    return *this;
  }

  /**
   * Multicast remove operators.
   */
  FD_DELEGATE_DELEGATE & operator -=(FD_DELEGATE_DELEGATE const & other)
  {
    impl_class::remove( static_cast<impl_class const &>( other ) );
    return *this;
  }

  template<typename F>
  FD_DELEGATE_DELEGATE & operator -=(boost::reference_wrapper<F> ref)
  {
    // impl_class::remove( static_cast<impl_class const &>( FD_DELEGATE_DELEGATE( ref.get_pointer() ) ) );
    impl_class::remove( FD_DELEGATE_DELEGATE( ref ) );
    return *this;
  }

  /**
   *
   */
  multicast::token add(FD_DELEGATE_DELEGATE const & other)
  {
    return impl_class::add( static_cast<impl_class const &>( other ) );
  }

  template<typename F>
  multicast::token add(boost::reference_wrapper<F> ref)
  {
    // return impl_class::add( static_cast<impl_class const &>( FD_DELEGATE_DELEGATE( ref.get_pointer() ) ) );
    return impl_class::add( FD_DELEGATE_DELEGATE( ref ) );
  }

  /**
   *
   */
  unsigned remove(FD_DELEGATE_DELEGATE const & other)
  {
    return impl_class::remove( static_cast<impl_class const &>( other ) );
  }

  template<typename F>
  unsigned remove(boost::reference_wrapper<F> ref)
  {
    // impl_class::remove( static_cast<impl_class const &>( FD_DELEGATE_DELEGATE( ref.get_pointer() ) ) );
    return impl_class::remove( FD_DELEGATE_DELEGATE( ref ) );
  }

private:
  /**
   *
   */
  template<typename F>
  FD_DELEGATE_DELEGATE & assign_to(F f, bool reset = false)
  {
    typedef typename fd::detail::delegate::get_function_tag<F>::type tag;
    this->assign_to( f, tag(), reset );
    return *this;
  }

  template<typename F>
  FD_DELEGATE_DELEGATE & assign_to(F f, fd::detail::delegate::function_obj_tag, bool reset = false)
  {
    this->functor( f , reset );
    return *this;
  }

  template<typename F>
  FD_DELEGATE_DELEGATE & assign_to(F * f, fd::detail::delegate::pointer_tag, bool reset = false)
  {
    this->functor( f , reset );
    return *this;
  }

#if FD_DELEGATE_NUM_ARGS > 0

  template<typename UR FD_DELEGATE_COMMA_SP FD_DELEGATE_TEMPLATE_PARMS2_SP, typename U>
  FD_DELEGATE_DELEGATE & assign_to(UR (U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2_SP), fd::detail::delegate::mfn_ptr_tag, bool reset = false)
  {
    struct select_stub
    {
      typedef UR (U::*TFxn)(FD_DELEGATE_TEMPLATE_ARGS2_SP);

      typedef typename boost::ct_if<( sizeof( TFxn ) <= detail::delegate::any_fxn_size ),
        BOOST_DEDUCED_TYPENAME impl_class::fast_mfn_delegate,
        BOOST_DEDUCED_TYPENAME impl_class::normal_mfn_delegate
      >::type type;
    };

    select_stub::type::assign_to( *this, fxn, reset );
    return *this;
  }

  template<typename UR FD_DELEGATE_COMMA_SP FD_DELEGATE_TEMPLATE_PARMS2_SP, typename U>
  FD_DELEGATE_DELEGATE & assign_to(UR (U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2_SP) const, fd::detail::delegate::mfn_ptr_tag, bool reset = false)
  {
    struct select_stub
    {
      typedef UR (U::*TFxn)(FD_DELEGATE_TEMPLATE_ARGS2_SP) const;

      typedef typename boost::ct_if<( sizeof( TFxn ) <= detail::delegate::any_fxn_size ),
        BOOST_DEDUCED_TYPENAME impl_class::fast_mfn_delegate,
        BOOST_DEDUCED_TYPENAME impl_class::normal_mfn_delegate
      >::type type;
    };

    select_stub::type::assign_to( *this, fxn, reset );
    return *this;
  }

#endif // #if FD_DELEGATE_NUM_ARGS > 0

#if defined(FD_MEM_FN_ENABLE_CDECL) || defined(BOOST_MEM_FN_ENABLE_CDECL)

#define FD_DELEGATE_MF_CC       __cdecl
#define FD_DELEGATE_CC_NAME(X)  X##_cdecl
#  include "./detail/delegate_mfn_cc.hpp"
#undef  FD_DELEGATE_MF_CC
#undef  FD_DELEGATE_CC_NAME

#endif  // #if defined(FD_MEM_FN_ENABLE_CDECL) || defined(BOOST_MEM_FN_ENABLE_CDECL)

#if defined(FD_MEM_FN_ENABLE_STDCALL) || defined(BOOST_MEM_FN_ENABLE_STDCALL)

#define FD_DELEGATE_MF_CC       __stdcall
#define FD_DELEGATE_CC_NAME(X)  X##_stdcall
#  include "./detail/delegate_mfn_cc.hpp"
#undef  FD_DELEGATE_MF_CC
#undef  FD_DELEGATE_CC_NAME

#endif  // #if defined(FD_MEM_FN_ENABLE_STDCALL) || defined(BOOST_MEM_FN_ENABLE_STDCALL)

#if defined(FD_MEM_FN_ENABLE_FASTCALL) || defined(BOOST_MEM_FN_ENABLE_FASTCALL)

#define FD_DELEGATE_MF_CC       __fastcall
#define FD_DELEGATE_CC_NAME(X)  X##_fastcall
#  include "./detail/delegate_mfn_cc.hpp"
#undef  FD_DELEGATE_MF_CC
#undef  FD_DELEGATE_CC_NAME

#endif  // #if defined(FD_MEM_FN_ENABLE_FASTCALL) || defined(BOOST_MEM_FN_ENABLE_FASTCALL)

  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2>
  FD_DELEGATE_DELEGATE & assign_to(UR (*fxn)(FD_DELEGATE_TEMPLATE_ARGS2), fd::detail::delegate::pointer_tag, bool reset = false)
  {
    this->function( fxn, reset );
    return *this;
  }

#if defined(FD_FUNCTION_ENABLE_STDCALL) || defined(BOOST_BIND_ENABLE_STDCALL)

#define FD_DELEGATE_ST
#define FD_DELEGATE_CC          __stdcall
#define FD_DELEGATE_CC_NAME(X)  X##_stdcall
#  include "./detail/delegate_cc.hpp"
#undef  FD_DELEGATE_ST
#undef  FD_DELEGATE_CC
#undef  FD_DELEGATE_CC_NAME

#endif // #if defined(FD_FUNCTION_ENABLE_STDCALL) || defined(BOOST_BIND_ENABLE_STDCALL)

#if defined(FD_FUNCTION_ENABLE_FASTCALL) || defined(BOOST_BIND_ENABLE_FASTCALL)

#define FD_DELEGATE_ST
#define FD_DELEGATE_CC          __fastcall
#define FD_DELEGATE_CC_NAME(X)  X##_fastcall
#  include "./detail/delegate_cc.hpp"
#undef  FD_DELEGATE_ST
#undef  FD_DELEGATE_CC
#undef  FD_DELEGATE_CC_NAME

#endif // #if defined(FD_FUNCTION_ENABLE_FASTCALL) || defined(BOOST_BIND_ENABLE_FASTCALL)

public:
  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2>
  struct resolution
  {
    template<typename F>
    static FD_DELEGATE_DELEGATE select(F f)
    {
      FD_DELEGATE_DELEGATE dg;
      impl_class::template resolution<UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS2>::functor( f ).swap( dg );
      return dg;
    }
  };  // class resolution
};

// ----------------------------------------------------------------------------------------------------

template<typename R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS, typename Allocator>
inline bool
operator <(FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, Allocator> const & d1,
           FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, Allocator> const & d2)
{
  return fd::detail::delegate::dh_util::compare_less_than( d1.get_holder(), d2.get_holder() );
}

template<typename R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS, typename Allocator>
inline void
swap(FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, Allocator> & d1,
     FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, Allocator> & d2)
{
  d1.swap( d2 );
}

#if !BOOST_WORKAROUND(BOOST_MSVC, <= 1200)

template<typename R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS, typename Allocator>
typename FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, Allocator>::result_type
FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, Allocator>
::operator ()(FD_DELEGATE_PARMS) const
{
  typename impl_class::fxn_table_type const * ftable =
    static_cast<typename impl_class::fxn_table_type const *>( this->get_holder().tbl_ptr );

  return ftable->invoke( this->get_holder() FD_DELEGATE_COMMA FD_DELEGATE_ARGS);
}

template<typename R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS, typename Allocator>
template<typename Combiner>
typename Combiner::result_type
FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, Allocator>
::operator ()(FD_DELEGATE_PARMS FD_DELEGATE_COMMA Combiner combine) const
{
  typedef fd::detail::delegate::FD_DELEGATE_ARGS_STRUCT<FD_DELEGATE_TEMPLATE_ARGS> args_type;
#if FD_DELEGATE_NUM_ARGS == 0
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
  args_type const args;
#else
  args_type const args = { };
#endif
#else
  args_type const args = { FD_DELEGATE_ARGS };
#endif

  typedef typename fd::detail::delegate::FD_DELEGATE_CALL_BOUND<BOOST_DEDUCED_TYPENAME detail::delegate::delegate_return_type<R>::type>::template caller<FD_DELEGATE_TEMPLATE_ARGS> call_bound_args;
  call_bound_args f( args );

  typedef fd::detail::delegate::multicast_info<Allocator> multicast_info_type;
  multicast_info_type minfo( this->get_holder() );

  typedef typename multicast_info_type::watch_list_type notification_type;
  notification_type notification = minfo.get_watch_list();

  typedef fd::detail::delegate::multicast_call_iterator<call_bound_args, BOOST_DEDUCED_TYPENAME notification_type::const_iterator> multicast_call_iterator;
  return combine( multicast_call_iterator( notification.begin(), notification.end(), f ),
    multicast_call_iterator( notification.end(), notification.end(), f ) );
}

#endif  // #if !BOOST_WORKAROUND(BOOST_MSVC, <= 1200)

// Poison comparisons between delegate objects of the same type.
template<typename R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS, typename Allocator>
void operator ==(FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, Allocator> const &,
                 FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, Allocator> const &);

template<typename R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS, typename Allocator>
void operator !=(FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, Allocator> const &,
                 FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, Allocator> const &);

// ----------------------------------------------------------------------------------------------------

#if !defined(FD_DELEGATE_NO_FUNCTION_TYPE_SYNTAX)

#if FD_DELEGATE_NUM_ARGS == 0
#  define FD_DELEGATE_PARTIAL_SPEC  R  (void)
#  define FD_DELEGATE_PARTIAL_SPEC2 UR (void)
#else
#  define FD_DELEGATE_PARTIAL_SPEC  R  (BOOST_PP_ENUM_PARAMS(FD_DELEGATE_NUM_ARGS,T))
#  define FD_DELEGATE_PARTIAL_SPEC2 UR (BOOST_PP_ENUM_PARAMS(FD_DELEGATE_NUM_ARGS,U))
#endif

/**
 * Delegate class with function type syntax (partial specialization version).
 */
template<typename R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS, typename Allocator>
class delegate<FD_DELEGATE_PARTIAL_SPEC, Allocator>
  : public FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, Allocator>
{
private:
  typedef FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, Allocator> base_type;
  typedef typename base_type::clear_type  clear_type;

public:
  typedef typename base_type::allocator_type allocator_type;

  // Adds signature for boost::lambda
  template<typename Args>
  struct sig
  {
    typedef typename base_type::result_type type;
  };

  template<typename Combiner>
  struct sig<delegate (FD_DELEGATE_TEMPLATE_ARGS FD_DELEGATE_COMMA Combiner)>
  {
    typedef typename Combiner::result_type type;
  };

  /**
  * Default constructor.
  */
  delegate()
    : base_type()
  {
  }

  // MSVC chokes if the following two constructors are collapsed into
  // one with a default parameter.
  template<typename F>
  delegate(F FD_DELEGATE_TARGET_FIX(const &) f
#ifndef BOOST_NO_SFINAE
    ,typename boost::enable_if_c
    <
    (::boost::type_traits::ice_not<(::boost::is_integral<F>::value)>::value),
    int
    >::type = 0
#endif // BOOST_NO_SFINAE
    )
    : base_type( f )
  {
  }

  delegate(base_type const & other)
    : base_type( static_cast<base_type const &>( other ) )
  {
  }

  delegate(delegate const & other)
    : base_type( static_cast<base_type const &>( other ) )
  {
  }

#ifndef BOOST_NO_SFINAE
  delegate(clear_type *) : base_type() { }
#else
  delegate(int zero) : base_type()
  {
    BOOST_ASSERT(zero == 0);
  }
#endif

  /**
  *
  */
  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2, typename U, typename T>
  delegate(UR (U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2), T t)
    : base_type( fxn, t )
  {
  }

  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2, typename U, typename T>
  delegate(UR (U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2) const, T t)
    : base_type( fxn, t )
  {
  }

#if defined(FD_MEM_FN_ENABLE_CDECL) || defined(BOOST_MEM_FN_ENABLE_CDECL)

#define FD_DELEGATE_MF_CC       __cdecl
#define FD_DELEGATE_CC_NAME(X)  X##_cdecl
#  include "./detail/delegate_mfn_cc_ps.hpp"
#undef  FD_DELEGATE_MF_CC
#undef  FD_DELEGATE_CC_NAME

#endif  // #if defined(FD_MEM_FN_ENABLE_CDECL) || defined(BOOST_MEM_FN_ENABLE_CDECL)

#if defined(FD_MEM_FN_ENABLE_STDCALL) || defined(BOOST_MEM_FN_ENABLE_STDCALL)

#define FD_DELEGATE_MF_CC       __stdcall
#define FD_DELEGATE_CC_NAME(X)  X##_stdcall
#  include "./detail/delegate_mfn_cc_ps.hpp"
#undef  FD_DELEGATE_MF_CC
#undef  FD_DELEGATE_CC_NAME

#endif  // #if defined(FD_MEM_FN_ENABLE_STDCALL) || defined(BOOST_MEM_FN_ENABLE_STDCALL)

#if defined(FD_MEM_FN_ENABLE_FASTCALL) || defined(BOOST_MEM_FN_ENABLE_FASTCALL)

#define FD_DELEGATE_MF_CC       __fastcall
#define FD_DELEGATE_CC_NAME(X)  X##_fastcall
#  include "./detail/delegate_mfn_cc_ps.hpp"
#undef  FD_DELEGATE_MF_CC
#undef  FD_DELEGATE_CC_NAME

#endif  // #if defined(FD_MEM_FN_ENABLE_FASTCALL) || defined(BOOST_MEM_FN_ENABLE_FASTCALL)

  template<typename F>
#ifndef BOOST_NO_SFINAE
  typename boost::enable_if_c
    <
    (::boost::type_traits::ice_not<(::boost::is_integral<F>::value)>::value),
    delegate &
    >::type
#else
  delegate &
#endif
    operator =(F f)
  {
    delegate( f ).swap( *this );
    return *this;
  }

  delegate & operator =(base_type const & other)
  {
    delegate( other ).swap( *this );
    return *this;
  }

  delegate & operator =(delegate const & other)
  {
    delegate( other ).swap( *this );
    return *this;
  }

#ifndef BOOST_NO_SFINAE
  delegate & operator =(clear_type *)
  {
    this->clear();
    return *this;
  }
#endif

  /**
  * Primary template.
  */
  template<class Signature> struct resolution;

  /**
  * Helper class template to support for overload resolution.
  * Partial specializations.
  */
  template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2>
  struct resolution<FD_DELEGATE_PARTIAL_SPEC2>
  {
    template<typename F>
    static delegate select(F f)
    {
      return base_type::template resolution<UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS2>::select( f );
    }
  };  // struct resolution<UR (FD_DELEGATE_TEMPLATE_ARGS2)>

};  // class delegate<R (FD_DELEGATE_TEMPLATE_ARGS), Allocator>

#undef  FD_DELEGATE_PARTIAL_SPEC
#undef  FD_DELEGATE_PARTIAL_SPEC2

#endif  // #if !defined(FD_DELEGATE_NO_FUNCTION_TYPE_SYNTAX)

} // end namespace fd

// Cleanup after ourselves...
#undef  FD_DELEGATE_TEMPLATE_PARMS
#undef  FD_DELEGATE_TEMPLATE_PARMS2
#undef  FD_DELEGATE_TEMPLATE_PARMS2_SP
#undef  FD_DELEGATE_TEMPLATE_PARM2_SP
#undef  FD_DELEGATE_TEMPLATE_ARGS2
#undef  FD_DELEGATE_TEMPLATE_ARGS
#undef  FD_DELEGATE_TEMPLATE_ARGS2_SP
#undef  FD_DELEGATE_TEMPLATE_ARG2_SP
#undef  FD_DELEGATE_PARMS
#undef  FD_DELEGATE_PARM
#undef  FD_DELEGATE_ARGS_AS_MEMBER
#undef  FD_DELEGATE_ARG_AS_MEMBER
#undef  FD_DELEGATE_BOUND_ARGS
#undef  FD_DELEGATE_BOUND_ARG
#undef  FD_DELEGATE_ARGS
#undef  FD_DELEGATE_ARGS_SP
#undef  FD_DELEGATE_ARG_SP
#undef  FD_DELEGATE_CALL_TRAITS_PARMS
#undef  FD_DELEGATE_CALL_TRAITS_PARM
#undef  FD_DELEGATE_ARG_TYPES
#undef  FD_DELEGATE_ARG_TYPE
#undef  FD_DELEGATE_BIND_PLACEHOLDER
#undef  FD_DELEGATE_DEFAULT_ALLOCATOR
#undef  FD_DELEGATE_COMMA
#undef  FD_DELEGATE_COMMA_SP
#undef  FD_DELEGATE_FXN_TABLE
#undef  FD_DELEGATE_STUB_BASE
#undef  FD_DELEGATE_NULL_TYPE
#undef  FD_DELEGATE_INVOKER
#undef  FD_DELEGATE_BIND
#undef  FD_DELEGATE_FUNCTOR_PTR
#undef  FD_DELEGATE_FUNCTOR_REF
#undef  FD_DELEGATE_FUNCTOR_OBJ
#undef  FD_DELEGATE_FUNCTOR_STATELESS
#undef  FD_DELEGATE_MFN_BIND_ON_PTR
#undef  FD_DELEGATE_MFN_BIND_ON_REF
#undef  FD_DELEGATE_MFN_BIND_ON_OBJ
#undef  FD_DELEGATE_MFN_BIND_ON_CUSTOM
#undef  FD_DELEGATE_MFN_ON_PTR
#undef  FD_DELEGATE_MFN_ON_REF
#undef  FD_DELEGATE_MFN_ON_OBJ
#undef  FD_DELEGATE_MFN_ON_CUSTOM
#undef  FD_DELEGATE_FUNCTION
#undef  FD_DELEGATE_MULTICAST
#undef  FD_DELEGATE_MULTICAST_TYPE
#undef  FD_DELEGATE_DELEGATE_IMPL
#undef  FD_DELEGATE_DELEGATE
#undef  FD_DELEGATE_ARGS_STRUCT
#undef  FD_DELEGATE_CALL_BOUND
