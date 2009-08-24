#ifndef FD_RESOLUTION_BASE_HEADER
#define FD_RESOLUTION_BASE_HEADER

#include <boost/config.hpp>

#if defined (BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)                     \
  || defined(BOOST_BCB_PARTIAL_SPECIALIZATION_BUG)                         \
  || !(BOOST_STRICT_CONFIG || !defined(__SUNPRO_CC) || __SUNPRO_CC > 0x540)
#  define FD_DELEGATE_NO_FUNCTION_TYPE_SYNTAX
#endif

#if !defined(FD_DELEGATE_NO_FUNCTION_TYPE_SYNTAX)

namespace fd
{

/**
 * Primary template.
 */
template<typename Signature>
class resolution;

} // namespace fd

#endif // have partial specialization

#endif // #ifndef FD_RESOLUTION_BASE_HEADER
