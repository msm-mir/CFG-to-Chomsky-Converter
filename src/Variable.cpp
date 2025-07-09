#include "Variable.h"

Variable::Variable() = default;

void Variable::inputVar(char& S, const string& variable) {
    bool i = true;
    for (char c : variable) {
        if (c >= 'A' && c <= 'Z') {
            if (i) {
                S = c;
                i = false;
            }
            this->var.insert(c);
        }
    }
}

void Variable::newStartVar(ProductionRule& production, const char& S) {
    string s;
    s = S;
    this->var.insert('0');
    production.rule.insert({'0', {s}});
    production.order.insert(production.order.begin(), '0');
}