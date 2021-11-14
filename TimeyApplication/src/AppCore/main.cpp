#include "timey_pch.h"
#include "Application.h"
#include "TimeyCore.h"


//#include "Core/DbClasses/DataBase.h"

#ifdef TIMEY_INTEGRATION_TESTS
	#include "Tests/IntegrationTest.h"
#endif

int main(int argc, char** argv) {

	Timey::Log::Init();

	Timey::Application* app;
	app = Timey::CreateApp();
#ifdef TIMEY_INTEGRATION_TESTS
	Timey::IntegrationTests::Run();
#endif
	app->Run();	

}

