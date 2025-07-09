#include <iostream>
#include "Variable.h"
#include "Terminal.h"
#include "ProductionRule.h"

using namespace std;

void print(const ProductionRule &, const Terminal &);

int main() {
    int cntRules;
    cin >> cntRules;

    string terminalsInput;
    cin.ignore();
    getline(cin, terminalsInput);

    Terminal terminal;
    terminal.inputTer(terminalsInput);

    string variablesInput;
    getline(cin, variablesInput);

    Variable variable;
    char startSymbol;
    variable.inputVariable(startSymbol, variablesInput);

    ProductionRule productionRule;
    while (cntRules--) {
        string rulesInput;
        getline(cin, rulesInput);

        productionRule.inputRule(rulesInput);
    }

    variable.newStartSymbol(productionRule, startSymbol);
    variable.removeUnusedSymbols(productionRule);
    productionRule.removeNonGrammarElementsRHS(variable, terminal);
    productionRule.findLambdaRHS(variable);
    productionRule.findAndRemoveSelfLoopRHS(variable);
    productionRule.findUselessRHS(variable, terminal);
    productionRule.findUnitRHS(variable);
    productionRule.findUselessRHS(variable, terminal);
    productionRule.findInaccessibleLHS(variable);
    terminal.terminals.erase('@');
    productionRule.newSymbolForTerminals(variable, terminal);
    productionRule.findDoubleSymbolsForNewSymbol(variable, terminal);
    print(terminal);

    return 0;
}

void print(const ProductionRule &production, const Terminal &terminal) {
    cout << production.order.size() << endl;

    for (char c: terminal.terminals) {
        cout << c << " ";
    }
    cout << endl;

    for (char c: production.order) {
        if (c == '0') cout << "S";
        cout << c << " ";
    }
    cout << endl;

    for (char c: production.order) {
        auto itP = production.rule.find(c);
        if (itP->first == '0') cout << "S";
        cout << itP->first << " -> ";

        for (auto p = itP->second.begin(); p != itP->second.end(); p++) {
            if (next(p) == itP->second.end()) cout << *p;
            else cout << *p << " | ";
        }
        cout << endl;
    }
}