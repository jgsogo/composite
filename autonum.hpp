#pragma once

#include <string>
#include <iostream>
#include "src/trait.hpp"


/*
 * Try to add something just to parts
 * */

class AutoNum {
public:
    explicit AutoNum();

public:
    int numId;
};


using AutoNumTrait = composite::Trait<AutoNum, AutoNum>;

void onPartAdded(const std::vector<std::reference_wrapper<AutoNum>> &, AutoNum &);
