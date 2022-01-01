#pragma once

#include <string>
#include <iostream>
#include "trait.hpp"


/*
 * Try to add something just to parts
 * */

class AutoNum {
public:
    explicit AutoNum();

public:
    int numId;
};


using AutoNumTrait = Trait<AutoNum, AutoNum>;

inline auto foo(std::vector<AutoNum>&, AutoNum&) {
    std::cout << "AutoNum --- la caña de españa" << std::endl;
}
