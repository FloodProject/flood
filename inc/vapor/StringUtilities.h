/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

namespace vapor {

//-----------------------------------//

std::string wstrtostr(const std::wstring &wstr);

//-----------------------------------//

std::wstring strtowstr(const std::string &str);

//-----------------------------------//

std::vector<std::string>& split(const std::string& s, char delim, 
								std::vector<std::string>& elems);

//-----------------------------------//

std::vector<std::string> split(const std::string& s, char delim);

//-----------------------------------//

} // end namespace