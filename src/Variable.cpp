#include "Variable.h"

Variable::Variable() = default;

void Variable::inputVar(char& S, const string& var) {
    bool i = true;
    for (char c : var) {
        if (c >= 'A' && c <= 'Z') {
            if (i) {
                S = c;
                i = false;
            }
            this->v.insert(c);
        }
    }
}