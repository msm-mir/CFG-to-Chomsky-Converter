#include "ProductionRule.h"

ProductionRule::ProductionRule() = default;

void ProductionRule::inputRule(const string &strRHS) {
    bool isLHS = true;
    string productionBody;
    char LHS;
    set<string> RHS;

    for (char charRHS: strRHS) {
        if (charRHS == '|') {
            RHS.insert(productionBody);
            productionBody.clear();
        }
        ruleCheck(charRHS, LHS, productionBody, isLHS);
    }
    RHS.insert(productionBody);
    this->rule.insert({LHS, RHS});
    this->order.push_back(LHS);
}

void ProductionRule::ruleCheck(const char &character, char &LHS, string &RHS, bool &isLHS) {
    if ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z') || (character == '@')) {
        if (isLHS && (character >= 'A' && character <= 'Z')) {
            LHS = character;
            isLHS = false;
        } else {
            RHS += character;
        }
    }
}

void ProductionRule::removeNonGrammarElementsRHS(const Variable &variable, const Terminal &terminal) {
    vector<string> RHSsToRemove;

    for (const auto &rules: this->rule) {
        for (const string &RHS: rules.second) {
            for (const char &c: RHS) {
                auto itV = variable.symbols.find(c);
                auto itT = terminal.terminals.find(c);

                if (itV == variable.symbols.end() && itT == terminal.terminals.end()) {
                    RHSsToRemove.push_back(RHS);
                    break;
                }
            }
        }
    }

    size_t idxV = 0;
    for (auto &rules: this->rule) {
        if (rules.second.find(RHSsToRemove.at(idxV)) != rules.second.end()) {
            rules.second.erase(RHSsToRemove.at(idxV));
            if (rules.second.empty())
                rules.second.insert("@");
            idxV++;
        }
    }
}

void ProductionRule::findLambdaRHS(Variable &variable) {
    bool didRemove = false;
    vector<char>::iterator highestRuleChanged;
    bool didPrevRuleChange = false;

    for (auto LHS = this->order.begin(); LHS != this->order.end(); LHS++) {
        if (didPrevRuleChange) {
            didPrevRuleChange = false;
            LHS = highestRuleChanged;
        }
        auto itR = this->rule.find(*LHS);
        for (auto RHS = itR->second.begin(); RHS != itR->second.end(); RHS++) {
            if (didRemove) {
                RHS = itR->second.begin();
                didRemove = false;
            }

            if (*RHS == "@") {
                didRemove = true;
                RHS = itR->second.erase(RHS);
                this->removeLambdaRHS(highestRuleChanged, didPrevRuleChange, *LHS);

                if (itR->second.empty()) {
                    if (!didPrevRuleChange) LHS--;
                    variable.symbols.erase(itR->first);
                    this->rule.erase(itR);
                    this->order.erase(LHS + 1);
                }

                if (didPrevRuleChange) break;
            }
        }
        if (didPrevRuleChange) LHS = highestRuleChanged;
    }
}

void ProductionRule::removeLambdaRHS(vector<char>::iterator &highestRuleChanged, bool &didPrevRuleChange, const char &LHS) {
    highestRuleChanged = find(this->order.begin(), this->order.end(), LHS);

    for (auto &rules: this->rule) {
        for (auto &RHS: rules.second) {
            string strRHS = RHS;
            if (strRHS.find(LHS) != string::npos) {
                strRHS.erase(strRHS.find(LHS), 1);
                if (strRHS.empty()) {
                    strRHS = "@";
                }
                if (RHS.size() != 1 || rules.first != RHS.at(0)) {
                    rules.second.insert(strRHS);

                    if (strRHS == "@" && find(this->order.begin(), this->order.end(), rules.first) < highestRuleChanged) {
                        highestRuleChanged = find(this->order.begin(), this->order.end(), rules.first);
                        didPrevRuleChange = true;
                    }
                }
            }
        }
    }
}

void ProductionRule::findAndRemoveSelfLoopRHS(Variable &variable) {
    bool didRemove = false;

    for (char LHS: this->order) {
        auto itR = this->rule.find(LHS);
        for (auto RHS = itR->second.begin(); RHS != itR->second.end(); RHS++) {
            if (didRemove) {
                RHS = itR->second.begin();
                didRemove = false;
            }
            if ((*RHS).size() == 1 && variable.symbols.find(((*RHS).at(0))) != variable.symbols.end() && (*RHS).at(0) == itR->first) {
                didRemove = true;
                RHS = itR->second.erase(RHS);
                if (itR->second.empty()) {
                    itR->second.insert("@");
                    this->findLambdaRHS(variable);
                }
            }
        }
    }
}

void ProductionRule::findUnitRHS(const Variable &variable) {
    bool didReplace = false;

    for (char LHS: this->order) {
        auto itR = this->rule.find(LHS);
        for (auto RHS = itR->second.begin(); RHS != itR->second.end(); RHS++) {
            if (didReplace) {
                RHS = itR->second.begin();
                didReplace = false;
            }
            if ((*RHS).size() == 1 && variable.symbols.find(((*RHS).at(0))) != variable.symbols.end()) {
                didReplace = true;
                string tmp = *RHS;
                RHS = itR->second.erase(RHS);
                this->removeUnitRHS(itR, tmp.at(0));
            }
        }
    }
}

void ProductionRule::removeUnitRHS(const map<char, set<string>>::iterator &itR, const char &RHS) {
    for (const string &strRHS: this->rule.find(RHS)->second) {
        itR->second.insert(strRHS);
    }
}