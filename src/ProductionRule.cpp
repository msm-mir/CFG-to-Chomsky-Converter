#include "ProductionRule.h"

ProductionRule::ProductionRule() = default;

void ProductionRule::inputRule(const string& pro) {
    bool b = true;
    string s;
    char first;
    set<string> second;

    for (char c : pro) {
        if (c == '|') {
            second.insert(s);
            s.clear();
        }
        ruleCheck(c, first, s, b);
    }
    second.insert(s);
    this->rule.insert({first, second});
    this->order.push_back(first);
}

void ProductionRule::ruleCheck(const char& c, char& first, string& s, bool& b) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '@')) {
        if (b && (c >= 'A' && c <= 'Z')) {
            first = c;
            b = false;
        } else {
            s += c;
        }
    }
}