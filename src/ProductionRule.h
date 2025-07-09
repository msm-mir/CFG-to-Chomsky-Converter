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
};

#endif //SRC_PRODUCTIONRULE_H
