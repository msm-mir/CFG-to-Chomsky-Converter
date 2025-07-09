#include <iostream>
#include "Variable.h"
#include "Terminal.h"
#include "ProductionRule.h"

using namespace std;

void printOutput(const ProductionRule &, const Terminal &);

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
    printOutput(productionRule, terminal);

    return 0;
}

void printOutput(const ProductionRule &production, const Terminal &terminal) {
    cout << production.order.size() << endl;

    for (char charTerminal: terminal.terminals) {
        cout << charTerminal << " ";
    }
    cout << endl;

    for (char LHS: production.order) {
        if (LHS == '0') cout << "S";
        cout << LHS << " ";
    }
    cout << endl;

    for (char LHS: production.order) {
        auto itR = production.rule.find(LHS);
        if (itR->first == '0') cout << "S";
        cout << itR->first << " -> ";

        for (auto RHS = itR->second.begin(); RHS != itR->second.end(); RHS++) {
            if (next(RHS) == itR->second.end()) cout << *RHS;
            else cout << *RHS << " | ";
        }
        cout << endl;
    }
}