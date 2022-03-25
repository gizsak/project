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

uint64_t capacity = pow(2, 30);
uint64_t bulkcapacity;

template <class Table>
static void lookupBench(benchmark::State &s)
{
    std::uniform_int_distribution<uint64_t> distrsparse;
    std::random_device rd;
    std::mt19937_64 engkey;
    std::mt19937_64 engvalue;

    std::vector<uint64_t> allkeys;
    std::vector<uint64_t> insertkeys;
    std::vector<uint64_t> succkeys;
    std::vector<uint64_t> unsucckeys;
    std::vector<uint64_t> lookupkeys;

    uint64_t key = 1;
    uint64_t value = 1;
    double tableloadfactor = 0;
    uint64_t unsucclookup = 0;
    double ratio = 0;
    double loadfactor;
    double range;
    pair<Node<uint64_t, uint64_t> *, bool> returnpair;

    uint64_t indx = 0;
    uint64_t succkeysnumber = capacity/128;

    int i = s.range(0);
    int ranges[5] = {3, 25, 50, 75, 98};
    range = (double)ranges[i] / 100;

    i = s.range(1);
    int loadfactors[6] = {25, 35, 45, 50, 70, 90};
    loadfactor = (double)loadfactors[i] / 100;

    while (allkeys.size() < pow(2, 30))
    {
        key = distrsparse(engkey);
        allkeys.push_back(key);
    }
    std::shuffle(allkeys.begin(), allkeys.end(), engvalue);
    
    indx=0;
    while (insertkeys.size() < (capacity * loadfactor))
    {
        key = allkeys[indx];
        insertkeys.push_back(key);
        indx++;
    }

  //  std::shuffle(insertkeys.begin(), insertkeys.end(), engvalue);
    indx=0;
    while (succkeys.size() < succkeysnumber)
    {
        succkeys.push_back(insertkeys[indx]);
        indx++;
    }

    indx=capacity * loadfactor;
    while (ratio < range)
    {   
        key = allkeys[indx];
        unsucckeys.push_back(key);
        ratio = (double) unsucckeys.size() / (succkeys.size() + unsucckeys.size());
        indx++;
    }

    lookupkeys.reserve(succkeys.size() + unsucckeys.size()); 
    lookupkeys.insert(lookupkeys.end(), succkeys.begin(), succkeys.end());
    lookupkeys.insert(lookupkeys.end(), unsucckeys.begin(), unsucckeys.end());

    std::shuffle(insertkeys.begin(), insertkeys.end(), engvalue);
    std::shuffle(lookupkeys.begin(), lookupkeys.end(), engvalue);
    allkeys.clear();
    succkeys.clear();
    unsucckeys.clear();

    if (Table::getName() == "Chained8")
    {
        if (loadfactor == 0.25 || loadfactor == 0.45)
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
    capacity = pow(2, 30);

    indx = 0;
    while (tableloadfactor < loadfactor)
    {
        key = insertkeys[indx];
        indx++;
        returnpair = table.emplace(key, key);
        if (returnpair.second == true)
        {
            tableloadfactor = (double)table.getSize() / capacity;
        }
    }

    insertkeys.clear();
    
    indx = 0;
    
    uint64_t *myvector = new uint64_t[lookupkeys.size()];

    while (indx < lookupkeys.size())
    {
        myvector[indx] = lookupkeys[indx];
        indx++;
    }
    
    indx = 0;
    double testratio = 0 ;
    uint64_t lookupsize = lookupkeys.size();
    lookupkeys.clear();

    for (auto _ : s)
    {
        while (indx < lookupsize)
        {
            key = myvector[indx];
            indx++;
            returnpair = table.find(key);
/*		if(returnpair.second==false){
		unsucclookup++;
	}
*/        }
    }

   delete[] myvector;
    testratio = (double) unsucclookup/indx;
    cout<<testratio<<endl;
    s.counters["Totlookup"] = lookupsize;
    s.counters["Loadfactor"] = loadfactor;
    s.counters["UnsuccesRatio"] = ratio;
}

BENCHMARK(lookupBench<Linearprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {1, 1}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Linearprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{3, 4}, {1, 1}})->Unit(benchmark::kSecond);
//BENCHMARK(lookupBench<Linearprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {1, 1}})->Unit(benchmark::kSecond);
//BENCHMARK(lookupBench<Linearprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{3, 4}, {1, 1}})->Unit(benchmark::kSecond);

BENCHMARK(lookupBench<Chained8<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {1, 1}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Chained8<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{3, 4}, {1, 1}})->Unit(benchmark::kSecond);
//BENCHMARK(lookupBench<Chained8<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {1, 1}})->Unit(benchmark::kSecond);
//BENCHMARK(lookupBench<Chained8<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{3, 4}, {1, 1}})->Unit(benchmark::kSecond);

BENCHMARK(lookupBench<Chained24<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {1, 1}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Chained24<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{3, 4}, {1, 1}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Chained24<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {1, 1}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Chained24<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{3, 4}, {1, 1}})->Unit(benchmark::kSecond);

/*
BENCHMARK(lookupBench<Quadraticprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {3, 3}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Quadraticprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {3, 3}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<RH<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {3, 3}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<RH<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {3, 3}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Cuckoo<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {3, 3}})->Unit(benchmark::kSecond);
// BENCHMARK(lookupBench<Cuckoo<uint64_t ,uint64_t,Murmur64,BulkAllocator<uint64_t,uint64_t>>>)->DenseRange(3,5)->Unit(benchmark::kSecond);
*/



BENCHMARK_MAIN();

