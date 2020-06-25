#pragma once

#include "timey_pch.h"

namespace Timey {
	
	class FileManager {

	public:
		inline static FileManager& getFileManager() {
			return *s_instance;
		};

		void operator=(const FileManager&) = delete;

		inline static bool FindIfFileExists(const std::string& filepath) {
			return s_instance->_impel_find_if_file_exist(filepath);
		};

		inline static bool RemoveFile(const std::string& filepath) {
			return s_instance->_impel_delete_file(filepath);

		};

	private:
		virtual bool _impel_find_if_file_exist(const std::string& filepath) = 0;
		virtual bool _impel_delete_file(const std::string& filepath) = 0;
	private:
		static std::shared_ptr<FileManager> s_instance;

	};

	#if  defined(TIMEY_PLATFORM_WINDOWS)
		
	class WindowsFileManager : public FileManager {

	private:

		virtual bool _impel_find_if_file_exist(const std::string& filepath) override;
		virtual bool _impel_delete_file(const std::string& filepath) override;

	};






	#elif defined(TIMEY_PLATFORM_LINUX)
		TIMEY_CORE_CRITICAL("Timey doesn't currently support linux");
	#else 
		TIMEY_CORE_CRITICAL("Timey doesn't support the current operating system.");
	#endif
	



}