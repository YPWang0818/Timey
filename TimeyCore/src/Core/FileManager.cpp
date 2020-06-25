#include "timey_pch.h"
#include "FileManager.h"


namespace Timey {


	#if defined(TIMEY_PLATFORM_WINDOWS)



	bool WindowsFileManager::_impel_find_if_file_exist(const std::string& filepath)
	{
		DWORD res = GetFileAttributesA(filepath.c_str());

		return (res != INVALID_FILE_ATTRIBUTES) &&
			!(res & FILE_ATTRIBUTE_DIRECTORY);
	};


	 bool WindowsFileManager::_impel_delete_file(const std::string& filepath)
	{
		return DeleteFileA(filepath.c_str());
	};



	 std::shared_ptr<FileManager> FileManager::s_instance = std::make_shared<WindowsFileManager>();


	#elif defined(TIMEY_PLATFORM_LINUX)
	TIMEY_CORE_CRITICAL("Timey doesn't currently support linux");
	#else 
	TIMEY_CORE_CRITICAL("Timey doesn't support the current operating system.");
	#endif


}