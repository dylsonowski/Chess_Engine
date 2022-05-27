#pragma once

#include <memory>
#include <typeinfo>
#include <typeindex>
#include <iostream>
#include <sstream>
#include <math.h>
#include <optional>
#include <fstream>
#include <random>
#include <functional>
#include <algorithm>

#include <vector>
#include <utility>
#include <string>
#include <map>
#include <array>

#include "Pale/Log_System.h"
#include "Pale/Engine_Macros.h"
#include "Pale.h"
#include "nlohmann/json.hpp"

#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#endif // PLATFORM_WINDOWS
