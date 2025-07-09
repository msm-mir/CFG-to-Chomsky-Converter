#ifndef SRC_PRODUCTIONRULE_H
#define SRC_PRODUCTIONRULE_H

#include <string>
#include <set>
#include <vector>
#include <map>
#include "Variable.h"
#include "Terminal.h"

using namespace std;

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
};

#endif //SRC_PRODUCTIONRULE_H
