#include "Engine.h"

int main(int argc, char* args[]) {

	std::cout << "Starting..." << std::endl;
	Velce::Engine* app = new Velce::Engine(1280, 720);
	app->Run();
	std::cout << "Closed." << std::endl;

	return 0;
}