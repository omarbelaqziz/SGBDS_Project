#pragma once 
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <list>
#include "../SGBDS_Project/preproc.h"
using namespace std;


/**
 * DataFile.in  |     ==============     ==============      ==============
 * Solution.in  | =>  | LexicalAna | =>  | LogicalAna |  =>  | StatsCalcu |  => Stats.in
 * Param.in     |     ==============     ==============      ==============
 */

// we need to implement singleton because one instance of LexicalAnalyser needs to be in work 

class LexicalAnalyser
{
private: 
    string pattern;
    ifstream solution_in; 

protected: 
    LexicalAnalyser(const string solution_filename, string pattern);
    static LexicalAnalyser* lexicalAnalyser_; 

public:
    static LexicalAnalyser * getInstance(const string solution_filename, string pattern);
    LexicalAnalyser(LexicalAnalyser &other) = delete; 
    void operator = (const LexicalAnalyser &) = delete; 
    

    // Business traitement 
    void fileMatchLexicalReqs(vector<vector<string>>& clusters) ;
    bool lineMatchRegEx(string line);
};

