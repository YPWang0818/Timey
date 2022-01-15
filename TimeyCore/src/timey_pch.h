#pragma once

#ifdef TIMEY_PLATFORM_WINDOWS
#include <Windows.h>
#endif 

//std headers
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <random>
#include <array>
#include <filesystem>

//porject internal headers

#define FMT_USE_NONTYPE_TEMPLATE_PARAMETERS 0 // Manually turned of using non-type parameters.
#include "Core/Log.h"
#include "Core/Utilty.h"


#define TIMEY_INTEGRATION_TESTS
