#include "ProductionRule.h"

ProductionRule::ProductionRule() = default;

void ProductionRule::inputRule(const string &strRHS) {
    bool b = true;
    string productionBody;
    char LHS;
    set<string> RHS;

    for (char c: strRHS) {
        if (c == '|') {
            RHS.insert(productionBody);
            productionBody.clear();
        }
        ruleCheck(c, LHS, productionBody, b);
    }
    RHS.insert(productionBody);
    this->rule.insert({LHS, RHS});
    this->order.push_back(LHS);
}

void ProductionRule::ruleCheck(const char &character, char &LHS, string &RHS, bool &b) {
    if ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z') || (character == '@')) {
        if (b && (character >= 'A' && character <= 'Z')) {
            LHS = character;
            b = false;
        } else {
            RHS += character;
        }
    }
}

void ProductionRule::removeNonGrammarElementsRHS(const Variable &variable, const Terminal &terminal) {
    vector<string> RHSsToRemove;

    for (const auto &r: this->rule) {
        for (const string &rhs: r.second) {
            for (const char &c: rhs) {
                auto itV = variable.symbols.find(c);
                auto itT = terminal.terminals.find(c);

                if (itV == variable.symbols.end() && itT == terminal.terminals.end()) {
                    RHSsToRemove.push_back(rhs);
                    break;
                }
            }
        }
    }

    size_t idxV = 0;
    for (auto &r : this->rule) {
        if (r.second.find(RHSsToRemove.at(idxV)) != r.second.end()) {
            r.second.erase(RHSsToRemove.at(idxV));
            idxV++;
        }
    }
}