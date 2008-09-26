#include "Log.h"
#include "Window.h"

using namespace vapor::log;
using namespace vapor::renderer;

namespace vapor {
	namespace core {

class Core
{
	Window	*window;
	Log		*logger;
	
public:
	Core(const string& logFile);
	~Core();

	void Core::createWindow(const string& title = "fuckEngine", const int width = 640,
		const int height = 480, const bool fullscreen = false);
};

} } // end namespaces