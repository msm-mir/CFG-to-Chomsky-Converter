#include "Variable.h"
#include "ProductionRule.h"

Variable::Variable() = default;

void Variable::inputVariable(char &startSymbol, const string &var) {
    bool b = true;
    for (char c: var) {
        if (c >= 'A' && c <= 'Z') {
            if (b) {
                startSymbol = c;
                b = false;
            }
            this->symbols.insert(c);
        }
    }
}

void Variable::newStartSymbol(ProductionRule &productionRule, const char &startSymbol) {
    string tmp;
    tmp = startSymbol;
    this->symbols.insert('0');
    productionRule.rule.insert({'0', {tmp}});
    productionRule.order.insert(productionRule.order.begin(), '0');
}

void Variable::removeUnusedSymbols(const ProductionRule &productionRule) {
    for (auto s = this->symbols.begin(); s != this->symbols.end(); s++) {
        auto itPR = std::find(productionRule.order.begin(), productionRule.order.end(), *s);
        if (itPR == productionRule.order.end()) {
            char c = *s;
            s--;
            this->symbols.erase(c);
        }
    }
}