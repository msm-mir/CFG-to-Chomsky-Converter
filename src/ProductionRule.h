#ifndef SRC_PRODUCTIONRULE_H
#define SRC_PRODUCTIONRULE_H
#include <string>
#include <set>
#include <vector>
#include <map>

using namespace std;

class ProductionRule {
public:
    map<char, set<string>> p;
    vector<char> order;
    ProductionRule();
};

#endif //SRC_PRODUCTIONRULE_H
