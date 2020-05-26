#include <iostream>
#include "Application.h"

int main(int argc, char** argv) {


	Timey::Application* app;
	app = Timey::CreateApp();

	app->Run();


};

