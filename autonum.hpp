#pragma once

#include <string>
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
