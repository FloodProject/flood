/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor {

//-----------------------------------//

class Downloader
{
public:

	// Initializes the downloader context.
	bool init();

	// Downloads a remote file.
	void download(const std::string& url);

protected:


};

//-----------------------------------//

} // end namespace