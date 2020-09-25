#include "timey_pch.h"
#include "Application.h"
#include "TimeyCore.h"



int main(int argc, char** argv) {

	Timey::Log::Init();

	Timey::Application* app;
	app = Timey::CreateApp();

	app->Run();

	/**const std::string test_db_path = "../databases/test.db";

	Timey::FileManager& file_manager = Timey::FileManager::getFileManager();

	if (file_manager.FindIfFileExists(test_db_path)){
		file_manager.RemoveFile(test_db_path);
	};
	**/

	

}

