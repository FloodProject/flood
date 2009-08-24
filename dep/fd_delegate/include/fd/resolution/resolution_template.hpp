#include "./detail/prolouge.hpp"

// typename U0, typename U1, ..., typename Un-1
#define FD_DELEGATE_TEMPLATE_PARMS2 BOOST_PP_ENUM_PARAMS(FD_DELEGATE_NUM_ARGS, typename U)

// U0, U1, ..., Un-1
#define FD_DELEGATE_TEMPLATE_ARGS2 BOOST_PP_ENUM_PARAMS(FD_DELEGATE_NUM_ARGS, U)

// U1, U2, ..., Un-1
#define FD_DELEGATE_TEMPLATE_ARG2_SP(J,I,D) BOOST_PP_CAT(U, BOOST_PP_INC(I))
#define FD_DELEGATE_TEMPLATE_ARGS2_SP BOOST_PP_ENUM(BOOST_PP_DEC(FD_DELEGATE_NUM_ARGS),FD_DELEGATE_TEMPLATE_ARG2_SP,BOOST_PP_EMPTY)

// Comma if nonzero number of arguments
#if FD_DELEGATE_NUM_ARGS == 0
#  define FD_DELEGATE_COMMA
#else
#  define FD_DELEGATE_COMMA ,
#endif // FD_DELEGATE_NUM_ARGS > 0


#define FD_DELEGATE_RESOLUTION        BOOST_JOIN(resolution,        FD_DELEGATE_NUM_ARGS)

namespace fd
{

/**
 * resolutionN. portable syntax.
 */
template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2>
class FD_DELEGATE_RESOLUTION
{
public:
  template<typename U>
  static UR (U::*select(UR (U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2)))(FD_DELEGATE_TEMPLATE_ARGS2)
  {
    return fxn;
  }

  template<typename U>
  static UR (U::*select(UR (U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2) const))(FD_DELEGATE_TEMPLATE_ARGS2) const
  {
    return fxn;
  }

#if FD_DELEGATE_NUM_ARGS > 0

  template<typename U>
  static UR (U::*select(UR (U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2_SP)))(FD_DELEGATE_TEMPLATE_ARGS2_SP)
  {
    return fxn;
  }

  template<typename U>
  static UR (U::*select(UR (U::*fxn)(FD_DELEGATE_TEMPLATE_ARGS2_SP) const))(FD_DELEGATE_TEMPLATE_ARGS2_SP) const
  {
    return fxn;
  }

#endif // #if FD_DELEGATE_NUM_ARGS > 0

#if defined(FD_MEM_FN_ENABLE_CDECL) || defined(BOOST_MEM_FN_ENABLE_CDECL)

#define FD_DELEGATE_MF_CC       __cdecl
#  include "./detail/resolution_mfn_cc_select.hpp"
#undef  FD_DELEGATE_MF_CC

#endif  // #if defined(FD_MEM_FN_ENABLE_CDECL) || defined(BOOST_MEM_FN_ENABLE_CDECL)

#if defined(FD_MEM_FN_ENABLE_STDCALL) || defined(BOOST_MEM_FN_ENABLE_STDCALL)

#define FD_DELEGATE_MF_CC       __stdcall
#  include "./detail/resolution_mfn_cc_select.hpp"
#undef  FD_DELEGATE_MF_CC

#endif  // #if defined(FD_MEM_FN_ENABLE_STDCALL) || defined(BOOST_MEM_FN_ENABLE_STDCALL)

#if defined(FD_MEM_FN_ENABLE_FASTCALL) || defined(BOOST_MEM_FN_ENABLE_FASTCALL)

#define FD_DELEGATE_MF_CC       __fastcall
#  include "./detail/resolution_mfn_cc_select.hpp"
#undef  FD_DELEGATE_MF_CC

#endif  // #if defined(FD_MEM_FN_ENABLE_FASTCALL) || defined(BOOST_MEM_FN_ENABLE_FASTCALL)

  static UR (*select(UR (*fxn)(FD_DELEGATE_TEMPLATE_ARGS2)))(FD_DELEGATE_TEMPLATE_ARGS2)
  {
    return fxn;
  }

#if defined(FD_FUNCTION_ENABLE_STDCALL) || defined(BOOST_BIND_ENABLE_STDCALL)

#define FD_DELEGATE_ST
#define FD_DELEGATE_CC          __stdcall
#  include "./detail/resolution_cc_select.hpp"
#undef  FD_DELEGATE_ST
#undef  FD_DELEGATE_CC

#endif // #if defined(FD_FUNCTION_ENABLE_STDCALL) || defined(BOOST_BIND_ENABLE_STDCALL)

#if defined(FD_FUNCTION_ENABLE_FASTCALL) || defined(BOOST_BIND_ENABLE_FASTCALL)

#define FD_DELEGATE_ST
#define FD_DELEGATE_CC          __fastcall
#  include "./detail/resolution_cc_select.hpp"
#undef  FD_DELEGATE_ST
#undef  FD_DELEGATE_CC

#endif // #if defined(FD_FUNCTION_ENABLE_FASTCALL) || defined(BOOST_BIND_ENABLE_FASTCALL)

};  // class FD_DELEGATE_RESOLUTION

// ----------------------------------------------------------------------------------------------------

#if !defined(FD_DELEGATE_NO_FUNCTION_TYPE_SYNTAX)

#if FD_DELEGATE_NUM_ARGS == 0
#  define FD_DELEGATE_PARTIAL_SPEC2 UR (void)
#else
#  define FD_DELEGATE_PARTIAL_SPEC2 UR (BOOST_PP_ENUM_PARAMS(FD_DELEGATE_NUM_ARGS,U))
#endif

/**
 * Partial specialization.
 */
template<typename UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_PARMS2>
class resolution<FD_DELEGATE_PARTIAL_SPEC2>
  : public FD_DELEGATE_RESOLUTION<UR FD_DELEGATE_COMMA FD_DELEGATE_TEMPLATE_ARGS2>
{
};

#undef  FD_DELEGATE_PARTIAL_SPEC2

#endif  // #if !defined(FD_DELEGATE_NO_FUNCTION_TYPE_SYNTAX)

} // end namespace fd

// Cleanup after ourselves...
#undef  FD_DELEGATE_TEMPLATE_PARMS2
#undef  FD_DELEGATE_TEMPLATE_ARGS2
#undef  FD_DELEGATE_TEMPLATE_ARGS2_SP
#undef  FD_DELEGATE_TEMPLATE_ARG2_SP
#undef  FD_DELEGATE_COMMA
#undef  FD_DELEGATE_RESOLUTION
