#include "Terminal.h"

Terminal::Terminal() = default;

void Terminal::inputTer(const string& ter) {
    for (char c : ter) {
        if (c >= 'a' && c <= 'z')
            this->t.insert(c);
    }
    this->t.insert('@');
}