#include "pch.h"
#include "StringOps.h"

string StringOps::rtrim(const string& str)
{
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end());

    return s;
}
string StringOps::ltrim(const string& str)
{
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));

    return s;
}
string StringOps::removeLastChar(const string& str)
{
    string s(str);
    if (!s.empty()) {
        s.pop_back();
    }
    return s;
}

vector<string> StringOps::split(const string& str)
{

    vector<string> tokens;

    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = str.find(" ", start)) != string::npos)
    {
        if (str.substr(start, end - start).find_first_not_of(' ') != std::string::npos)
            tokens.push_back(str.substr(start, end - start));

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}
bool StringOps::stringToBool(string const& str) {
    return str != "0";
}
