#ifndef SRC_VARIABLE_H
#define SRC_VARIABLE_H
#include <set>
#include <string>

using namespace std;

class Variable {
public:
    set<char> v;
    Variable();
    void inputVar(char&, const string&);
};

#endif //SRC_VARIABLE_H
