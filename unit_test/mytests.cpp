#include <bits/stdc++.h>

#include "../src/Linearprobing.h"
#include "../src/Quadraticprobing.h"
#include "../src/Chained8.h"
#include "../src/Chained24.h"
#include "../src/Cuckoo.h"
#include "../src/RH.h"
#include <array>
#include "../src/HashFunction.h"
#include "../src/Allocator.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <string>

#include <gtest/gtest.h>
#include "mytests.h"

using namespace std;




map<uint64_t, uint64_t> elements;
map<uint64_t, uint64_t> deletedelements;
map<uint64_t, uint64_t> remainedelements;
std::uniform_int_distribution<uint64_t> distr;
std::random_device rd;
std::mt19937_64 engkey(rd());
std::mt19937_64 engvalue(rd());
uint64_t capacity = pow(2,16);
uint64_t key;
uint64_t value;
pair<Node<uint64_t, uint64_t> *, bool> returnpair;
pair<Cnode<uint64_t, uint64_t> *, bool> chainedreturnpair;


void populate()
{
    while (elements.size() < pow(2, 15))
    {
        key = distr(engkey);
        value = distr(engvalue);
        if (key != 0)
            elements.emplace(key, value);
    }
}
void populate2()
{
    for (auto const &pair : elements)
    {
        key = pair.first;
        value = pair.second;
        deletedelements.insert(*(elements.find(key)));
        if (deletedelements.size() == pow(2, 14))
            break;
    }
    for (auto const &pair : deletedelements)
    {
        key = pair.first;
        elements.erase(elements.find(key));
    }
    remainedelements = elements;
}
void cleardata()
{
    elements.clear();
    deletedelements.clear();
    remainedelements.clear();
}
 
int main(int argc, char *argv[])
{
   

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
