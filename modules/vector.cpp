#include "vector.h"
std::vector<std::string> AppendToVector(std::vector<std::string> v, std::string x) {
    if(!(std::find(v.begin(), v.end(), x) != v.end()))
        v.push_back(x);
    return v;
}

std::vector<std::string> WordToVector(std::string s) {
    std::string tmp; 
    std::stringstream ss(s);
    std::vector<std::string> words;
    while(getline(ss, tmp, ','))
        words.push_back(tmp);
    return words;
}