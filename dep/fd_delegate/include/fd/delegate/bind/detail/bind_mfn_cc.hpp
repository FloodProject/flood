// FD.Bind
// Do not include this header directly.

/**
 * bind() for member function with calling convention.
 */
template<typename R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS, typename U, typename T>
FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS>
bind(R (FD_DELEGATE_MF_CC U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS), T t)
{
  return FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS>( fxn, t );
}

template<typename R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS, typename U, typename T>
FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS>
bind(R (FD_DELEGATE_MF_CC U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS) const, T t)
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
bind(R (FD_DELEGATE_MF_CC U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS), T t, Allocator)
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
bind(R (FD_DELEGATE_MF_CC U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS) const, T t, Allocator)
{
#  if defined(__sgi) && defined(_COMPILER_VERSION) && _COMPILER_VERSION <= 730 && !defined(BOOST_STRICT_CONFIG)
  // The library shipping with MIPSpro 7.3.1.3m has a broken allocator<void>
  return FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, typename Allocator::template rebind<delegate_base>::other>( fxn, t );
#  else
  return FD_DELEGATE_DELEGATE<R FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS, typename Allocator::template rebind<void>::other>( fxn, t );
#  endif
}

#endif // #if !defined(FD_DELEGATE_NO_STD_ALLOCATOR) && !defined(BOOST_NO_SFINAE)