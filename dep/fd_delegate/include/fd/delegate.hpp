//  FD.Delegate Library

//  Copyright Douglas Gregor 2001-2003. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org/libs/function

// William Kempf, Jesse Jones and Karl Nelson were all very helpful in the
// design of this library.

//  Copyright JaeWook Choi 2007. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

// Boost.Function's interfaces are copied and incorporated into FD.Delegate.

#include <boost/preprocessor/iterate.hpp>
#include <boost/detail/workaround.hpp>

#ifndef FD_DELEGATE_MAX_ARGS
#  define FD_DELEGATE_MAX_ARGS 10
#endif // FD_DELEGATE_MAX_ARGS

// Include the prologue here so that the use of file-level iteration
// in anything that may be included by delegate_template.hpp doesn't break
#include "./delegate/detail/prolouge.hpp"

// Visual Age C++ doesn't handle the file iteration well
#if BOOST_WORKAROUND(__IBMCPP__, >= 500)
#  if FD_DELEGATE_MAX_ARGS >= 0
#    include "./delegate/delegate0.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 1
#    include "./delegate/delegate1.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 2
#    include "./delegate/delegate2.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 3
#    include "./delegate/delegate3.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 4
#    include "./delegate/delegate4.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 5
#    include "./delegate/delegate5.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 6
#    include "./delegate/delegate6.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 7
#    include "./delegate/delegate7.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 8
#    include "./delegate/delegate8.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 9
#    include "./delegate/delegate9.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 10
#    include "./delegate/delegate10.hpp"
#  endif
#else

#  define BOOST_PP_ITERATION_PARAMS_1 (3,(0,FD_DELEGATE_MAX_ARGS,<fd/delegate/detail/delegate_iterate.hpp>))
#  include BOOST_PP_ITERATE()
#  undef BOOST_PP_ITERATION_PARAMS_1
#endif
