#pragma once
typedef const char* stringToDLL;    // String passed GM --> DLL
typedef char* stringFromDLL;        // String passed DLL -- > GM
typedef double GMBOOL;
typedef double GMINT;

#define GMTRUE 1.0
#define GMFALSE 0.0

#define gmx extern "C" __declspec(dllexport)

/* Regular includes */
#include <string>
#include <string.h>
#include <iostream>
using namespace std; // Fight me

//__declspec(dllimport) void (*CreateAsynEventWithDSMap)(int, int);
//__declspec(dllimport) int (*CreateDsMap)(int _num, ...);
//__declspec(dllimport) bool (*DsMapAddDouble)(int _index, char* _pKey, double value);
//__declspec(dllimport) bool (*DsMapAddString)(int _index, char* _pKey, char* pVal);

//__declspec(dllimport) double RegisterCallbacks(char* arg1, char* arg2, char* arg3, char* arg4);

/**
* @brief This is the static helper class.
*        Access is namespace-like: gmu::function
*/
namespace gmu {

    /**
    * @param s String to convert to char*
    * @brief Convert string to char*
    */
    static char* string_to_charptr(string s)
    {
        return _strdup(s.c_str());
    }


    /**
    * @param s String to convert to const char*
    * @brief Convert string to const char*
    */
    static const char* string_to_constcharptr(string s)
    {
        return s.c_str();
    }

    /**
    * @param p const char* to convert
    * @brief Convert const char* to string
    */
    static string constcharptr_to_string(const char* p)
    {
        return string(p);
    }

    /**
    * @param s String to print
    * @brief Prints a string to GMS debug console
    */
    static void debugmessage(string s)
    {
        std::cout << s << std::endl;
    }

};
