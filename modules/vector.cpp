#include "vector.h"
std::vector<std::string> AppendToVector(std::vector<std::string> v, std::string x) {
    if(!(std::find(v.begin(), v.end(), x) != v.end())) {
        v.push_back(x);
    }
    return v;
}