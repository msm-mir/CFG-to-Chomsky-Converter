#include <iostream>
#include "Variable.h"
#include "Terminal.h"
#include "ProductionRule.h"

using namespace std;

void deleteInaccessibleCheck(ProductionRule &, Variable &);

void deleteInaccessible(ProductionRule &, const Variable &, vector<pair<char, bool>> &,
                        queue <vector<pair<char, bool>>::iterator> &);

void newVarForTer(ProductionRule &, Variable &, const Terminal &);

void replaceTerToVal(ProductionRule &, const char &, const char &);

void newVarForTwoVar(ProductionRule &, Variable &, const Terminal &);

void findVarForTwoVar(ProductionRule &, Variable &, string &);

bool checkExistVar(ProductionRule &, string &);

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
    deleteInaccessibleCheck(variable);
    terminal.terminals.erase('@');
    newVarForTer(variable, terminal);
    newVarForTwoVar(variable, terminal);
    print(terminal);

    return 0;
}

void deleteInaccessibleCheck(ProductionRule &production, Variable &variable) {
    vector<pair<char, bool>> check;
    check.reserve(production.order.size());

    for (char c: production.order) {
        auto itV = variable.symbols.find(c);
        if (c == '0' || c == 'S') check.emplace_back(c, true);
        else check.emplace_back(c, false);
    }

    queue <vector<pair<char, bool>>::iterator> visit;
    visit.push(check.begin());

    while (!visit.empty()) {
        deleteInaccessible(production, variable, check, visit);
    }

    for (auto ch: check) {
        if (!ch.second) {
            production.rule.erase(ch.first);
            production.order.erase(std::find(production.order.begin(), production.order.end(), ch.first));
            variable.symbols.erase(ch.first);
        }
    }
}

void deleteInaccessible(ProductionRule &production, const Variable &variable, vector<pair<char, bool>> &check,
                        queue <vector<pair<char, bool>>::iterator> &visit) {
    auto itP = production.rule.find(visit.front()->first);
    visit.pop();
    for (const auto &s: itP->second) {
        for (char c: s) {
            if (variable.symbols.find(c) != variable.symbols.end()) {
                auto itCH = find_if(check.begin(), check.end(),
                                    [c](const pair<char, bool> &pr) {
                                        return pr.first == c;
                                    });
                if (!itCH->second) {
                    itCH->second = true;
                    visit.push(itCH);
                }
            }
        }
    }
}

void newVarForTer(ProductionRule &production, Variable &variable, const Terminal &terminal) {
    for (char c: terminal.terminals) {
        for (char i: variable.symbols) {
            auto itP = production.rule.find(i);
            string tmp;
            tmp = c;
            if (itP->second.size() == 1 && *(itP->second.begin()) == tmp) {
                replaceTerToVal(production, c, i);
                break;
            }
            if ((i + 1 >= 'A' && i + 1 <= 'Z') && variable.symbols.find((char) (i + 1)) == variable.symbols.end()) {
                i++;
                variable.symbols.insert(i);
                production.order.push_back(i);

                string s;
                s = c;
                production.rule.insert({i, {s}});

                replaceTerToVal(production, c, i);
                break;
            }
        }
    }
}

void replaceTerToVal(ProductionRule &production, const char &before, const char &after) {
    for (auto &p: production.rule) {
        if (p.first == after) continue;
        set<string> update;

        for (auto &s: p.second) {
            if (s.size() == 1 && s.at(0) == before) {
                update.insert(s);
                continue;
            }
            string tmp = s;
            for (auto &c: tmp) {
                if (c == before) {
                    c = after;
                }
            }
            update.insert(tmp);
        }
        p.second = update;
    }
}

void newVarForTwoVar(ProductionRule &production, Variable &variable, const Terminal &terminal) {
    for (auto &p: production.rule) {
        set<string> update;
        for (auto s: p.second) {
            if (s.size() == 1 && terminal.terminals.find(s.at(0)) != terminal.terminals.end()) {
                update.insert(s);
                continue;
            }
            string str = s;
            while (str.size() > 2) {
                findVarForTwoVar(production, variable, str);
            }
            update.insert(str);
        }
        p.second = update;
    }
}

void findVarForTwoVar(ProductionRule &production, Variable &variable, string &str) {
    if (!checkExistVar(production, str)) {
        string rep;
        rep = str.substr(0, 2);

        for (char i: variable.symbols) {
            if ((i + 1 >= 'A' && i + 1 <= 'Z') && variable.symbols.find((char) (i + 1)) == variable.symbols.end()) {
                i++;
                variable.symbols.insert(i);
                production.order.push_back(i);
                production.rule.insert({i, {rep}});

                size_t pos;
                string tmp = str;

                while ((pos = tmp.find(rep)) != string::npos) {
                    tmp.replace(pos, rep.length(), 1, i);
                }
                str = tmp;
                return;
            }
        }
    }
}

bool checkExistVar(ProductionRule &production, string &str) {
    for (auto c = str.begin(); c != str.end(); c++) {
        string find;
        find = *c;
        find += (char) *(c + 1);

        for (auto p: production.rule) {
            auto itP = p.second.find(find);

            if (itP != p.second.end() && p.second.size() == 1) {
                size_t pos;
                string tmp = str;

                while ((pos = tmp.find(find)) != string::npos) {
                    tmp.replace(pos, find.length(), 1, p.first);
                }
                str = tmp;
                return true;
            }
        }
    }
    return false;
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