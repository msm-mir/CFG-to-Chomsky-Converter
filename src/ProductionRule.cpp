#include "ProductionRule.h"

ProductionRule::ProductionRule() = default;

void ProductionRule::inputRule(const string &strRHS) {
    bool isLHS = true;
    string productionBody;
    char LHS;
    set<string> RHS;

    for (char c: strRHS) {
        if (c == '|') {
            RHS.insert(productionBody);
            productionBody.clear();
        }
        ruleCheck(c, LHS, productionBody, isLHS);
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
    for (auto &r: this->rule) {
        if (r.second.find(RHSsToRemove.at(idxV)) != r.second.end()) {
            r.second.erase(RHSsToRemove.at(idxV));
            if (r.second.empty())
                r.second.insert("@");
            idxV++;
        }
    }
}

void ProductionRule::findLambdaRHS(Variable &variable) {
    bool didRemove = false;
    vector<char>::iterator highestRuleChanged;
    bool didPrevRuleChange = false;

    for (auto p = this->order.begin(); p != this->order.end(); p++) {
        if (didPrevRuleChange) {
            didPrevRuleChange = false;
            p = highestRuleChanged;
        }
        auto itP = this->rule.find(*p);
        for (auto s = itP->second.begin(); s != itP->second.end(); s++) {
            if (didRemove) {
                s = itP->second.begin();
                didRemove = false;
            }

            if (*s == "@") {
                didRemove = true;
                s = itP->second.erase(s);
                this->removeLambdaRHS(highestRuleChanged, didPrevRuleChange, *p);

                if (itP->second.empty()) {
                    if (!didPrevRuleChange) p--;
                    variable.symbols.erase(itP->first);
                    this->rule.erase(itP);
                    this->order.erase(p + 1);
                }

                if (didPrevRuleChange) break;
            }
        }
        if (didPrevRuleChange) p = highestRuleChanged;
    }
}

void ProductionRule::removeLambdaRHS(vector<char>::iterator &high, bool &change, const char &c) {
    high = find(this->order.begin(), this->order.end(), c);

    for (auto &i: this->rule) {
        for (auto &j: i.second) {
            string tmp = j;
            if (tmp.find(c) != string::npos) {
                tmp.erase(tmp.find(c), 1);
                if (tmp.empty()) {
                    tmp = "@";
                }
                if (j.size() != 1 || i.first != j.at(0)) {
                    i.second.insert(tmp);

                    if (tmp == "@" &&
                        find(this->order.begin(), this->order.end(), i.first) < high) {
                        high = find(this->order.begin(), this->order.end(), i.first);
                        change = true;
                    }
                }
            }
        }
    }
}