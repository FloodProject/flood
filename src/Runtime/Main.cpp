/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Runtime/API.h"
#include "Runtime/Runtime.h"
#include "Core/Utilities.h"

#include <anyoption.h>

//-----------------------------------//

static int ValidateOptions(AnyOption& options, String& file)
{
	for( int i = 0; i < options.getArgc(); ++i )
	{
		file = options.getArgv(i);
		break;
	}

	// If no file was passed then we have nothing to do.
	if( file.empty() )
	{
		options.printAutoUsage();
		return EXIT_FAILURE;
	}

	// Check that the file is a DLL.
	String extension = PathGetFileExtension(file);
	
	if( StringCompareInsensitive(extension, "dll") != 0 )
	{
		LogError("The file is not a DLL");
		return EXIT_FAILURE;
	}

	// Check if the file exists.
	if( !FileExists(file) )
	{
		LogError("The file could not be found");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

//-----------------------------------//

int main(int argc, char** argv)
{
	int ret = EXIT_SUCCESS;

	// Setup default error handling strategy.
	// ...

	// Setup the default log stream.
	Log logger;
	LogSetDefault(&logger);

	// Setup the command line handling.
	AnyOption options;

	// Use this to active debug mode.
	options.setFlag("debug", 'd');

	options.setVerbose();
	options.autoUsagePrint(true);

	// Process the command line arguments.
	options.processCommandArgs(argc, argv);

	String file;
	ret = ValidateOptions(options, file);

	if( ret != EXIT_SUCCESS )
	{
		options.printUsage();
		return ret;
	}

	Runtime runtime;
	runtime.init();
	runtime.shutdown();

	return ret;
}