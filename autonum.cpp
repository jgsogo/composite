#include "autonum.hpp"

static int nItem = 0;

AutoNum::AutoNum() {
    numId = nItem++;
}