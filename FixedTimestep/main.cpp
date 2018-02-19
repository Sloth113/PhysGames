#include "FixedTimestepApp.h"
//#include ""

int main() {
	
	// allocation
	auto app = new FixedTimestepApp();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}