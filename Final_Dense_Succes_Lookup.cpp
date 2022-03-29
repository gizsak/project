#include <bits/stdc++.h>
#include "Linearprobing.h"
#include "Quadraticprobing.h"
#include "Chained8.h"
#include "Chained24.h"
#include "MCuckoo.h"
#include "RH.h"
#include <array>
#include "HashFunction.h"
#include "Allocator.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <string>
#include <benchmark/benchmark.h>

using namespace std;

uint64_t capacity = pow(2, 24);
uint64_t bulkcapacity;

template <class Table>
static void lookupBenchSucces(benchmark::State &s)
{
    std::uniform_int_distribution<uint64_t> distrsparse;
    std::mt19937_64 engkey;
    std::mt19937_64 engvalue;

    std::vector<uint64_t> insertkeys;
    std::vector<uint64_t> succkeys;

    pair<Node<uint64_t, uint64_t> *, bool> returnpair;
    int i = s.range(0);
    int ranges[6] = {25, 35, 45, 50, 70, 90};
    double loadfactor = (double)ranges[i] / 100;
    uint64_t succkeysnumber = capacity*loadfactor/4;

    uint64_t key = 1;
    uint64_t value = 1;
    double tableloadfactor = 0;
    uint64_t unsucclookup = 0;
    uint64_t index = 0;
    while (insertkeys.size() < (capacity * loadfactor))
    {
        insertkeys.push_back(key);
        key++;
    }
    index = 0;
    while(succkeys.size()<succkeysnumber){
        succkeys.push_back(insertkeys[index]);
        index++;
    }
    std::shuffle(insertkeys.begin(), insertkeys.end(), engvalue);
    std::shuffle(succkeys.begin(), succkeys.end(), engvalue);

    if (Table::getName() == "Chained8")
    {
        if (loadfactor  == 0.45)
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
       
            bulkcapacity = capacity / 4;
            capacity = capacity / 4;
    }

    Table table(capacity, bulkcapacity);
    capacity = pow(2, 24);

    index = 0;

    while (tableloadfactor < loadfactor)
    {
        key = insertkeys[index];
        index++;
        returnpair = table.emplace(key, key);
        if (returnpair.second == true)
        {
            tableloadfactor = (double)table.getSize() / capacity;
        }
    }
    insertkeys.clear();
    index = 0;
    uint64_t *lookuparray = new uint64_t[succkeys.size()];
    while (index < succkeys.size())
    {
        lookuparray[index] = succkeys[index];
        index++;
    }
    uint64_t size = index;
    succkeys.clear();
    index = 0;

    for (auto _ : s)
    {
        while (index < size)
        {
            key = lookuparray[index];
            index++;
            returnpair = table.find(key);
            if (returnpair.second == false)
            {
                unsucclookup++;
            }
        }
    }
    delete[] lookuparray;
    s.counters["Totlookup"] = index;
    s.counters["Loadfactor"] = loadfactor;
    s.counters["UnsuccesRatio"] = (double)unsucclookup / index;
}
/*
BENCHMARK(lookupBenchSucces<Linearprobing<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("LPMAdd")->DenseRange(0, 5)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchSucces<Linearprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("LPMult")->DenseRange(0, 5)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchSucces<Linearprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("LPMurmur")->DenseRange(0, 5)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchSucces<Linearprobing<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("LPTab")->DenseRange(0, 5)->Unit(benchmark::kSecond);

BENCHMARK(lookupBenchSucces<Chained8<uint64_t, uint64_t, MultiplyAddShift, Allocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained8MAdd")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchSucces<Chained8<uint64_t, uint64_t, MultiplyShift, Allocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained8Mult")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchSucces<Chained8<uint64_t, uint64_t, Murmur64, Allocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained8Murmur")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchSucces<Chained8<uint64_t, uint64_t, TabulationHashing, Allocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained8Tab")->DenseRange(0, 2)->Unit(benchmark::kSecond);

BENCHMARK(lookupBenchSucces<Chained24<uint64_t, uint64_t, MultiplyAddShift, Allocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained24MAdd")->DenseRange(0, 3)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchSucces<Chained24<uint64_t, uint64_t, MultiplyShift, Allocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained24Mult")->DenseRange(0, 3)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchSucces<Chained24<uint64_t, uint64_t, Murmur64, Allocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained24Murmur")->DenseRange(0, 3)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchSucces<Chained24<uint64_t, uint64_t, TabulationHashing, Allocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained24Tab")->DenseRange(0, 3)->Unit(benchmark::kSecond);

BENCHMARK(lookupBenchSucces<Quadraticprobing<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("QPMAddH")->DenseRange(3, 5)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchSucces<Quadraticprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("QPMultH")->DenseRange(3, 5)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchSucces<Quadraticprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("QPMurmurH")->DenseRange(3, 5)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchSucces<Quadraticprobing<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("QPTabH")->DenseRange(3, 5)->Unit(benchmark::kSecond);

BENCHMARK(lookupBenchSucces<RH<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("RHMAdd1")->DenseRange(3, 5)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchSucces<RH<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("RHMult1")->DenseRange(3, 5)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchSucces<RH<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("RHMurmur1")->DenseRange(3, 5)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchSucces<RH<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("RHTab1")->DenseRange(3, 5)->Unit(benchmark::kSecond);
*/
BENCHMARK(lookupBench<Quadraticprobing<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("QPMAddT")->Ranges({{0, 2}, {3, 4}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Quadraticprobing<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("QPMAddT")->Ranges({{0, 2}, {5, 5}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Quadraticprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("QPMultT")->Ranges({{0, 2}, {3, 4}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Quadraticprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("QPMultT")->Ranges({{0, 2}, {5, 5}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Quadraticprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("QPMurmurT")->Ranges({{0, 2}, {3, 4}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Quadraticprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("QPMurmurT")->Ranges({{0, 2}, {5, 5}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Quadraticprobing<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("QPTabT")->Ranges({{0, 2}, {3, 4}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<Quadraticprobing<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("QPTabT")->Ranges({{0, 2}, {5, 5}})->Unit(benchmark::kSecond);

BENCHMARK(lookupBench<RH<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("RHMAdd2")->Ranges({{0, 2}, {3, 4}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<RH<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("RHMAdd2")->Ranges({{0, 2}, {5, 5}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<RH<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("RHMult2")->Ranges({{0, 2}, {3, 4}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<RH<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("RHMult2")->Ranges({{0, 2}, {5, 5}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<RH<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("RHMurmur2")->Ranges({{0, 2}, {3, 4}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<RH<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("RHMurmur2")->Ranges({{0, 2}, {5, 5}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<RH<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("RHTab2")->Ranges({{0, 2}, {3, 4}})->Unit(benchmark::kSecond);
BENCHMARK(lookupBench<RH<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("RHTab2")->Ranges({{0, 2}, {5, 5}})->Unit(benchmark::kSecond);

//BENCHMARK(lookupBenchSucces<MCuckoo<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("CuckooMAdd")->DenseRange(3, 5)->Unit(benchmark::kSecond);
//BENCHMARK(lookupBenchSucces<MCuckoo<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("CuckooMult")->DenseRange(3, 5)->Unit(benchmark::kSecond);
//BENCHMARK(lookupBenchSucces<MCuckoo<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("CuckooTab")->DenseRange(3, 5)->Unit(benchmark::kSecond);


BENCHMARK_MAIN();

