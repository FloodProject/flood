//  FD.Bind Library

//  Copyright JaeWook Choi 2007. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/preprocessor/iterate.hpp>
#include <boost/detail/workaround.hpp>

#ifndef FD_DELEGATE_MAX_ARGS
#  define FD_DELEGATE_MAX_ARGS 10
#endif // FD_DELEGATE_MAX_ARGS

// Include the prologue here so that the use of file-level iteration
// in anything that may be included by delegate_template.hpp doesn't break
#include "./bind/detail/prolouge.hpp"

// Visual Age C++ doesn't handle the file iteration well
#if BOOST_WORKAROUND(__IBMCPP__, >= 500)
#  if FD_DELEGATE_MAX_ARGS >= 0
#    include "./bind/bind0.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 1
#    include "./bind/bind1.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 2
#    include "./bind/bind2.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 3
#    include "./bind/bind3.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 4
#    include "./bind/bind4.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 5
#    include "./bind/bind5.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 6
#    include "./bind/bind6.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 7
#    include "./bind/bind7.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 8
#    include "./bind/bind8.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 9
#    include "./bind/bind9.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 10
#    include "./bind/bind10.hpp"
#  endif
#else

#  define BOOST_PP_ITERATION_PARAMS_1 (3,(0,FD_DELEGATE_MAX_ARGS,<fd/delegate/bind/detail/bind_iterate.hpp>))
#  include BOOST_PP_ITERATE()
#  undef BOOST_PP_ITERATION_PARAMS_1
#endif
