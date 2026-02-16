#ifndef UTILS_H
#define UTILS_H

#include <string>

using namespace std;

// Removes leading and trailing whitespace (spaces, tabs, newlines)
string trim(const string& str);

// Converts string to uppercase (useful for normalizing IDs like 'a' → 'A')
string toUpper(const string& str);

#endif // UTILS_H
