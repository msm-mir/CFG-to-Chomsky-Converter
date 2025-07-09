#ifndef SRC_TERMINAL_H
#define SRC_TERMINAL_H
#include <set>
#include <string>

using namespace std;

class Terminal {
public:
    set<char> t;
    Terminal();
    void inputTer(Terminal&, const string&);
};

#endif //SRC_TERMINAL_H
