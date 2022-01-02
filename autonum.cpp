#include "autonum.hpp"

static int nItem = 0;

AutoNum::AutoNum() {
    numId = nItem++;
}

void foo(const std::vector<std::reference_wrapper<AutoNum>> &, AutoNum &) {
    std::cout << "AutoNum --- la caña de españa" << std::endl;
}