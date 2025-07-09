#ifndef SRC_VARIABLE_H
#define SRC_VARIABLE_H

#include <set>
#include <string>
#include <algorithm>

using namespace std;

class ProductionRule;

class Variable {
public:
    set<char> symbols;

    Variable();

    void inputVariable(char &, const string &);

    void newStartSymbol(ProductionRule &, const char &);

    void removeUnusedSymbols(const ProductionRule &);
};

#endif //SRC_VARIABLE_H
