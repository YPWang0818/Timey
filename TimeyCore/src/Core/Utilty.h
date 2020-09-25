#pragma once
#include <memory>
#include <utility>


#define TIMEY_BIND_CALLBACK(func) std::bind(&func, this , std::placeholders::_1)
#define TIMEY_BIND_STATIC_CALLBACK(func) std::bind(&func, std::placeholders::_1)

namespace Timey {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) {
		return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
	};


	template<typename U>
	using Ref = std::shared_ptr<U>;

	template<typename U, typename ... Args>
	constexpr Ref<U> CreateRef(Args&& ... args) {
		return std::shared_ptr<U>(new U(std::forward<Args>(args)...));
	};


}