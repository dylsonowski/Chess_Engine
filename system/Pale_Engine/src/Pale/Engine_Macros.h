#pragma once

#ifdef PLATFORM_WINDOWS
	#ifdef BUILD_DLL
		#define PALE_API __declspec(dllexport)
	#else
		#define PALE_API __declspec(dllimport)
	#endif // BUILD_DLL
#else
	#error This app can only run on Windows OS!
#endif // PLATFORM_WINDOWS

//--- Errors codes ---//
#define VEC_OUT_OF_RANGE "Read attempt from out of vector range!"

//--- Warnings codes ---//