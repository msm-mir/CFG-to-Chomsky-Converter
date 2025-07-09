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