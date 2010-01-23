/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/StringUtilities.h"
#include <sstream>

namespace vapor {

//-----------------------------------//

#ifdef VAPOR_PLATFORM_WINDOWS

std::string wstrtostr(const std::wstring &wstr)
{
    // Convert a Unicode string to an ASCII string
    std::string strTo;
    char *szTo = new char[wstr.length() + 1];
    szTo[wstr.size()] = '\0';
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);
    strTo = szTo;
    delete[] szTo;
    return strTo;
}

#endif

//-----------------------------------//

#ifdef VAPOR_PLATFORM_WINDOWS

std::wstring strtowstr(const std::string &str)
{
    // Convert an ASCII string to a Unicode String
    std::wstring wstrTo;
    wchar_t *wszTo = new wchar_t[str.length() + 1];
    wszTo[str.size()] = L'\0';
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wszTo, (int)str.length());
    wstrTo = wszTo;
    delete[] wszTo;
    return wstrTo;
}

#endif

//-----------------------------------//

std::vector<std::string>& split(const std::string &s, char delim, 
								std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    
	while(std::getline(ss, item, delim)) 
	{
        elems.push_back(item);
    }
    
	return elems;
}

//-----------------------------------//

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    return split(s, delim, elems);
}

//-----------------------------------//

} // end namespace