#pragma once 

#include <iostream>
#include <vector>
#include <string>
#include <bits/stdc++.h>

using namespace std;

class StringOps
{

public:
    static vector<string> split(const string& str);
    static string rtrim(const string& str);
    static string ltrim(const string& str);
    static string removeLastChar(const string& str);
    static bool stringToBool(string const& str);
};

