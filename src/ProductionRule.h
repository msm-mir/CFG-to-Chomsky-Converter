#ifndef SRC_PRODUCTIONRULE_H
#define SRC_PRODUCTIONRULE_H

#include <string>
#include <set>
#include <vector>
#include <map>

using namespace std;

class ProductionRule {
public:
    map<char, set<string>> rule;
    vector<char> order;

    ProductionRule();

    void inputRule(const string &);

    static void ruleCheck(const char &, char &, string &, bool &);
};

#endif //SRC_PRODUCTIONRULE_H
