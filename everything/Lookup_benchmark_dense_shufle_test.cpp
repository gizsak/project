#include <bits/stdc++.h>
#include "Linearprobing.h"
#include "Quadraticprobing.h"
#include "Chained8.h"
#include "Chained24.h"
#include "Cuckoo.h"
#include "RH.h"
#include <array>
#include "Hashfunction.h"
#include "Allocator.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <string>
#include <benchmark/benchmark.h>

using namespace std;

uint64_t capacity = pow(2, 27);
uint64_t bulkcapacity;
//std::uniform_int_distribution<uint64_t> distrsparse;
//std::random_device rd;
//std::mt19937_64 engkey(rd());
//std::mt19937_64 engvalue(rd());

//std::default_random_engine gen(rd());

pair<Node<uint64_t, uint64_t> *, bool> returnpair;

template <class Table>
static void lookupBench(benchmark::State &s)
{   
   std::random_device rd;
   std::mt19937_64 engkey;
   std::mt19937_64 engvalue;
   std::default_random_engine gen;

    std::vector<uint64_t> insertkeys;
    std::vector<uint64_t> succkeys;
    std::vector<uint64_t> unsucckeys;
    std::vector<uint64_t> lookupkeys;
    std::vector<uint64_t> allkeys;

    uint64_t key = 2;
    uint64_t value = 1;
    double tableloadfactor = 0;
    uint64_t totlookup = 0;
    uint64_t unsucclookup = 0;
    double ratio = 0;
    double loadfactor;
    double range;

    int i = s.range(0);
    int ranges[5] = {8,25, 50, 75,99};
    range = (double)ranges[i] / 100;

    i = s.range(1);
    int loadfactors[6] = {25, 35, 45, 50, 70, 90};
    loadfactor = (double)loadfactors[i] / 100;
    
    std::uniform_int_distribution<uint64_t> distrdense(2,capacity);
   std::uniform_int_distribution<uint64_t> distrdense2;

    while(insertkeys.size()<capacity*loadfactor){
//	key = distrdense(engkey);
    	insertkeys.push_back(key);
	key++;
    }
    uint64_t d = 0;
    while(succkeys.size()<pow(2,25) ){
        succkeys.push_back(insertkeys[d]);
        d++;
    }
    while (ratio < range)
    {   
	key = distrdense2(engkey);

        unsucckeys.push_back(key);
	//key++;
        ratio = (double)unsucckeys.size() /( unsucckeys.size()+ succkeys.size() );
    }


/*
    while(allkeys.size()<capacity){
	allkeys.push_back(distrdense(engkey));
//	key++;
    }
//    std::shuffle(allkeys.begin(), allkeys.end(), engvalue);

    uint64_t indx = 0;
    while (insertkeys.size() < (capacity * loadfactor))
    {
        insertkeys.push_back(allkeys[indx]);
        indx++;
    }
    uint64_t d = 0;
    while (succkeys.size() < pow(2, 23))
    {
        succkeys.push_back(insertkeys[d]);
        d++;
    }
    while (ratio < range)
    {
        unsucckeys.push_back(allkeys[indx]);
        ratio = (double)unsucckeys.size() /( unsucckeys.size()+ succkeys.size() );
        indx++;
    }
*/
    lookupkeys.reserve(succkeys.size() + unsucckeys.size()); // preallocate memory
    lookupkeys.insert(lookupkeys.end(), succkeys.begin(), succkeys.end());
    lookupkeys.insert(lookupkeys.end(), unsucckeys.begin(), unsucckeys.end());

    succkeys.clear();
    unsucckeys.clear();

    //std::shuffle(insertkeys.begin(), insertkeys.end(), engvalue);
    std::shuffle(insertkeys.begin(), insertkeys.end(), engvalue);
    std::shuffle(lookupkeys.begin(), lookupkeys.end(), engvalue);

    if (Table::getName() == "Chained8")
    {
        if (loadfactor == 0.25 || loadfactor == 0.45 )
        {
            bulkcapacity = capacity;
            capacity = capacity / 2;
        }
        else
        {
            bulkcapacity = capacity;
            capacity = capacity;
        }
    }
    if (Table::getName() == "Chained24")
    {
        if (loadfactor == 0.50 || loadfactor == 0.45)
        {
            bulkcapacity = capacity / 4;
            capacity = capacity / 2;
        }
        else
        {
            bulkcapacity = capacity / 4;
            capacity = capacity / 2;
        }
    }
    Table table(capacity, bulkcapacity);
    capacity = pow(2, 27);

    d = 0;
    while (tableloadfactor < loadfactor)
    {
        key = insertkeys[d];
        d++;
        returnpair = table.emplace(key, key);
        if (returnpair.second == true)
        {
            tableloadfactor = (double)table.getSize() / capacity;
//	      tableloadfactor = d / capacity;
        }
    }
    insertkeys.clear();
  d = 0;
    uint64_t *myvector = new uint64_t[lookupkeys.size()];
    while(d<lookupkeys.size()){
        myvector[d] = lookupkeys[d];
        d++;
    }
    uint64_t size = d;
    lookupkeys.clear();
    d = 0;
    uint64_t k = 0;
    for (auto _ : s)
    {
        while (d < size)
        {
            key = myvector[d];
            d++;
            returnpair = table.find(key);
        }
    }
    delete[] myvector;
       if(ratio<0.11){
        ratio = 0;
}
   if(ratio>0.8){
        ratio = 100;
}
    s.counters["Totlookup"] = d;
    s.counters["Loadfactor"] = loadfactor;
    s.counters["UnsuccesRatio"] = ratio;
}
/*
BENCHMARK(lookupBench<Linearprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("LPMult")->Ranges({{0, 2}, {0, 2}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Linearprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("LPMult")->Ranges({{3, 4}, {0, 2}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Linearprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Name("LPMurmur")->Ranges({{0, 2}, {0, 2}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Linearprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Name("LPMurmur")->Ranges({{3, 4}, {0, 2}})->Unit(benchmark::kSecond);

BENCHMARK(lookupBench<Linearprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("LPMult")->Ranges({{0, 2}, {3, 4}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Linearprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("LPMult")->Ranges({{3, 4}, {3, 4}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Linearprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("LPMult")->Ranges({{0, 2}, {5, 5}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Linearprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("LPMult")->Ranges({{3, 4}, {5, 5}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Linearprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Name("LPMurmur")->Ranges({{0, 2}, {3, 4}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Linearprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Name("LPMurmur")->Ranges({{3, 4}, {3, 4}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Linearprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Name("LPMurmur")->Ranges({{0, 2}, {5, 5}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Linearprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Name("LPMurmur")->Ranges({{3, 4}, {5, 5}})->Unit(benchmark::kSecond);
*/
BENCHMARK(lookupBench<Quadraticprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("QPMult")->Ranges({{0, 2}, {3, 4}})->Unit(benchmark::kSecond);
//BENCHMARK(lookupBench<Quadraticprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("QPMult")->Ranges({{3, 4}, {3, 4}})->Unit(benchmark::kSecond);
//BENCHMARK(lookupBench<Quadraticprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("QPMult")->Ranges({{0, 2}, {5, 5}})->Unit(benchmark::kSecond);
//BENCHMARK(lookupBench<Quadraticprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("QPMult")->Ranges({{3, 4}, {5, 5}})->Unit(benchmark::kSecond);
//BENCHMARK(lookupBench<Quadraticprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Name("QPMurmur")->Ranges({{0, 2}, {3, 4}})->Unit(benchmark::kSecond);
//BENCHMARK(lookupBench<Quadraticprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Name("QPMurmur")->Ranges({{3, 4}, {3, 4}})->Unit(benchmark::kSecond);
//BENCHMARK(lookupBench<Quadraticprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Name("QPMurmur")->Ranges({{0, 2}, {5, 5}})->Unit(benchmark::kSecond);
//BENCHMARK(lookupBench<Quadraticprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Name("QPMurmur")->Ranges({{3, 4}, {5, 5}})->Unit(benchmark::kSecond);

BENCHMARK(lookupBench<RH<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("RHMult")->Ranges({{0, 2}, {3, 4}})->Unit(benchmark::kSecond);
//BENCHMARK(lookupBench<RH<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("RHMult")->Ranges({{3, 4}, {3, 4}})->Unit(benchmark::kSecond);
//BENCHMARK(lookupBench<RH<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("RHMult")->Ranges({{0, 2}, {5, 5}})->Unit(benchmark::kSecond);
/*
BENCHMARK(lookupBench<RH<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("RHMult")->Ranges({{3, 4}, {5, 5}})->Unit(benchmark::kSecond);

BENCHMARK(lookupBench<RH<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Name("RHMurmur")->Ranges({{0, 2}, {3, 4}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<RH<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Name("RHMurmur")->Ranges({{3, 4}, {3, 4}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<RH<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Name("RHMurmur")->Ranges({{0, 2}, {5, 5}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<RH<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Name("RHMurmur")->Ranges({{3, 4}, {5, 5}})->Unit(benchmark::kSecond);

BENCHMARK(lookupBench<Chained8<uint64_t, uint64_t, MultiplyShift,Allocator<uint64_t, uint64_t>>>)->Name("Chained8Mult")->Ranges({{0, 2}, {0, 2}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Chained8<uint64_t, uint64_t, MultiplyShift, Allocator<uint64_t, uint64_t>>>)->Name("Chained8Mult")->Ranges({{3, 3}, {0, 2}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Chained8<uint64_t, uint64_t, MultiplyShift, Allocator<uint64_t, uint64_t>>>)->Name("Chained8Mult")->Ranges({{4, 4}, {0, 2}})->Unit(benchmark::kSecond);

BENCHMARK(lookupBench<Chained8<uint64_t, uint64_t, Murmur64, Allocator<uint64_t, uint64_t>>>)->Name("Chained8Murmur")->Ranges({{0, 2}, {0, 2}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Chained8<uint64_t, uint64_t, Murmur64, Allocator<uint64_t, uint64_t>>>)->Name("Chained8Murmur")->Ranges({{3, 3}, {0, 2}})->Unit(benchmark::kSecond);
//BENCHMARK(lookupBench<Chained8<uint64_t, uint64_t, Murmur64, Allocator<uint64_t, uint64_t>>>)->Name("Chained8Murmur")->Ranges({{4, 4}, {0, 2}})->Unit(benchmark::kSecond);


BENCHMARK(lookupBench<Chained24<uint64_t, uint64_t, MultiplyShift, Allocator<uint64_t, uint64_t>>>)->Name("Chained24Mult")->Ranges({{0, 2}, {0, 2}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Chained24<uint64_t, uint64_t, MultiplyShift, Allocator<uint64_t, uint64_t>>>)->Name("Chained24Mult")->Ranges({{3, 3}, {0, 2}})->Unit(benchmark::kSecond);
*/
//BENCHMARK(lookupBench<Chained24<uint64_t, uint64_t, MultiplyShift, Allocator<uint64_t, uint64_t>>>)->Name("Chained24Mult")->Ranges({{4, 4}, {0, 2}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Chained24<uint64_t, uint64_t, Murmur64, Allocator<uint64_t, uint64_t>>>)->Name("Chained24Murmur")->Ranges({{0, 2}, {1, 2}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Chained24<uint64_t, uint64_t, Murmur64, Allocator<uint64_t, uint64_t>>>)->Name("Chained24Murmur")->Ranges({{3, 3}, {0, 2}})->Unit(benchmark::kSecond);
//BENCHMARK(lookupBench<Chained24<uint64_t, uint64_t, Murmur64, Allocator<uint64_t, uint64_t>>>)->Name("Chained24Murmur")->Ranges({{4, 4}, {0, 2}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Chained24<uint64_t, uint64_t, MultiplyShift, Allocator<uint64_t, uint64_t>>>)->Name("Chained24Mult")->Ranges({{0, 2}, {3, 3}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Chained24<uint64_t, uint64_t, MultiplyShift, Allocator<uint64_t, uint64_t>>>)->Name("Chained24Mult")->Ranges({{3, 3}, {3, 3}})->Unit(benchmark::kSecond);
//BENCHMARK(lookupBench<Chained24<uint64_t, uint64_t, MultiplyShift, Allocator<uint64_t, uint64_t>>>)->Name("Chained24Mult")->Ranges({{4, 4}, {3, 3}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Chained24<uint64_t, uint64_t, Murmur64, Allocator<uint64_t, uint64_t>>>)->Name("Chained24Murmur")->Ranges({{0, 2}, {3, 3}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Chained24<uint64_t, uint64_t, Murmur64, Allocator<uint64_t, uint64_t>>>)->Name("Chained24Murmur")->Ranges({{3, 3}, {3, 3}})->Unit(benchmark::kSecond);
//BENCHMARK(lookupBench<Chained24<uint64_t, uint64_t, Murmur64, Allocator<uint64_t, uint64_t>>>)->Name("Chained24Murmur")->Ranges({{4, 4}, {3, 3}})->Unit(benchmark::kSecond);

BENCHMARK_MAIN();

