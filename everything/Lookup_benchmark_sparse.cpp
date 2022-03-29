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
std::uniform_int_distribution<uint64_t> distrsparse;
std::random_device rd;
std::mt19937_64 engkey(rd());
std::mt19937_64 engvalue(rd());

template <class Table>
static void lookupBench(benchmark::State &s)
{
    double loadfactor;
    pair<Node<uint64_t, uint64_t> *, bool> returnpair;

    int i = s.range(0);
    int ranges[3] = {25, 50, 75};
    double range = (double)ranges[i] / 100;

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

    uint64_t key = 1;
    uint64_t value = 1;
    double tableloadfactor = 0;
    uint64_t totlookup = 0;
    uint64_t unsucclookup = 0;
    uint64_t cap = pow(2, 23);
    uint64_t *containedkeys = new uint64_t[cap];
    uint64_t containedkeycapacity = 0;
    uint64_t c = 0;
    double ratio = 0;

    while (tableloadfactor < loadfactor)
    {
        key = distrsparse(engkey);
        value = distrsparse(engvalue);
        returnpair = table.emplace(key, value);
        if (returnpair.second == true)
        {
            tableloadfactor = (double)table.getSize() / capacity;
            if (containedkeycapacity < pow(2, 23))
            {
                containedkeys[containedkeycapacity] = key;
                containedkeycapacity++;
            }
        }
    }

    for (auto _ : s)
    {
        while (c < pow(2, 23))
        {
            key = containedkeys[c];
            returnpair = table.find(key);
            c++;
            totlookup++;
            if (returnpair.second == false)
            {
                unsucclookup++;
            }
            ratio = (double)unsucclookup / totlookup;
        }
        while (ratio < range)
        {
            key = distrsparse(engkey);
            returnpair = table.find(key);
            totlookup++;
            if (returnpair.second == false)
            {
                unsucclookup++;
                ratio = (double)unsucclookup / totlookup;
            }
        }
        s.counters["Totlookup"] = totlookup;
        s.counters["Loadfactor"] = loadfactor;
        s.counters["UnsuccesRatio"] = ratio;
    }
    delete[] containedkeys;
    cout << totlookup << endl;
    cout<<unsucclookup<<endl;
}

BENCHMARK(lookupBench<Linearprobing<uint64_t ,uint64_t,MultiplyShift,BulkAllocator<uint64_t,uint64_t>>>)->Ranges({{0,2},{3,3}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Linearprobing<uint64_t ,uint64_t,Murmur64,BulkAllocator<uint64_t,uint64_t>>>)->Ranges({{0,2},{3,3}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Quadraticprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {3, 3}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Quadraticprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {3, 3}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<RH<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {3, 3}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<RH<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {3, 3}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Cuckoo<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {3, 3}})->Unit(benchmark::kSecond);
//BENCHMARK(lookupBench<Cuckoo<uint64_t ,uint64_t,Murmur64,BulkAllocator<uint64_t,uint64_t>>>)->DenseRange(3,5)->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Chained8<uint64_t ,uint64_t,MultiplyShift,BulkAllocator<uint64_t,uint64_t>>>)->Ranges({{0,2},{0,2}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Chained8<uint64_t ,uint64_t,Murmur64,BulkAllocator<uint64_t,uint64_t>>>)->Ranges({{0,2},{0,2}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Chained24<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {0, 2}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Chained24<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Ranges({{0, 2}, {0, 2}})->Unit(benchmark::kSecond);

BENCHMARK_MAIN();
