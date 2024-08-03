#pragma once
#include "../../../dependencies/STD_dependencies.hpp"

#define LE_CODE(code) code
#define LE_ASSERT(condition) if(!condicion) __debugbreak()
#define LE_REPORT(size, ...) Le::report(__FILE__, __LINE__, size, __VA_ARGS__)
#define LE_REPORT_ERROR(size, ...) Le::report_error(__FILE__, __LINE__, size, __VA_ARGS__)
#define LE_REPORT_WARNING(size, ...) Le::report_warning(__FILE__, __LINE__, size, __VA_ARGS__)

#ifdef NDEBUG
#define LE_DEBUG_CODE(code) code
#define LE_DEBUG_BREAK __debugbreak()
#define LE_DEBUG_ASSERT(x) LE_ASSERT(x)
#define LE_DEBUG_REPORT(size, ...) LE_REPORT(size, __VA_ARGS__)
#define LE_DEBUG_REPORT_ERROR(size, ...) LE_REPORT_ERROR(size, __VA_ARGS__)
#define LE_DEBUG_REPORT_WARNING(size, ...) LE_REPORT_WARNING(size, __VA_ARGS__)
#else
#define LE_DEBUG_CODE(code)
#define LE_DEBUG_BREAK
#define LE_DEBUG_ASSERT(x)
#define LE_DEBUG_REPORT(size, ...)
#define LE_DEBUG_REPORT_ERROR(size, ...)
#define LE_DEBUG_REPORT_WARNING(size, ...)
#endif

namespace Le{

inline size_t reportCounter = 0;

void report(const char* file, size_t line, int numberOfArguments, ...){
    va_list args;
    va_start(args, numberOfArguments);
    std::cout << reportCounter++ << "- Report On File "
    << file << ", Line " << line << ":\n:: ";
    for(int i = 0; i < numberOfArguments; i++){
        std::cout << va_arg(args, const char*);
    }std::cout << std::endl;
    va_end(args);
}

void report_error(const char* file, size_t line, int numberOfArguments, ...){
    va_list args;
    va_start(args, numberOfArguments);
    static size_t errorConter = 0;
    std::cout << reportCounter++ << "- [ERROR] " << errorConter++ << "- Error On File "
    << file << ", Line " << line << ":\n:: ";
    for(int i = 0; i < numberOfArguments; i++){
        std::cout << va_arg(args, const char*);
    }std::cout << std::endl;
    va_end(args);
}

void report_warning(const char* file, size_t line, int numberOfArguments, ...){
    va_list args;
    va_start(args, numberOfArguments);
    static size_t warningCounter = 0;
    std::cout << reportCounter++ << "- [WARNING] " << warningCounter++ << "- Warning On File "
    << file << ", Line " << line << ":\n:: ";
    for(int i = 0; i < numberOfArguments; i++){
        std::cout << va_arg(args, const char*);
    }std::cout << std::endl;

    va_end(args);
}


}



