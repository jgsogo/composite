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


void foo(const std::vector<std::reference_wrapper<AutoNum>> &, AutoNum &);
