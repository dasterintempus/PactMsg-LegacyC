#ifndef PACT_CONFIG_H
#define PACT_CONFIG_H

//Detect OS
#if defined(_WIN32) || defined(__WIN32__)
	#define PACT_OS_WIN
	#ifndef WIN32
		#define WIN32
	#endif 
#elif defined(linux) || defined(__linux)
	#define PACT_OS_LINUX
#else
	#error Unsupported operating system
#endif

//Symbol exporting
#ifdef PACT_DLL
	#ifdef _MSC_VER
		#define PACT_EXPORT __declspec(dllexport)
		#define PACT_IMPORT __declspec(dllimport)
	#endif
	#ifdef __GNUC__ >= 4
		#define PACT_EXPORT __attribute__ ((__visibility__ ("default")))
		#define PACT_IMPORT __attribute__ ((__visibility__ ("default")))
	#endif
#else
	#define PACT_EXPORT
	#define PACT_IMPORT
#endif

#ifdef PACT_DEBUG
	#define PACT_DEBUG_PRINT 1
#else
	#define PACT_DEBUG_PRINT 0
#endif

#endif