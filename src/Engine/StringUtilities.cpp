/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/StringUtilities.h"
#include <sstream>

namespace vapor {

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