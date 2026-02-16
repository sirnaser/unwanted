#include "Utils.h"
#include <algorithm>
#include <cctype>

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";

    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

string toUpper(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(),
              [](unsigned char c) { return toupper(c); });
    return result;
}
