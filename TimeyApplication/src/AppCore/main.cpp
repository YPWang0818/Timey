#include "timey_pch.h"
#include "Application.h"
#include "TimeyCore.h"
#include "Core/DbClasses/DataBase.h"


int main(int argc, char** argv) {

	Timey::Log::Init();

	Timey::Application* app;
	app = Timey::CreateApp();
	//Timey::print_res();
	app->Run();	

}

