#include "Variable.h"

Variable::Variable() = default;

void Variable::inputVariable(char& startSymbol, const string& var) {
    bool b = true;
    for (char c : var) {
        if (c >= 'A' && c <= 'Z') {
            if (b) {
                startSymbol = c;
                b = false;
            }
            this->symbols.insert(c);
        }
    }
}

void Variable::newStartSymbol(ProductionRule& production, const char& startSymbol) {
    string tmp;
    tmp = startSymbol;
    this->symbols.insert('0');
    production.rule.insert({'0', {tmp}});
    production.order.insert(production.order.begin(), '0');
}