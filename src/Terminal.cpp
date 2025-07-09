#include "Terminal.h"

Terminal::Terminal() = default;

void inputTer(Terminal& terminal, const string& ter) {
    for (char c : ter) {
        if (c >= 'a' && c <= 'z')
            terminal.t.insert(c);
    }
    terminal.t.insert('@');
}