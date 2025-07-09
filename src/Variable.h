#ifndef SRC_VARIABLE_H
#define SRC_VARIABLE_H
#include <set>
#include <string>
#include "ProductionRule.h"

using namespace std;

class Variable {
public:
    set<char> symbols;
    Variable();
    void inputVariable(char&, const string&);
    void newStartSymbol(ProductionRule&, const char&);
};

#endif //SRC_VARIABLE_H
