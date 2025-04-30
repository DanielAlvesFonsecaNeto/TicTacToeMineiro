#include "No.hpp"
#include <algorithm>

No::No(No* pai) : pai(pai) {}

No::~No() {
    for (No* filho : filhos) {
        delete filho;
    }
}

void deletaNo(No* no) {
    if (!no) return;

    if (no->pai) {
        auto& filhos = no->pai->filhos;
        auto it = std::find(filhos.begin(), filhos.end(), no);
        if (it != filhos.end()) filhos.erase(it);
    }

    delete no;
}