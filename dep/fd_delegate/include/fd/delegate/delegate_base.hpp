#ifndef FD_DELEGATE_BASE_HEADER
#define FD_DELEGATE_BASE_HEADER

#include <stdexcept>
#include <typeinfo>
#include <deque>
#include <algorithm>
#include <boost/config.hpp>
#include <boost/pending/ct_if.hpp>
#include <boost/ref.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_stateless.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_member_function_pointer.hpp>
#include <boost/call_traits.hpp>
#include <boost/detail/workaround.hpp>
#ifndef BOOST_NO_SFINAE
#  include <boost/utility/enable_if.hpp>
#else
#  include <boost/mpl/bool.hpp>
#  include <boost/assert.hpp>
#endif
#include <boost/function_equal.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/mem_fn.hpp>
#include <boost/static_assert.hpp>

// Borrowed from Boost.Python library: determines the cases where we
// need to use std::type_info::name to compare instead of operator==.
# if (defined(__GNUC__) && __GNUC__ >= 3) \
  || defined(_AIX) \
  || (   defined(__sgi) && defined(__host_mips))
#  include <cstring>
#  define FD_DELEGATE_COMPARE_TYPE_ID(X,Y) \
  (std::strcmp((X).name(),(Y).name()) == 0)
# else
#  define FD_DELEGATE_COMPARE_TYPE_ID(X,Y) ((X)==(Y))
#endif

#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300 || defined(__ICL) && __ICL <= 600 || defined(__MWERKS__) && __MWERKS__ < 0x2406 && !defined(BOOST_STRICT_CONFIG)
#  define FD_DELEGATE_TARGET_FIX(x) x
#else
#  define FD_DELEGATE_TARGET_FIX(x)
#endif // not MSVC

#if defined (BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)                     \
  || defined(BOOST_BCB_PARTIAL_SPECIALIZATION_BUG)                         \
  || !(BOOST_STRICT_CONFIG || !defined(__SUNPRO_CC) || __SUNPRO_CC > 0x540)
#  define FD_DELEGATE_NO_FUNCTION_TYPE_SYNTAX
#endif

#if !BOOST_WORKAROUND(__BORLANDC__, < 0x600)
#  define FD_DELEGATE_ENABLE_IF_NOT_INTEGRAL(Functor,Type)                    \
      typename boost::enable_if_c<(::boost::type_traits::ice_not<           \
                            (::boost::is_integral<Functor>::value)>::value),  \
                            Type>::type

#  define FD_DELEGATE_ENABLE_IF_EQUALITY_COMPARABLE(Type)                         \
      typename boost::enable_if_c<(::fd::is_equality_comparable<Type>::value),  \
                            bool>::type

#  define FD_DELEGATE_DISABLE_IF_EQUALITY_COMPARABLE(Type)                        \
      typename boost::disable_if_c<(::fd::is_equality_comparable<Type>::value), \
                            bool>::type

#else
// BCC doesn't recognize this depends on a template argument and complains
// about the use of 'typename'
#  define FD_DELEGATE_ENABLE_IF_NOT_INTEGRAL(Functor,Type)                    \
      boost::enable_if_c<(::boost::type_traits::ice_not<                    \
                            (::boost::is_integral<Functor>::value)>::value),  \
                            Type>::type

#  define FD_DELEGATE_ENABLE_IF_EQUALITY_COMPARABLE(Type)                \
      boost::enable_if_c<(::fd::is_equality_comparable<Type>::value),  \
                        bool>::type

#  define FD_DELEGATE_DISABLE_IF_EQUALITY_COMPARABLE(Type)               \
      boost::disable_if_c<(::fd::is_equality_comparable<Type>::value), \
                        bool>::type
#endif

#  define FD_DELEGATE_EQUALITY_COMPARABLE_TYPE(Type)            \
  namespace fd {                                                \
  template<>                                                    \
  struct is_equality_comparable<Type>                           \
  {                                                             \
    BOOST_STATIC_CONSTANT( bool, value = true );                \
  }; }

#ifdef BOOST_NO_VOID_RETURNS
#  define FD_DELEGATE_NO_VOID_RETURNS
#endif

#ifdef BOOST_NO_STD_ALLOCATOR
#  define FD_DELEGATE_NO_STD_ALLOCATOR
#endif

#ifndef FD_DELEGATE_NO_VOID_RETURNS
#  define FD_DELEGATE_VOID_RETURN_TYPE void
#  define FD_DELEGATE_RETURN(X) X
#else
#  define FD_DELEGATE_VOID_RETURN_TYPE fd::detail::delegate::unusable
#  define FD_DELEGATE_RETURN(X) X; return FD_DELEGATE_VOID_RETURN_TYPE()
#endif

#if ( defined(__INTEL_COMPILER) && ( defined(_WIN32) || defined(__WIN32__) || defined(WIN32) ) ) || defined(_MSC_VER)
#  define FD_DELEGATE_FASTCALL __fastcall
#else
#  define FD_DELEGATE_FASTCALL
#endif

#if defined (_MSC_VER)
#  define FD_NO_CAST_POINTER_TO_MEMBER_UNRELATED_CLASS
#endif

#if !defined(FD_DELEGATE_NO_FUNCTION_TYPE_SYNTAX)

namespace fd
{

#ifndef FD_DELEGATE_NO_STD_ALLOCATOR
#if defined(__sgi) && defined(_COMPILER_VERSION) && _COMPILER_VERSION <= 730 && !defined(BOOST_STRICT_CONFIG)
// The library shipping with MIPSpro 7.3.1.3m has a broken allocator<void>
class delegate_base;

template<typename Signature, typename Allocator = std::allocator<delegate_base> >
class delegate;
#else
  template<typename Signature, typename Allocator = std::allocator<void> >
  class delegate;
#endif
#else
  template<typename Signature, typename Allocator = int >
  class delegate;
#endif

template<typename Signature, typename Allocator> inline
void swap(delegate<Signature, Allocator> & d1,
          delegate<Signature, Allocator> & d2)
{
  d1.swap( d2 );
}

} // namespace fd

#endif // have partial specialization

namespace fd
{

namespace multicast
{
  // Forward declaration.
  class token;
}

/**
 * boost::function_equal() is brought into fd namespace.
 */
using boost::function_equal;
using boost::function_equal_impl;

namespace detail { namespace delegate
{

/**
 * A trick to retrieve the address of the given object without using & operator.
 */
template<typename T>
T * addressof(T & v)
{
#if defined(BOOST_MSVC) && (BOOST_MSVC < 1300)
  return &v;
#else
  return boost::addressof( v );
#endif
}

/**
 * boost::get_pointer() s are brought into fd::detail::delegate namespace.
 */
using boost::get_pointer;

/**
 * boost::type_traits::yes_type and no_type are brought into fd::detail::delegate namespace.
 */
using boost::type_traits::yes_type;
using boost::type_traits::no_type;

/**
 * Helper class template for is_<T>::template pointer_of<U>
 */
template<typename U>
struct is_pointer_of_helper
{
  static yes_type tester_(U const volatile *);
  static no_type  tester_(...);
};

/**
 * Helper class template for is_<T>::template reference_of<U>
 */
template<typename U>
struct is_reference_of_helper
{
  static yes_type tester_(U const volatile *);
  static no_type  tester_(...);
};

template<typename T>
struct is_
{
  template<typename U>
  struct pointer_of_impl
  {
    static T testee_;
    BOOST_STATIC_CONSTANT( bool, value = sizeof( yes_type ) == sizeof( is_pointer_of_helper<U>::tester_( testee_ ) ) );
  };

  /**
   * Template type trait to determine whether or not the specified parameterized type P
   * derived from the template parameter declaration of T is a matching a pointer type of
   * the underlying type U. If type P is one of matching compound types of U *, U const *,
   * U volatile * or U const volatile *, the evaluated template value holds true.
   */
  template<typename U>
  struct pointer_of_
    : boost::integral_constant<bool, (::fd::detail::delegate::is_<T>::template pointer_of_impl<U>::value)> { };

  template<typename U>
  struct reference_of_impl
  {
    static T testee_;
    BOOST_STATIC_CONSTANT( bool, value =
      (::boost::type_traits::ice_and<
      ( sizeof( yes_type ) == sizeof( is_pointer_of_helper<U>::tester_( &testee_ ) ) ),
      ( ::boost::is_reference<T>::value )>::value) );
  };

  /**
   * Template type trait to determine whether or not the specified parameterized type P
   * derived from the template parameter declaration of T is a matching a reference type of
   * the underlying type U. If type P is one of matching compound types of U &, U const &,
   * U volatile & or U const volatile &, the evaluated template value holds true.
   */
  template<typename U>
  struct reference_of_
    : boost::integral_constant<bool, (::fd::detail::delegate::is_<T>::template reference_of_impl<U>::value)> { };

};

/**
 * Helper class to determine proper const-ness according to template parameter declaration of T.
 */
template<typename T>
struct select_void_pointer
{
  typedef
    typename boost::ct_if<(::boost::is_const<T>::value), void const *, void *>::type
    type;
};

/**
 * The unusable class is a placeholder for unused function arguments
 * It is also completely unusable except that it constructible from
 * anything. This helps compilers without partial specialization to
 * handle Boost.Delegate objects returning void.
 */
struct unusable
{
  unusable() { }
  template<typename T> unusable(T const &) { }
};

/**
 * Helps some compilers which can not distinguish overload resolution between pointer and function object.
 * See assign_to().
 */
struct pointer_tag      { typedef pointer_tag type;  };
struct mfn_ptr_tag      { typedef mfn_ptr_tag type; };
struct function_obj_tag { typedef function_obj_tag type; };

template<typename F>
struct get_function_tag
{
  typedef typename boost::ct_if<
    (::boost::is_pointer<F>::value),
    pointer_tag,
    boost::ct_if<
      (::boost::is_member_function_pointer<F>::value),
      mfn_ptr_tag,
      function_obj_tag
    >
  >::type type_1;
  typedef typename type_1::type type;
};

/**
 * Determine the return type. This supports compilers that do not support
 * void returns or partial specialization by silently changing the return
 * type to "unusable".
 */
template<typename T> struct delegate_return_type       { typedef T        type; };
template<>           struct delegate_return_type<void> { typedef unusable type; };

/**
 * A type that is only used for comparisons against zero
 */
struct useless_clear_type { };

/**
 * Define a generic function.
 */
typedef void generic_fxn();

class alignment_dummy_base1 { };
class alignment_dummy_base2 { };

class alignment_dummy_s : alignment_dummy_base1 { };                         // single inheritance.
class alignment_dummy_m : alignment_dummy_base1, alignment_dummy_base2 { };  // multiple inheritance.
class alignment_dummy_v : virtual alignment_dummy_base1 { };                 // virtual inheritance.
class alignment_dummy_u;                                                     // unknown (incomplete).

typedef void (alignment_dummy_s::*mfn_ptr_s)();  // member function pointer of single inheritance class.
typedef void (alignment_dummy_m::*mfn_ptr_m)();  // member function pointer of multiple inheritance class.
typedef void (alignment_dummy_v::*mfn_ptr_v)();  // member function pointer of virtual inheritance class.
typedef void (alignment_dummy_u::*mfn_ptr_u)();  // member function pointer of unknown (incomplete) class.

typedef void (alignment_dummy_m::*generic_mfn_ptr)();

union max_align_for_funtion_pointer
{
  void const * dummy_vp;
  generic_fxn * dummy_fp;
  boost::ct_if<( sizeof( generic_mfn_ptr ) < sizeof( mfn_ptr_s ) ), generic_mfn_ptr, mfn_ptr_s>::type dummy_mfp1;
  boost::ct_if<( sizeof( generic_mfn_ptr ) < sizeof( mfn_ptr_m ) ), generic_mfn_ptr, mfn_ptr_m>::type dummy_mfp2;
  boost::ct_if<( sizeof( generic_mfn_ptr ) < sizeof( mfn_ptr_v ) ), generic_mfn_ptr, mfn_ptr_v>::type dummy_mfp3;
  boost::ct_if<( sizeof( generic_mfn_ptr ) < sizeof( mfn_ptr_u ) ), generic_mfn_ptr, mfn_ptr_u>::type dummy_mfp4;
};

BOOST_STATIC_CONSTANT( unsigned, any_fxn_size = sizeof( max_align_for_funtion_pointer ) );

union any_fxn_pointer
{
  void const * obj_ptr;
  generic_fxn * fxn_ptr;
  generic_mfn_ptr mfn_ptr;

  max_align_for_funtion_pointer m_;
};

/**
 *
 */
template<typename TFxn>
struct simplify_mfn
{
#ifdef FD_NO_CAST_POINTER_TO_MEMBER_UNRELATED_CLASS

  union fconv { TFxn fxn; generic_mfn_ptr gfxn; };
  static generic_mfn_ptr convert(TFxn fxn)
  {
    BOOST_STATIC_ASSERT( sizeof( TFxn ) <= any_fxn_size );
    fconv fcv = { fxn };
    return fcv.gfxn;
  }

  static TFxn restore(generic_mfn_ptr gfxn)
  {
    fconv fcv; fcv.gfxn = gfxn;
    return fcv.fxn;
  }

#else // #ifdef FD_NO_CAST_POINTER_TO_MEMBER_UNRELATED_CLASS

  static generic_mfn_ptr convert(TFxn fxn)
  {
    BOOST_STATIC_ASSERT( sizeof( TFxn ) <= any_fxn_size );
    return reinterpret_cast<generic_mfn_ptr>( fxn );
  }

  static TFxn restore(generic_mfn_ptr gfxn)
  {
    return reinterpret_cast<TFxn>( gfxn );
  }

#endif  // #ifdef FD_NO_CAST_POINTER_TO_MEMBER_UNRELATED_CLASS

};

/**
 * Data hold to store callable entities.
 */
struct delegate_holder
{
  // Stores the object pointer. The pointer stored varies according to what the user is
  // storing for and how it is being stored.
  // For function object, it is a pointer to the function object which is being stored.
  // For member function, it can be either a pointer to the object or a pointer to a smart
  // pointer object which points the object on which member is being called.
  // For free function, it is not used.
  // For multicast delegate, it is a pointer to the container (std::deque) which maintains
  // the list of delegates to be called.
  void const * obj_ptr;

  // Stores any function pointer whose size is smaller than size of the any_fxn_size so that
  // fit into the structure. It will be 8 bytes in most compilers.
  any_fxn_pointer any;

  // Stores the pointer to function reference table generated at compile time to manipulate
  // obj_ptr and fxn_ptr above according to the requirement of function type of the callable
  // entity stored in delegate. It can be thought of as static typing or static binding version
  // of virtual function table in C++. Refer to Boost Interface Library (BIL) for more details.
  void const * tbl_ptr;
};

/**
 * fxn_table_base structure contains function pointers entries required for the basic
 * behavioral requirement of delegates. These requirements are applied to all delegates
 * in the defined specific situation. For example, delete_obj() will be called when a
 * delegate goes out of its scope whether delete_obj() actually performs some clean-up
 * or not is totally depending on the requirement for the specific function type.
 *
 * The most important entry of function reference table, which is invoke(), is dependent
 * on the function type signature, therefore fxn_tableN class template are to be defined
 * for different function type signature and they are all inherited from fxn_table_base.
 */
struct fxn_table_base
{
  /* General information query */

  // Returns the size of the underlying callable entity stored. But, for mutlicast, it
  // returns the number of delegates in the list instead.
  unsigned (*size_of_fxn)(void const *);
  // Returns the type information of the underlying callable entity stored.
  std::type_info const & (*type_of_fxn)(void const *);
  // Compares equality between delegates of the same type.
  bool (*compare_equal_same_type)(delegate_holder const &, delegate_holder const &);
  // Non contextual memory comparison between two arbitrary delegates.
  int (*memcmp_delegate)(delegate_holder const &, delegate_holder const &);
  // Determines whether or not the underlying callable entity is a kind of function object.
  bool (*is_functor)();
  // Determines whether or not the delegate is empty.
  bool (*is_empty)();

  /* Object manager */

  // Copies the source into delegate_holder.obj_ptr.
  void (FD_DELEGATE_FASTCALL *copy_obj)(delegate_holder &, void const *);
  // Deletes delegate_holder.obj_ptr.
  void (FD_DELEGATE_FASTCALL *delete_obj)(delegate_holder &);

  /* Multicast support */

  // Determines whether or not it is a multicast delegate.
  bool (*is_multicast)();
  // Adds one or more delegates into the list.
  unsigned (*add_delegates)(void const *, delegate_holder const &, void *);
  // Removes one or more delegates from the list.
  unsigned (*remove_delegates)(void const *, delegate_holder const &);
  // Determines whether or not the specified delegate exists in multicast.
  bool (*find_delegate)(void const *, delegate_holder const &);

};  // struct fxn_table_base

/**
 * Delegate Holder utility functions.
 */
struct dh_util
{

/**
 * Meaningless memory addresses comparison.
 */
static int compare_memory(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs)
{
  if(dh_lhs.obj_ptr == dh_rhs.obj_ptr)
  {
    fxn_table_base const * ftable_base_lhs = static_cast<fxn_table_base const *>( dh_lhs.tbl_ptr );
    fxn_table_base const * ftable_base_rhs = static_cast<fxn_table_base const *>( dh_rhs.tbl_ptr );

    unsigned sz_fn_cmp = ftable_base_lhs->
      size_of_fxn( dh_lhs.obj_ptr ) < ftable_base_rhs->size_of_fxn( dh_rhs.obj_ptr )
      ? ftable_base_lhs->size_of_fxn( dh_lhs.obj_ptr )
      : ftable_base_rhs->size_of_fxn( dh_rhs.obj_ptr );

    int memcmp_result = ::memcmp( dh_rhs.any.obj_ptr, dh_rhs.any.obj_ptr, sz_fn_cmp );
    return 0 == memcmp_result
      ? ftable_base_lhs->size_of_fxn( dh_lhs.obj_ptr ) - ftable_base_rhs->size_of_fxn( dh_rhs.obj_ptr )
      : memcmp_result;
  }
  else
  {
    return static_cast<int>( static_cast<char const *>( dh_lhs.obj_ptr ) - static_cast<char const *>( dh_rhs.obj_ptr ) );
  }
}

/**
 * Compares equality of two delegates.
 */
static bool compare_holder(delegate_holder const & lhs, delegate_holder const & rhs)
{
  typedef fd::detail::delegate::fxn_table_base fxn_table_base_type;

  fxn_table_base_type const * ftable_base_lhs = static_cast<fxn_table_base_type const *>( lhs.tbl_ptr );
  fxn_table_base_type const * ftable_base_rhs = static_cast<fxn_table_base_type const *>( rhs.tbl_ptr );

  return FD_DELEGATE_COMPARE_TYPE_ID( ftable_base_lhs->type_of_fxn(lhs.obj_ptr), ftable_base_rhs->type_of_fxn(rhs.obj_ptr) ) &&
    ftable_base_lhs->compare_equal_same_type( lhs, rhs );
}

/**
 * Compares less than.
 */
static bool compare_less_than(delegate_holder const & lhs, delegate_holder const & rhs)
{
  typedef fd::detail::delegate::fxn_table_base fxn_table_base_type;

  if( static_cast<fxn_table_base_type const *>( rhs.tbl_ptr )->is_multicast() )
  {
    return 0 > static_cast<fxn_table_base_type const *>( rhs.tbl_ptr )->memcmp_delegate( rhs, lhs );
  }
  else
  {
    return 0 < static_cast<fxn_table_base_type const *>( lhs.tbl_ptr )->memcmp_delegate( lhs, rhs );
  }
}

};  // struct dh_util

/**
 * When a pointer or a reference, to function object or to the bound object on which
 * member function call is being made, is stored into delegate_holder.obj_ptr. It is
 * caller's responsibility to verify and make sure that the object stored is still valid
 * and intact at the moment of the invocation. Therefore, when a delegate is copied we just
 * need to copy the void pointer and when it is destructed as it goes out of the scope,
 * we do not need to do anything at all.
 */
struct ptr_manager
{
  static void FD_DELEGATE_FASTCALL copy_obj(delegate_holder & dh_dst, void const * src_ptr)
  {
    dh_dst.obj_ptr = src_ptr;
  }

  static void FD_DELEGATE_FASTCALL delete_obj(delegate_holder &) { }
};

/**
 * For non-multicast delegate, we do nothing.
 */
struct single_cast_stub
{
  static bool is_multicast() { return false; }
  static unsigned add_delegates(void const *, delegate_holder const &, void *) { return 0; }
  static unsigned remove_delegates(void const *, delegate_holder const &) { return 0; }
  static bool find_delegate(void const *, delegate_holder const &) { return false; }
};

/**
 * null_type stub.
 */
struct null_type
{
  static unsigned size_of_fxn(void const *) { return 0; }

  static std::type_info const & type_of_fxn(void const *) { return typeid( null_type ); }

  // Comparing two null types returns true always.
  static bool compare_equal_same_type(delegate_holder const &, delegate_holder const &) { return true; }

  static int memcmp_delegate(delegate_holder const & dh_lhs, delegate_holder const & dh_rhs) { return 0; }

  static bool is_functor() { return false; }

  static bool is_empty() { return true; }

  // Gets static function reference table.
  static fxn_table_base const * get_table()
  {
    // Initialized the base function reference table.
    static fxn_table_base const static_ftable_base =
    {
      &size_of_fxn, &type_of_fxn, &compare_equal_same_type, &memcmp_delegate, &is_functor, &is_empty,
      &ptr_manager::copy_obj, &ptr_manager::delete_obj,
      &single_cast_stub::is_multicast, &single_cast_stub::add_delegates, &single_cast_stub::remove_delegates, &single_cast_stub::find_delegate,
    };

    return &static_ftable_base;
  }
};  // struct null_type

// ----------------------------------------------------------------------------------------------------
#ifdef BOOST_NO_SFINAE
// These routines perform comparisons between a FD.Delegate
// object and an arbitrary function object (when the last
// parameter is boost::mpl::bool_<false>) or against zero (when the
// last parameter is boost::mpl::bool_<true>). They are only necessary
// for compilers that don't support SFINAE.
template<typename Delegate, typename F>
bool compare_equal_impl(Delegate const & d, F const &, int, boost::mpl::bool_<true>)
{ return d.empty(); }

template<typename Delegate, typename F>
bool compare_not_equal_impl(Delegate const & d, F const &, int,
                  boost::mpl::bool_<true>)
{ return !d.empty(); }

template<typename Delegate, typename F>
bool compare_equal_impl(Delegate const & d, F const & f, long,
              boost::mpl::bool_<false>)
{
  if( F const * fp = d.template target<F>() )
    return function_equal( *fp, f );
  else return false;
}

template<typename Delegate, typename F>
bool compare_equal_impl(Delegate const & d, boost::reference_wrapper<F> const & f,
              int, boost::mpl::bool_<false> )
{
  if( F const * fp = d.template target<F>() )
    return fp == f.get_pointer();
  else return false;
}

template<typename Delegate, typename F>
bool compare_not_equal_impl( Delegate const & d, F const & f, long,
                  boost::mpl::bool_<false> )
{
  if(F const * fp = d.template target<F>())
    return !function_equal(*fp, f);
  else return true;
}

template<typename Delegate, typename F>
bool compare_not_equal_impl(Delegate const & d,
                  boost::reference_wrapper<F> const & f, int,
                  boost::mpl::bool_<false>)
{
  if (F const * fp = d.template target<F>())
    return fp != f.get_pointer();
  else return true;
}
#endif // BOOST_NO_SFINAE

// ----------------------------------------------------------------------------------------------------

#ifdef BOOST_NO_SFINAE
// These routines perform address comparison between FD.Delegate
// objects of same type (when the last parameter is boost::mpl::bool_<false>)
// or value comparison through operator ==() between FD.Delegate objects of
// same type (when the last parameter is boost::mpl::bool_<true>). They are
// only necessary for compilers that don't support SFINAE.

template<typename T>
bool compare_equal_impl(T const & lhs, T const & rhs, boost::mpl::bool_<true>)
{
  // FD_DELEGATE_EQUALITY_COMPARABLE_TYPE should implement the EqualityComparable concept and
  // operator ==() for type T, at least, should be available.
  return lhs == rhs;
}

template<typename T>
bool compare_equal_impl(T const & lhs, T const & rhs, boost::mpl::bool_<false>)
{
  return false;
}
#endif // BOOST_NO_SFINAE

} } // namespace detail::delegate

/**
 * The delegate_base class contains the basic elements needed for the
 * delegate_impl1, delegate_impl2, delegate_impl3, etc. classes. It is common
 * to all delegates (and as such can be used to tell if we have one of the
 * delegate_implN objects).
 *
 * Implements concepts of "CopyConstructible" and "Assignable",
 */
class delegate_base
{
private:
  friend class fd::multicast::token;

  typedef fd::detail::delegate::fxn_table_base  fxn_table_base_type;

  /**
   *
   */
  fd::detail::delegate::delegate_holder dh_;

protected:
  typedef fd::detail::delegate::delegate_holder delegate_holder;

  explicit delegate_base(void const * null_tbl_ptr)
  {
    dh_.obj_ptr = 0;
    dh_.any.mfn_ptr = 0;
    dh_.tbl_ptr = null_tbl_ptr;
  }

  delegate_holder & get_holder()
  {
    return dh_;
  }

public:
  delegate_base()
  {
    dh_.obj_ptr = 0;
    dh_.any.mfn_ptr = 0;
    dh_.tbl_ptr = fd::detail::delegate::null_type::get_table();
  }

  explicit delegate_base(delegate_holder const & dh)
  {
    static_cast<fxn_table_base_type const *>( dh.tbl_ptr )->copy_obj( dh_, dh.obj_ptr );
    dh_.any.mfn_ptr = dh.any.mfn_ptr;
    dh_.tbl_ptr = dh.tbl_ptr;
  }

  delegate_base(delegate_base const & other)
  {
    static_cast<fxn_table_base_type const *>( other.dh_.tbl_ptr )->copy_obj( dh_, other.dh_.obj_ptr );
    dh_.any.mfn_ptr = other.dh_.any.mfn_ptr;
    dh_.tbl_ptr = other.dh_.tbl_ptr;
  }

  ~delegate_base()
  {
    static_cast<fxn_table_base_type const *>( dh_.tbl_ptr )->delete_obj( dh_ );
  }

  delegate_holder const & get_holder() const
  {
    return dh_;
  }

  delegate_base & operator =(delegate_base const & other)
  {
    if( this != &other )
    {
      delegate_base( other ).swap( *this );
    }
    return *this;
  }

  void swap(delegate_base & other)
  {
    std::swap( dh_.obj_ptr, other.dh_.obj_ptr );
    std::swap( dh_.any.mfn_ptr, other.dh_.any.mfn_ptr );
    std::swap( dh_.tbl_ptr, other.dh_.tbl_ptr );
  }

  /**
   *
   */
  bool empty() const
  {
    return static_cast<fxn_table_base_type const *>( dh_.tbl_ptr )->is_empty();
  }

  /**
   * std::type_info.
   */
  std::type_info const & type() const
  {
    return static_cast<fxn_table_base_type const *>( dh_.tbl_ptr )->type_of_fxn( dh_.obj_ptr );
  }

  /**
   *
   */
  template<typename F>
  F * target()
  {
    fxn_table_base_type const * ftable_base = static_cast<fxn_table_base_type const *>( dh_.tbl_ptr );

    if( FD_DELEGATE_COMPARE_TYPE_ID( ftable_base->type_of_fxn( dh_.obj_ptr ), typeid( F ) ) )
    {
      if( ftable_base->is_functor() )
      {
        return static_cast<F *>( const_cast<typename fd::detail::delegate::select_void_pointer<F>::type>( dh_.obj_ptr ) );
      }
      else
      {
        // See boost::function_base::get_functor_pointer().
        return reinterpret_cast<F *>( &dh_.any.fxn_ptr );
      }
    }
    else
    {
      return 0;
    }
  }

  /**
   *
   */
  template<typename F>
#if defined(BOOST_MSVC) && BOOST_WORKAROUND(BOOST_MSVC, < 1300)
  F const * target(F * = 0) const
#else
  F const * target() const
#endif
  {
    fxn_table_base_type const * ftable_base = static_cast<fxn_table_base_type const *>( dh_.tbl_ptr );

    if( FD_DELEGATE_COMPARE_TYPE_ID( ftable_base->type_of_fxn( dh_.obj_ptr ), typeid( F ) ) )
    {
      if( ftable_base->is_functor() )
      {
        return static_cast<F const *>( dh_.obj_ptr );
      }
      else
      {
        // See boost::function_base::get_functor_pointer().
        return reinterpret_cast<F const *>( &dh_.any.fxn_ptr );
      }
    }
    else
    {
      return 0;
    }
  }

  /**
   *
   */
  template<typename F>
  bool contains(F const & f) const
  {
    fxn_table_base_type const * ftable_base = static_cast<fxn_table_base_type const *>( dh_.tbl_ptr );

#if defined(BOOST_MSVC) && BOOST_WORKAROUND(BOOST_MSVC, < 1300)
    if( F const * fp = this->target( (F *)0 ) )
    {
#else
    if( F const * fp = this->template target<F>() )
    {
#endif
      return function_equal( *fp, f );
    }
    else
    {
      return false;
    }
  }

#if defined(__GNUC__) && __GNUC__ == 3 && __GNUC_MINOR__ <= 3
  // GCC 3.3 and newer cannot copy with the global operator==, due to
  // problems with instantiation of function return types before it
  // has been verified that the argument types match up.
  template<typename F>
  BOOST_FUNCTION_ENABLE_IF_NOT_INTEGRAL(F, bool)
    operator==(F f) const
  {
    if (F const * fp = target<F>())
      return function_equal( *fp, f );
    else return false;
  }

  template<typename F>
  BOOST_FUNCTION_ENABLE_IF_NOT_INTEGRAL(F, bool)
    operator!=(F g) const
  {
    if (F const * fp = target<F>())
      return !function_equal( *fp, f );
    else return true;
  }
#endif

};  // struct delegate_base

// ----------------------------------------------------------------------------------------------------

#ifndef BOOST_NO_SFINAE
inline bool operator==(delegate_base const & d,
                       fd::detail::delegate::useless_clear_type *)
{
  return d.empty();
}

inline bool operator!=(delegate_base const & d,
                       fd::detail::delegate::useless_clear_type *)
{
  return !d.empty();
}

inline bool operator==(fd::detail::delegate::useless_clear_type *,
                       delegate_base const & d)
{
  return d.empty();
}

inline bool operator!=(fd::detail::delegate::useless_clear_type *,
                       delegate_base const & d)
{
  return !d.empty();
}
#endif

#ifdef BOOST_NO_SFINAE
// Comparisons between delegate objects and arbitrary function objects
template<typename F>
inline bool operator==(delegate_base const & d, F f)
{
  typedef boost::mpl::bool_<(::boost::is_integral<F>::value)> integral;
  return fd::detail::delegate::compare_equal_impl(d, f, 0, integral());
}

template<typename F>
inline bool operator==(F f, delegate_base const & d)
{
  typedef boost::mpl::bool_<(::boost::is_integral<F>::value)> integral;
  return fd::detail::delegate::compare_equal_impl(d, f, 0, integral());
}

template<typename F>
inline bool operator!=(delegate_base const & d, F f)
{
  typedef boost::mpl::bool_<(::boost::is_integral<F>::value)> integral;
  return fd::detail::delegate::compare_not_equal_impl(d, f, 0, integral());
}

template<typename F>
inline bool operator!=(F f, delegate_base const & d)
{
  typedef boost::mpl::bool_<(::boost::is_integral<F>::value)> integral;
  return fd::detail::delegate::compare_not_equal_impl(d, f, 0, integral());
}
#else

#  if !(defined(__GNUC__) && __GNUC__ == 3 && __GNUC_MINOR__ <= 3)
// Comparisons between delegate objects and arbitrary function
// objects. GCC 3.3 and before has an obnoxious bug that prevents this
// from working.
template<typename F>
FD_DELEGATE_ENABLE_IF_NOT_INTEGRAL(F, bool)
operator==(delegate_base const & d, F f)
{
  if (F const * fp = d.template target<F>())
    return function_equal(*fp, f);
  else return false;
}

template<typename F>
FD_DELEGATE_ENABLE_IF_NOT_INTEGRAL(F, bool)
operator==(F f, delegate_base const & d)
{
  if (F const * fp = d.template target<F>())
    return function_equal(f, *fp);
  else return false;
}

template<typename F>
FD_DELEGATE_ENABLE_IF_NOT_INTEGRAL(F, bool)
operator!=(delegate_base const & d, F f)
{
  if (F const * fp = d.template target<F>())
    return !function_equal(*fp, f);
  else return true;
}

template<typename F>
FD_DELEGATE_ENABLE_IF_NOT_INTEGRAL(F, bool)
operator!=(F f, delegate_base const & d)
{
  if (F const * fp = d.template target<F>())
    return !function_equal(f, *fp);
  else return true;
}
#  endif

template<typename F>
FD_DELEGATE_ENABLE_IF_NOT_INTEGRAL(F, bool)
operator==(delegate_base const & d, boost::reference_wrapper<F> f)
{
  if (F const * fp = d.template target<F>())
    return fp == f.get_pointer();
  else return false;
}

template<typename F>
FD_DELEGATE_ENABLE_IF_NOT_INTEGRAL(F, bool)
operator==(boost::reference_wrapper<F> f, delegate_base const & d)
{
  if (F const * fp = d.template target<F>())
    return f.get_pointer() == fp;
  else return false;
}

template<typename F>
FD_DELEGATE_ENABLE_IF_NOT_INTEGRAL(F, bool)
operator!=(delegate_base const & d, boost::reference_wrapper<F> f)
{
  if (F const * fp = d.template target<F>())
    return fp != f.get_pointer();
  else return true;
}

template<typename F>
FD_DELEGATE_ENABLE_IF_NOT_INTEGRAL(F, bool)
operator!=(boost::reference_wrapper<F> f, delegate_base const & d)
{
  if (F const * fp = d.template target<F>())
    return f.get_pointer() != fp;
  else return true;
}

#endif // Compiler supporting SFINAE

// ----------------------------------------------------------------------------------------------------

/**
 * Compare the objects of the same type.
 */
template<typename T>
struct is_equality_comparable
{
  BOOST_STATIC_CONSTANT( bool, value = false );
};

#ifdef BOOST_NO_SFINAE

template<typename T>
bool compare_equal(T const & lhs, T const & rhs)
{
  typedef boost::mpl::bool_<(::fd::is_equality_comparable<T>::value)> integral;
  return fd::detail::delegate::compare_equal_impl( lhs, rhs, integral() );
}
#else

template<typename T>
FD_DELEGATE_ENABLE_IF_EQUALITY_COMPARABLE(T)
compare_equal(T const & lhs, T const & rhs)
{
  // FD_DELEGATE_EQUALITY_COMPARABLE_TYPE should implement the EqualityComparable concept and
  // operator ==() for type T, at least, should be available.
  return lhs == rhs;
}

template<typename T>
FD_DELEGATE_DISABLE_IF_EQUALITY_COMPARABLE(T)
compare_equal(T const &, T const &)
{
  return false;
}
#endif // Compiler supporting SFINAE

// --------------------------------------------------------------------------------

/**
 * The bad_function_call exception class is thrown when an empty
 * delegate object is invoked.
 */
class bad_function_call : public std::runtime_error
{
public:
  bad_function_call() : std::runtime_error("call to empty delegate") { }
};

namespace detail { namespace delegate {

/**
 * We can only store the copy of an arbitrary object by allocating it on heap memory since the size of
 * the object is unknown. And, in such cases, the stored object's life time will be managed by delegate.
 * For member function, it is possible to store a smart pointer object which points the object on which
 * member function call is being invoked, then the life time of the target object can be automatically
 * maintained by the mechanism of the supplied smart pointer.
 * Note that multicast also uses obj_manager<> to manage a container to maintain the list of delegates
 * internally.
 */
template<typename T, typename Allocator>
struct obj_manager
{
  static void FD_DELEGATE_FASTCALL copy_obj(delegate_holder & dh_dst, void const * src_ptr)
  {
#ifndef FD_DELEGATE_NO_STD_ALLOCATOR
    typedef typename Allocator::template rebind<T>::other allocator_type;
    typedef typename allocator_type::pointer pointer_type;
#else
    typedef T * pointer_type;
#endif // FD_DELEGATE_NO_STD_ALLOCATOR

#  ifndef FD_DELEGATE_NO_STD_ALLOCATOR
    allocator_type allocator;
#  endif // FD_DELEGATE_NO_STD_ALLOCATOR

#  ifndef FD_DELEGATE_NO_STD_ALLOCATOR
    pointer_type dst_obj_ptr = allocator.allocate( 1 );
    // Uses placement new instead of allocator::construct() so that
    // std::auto_ptr works as well.
    // new ( dst_obj_ptr ) T( *static_cast<pointer_type>( const_cast<void *>( src_ptr ) ) );
    allocator.construct( dst_obj_ptr, *static_cast<pointer_type>( const_cast<void *>( src_ptr ) ) );

    dh_dst.obj_ptr = dst_obj_ptr;
#  else
    dh_dst.obj_ptr = new T( *static_cast<pointer_type>( const_cast<void *>( src_ptr ) ) );
#  endif // FD_DELEGATE_NO_STD_ALLOCATOR
  }

  static void FD_DELEGATE_FASTCALL delete_obj(delegate_holder & dh)
  {
#ifndef FD_DELEGATE_NO_STD_ALLOCATOR
    typedef typename Allocator::template rebind<T>::other allocator_type;
    typedef typename allocator_type::pointer pointer_type;
#else
    typedef T * pointer_type;
#endif // FD_DELEGATE_NO_STD_ALLOCATOR

#  ifndef FD_DELEGATE_NO_STD_ALLOCATOR
    allocator_type allocator;
#  endif // FD_DELEGATE_NO_STD_ALLOCATOR

#  ifndef FD_DELEGATE_NO_STD_ALLOCATOR
    pointer_type victim = static_cast<pointer_type>( const_cast<void *>( dh.obj_ptr ) );

    // Destroy and deallocate the functor
    // victim->~T();
    allocator.destroy( victim );
    allocator.deallocate( victim, 1 );
#  else
    delete static_cast<pointer_type>( const_cast<void *>( dh.obj_ptr ) );
#  endif // FD_DELEGATE_NO_STD_ALLOCATOR
  }
};  // struct obj_manager

// --------------------------------------------------------------------------------

typedef boost::shared_ptr<delegate_base const>  delegate_shared_ptr;
typedef boost::weak_ptr<delegate_base const>    delegate_weak_ptr;

template<typename Allocator>
class multicast_info
{
public:
#ifndef FD_DELEGATE_NO_STD_ALLOCATOR
  typedef std::deque<delegate_shared_ptr, typename Allocator::template rebind<delegate_shared_ptr>::other>  delegate_list_type;
  typedef std::deque<delegate_weak_ptr, typename Allocator::template rebind<delegate_weak_ptr>::other>      watch_list_type;
#else
  typedef std::deque<delegate_shared_ptr> delegate_list_type;
  typedef std::deque<delegate_weak_ptr>   watch_list_type;
#endif // FD_DELEGATE_NO_STD_ALLOCATOR

private:
  delegate_list_type dg_list_;

  struct deep_copy
  {
    delegate_list_type & dg_list_;

    explicit deep_copy(delegate_list_type & dg_list) : dg_list_( dg_list ) { }

    void operator ()(delegate_shared_ptr const & dg_sptr) const
    {
      dg_list_.push_back( delegate_shared_ptr( new delegate_base( *dg_sptr ) ) );
    }
  };

public:
  delegate_list_type       &  delegate_list()        { return dg_list_; }
  delegate_list_type const &  delegate_list() const  { return dg_list_; }

  multicast_info() : dg_list_() { }

  multicast_info(multicast_info const & other)
  {
    // Deep copy; the contents of the smart pointer (i.e. delegate_base) are copied.
    std::for_each( other.dg_list_.begin(), other.dg_list_.end(), deep_copy( dg_list_ ) );
  }

  explicit multicast_info(delegate_holder const & dh)
  {
    typedef fd::detail::delegate::fxn_table_base  fxn_table_base_type;

    fxn_table_base_type const * ftable_base = static_cast<fxn_table_base_type const *>( dh.tbl_ptr );

    if( ftable_base )
    {
      if( ftable_base->is_multicast() )
      {
        multicast_info const & other = *static_cast<multicast_info const *>( dh.obj_ptr );
        dg_list_ = other.dg_list_;  // Shallow copy; the smart pointers are copied.
      }
      else if( !ftable_base->is_empty() )
      {
        dg_list_.push_back( delegate_shared_ptr(new delegate_base ( dh ) ) );
      }
    }
  }

  multicast_info & operator =(multicast_info const & other)
  {
    if( this != &other )
    {
      multicast_info( other ).swap( *this );
    }
    return *this;
  }

  void swap(multicast_info & other)
  {
    std::swap( dg_list_, other.dg_list_ );
  }

  watch_list_type get_watch_list() const
  {
#ifndef BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS
    return watch_list_type( dg_list_.begin(), dg_list_.end() );
#else
    watch_list_type wlst;
    std::copy( dg_list_.begin(), dg_list_.end(), std::back_inserter( wlst ) );
    return wlst;
#endif
  }

};  // class multicast_info

struct is_callable
{
  bool operator ()(delegate_weak_ptr const & dg_wptr)
  {
    return !dg_wptr.expired();
  }
}; // class multicast_info

/**
 *
 */
template<typename Allocator>
struct multicast_shared_list_manager
{
  typedef typename multicast_info<Allocator>::delegate_list_type delegate_list_type;

  static void FD_DELEGATE_FASTCALL copy_obj(delegate_holder & dh_dst, void const * src_ptr)
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
    allocator.construct( minfo_ptr, *static_cast<pointer_type>( const_cast<void *>( src_ptr ) ) );
#  else
    pointer_type minfo_ptr = new multicast_info<Allocator>( *static_cast<pointer_type>( const_cast<void *>( src_ptr ) ) );
#  endif // FD_DELEGATE_NO_STD_ALLOCATOR

    dh_dst.obj_ptr = minfo_ptr;
  }

  static void FD_DELEGATE_FASTCALL delete_obj(delegate_holder & dh)
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
    pointer_type victim = static_cast<pointer_type>( const_cast<void *>( dh.obj_ptr ) );
    allocator.destroy( victim );
    allocator.deallocate( victim, 1 );
#  else
    delete static_cast<pointer_type>( const_cast<void *>( dh.obj_ptr ) );
#  endif // FD_DELEGATE_NO_STD_ALLOCATOR
  }

};  // struct multicast_shared_list_manager

/**
 *
 */
struct compare_holder_helper
{
  delegate_holder const & dh_;

  explicit compare_holder_helper(delegate_holder const & dh) : dh_( dh ) { }

  bool operator()(delegate_shared_ptr const & other) const
  {
    return dh_util::compare_holder( dh_, other->get_holder() );
  }
};

/**
 *
 */
template<typename Allocator>
struct multicast_stub
{
  typedef typename multicast_info<Allocator>::delegate_list_type delegate_list_type;

  static bool is_multicast() { return true; }

  static unsigned add_delegates(void const * obj_ptr, delegate_holder const & dh, void * list_added_ptr)
  {
    delegate_list_type & dg_list_dst =
      static_cast<multicast_info<Allocator> *>( const_cast<void *>( obj_ptr ) )->delegate_list();

    std::deque<delegate_weak_ptr> & dg_wlist_added = *static_cast<std::deque<delegate_weak_ptr> *>( list_added_ptr );

    if( static_cast<fxn_table_base const *>( dh.tbl_ptr )->is_multicast() )
    { // Adds multiple delegates

      delegate_list_type * dg_list_src_ptr = static_cast<delegate_list_type *>( const_cast<void *>( dh.obj_ptr ) );
      typename delegate_list_type::const_iterator it_src = dg_list_src_ptr->begin(), it_src_e = dg_list_src_ptr->end();

      for( ; it_src != it_src_e; ++it_src )
      {
        typename delegate_list_type::iterator it_dst_f =
          std::find_if( dg_list_dst.begin(), dg_list_dst.end(), compare_holder_helper( (*it_src)->get_holder() ) );
        if( dg_list_dst.end() == it_dst_f )
        { // not found

          dg_list_dst.push_back( delegate_shared_ptr( new delegate_base( (*it_src)->get_holder() ) ) );
          dg_wlist_added.push_back( delegate_weak_ptr( dg_list_dst.back() ) );
        }
        else
        {
          dg_wlist_added.push_back( delegate_weak_ptr( *it_dst_f ) );
        }
      }

      return static_cast<unsigned>( dg_list_dst.size() );
    }
    else
    { // Adds a single delegate

      typename delegate_list_type::iterator it_dst_f =
        std::find_if( dg_list_dst.begin(), dg_list_dst.end(), compare_holder_helper(  dh ) );
      if( dg_list_dst.end() == it_dst_f )
      { // not found

        dg_list_dst.push_back( delegate_shared_ptr( new delegate_base( dh ) ) );
        dg_wlist_added.push_back( delegate_weak_ptr( dg_list_dst.back() ) );
      }
      else
      {
        dg_wlist_added.push_back( delegate_weak_ptr( *it_dst_f ) );
      }

      return static_cast<unsigned>( dg_list_dst.size() );
    }
  }

  static unsigned remove_delegates(void const * obj_ptr, delegate_holder const & dh)
  {
    delegate_list_type & dg_list_dst =
      static_cast<multicast_info<Allocator> *>( const_cast<void *>( obj_ptr ) )->delegate_list();

    if( static_cast<fxn_table_base const *>( dh.tbl_ptr )->is_multicast() )
    { // Removes multiple delegates

      delegate_list_type * dg_list_src_ptr = static_cast<delegate_list_type *>( const_cast<void *>( dh.obj_ptr ) );
      typename delegate_list_type::const_iterator it_src = dg_list_src_ptr->begin(), it_src_e = dg_list_src_ptr->end();

      typename delegate_list_type::iterator it_dst_e = dg_list_dst.end();
      for( ; it_src != it_src_e; ++it_src )
      {
        it_dst_e =
          std::remove_if( dg_list_dst.begin(), it_dst_e, compare_holder_helper( (*it_src)->get_holder() ) );
      }
      dg_list_dst.erase( it_dst_e, dg_list_dst.end() );

      return static_cast<unsigned>( dg_list_dst.size() );
    }
    else
    { // Removes a single delegate

      dg_list_dst.erase(
        std::remove_if( dg_list_dst.begin(), dg_list_dst.end(), compare_holder_helper( dh ) ),
        dg_list_dst.end() );

      return static_cast<unsigned>( dg_list_dst.size() );
    }
  }

  static bool find_delegate(void const * obj_ptr, delegate_holder const & dh)
  {
    delegate_list_type & dg_list =
      static_cast<multicast_info<Allocator> *>( const_cast<void *>( obj_ptr ) )->delegate_list();

    return dg_list.end() !=
      std::find_if( dg_list.begin(), dg_list.end(), compare_holder_helper( dh ) );
  }
};  // struct multicast_stub

// ----------------------------------------------------------------------------------------------------

/**
 *
 */
inline bool has_empty_target(delegate_base const * dp)
{
  return dp->empty();
}

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1310)
inline bool has_empty_target(void const *)
{
  return false;
}
#else
inline bool has_empty_target(...)
{
  return false;
}
#endif

// ----------------------------------------------------------------------------------------------------

template<typename R>
struct default_value
{
  static R get()
  {
    typedef typename boost::remove_reference<R>::type value_type;
    static value_type r = value_type();
    return r;
  }
};

template<>
struct default_value<FD_DELEGATE_VOID_RETURN_TYPE>
{
  static FD_DELEGATE_VOID_RETURN_TYPE get()
  {
    FD_DELEGATE_RETURN( unusable() );
  }
};

} } // namespace detail::delegate

// ----------------------------------------------------------------------------------------------------

namespace multicast
{

/**
 *
 */
class token
{
private:
  friend bool operator <(token const &, token const &);
  friend bool operator ==(token const &, token const &);

  typedef fd::detail::delegate::fxn_table_base  fxn_table_base_type;
  typedef fd::detail::delegate::delegate_holder delegate_holder;
  typedef std::deque<fd::detail::delegate::delegate_weak_ptr> delegate_watch_list;

  /**
   *
   */
  delegate_base * dg_ref_ptr_;

  delegate_base dg_added_;
  delegate_watch_list dg_wlist_added_;

  void const * null_type_tbl_ptr_;

  struct not_expired
  {
    bool operator ()(fd::detail::delegate::delegate_weak_ptr const & dg_wptr) const
    {
      return !dg_wptr.expired();
    }
  };

public:
  token()
    : dg_ref_ptr_( 0 ), dg_added_()
    , null_type_tbl_ptr_( detail::delegate::null_type::get_table() )
  {
  }

  token(delegate_base * dg_ptr, delegate_base const & dg_added
    , delegate_watch_list dg_wlist_added, void const * null_type_tbl_ptr )
    : dg_ref_ptr_( dg_ptr ), dg_added_( dg_added )
    , dg_wlist_added_( dg_wlist_added ), null_type_tbl_ptr_( null_type_tbl_ptr )
  {
  }

  unsigned count() const
  {
    return static_cast<unsigned>( dg_wlist_added_.size() );
  }

  unsigned remove()
  {
    unsigned cnt_delegate = 0;

    if( valid() )
    {
      delegate_watch_list dg_wlist_to_remove = dg_wlist_added_;
      dg_wlist_added_.clear();

      delegate_holder & dh_ref = dg_ref_ptr_->get_holder();

      fxn_table_base_type const * ftable_base_ref = static_cast<fxn_table_base_type const *>( dh_ref.tbl_ptr );
      if( ftable_base_ref->is_multicast() )
      {
        delegate_watch_list::const_iterator it = dg_wlist_to_remove.begin(), it_e = dg_wlist_to_remove.end();
        for( ; it != it_e; ++it )
        {
          fd::detail::delegate::delegate_shared_ptr dg_sptr = it->lock();
          cnt_delegate = ftable_base_ref->remove_delegates( dh_ref.obj_ptr, dg_sptr->get_holder() );
        }

        if( 0 == cnt_delegate )
        { // Transform multicast delegate into an empty delegate.

          ftable_base_ref->delete_obj( dh_ref );
          dh_ref.obj_ptr = 0;
          dh_ref.any.fxn_ptr = 0;
          dh_ref.tbl_ptr = null_type_tbl_ptr_;
        }
      }
    }

    return cnt_delegate;
  }

  bool valid() const
  {
    return dg_ref_ptr_ && dg_wlist_added_.end() !=
      std::find_if( dg_wlist_added_.begin(), dg_wlist_added_.end(), not_expired() );
  }

};  // class token

inline bool operator <(token const & t1, token const & t2)
{
  if( t1.dg_ref_ptr_ == t2.dg_ref_ptr_ )
    return fd::detail::delegate::dh_util::compare_less_than( t1.dg_added_.get_holder(), t2.dg_added_.get_holder() );
  else
    return t1.dg_ref_ptr_ < t2.dg_ref_ptr_;
}

inline bool operator ==(token const & t1, token const & t2)
{
  return ( t1.dg_ref_ptr_ == t2.dg_ref_ptr_ ) &&
    fd::detail::delegate::dh_util::compare_holder( t1.dg_added_.get_holder(), t2.dg_added_.get_holder() );
}

/**
 *
 */
class scoped_token : public token
{
private:
  mutable bool dismiss_;

public:
  scoped_token(token const & tok) : token( tok ), dismiss_( false ) { }
  ~scoped_token() { if( !dismiss_ ) this->remove(); }
  void dismiss() const { dismiss_ = true; }

private:
  scoped_token();
  explicit scoped_token(scoped_token const &);
  scoped_token & operator =(scoped_token const &);
};

} // namespace multicast

} // namespace fd

#  undef FD_DELEGATE_ENABLE_IF_NOT_INTEGRAL
#  undef FD_DELEGATE_ENABLE_IF_EQUALITY_COMPARABLE
#  undef FD_DELEGATE_DISABLE_IF_EQUALITY_COMPARABLE
#  undef FD_DELEGATE_COMPARE_TYPE_ID

#endif // FD_DELEGATE_BASE_HEADER
