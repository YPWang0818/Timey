#include "timey_pch.h"
#include "Application.h"
#include "TimeyCore.h"

int main(int argc, char** argv) {

	Timey::Log::Init();

	Timey::Application* app;
	app = Timey::CreateApp();

	app->Run();


};

