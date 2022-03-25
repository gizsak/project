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
std::uniform_int_distribution<uint64_t> distrdense(1,pow(2,30));
std::random_device rd;
std::mt19937_64 engkey(rd());
std::mt19937_64 engvalue(rd());

std::default_random_engine gen(rd());

pair<Node<uint64_t, uint64_t> *, bool> returnpair;

template <class Table>
static void lookupBench(benchmark::State &s)
{   
    std::vector<uint64_t> myvector;
    uint64_t key = 1;
    uint64_t value = 1;
    double tableloadfactor = 0;
    uint64_t totlookup = 0;
    uint64_t unsucclookup = 0;
    double ratio = 0;
    double loadfactor;
    double range;

    int i = s.range(0);
    int ranges[3] = {25, 50, 75};
    range = (double)ranges[i] / 100;

    i = s.range(1);
    int loadfactors[6] = {25, 35, 45, 50, 70, 90};
    loadfactor = (double)loadfactors[i] / 100;

    if (Table::getName() == "Chained8")
    {
        if (loadfactor == 0.25)
        {
            bulkcapacity = capacity;
            capacity = capacity/2;
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

    while (tableloadfactor < loadfactor)
    {
        key = distrdense(engkey);
        value = distrdense(engvalue);
        returnpair = table.emplace(key, value);
        if (returnpair.second == true)
        {
            tableloadfactor = (double)table.getSize() / capacity;
            if (myvector.size() < pow(2, 23))
            {
                myvector.push_back(key);
                totlookup++;
            }
        }
    }
    while (ratio < range)
        {
            key = distrdense(engkey);
            returnpair = table.find(key);
            if (returnpair.second == false)
            {
                myvector.push_back(key);
                totlookup++;
                unsucclookup++;
                ratio = (double)unsucclookup / totlookup;
            }
        }
    std::shuffle (myvector.begin(), myvector.end(), gen);
//	totlookup = 0;
//	unsucclookup = 0;
    for (auto _ : s)
    {
        for(uint64_t k : myvector)
        {
//		table.find(k);	
	returnpair =  table.find(k);
//	totlookup++;
//        if (returnpair.second == false)
//            unsucclookup++;
        }
    }
	myvector.clear();
//double   r = (double)unsucclookup / totlookup;
    s.counters["Totlookup"] = totlookup;
    s.counters["Loadfactor"] = loadfactor;
    s.counters["UnsuccesRatio"] = ratio;
    
//    cout << totlookup << endl;
//    cout<<unsucclookup<<endl;
	cout<<myvector.size();
}

BENCHMARK(lookupBench<Linearprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {0, 5}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Linearprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {0, 5}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Quadraticprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {3, 5}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Quadraticprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {3, 5}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<RH<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {3, 5}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<RH<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {3, 5}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Cuckoo<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {3, 5}})->Unit(benchmark::kSecond);
// BENCHMARK(lookupBench<Cuckoo<uint64_t ,uint64_t,Murmur64,BulkAllocator<uint64_t,uint64_t>>>)->Ranges({{0,2},{5,5}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Chained8<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {0, 2}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Chained8<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {0, 2}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Chained24<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {0, 2}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Chained24<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {0, 2}})->Unit(benchmark::kSecond);

BENCHMARK_MAIN();
