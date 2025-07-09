#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <algorithm>
#include <string>
#include "Variable.h"
#include "Terminal.h"
#include "ProductionRule.h"

using namespace std;

void inputVar(Variable&, char&, const string&);
void inputPro(ProductionRule&, const string&);
void proCheck(const char&, char&, string&, bool&);
void newStartVar(ProductionRule&, Variable&, const char&);
void deleteUnsubCheck(ProductionRule&, Variable&, const Terminal&);
void deleteNullCheck(ProductionRule&, Variable&);
void deleteNull(ProductionRule&, vector<char>::iterator&, bool&, char);
void deleteUnitLoopCheck(ProductionRule&, Variable&);
void deleteUnitCheck(ProductionRule&, const Variable&);
void deleteUnit(ProductionRule&, map<char, set<string>>::iterator, char);
void deleteUselessCheck(ProductionRule&, Variable&, const Terminal&);
void deleteInaccessibleCheck(ProductionRule&, Variable&);
void deleteInaccessible(ProductionRule&, const Variable&, vector<pair<char, bool>>&, queue<vector<pair<char, bool>>::iterator>&);
void newVarForTer(ProductionRule&, Variable&, const Terminal&);
void replaceTerToVal(ProductionRule&, const char&, const char&);
void newVarForTwoVar(ProductionRule&, Variable&, const Terminal&);
void findVarForTwoVar(ProductionRule&, Variable&, string&);
bool checkExistVar(ProductionRule&, string&);
void print(const ProductionRule&, const Terminal&);

int main() {
    int cntP;
    cin >> cntP;

    string ter;
    cin.ignore();
    getline(cin, ter);

    Terminal terminal;
    terminal.inputTer(ter);

    string var;
    getline(cin, var);

    Variable variable;
    char S;
    inputVar(variable, S, var);

    ProductionRule production;
    while (cntP--) {
        string pro;
        getline(cin, pro);

        inputPro(production, pro);
    }

    newStartVar(production, variable, S);
    deleteUnsubCheck(production, variable, terminal);
    deleteNullCheck(production, variable);
    deleteUnitLoopCheck(production, variable);
    deleteUselessCheck(production, variable, terminal);
    deleteUnitCheck(production, variable);
    deleteUselessCheck(production, variable, terminal);
    deleteInaccessibleCheck(production, variable);
    terminal.t.erase('@');
    newVarForTer(production, variable, terminal);
    newVarForTwoVar(production, variable, terminal);
    print(production, terminal);

    return 0;
}

void inputVar(Variable& variable, char& S, const string& var) {
    bool i = true;
    for (char c : var) {
        if (c >= 'A' && c <= 'Z') {
            if (i) {
                S = c;
                i = false;
            }
            variable.v.insert(c);
        }
    }
}

void inputPro(ProductionRule& production, const string& pro) {
    bool b = true;
    string s;
    char first;
    set<string> second;

    for (char c : pro) {
        if (c == '|') {
            second.insert(s);
            s.clear();
        }
        proCheck(c, first, s, b);
    }
    second.insert(s);
    production.p.insert({first, second});
    production.order.push_back(first);
}

void proCheck(const char& c, char& first, string& s, bool& b) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '@')) {
        if (b && (c >= 'A' && c <= 'Z')) {
            first = c;
            b = false;
        } else {
            s += c;
        }
    }
}

void newStartVar(ProductionRule& production, Variable& variable, const char& S) {
    string s;
    s = S;
    variable.v.insert('0');
    production.p.insert({'0', {s}});
    production.order.insert(production.order.begin(), '0');
}

void deleteUnsubCheck(ProductionRule& production, Variable& variable, const Terminal& terminal) {
    for (auto v = variable.v.begin(); v != variable.v.end(); v++) {
        auto itP = std::find(production.order.begin(), production.order.end(), *v);
        if (itP == production.order.end()) {
            char c = *v;
            v--;
            variable.v.erase(c);
        }
    }

    bool del = false;
    for (char c : production.order) {
        auto itP = production.p.find(c);
        for (auto s = itP->second.begin(); s != itP->second.end(); s++) {
            if (del) {
                s = itP->second.begin();
                del = false;
            }
            for (char i : *s) {
                auto itV = variable.v.find(i);
                auto itT = terminal.t.find(i);

                if (itV == variable.v.end() && itT == terminal.t.end()) {
                    del = true;
                    break;
                }
            }
            if (del) {
                s = itP->second.erase(s);
                if (itP->second.empty()) {
                    itP->second.insert("@");
                }
            }
        }
    }
}

void deleteNullCheck(ProductionRule& production, Variable& variable) {
    bool b = false;
    vector<char>::iterator high;
    bool change = false;

    for (auto c = production.order.begin(); c != production.order.end(); c++) {
        if (change) {
            change = false;
            c = high;
        }
        auto itP = production.p.find(*c);
        for (auto s = itP->second.begin(); s != itP->second.end(); s++) {
            if (b) {
                s = itP->second.begin();
                b = false;
            }

            if (*s == "@") {
                b = true;
                s = itP->second.erase(s);
                deleteNull(production, high, change, *c);

                if (itP->second.empty()) {
                    if (!change) c--;
                    variable.v.erase(itP->first);
                    production.p.erase(itP);
                    production.order.erase(c+1);
                }

                if (change) break;
            }
        }
        if (change) c = high;
    }
}

void deleteNull(ProductionRule& production, vector<char>::iterator& high, bool& change, char c) {
    high = find(production.order.begin(), production.order.end(), c);

    for (auto & i : production.p) {
        for (auto & j : i.second) {
            string tmp = j;
            if (tmp.find(c) != string::npos) {
                tmp.erase(tmp.find(c), 1);
                if (tmp.empty()) {
                    tmp = "@";
                }
                if (j.size() != 1 || i.first != j.at(0)) {
                    i.second.insert(tmp);

                    if (tmp == "@" &&
                        find(production.order.begin(), production.order.end(), i.first) < high) {
                        high = find(production.order.begin(), production.order.end(), i.first);
                        change = true;
                    }
                }
            }
        }
    }
}

void deleteUnitLoopCheck(ProductionRule& production, Variable& variable) {
    bool b = false;

    for (char c : production.order) {
        auto itP = production.p.find(c);
        for (auto s = itP->second.begin(); s != itP->second.end(); s++) {
            if (b) {
                s = itP->second.begin();
                b = false;
            }
            if ((*s).size() == 1 && variable.v.find(((*s).at(0))) != variable.v.end() && (*s).at(0) == itP->first) {
                b = true;
                s = itP->second.erase(s);
                if (itP->second.empty()) {
                    itP->second.insert("@");
                    deleteNullCheck(production, variable);
                }
            }
        }
    }
}

void deleteUnitCheck(ProductionRule& production, const Variable& variable) {
    bool b = false;

    for (char c : production.order) {
        auto itP = production.p.find(c);
        for (auto s = itP->second.begin(); s != itP->second.end(); s++) {
            if (b) {
                s = itP->second.begin();
                b = false;
            }
            if ((*s).size() == 1 && variable.v.find(((*s).at(0))) != variable.v.end()) {
                b = true;
                string tmp = *s;
                s = itP->second.erase(s);
                deleteUnit(production, itP, tmp.at(0));
            }
        }
    }
}

void deleteUnit(ProductionRule& production, map<char, set<string>>::iterator itP, char c) {
for (const string& s : production.p.find(c)->second) {
itP->second.insert(s);
}
}

void deleteUselessCheck(ProductionRule& production, Variable& variable, const Terminal& terminal) {
    bool b = false;

    for (char c : production.order) {
        auto itP = production.p.find(c);

        bool findTer = false;
        for (char t : terminal.t) {
            string st;
            st = t;
            if (itP->second.find(st) != itP->second.end()) {
                findTer = true;
                break;
            }
        }
        if (findTer) continue;

        for (auto s = itP->second.begin(); s != itP->second.end(); s++) {
            if (b) {
                s = itP->second.begin();
                b = false;
            }

            int cntVar = 0;
            bool itself = false;
            for (char i : *s) {
                if (variable.v.find(i) != variable.v.end()) cntVar++;
                if (i == itP->first) itself = true;
            }

            if (cntVar == 1 && itself) {
                b = true;
                s = itP->second.erase(s);
                if (itP->second.empty()) {
                    itP->second.insert("@");
                    deleteNullCheck(production, variable);
                }
            }
        }
    }
}

void deleteInaccessibleCheck(ProductionRule& production, Variable& variable) {
    vector<pair<char, bool>> check;
    check.reserve(production.order.size());

    for (char c : production.order) {
        auto itV = variable.v.find(c);
        if (c == '0' || c == 'S') check.emplace_back(c, true);
        else check.emplace_back(c, false);
    }

    queue<vector<pair<char, bool>>::iterator> visit;
    visit.push(check.begin());

    while (!visit.empty()) {
        deleteInaccessible(production, variable, check, visit);
    }

    for (auto ch : check) {
        if (!ch.second) {
            production.p.erase(ch.first);
            production.order.erase(std::find(production.order.begin(), production.order.end(), ch.first));
            variable.v.erase(ch.first);
        }
    }
}

void deleteInaccessible(ProductionRule& production, const Variable& variable, vector<pair<char, bool>>& check, queue<vector<pair<char, bool>>::iterator>& visit) {
auto itP = production.p.find(visit.front()->first);
visit.pop();
for (const auto& s : itP->second) {
for (char c : s) {
if (variable.v.find(c) != variable.v.end()) {
auto itCH = find_if(check.begin(), check.end(),
                    [c](const pair<char, bool>& pr){
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

void newVarForTer(ProductionRule& production, Variable& variable, const Terminal& terminal) {
    for (char c : terminal.t) {
        for (char i : variable.v) {
            auto itP = production.p.find(i);
            string tmp;
            tmp = c;
            if (itP->second.size() == 1 && *(itP->second.begin()) == tmp) {
                replaceTerToVal(production, c, i);
                break;
            }
            if ((i+1 >= 'A' && i+1 <= 'Z') && variable.v.find((char)(i+1)) == variable.v.end()) {
                i++;
                variable.v.insert(i);
                production.order.push_back(i);

                string s;
                s = c;
                production.p.insert({i, {s}});

                replaceTerToVal(production, c, i);
                break;
            }
        }
    }
}

void replaceTerToVal(ProductionRule& production, const char& before, const char& after) {
    for (auto& p : production.p) {
        if (p.first == after) continue;
        set<string> update;

        for (auto& s : p.second) {
            if (s.size() == 1 && s.at(0) == before) {
                update.insert(s);
                continue;
            }
            string tmp = s;
            for (auto& c : tmp) {
                if (c == before) {
                    c = after;
                }
            }
            update.insert(tmp);
        }
        p.second = update;
    }
}

void newVarForTwoVar(ProductionRule& production, Variable& variable, const Terminal& terminal) {
    for (auto& p : production.p) {
        set<string> update;
        for (auto s : p.second) {
            if (s.size() == 1 && terminal.t.find(s.at(0)) != terminal.t.end()) {
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

void findVarForTwoVar(ProductionRule& production, Variable& variable, string& str) {
    if (!checkExistVar(production, str)) {
        string rep;
        rep = str.substr(0, 2);

        for (char i : variable.v) {
            if ((i+1 >= 'A' && i+1 <= 'Z') && variable.v.find((char)(i+1)) == variable.v.end()) {
                i++;
                variable.v.insert(i);
                production.order.push_back(i);
                production.p.insert({i, {rep}});

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

bool checkExistVar(ProductionRule& production, string& str) {
    for (auto c = str.begin(); c != str.end(); c++) {
        string find;
        find = *c;
        find += (char)*(c + 1);

        for (auto p : production.p) {
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

void print(const ProductionRule& production, const Terminal& terminal) {
    cout << production.order.size() << endl;

    for (char c : terminal.t) {
        cout << c << " ";
    }
    cout << endl;

    for (char c : production.order) {
        if (c == '0') cout << "S";
        cout << c << " ";
    }
    cout << endl;

    for (char c : production.order) {
        auto itP = production.p.find(c);
        if (itP->first == '0') cout << "S";
        cout << itP->first << " -> ";

        for (auto p = itP->second.begin(); p != itP->second.end(); p++) {
            if (next(p) == itP->second.end()) cout << *p;
            else cout << *p << " | ";
        }
        cout << endl;
    }
}