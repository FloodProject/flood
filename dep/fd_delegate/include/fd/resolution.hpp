//  FD.Resolution Library

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
#include "./resolution/detail/prolouge.hpp"

// Visual Age C++ doesn't handle the file iteration well
#if BOOST_WORKAROUND(__IBMCPP__, >= 500)
#  if FD_DELEGATE_MAX_ARGS >= 0
#    include "./resolution/resolution0.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 1
#    include "./resolution/resolution1.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 2
#    include "./resolution/resolution2.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 3
#    include "./resolution/resolution3.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 4
#    include "./resolution/resolution4.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 5
#    include "./resolution/resolution5.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 6
#    include "./resolution/resolution6.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 7
#    include "./resolution/resolution7.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 8
#    include "./resolution/resolution8.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 9
#    include "./resolution/resolution9.hpp"
#  endif
#  if FD_DELEGATE_MAX_ARGS >= 10
#    include "./resolution/resolution10.hpp"
#  endif
#else

#  define BOOST_PP_ITERATION_PARAMS_1 (3,(0,FD_DELEGATE_MAX_ARGS,<fd/resolution/detail/resolution_iterate.hpp>))
#  include BOOST_PP_ITERATE()
#  undef BOOST_PP_ITERATION_PARAMS_1
#endif
