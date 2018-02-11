#include "Tutorial1App.h"

int main() {
	
	// allocation
	auto app = new Tutorial1App();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}