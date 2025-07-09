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

void ProductionRule::findUselessRHS(Variable &variable, const Terminal &terminal) {
    bool didRemove = false;

    for (char LHS: this->order) {
        auto itR = this->rule.find(LHS);

        bool didFindTerminal = false;
        for (char charTerminal: terminal.terminals) {
            string srtTerminal;
            srtTerminal = charTerminal;
            if (itR->second.find(srtTerminal) != itR->second.end()) {
                didFindTerminal = true;
                break;
            }
        }
        if (didFindTerminal) continue;

        for (auto RHS = itR->second.begin(); RHS != itR->second.end(); RHS++) {
            if (didRemove) {
                RHS = itR->second.begin();
                didRemove = false;
            }

            int cntOfVariables = 0;
            bool variableIsLHS = false;
            for (char charRHS: *RHS) {
                if (variable.symbols.find(charRHS) != variable.symbols.end()) cntOfVariables++;
                if (charRHS == itR->first) variableIsLHS = true;
            }

            if (cntOfVariables == 1 && variableIsLHS) {
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

void ProductionRule::findInaccessibleLHS(Variable &variable) {
    vector<pair<char, bool>> checkVarIsAccessible;
    checkVarIsAccessible.reserve(this->order.size());

    for (char LHS: this->order) {
        auto itV = variable.symbols.find(LHS);
        if (LHS == '0' || LHS == 'S') checkVarIsAccessible.emplace_back(LHS, true);
        else checkVarIsAccessible.emplace_back(LHS, false);
    }

    queue <vector<pair<char, bool>>::iterator> visitVariables;
    visitVariables.push(checkVarIsAccessible.begin());

    while (!visitVariables.empty()) {
        this->removeInaccessibleLHS(variable, checkVarIsAccessible, visitVariables);
    }

    for (auto ch: checkVarIsAccessible) {
        if (!ch.second) {
            this->rule.erase(ch.first);
            this->order.erase(std::find(this->order.begin(), this->order.end(), ch.first));
            variable.symbols.erase(ch.first);
        }
    }
}

void ProductionRule::removeInaccessibleLHS(const Variable &variable, vector<pair<char, bool>> &checkVarIsAccessible, queue <vector<pair<char, bool>>::iterator> &visitVariables) {
    auto itR = this->rule.find(visitVariables.front()->first);
    visitVariables.pop();
    for (const auto &RHS: itR->second) {
        for (char charRHS: RHS) {
            if (variable.symbols.find(charRHS) != variable.symbols.end()) {
                auto itCH = find_if(checkVarIsAccessible.begin(), checkVarIsAccessible.end(),
                                    [charRHS](const pair<char, bool> &pr) {
                                        return pr.first == charRHS;
                                    });
                if (!itCH->second) {
                    itCH->second = true;
                    visitVariables.push(itCH);
                }
            }
        }
    }
}

void ProductionRule::newSymbolForTerminals(Variable &variable, const Terminal &terminal) {
    for (char charTerminal: terminal.terminals) {
        for (char charVariable: variable.symbols) {
            auto itR = this->rule.find(charVariable);
            string strTerminal;
            strTerminal = charTerminal;
            if (itR->second.size() == 1 && *(itR->second.begin()) == strTerminal) {
                this->replaceNewSymbolForTerminal(charTerminal, charVariable);
                break;
            }
            if ((charVariable + 1 >= 'A' && charVariable + 1 <= 'Z') && variable.symbols.find((char) (charVariable + 1)) == variable.symbols.end()) {
                charVariable++;
                variable.symbols.insert(charVariable);
                this->order.push_back(charVariable);

                string strTer;
                strTer = charTerminal;
                this->rule.insert({charVariable, {strTer}});

                this->replaceNewSymbolForTerminal(charTerminal, charVariable);
                break;
            }
        }
    }
}

void ProductionRule::replaceNewSymbolForTerminal(const char &before, const char &after) {
    for (auto &rules: this->rule) {
        if (rules.first == after) continue;
        set<string> updateRHSs;

        for (auto &RHS: rules.second) {
            if (RHS.size() == 1 && RHS.at(0) == before) {
                updateRHSs.insert(RHS);
                continue;
            }
            string strRHS = RHS;
            for (auto &charRHS: strRHS) {
                if (charRHS == before) {
                    charRHS = after;
                }
            }
            updateRHSs.insert(strRHS);
        }
        rules.second = updateRHSs;
    }
}

void ProductionRule::findDoubleSymbolsForNewSymbol(Variable &variable, const Terminal &terminal) {
    for (auto &rules: this->rule) {
        set<string> updateRHSs;
        for (auto RHS: rules.second) {
            if (RHS.size() == 1 && terminal.terminals.find(RHS.at(0)) != terminal.terminals.end()) {
                updateRHSs.insert(RHS);
                continue;
            }
            string strRHS = RHS;
            while (strRHS.size() > 2) {
                this->newSymbolForDoubleSymbols(variable, strRHS);
            }
            updateRHSs.insert(strRHS);
        }
        rules.second = updateRHSs;
    }
}

void ProductionRule::newSymbolForDoubleSymbols(Variable &variable, string &strRHS) {
    if (!this->checkExistVar(strRHS)) {
        string doubleSymbols;
        doubleSymbols = strRHS.substr(0, 2);

        for (char charSymbol: variable.symbols) {
            if ((charSymbol + 1 >= 'A' && charSymbol + 1 <= 'Z') && variable.symbols.find((char) (charSymbol + 1)) == variable.symbols.end()) {
                charSymbol++;
                variable.symbols.insert(charSymbol);
                this->order.push_back(charSymbol);
                this->rule.insert({charSymbol, {doubleSymbols}});

                size_t posStrRHS;
                string tmpStrRHS = strRHS;

                while ((posStrRHS = tmpStrRHS.find(doubleSymbols)) != string::npos) {
                    tmpStrRHS.replace(posStrRHS, doubleSymbols.length(), 1, charSymbol);
                }
                strRHS = tmpStrRHS;
                return;
            }
        }
    }
}