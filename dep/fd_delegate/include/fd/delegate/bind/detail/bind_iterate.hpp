#if !defined(BOOST_PP_IS_ITERATING)
# error FD.Bind - do not include this file!
#endif

#define FD_DELEGATE_NUM_ARGS BOOST_PP_ITERATION()
#  include "../../../delegate/detail/maybe_include.hpp"
#  include "./maybe_include.hpp"
#undef FD_DELEGATE_NUM_ARGS
