#pragma once

#ifdef _WIN32
    #define BENCHTOOLS_WIN
#elif (__unix__) || (__linux__)
    #define BENCHTOOLS_LIN
#elif __APPLE__
    #define BENCHTOOLS_MAC
#endif
