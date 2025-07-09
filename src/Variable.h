#ifndef SRC_VARIABLE_H
#define SRC_VARIABLE_H
#include <set>
#include <string>
#include "ProductionRule.h"

using namespace std;

class Variable {
public:
    set<char> var;
    Variable();
    void inputVar(char&, const string&);
    void newStartVar(ProductionRule&, const char&);
};

#endif //SRC_VARIABLE_H
