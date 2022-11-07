#include "LexicalAnalyser.h"

using namespace std;

LexicalAnalyser *LexicalAnalyser::lexicalAnalyser_ = nullptr;

LexicalAnalyser::LexicalAnalyser(const string solution_filename, string pattern)
{
    this->pattern = pattern;
    this->solution_in.open(solution_filename);

    if (!this->solution_in.is_open())
    {
        cerr << "Sorry we can't open the file : " << solution_filename << endl;
        exit(-1);
    }
}

LexicalAnalyser *LexicalAnalyser::getInstance(const string solution_filename, string pattern)
{
    if (lexicalAnalyser_ == nullptr)
    {
        lexicalAnalyser_ = new LexicalAnalyser(solution_filename, pattern);
    }
    return lexicalAnalyser_;
}

// Business Traitement
bool LexicalAnalyser::lineMatchRegEx(string line)
{
    return regex_match(line, regex(pattern));
}

void LexicalAnalyser::fileMatchLexicalReqs(vector<vector<string>> &clusters)
{

    string line;
    int LINES_NUMBER = 0;
    while (getline(solution_in, line))
    {
        vector<string> tokens; 
        LINES_NUMBER++;
        if (lineMatchRegEx(line))
        {
            tokens = StringOps::split(StringOps::ltrim(line)); 
            // delete cluster
            tokens.erase(tokens.begin());
            // delete 2123
            tokens.erase(tokens.begin());
            // delete :
            tokens.erase(tokens.begin());
            // delete last ;
            tokens.pop_back();  

            clusters.push_back(tokens); 
        }
        else
        {
            cerr << "Line : " << LINES_NUMBER << RED << " doesn't match the given pattern" << RESET << endl;
        }
    }
}