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

pair<Node<uint64_t, uint64_t> *, bool> returnpair;

template <class Table>
static void insertionBench(benchmark::State &s)
{
    std::uniform_int_distribution<uint64_t> distrsparse;
    std::mt19937_64 engkey;
    std::mt19937_64 engvalue;

    std::vector<uint64_t> extrakeys;
    std::vector<uint64_t> insertkeys;
    std::vector<uint64_t> totinsertkeys;

    uint64_t key = 1;
    uint64_t value = 1;
    uint64_t totinsert = 0;
    double tableloadfactor = 0;
    uint64_t succinsert = 0;

    uint64_t index = 0;

    int i = s.range(0);
    int ranges[6] = {25, 35, 45, 50, 70, 90};
    double loadfactor = (double)ranges[i] / 100;

    while (insertkeys.size() < capacity * loadfactor)
    {
        key = distrsparse(engkey);
        insertkeys.push_back(key);
    }
   
    while (extrakeys.size() < capacity * 0.01)
    {
        key = distrsparse(engkey);
        extrakeys.push_back(key);
    }
    

    if (Table::getName() == "Chained8")
    {
        if (loadfactor == 0.45)
        {
            bulkcapacity = capacity / 2;
            capacity = capacity / 2;
        }
        else
        {
            bulkcapacity = capacity / 2;
            capacity = capacity;
        }
    }
    if (Table::getName() == "Chained24")
    {
       
            bulkcapacity = capacity / 4;
            capacity = capacity / 2;
    }
    Table table(capacity, bulkcapacity);
    capacity = pow(2, 24);

    std::shuffle(insertkeys.begin(), insertkeys.end(), engvalue);
    std::shuffle(extrakeys.begin(), extrakeys.end(), engvalue);

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
    uint64_t *insertarray = new uint64_t[extrakeys.size()];
    while (index < extrakeys.size())
    {
        extrakeys[index] = extrakeys[index];
        index++;
    }
    uint64_t size = index;
    extrakeys.clear();

    index = 0;

    for (auto _ : s)
    {
        while (index < size)
        {
            key = insertarray[index];
            index++;
            returnpair = table.emplace(key, key);
            if (returnpair.second == true)
            {
                succinsert++;
            }
        }
    }
    delete[] insertarray;
    s.counters["Totinsert"] = index;
    s.counters["Loadfactor"] = loadfactor;
}

BENCHMARK(insertionBench<Linearprobing<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("LPMAdd")->DenseRange(0, 5)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Linearprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("LPMult")->DenseRange(0, 5)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Linearprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("LPMurmur")->DenseRange(0, 5)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Linearprobing<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("LPTab")->DenseRange(0, 5)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<Chained8<uint64_t, uint64_t, MultiplyAddShift, Allocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained8MAdd")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained8<uint64_t, uint64_t, MultiplyShift, Allocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained8Mult")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained8<uint64_t, uint64_t, Murmur64, Allocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained8Murmur")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained8<uint64_t, uint64_t, TabulationHashing, Allocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained8Tab")->DenseRange(0, 2)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<Chained24<uint64_t, uint64_t, MultiplyAddShift, Allocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained24MAdd")->DenseRange(0, 3)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained24<uint64_t, uint64_t, MultiplyShift, Allocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained24Mult")->DenseRange(0, 3)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained24<uint64_t, uint64_t, Murmur64, Allocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained24Murmur")->DenseRange(0, 3)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained24<uint64_t, uint64_t, TabulationHashing, Allocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained24Tab")->DenseRange(0, 3)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<Chained8<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("BulkChained8MAdd")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained8<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("BulkChained8Mult")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained8<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("BulkChained8Murmur")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained8<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("BulkChained8Tab")->DenseRange(0, 2)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<Chained24<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("BulkChained24MAdd")->DenseRange(0, 3)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained24<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("BulkChained24Mult")->DenseRange(0, 3)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained24<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("BulkChained24Murmur")->DenseRange(0, 3)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained24<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("BulkChained24Tab")->DenseRange(0, 3)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<Quadraticprobing<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("QPMAddH")->DenseRange(3, 5)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Quadraticprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("QPMultH")->DenseRange(3, 5)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Quadraticprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("QPMurmurH")->DenseRange(3, 5)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Quadraticprobing<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("QPTabH")->DenseRange(3, 5)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<RH<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("RHMAdd1")->DenseRange(3, 5)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<RH<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("RHMult1")->DenseRange(3, 5)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<RH<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("RHMurmur1")->DenseRange(3, 5)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<RH<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("RHTab1")->DenseRange(3, 5)->Unit(benchmark::kSecond);

//BENCHMARK(insertionBench<MCuckoo<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("CuckooMAdd")->DenseRange(3, 5)->Unit(benchmark::kSecond);
//BENCHMARK(insertionBench<MCuckoo<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("CuckooMult")->DenseRange(3, 5)->Unit(benchmark::kSecond);
//BENCHMARK(insertionBench<MCuckoo<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("CuckooTab")->DenseRange(3, 5)->Unit(benchmark::kSecond);
BENCHMARK_MAIN();

