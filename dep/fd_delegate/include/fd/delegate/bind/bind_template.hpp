#include "./detail/prolouge.hpp"

// typename T0, typename T1, ..., typename Tn-1
#define FD_DELEGATE_TEMPLATE_PARMS BOOST_PP_ENUM_PARAMS(FD_DELEGATE_NUM_ARGS, typename T)

// T0, T1, ..., Tn-1
#define FD_DELEGATE_TEMPLATE_ARGS BOOST_PP_ENUM_PARAMS(FD_DELEGATE_NUM_ARGS, T)

// Comma if nonzero number of arguments
#if FD_DELEGATE_NUM_ARGS == 0
#  define FD_DELEGATE_COMMA
#else
#  define FD_DELEGATE_COMMA ,
#endif // FD_DELEGATE_NUM_ARGS > 0

#define FD_DELEGATE_DELEGATE          BOOST_JOIN(delegate,          FD_DELEGATE_NUM_ARGS)

namespace fd
{

template<typename R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS, typename U, typename T>
FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS>
bind(R (U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS), T t)
{
  return FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS>( fxn, t );
}

template<typename R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS, typename U, typename T>
FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS>
bind(R (U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS) const, T t)
{
  return FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS>( fxn, t );
}

#if !defined(FD_DELEGATE_NO_STD_ALLOCATOR) && !defined(BOOST_NO_SFINAE)

template<typename R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS, typename U, typename T, typename Allocator>
#  if defined(__sgi) && defined(_COMPILER_VERSION) && _COMPILER_VERSION <= 730 && !defined(BOOST_STRICT_CONFIG)
FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, typename Allocator::template rebind<delegate_base>::other>
#  else
FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, typename Allocator::template rebind<void>::other>
#  endif
bind(R (U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS), T t, Allocator)
{
#  if defined(__sgi) && defined(_COMPILER_VERSION) && _COMPILER_VERSION <= 730 && !defined(BOOST_STRICT_CONFIG)
  // The library shipping with MIPSpro 7.3.1.3m has a broken allocator<void>
  return FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, typename Allocator::template rebind<delegate_base>::other>( fxn, t );
#  else
  return FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, typename Allocator::template rebind<void>::other>( fxn, t );
#  endif
}

template<typename R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS, typename U, typename T, typename Allocator>
#  if defined(__sgi) && defined(_COMPILER_VERSION) && _COMPILER_VERSION <= 730 && !defined(BOOST_STRICT_CONFIG)
FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, typename Allocator::template rebind<delegate_base>::other>
#  else
FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, typename Allocator::template rebind<void>::other>
#  endif
bind(R (U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS) const, T t, Allocator)
{
#  if defined(__sgi) && defined(_COMPILER_VERSION) && _COMPILER_VERSION <= 730 && !defined(BOOST_STRICT_CONFIG)
  // The library shipping with MIPSpro 7.3.1.3m has a broken allocator<void>
  return FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, typename Allocator::template rebind<delegate_base>::other>( fxn, t );
#  else
  return FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, typename Allocator::template rebind<void>::other>( fxn, t );
#  endif
}

#endif  // #if !defined(FD_DELEGATE_NO_STD_ALLOCATOR) && !defined(BOOST_NO_SFINAE)

#if defined(FD_MEM_FN_ENABLE_CDECL) || defined(BOOST_MEM_FN_ENABLE_CDECL)

#define FD_DELEGATE_MF_CC       __cdecl
#  include "./detail/bind_mfn_cc.hpp"
#undef  FD_DELEGATE_MF_CC

#endif  // #if defined(FD_MEM_FN_ENABLE_CDECL) || defined(BOOST_MEM_FN_ENABLE_CDECL)

#if defined(FD_MEM_FN_ENABLE_STDCALL) || defined(BOOST_MEM_FN_ENABLE_STDCALL)

#define FD_DELEGATE_MF_CC       __stdcall
#  include "./detail/bind_mfn_cc.hpp"
#undef  FD_DELEGATE_MF_CC

#endif  // #if defined(FD_MEM_FN_ENABLE_STDCALL) || defined(BOOST_MEM_FN_ENABLE_STDCALL)

#if defined(FD_MEM_FN_ENABLE_FASTCALL) || defined(BOOST_MEM_FN_ENABLE_FASTCALL)

#define FD_DELEGATE_MF_CC       __fastcall
#  include "./detail/bind_mfn_cc.hpp"
#undef  FD_DELEGATE_MF_CC

#endif  // #if defined(FD_MEM_FN_ENABLE_FASTCALL) || defined(BOOST_MEM_FN_ENABLE_FASTCALL)

} // end namespace fd

// Cleanup after ourselves...
#undef  FD_DELEGATE_TEMPLATE_PARMS
#undef  FD_DELEGATE_TEMPLATE_ARGS
#undef  FD_DELEGATE_COMMA
#undef  FD_DELEGATE_DELEGATE
