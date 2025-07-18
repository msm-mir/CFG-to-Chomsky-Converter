#ifndef SRC_PRODUCTIONRULE_H
#define SRC_PRODUCTIONRULE_H

#include <string>
#include <set>
#include <vector>
#include <map>
#include <queue>

using namespace std;

class Variable;
class Terminal;

class ProductionRule {
public:
    map<char, set<string>> rule;
    vector<char> order;

    ProductionRule();

    void inputRule(const string &);

    static void ruleCheck(const char &, char &, string &, bool &);

    void removeNonGrammarElementsRHS(const Variable &, const Terminal &);

    void findLambdaRHS(Variable &);

    void removeLambdaRHS(vector<char>::iterator &, bool &, const char &);

    void findAndRemoveSelfLoopRHS(Variable &);

    void findUnitRHS(const Variable &);

    void removeUnitRHS(const map<char, set<string>>::iterator &, const char &);

    void findUselessRHS(Variable &, const Terminal &);

    void findInaccessibleLHS(Variable &);

    void removeInaccessibleLHS(const Variable &, vector<pair<char, bool>> &, queue <vector<pair<char, bool>>::iterator> &);

    void newSymbolForTerminals(Variable &, const Terminal &);

    void replaceNewSymbolForTerminal(const char &, const char &);

    void findDoubleSymbolsForNewSymbol(Variable &, const Terminal &);

    void newSymbolForDoubleSymbols(Variable &, string &);

    bool checkIfSymbolExistsForDoubleSymbol(string &);
};

#endif //SRC_PRODUCTIONRULE_H
