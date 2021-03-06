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

uint64_t capacity = pow(2, 27);
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
        insertkeys.push_back(key);
        key++;
    }

    while (extrakeys.size() < capacity * 0.01)
    {
        extrakeys.push_back(key);
        key++;
    }

    totinsertkeys.reserve(extrakeys.size());
    // totinsertkeys.insert(totinsertkeys.end(), insertkeys.begin(), insertkeys.end());
    totinsertkeys.insert(totinsertkeys.end(), extrakeys.begin(), extrakeys.end());

    extrakeys.clear();
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

    std::shuffle(insertkeys.begin(), insertkeys.end(), engvalue);
    // std::shuffle(extrakeys.begin(), extrakeys.end(), engvalue);
    std::shuffle(totinsertkeys.begin(), totinsertkeys.end(), engvalue);

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
    uint64_t *insertarray = new uint64_t[totinsertkeys.size()];
    while (index < totinsertkeys.size())
    {
        insertarray[index] = totinsertkeys[index];
        index++;
    }
    uint64_t size = index;
    totinsertkeys.clear();

    index = 0;
    uint64_t stab = 0;
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

/*
BENCHMARK(insertionBench<Linearprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("LPMult")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Linearprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("LPMult")->DenseRange(3, 4)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Linearprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("LPMult")->DenseRange(5, 5)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<Linearprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Name("LPMurmur")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Linearprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Name("LPMurmur")->DenseRange(3, 4)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Linearprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Name("LPMurmur")->DenseRange(5, 5)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<Quadraticprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("QPMult")->DenseRange(3, 4)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Quadraticprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("QPMult")->DenseRange(5, 5)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<Quadraticprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Name("QPMurmur")->DenseRange(3, 4)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Quadraticprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Name("QPMurmur")->DenseRange(5, 5)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<RH<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("RHMult")->DenseRange(3, 4)->Unit(benchmark::kSecond);
//BENCHMARK(insertionBench<RH<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("RHMult")->DenseRange(5, 5)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<RH<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Name("RHMurmur")->DenseRange(3, 4)->Unit(benchmark::kSecond);
//BENCHMARK(insertionBench<RH<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Name("RHMurmur")->DenseRange(5, 5)->Unit(benchmark::kSecond);

//BENCHMARK(insertionBench<Chained8<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("Chained8Mult")->DenseRange(0, 2)->Unit(benchmark::kSecond);
//BENCHMARK(insertionBench<Chained8<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Name("Chained8Murmur")->DenseRange(0, 2)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<Chained24<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("Chained24Mult")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained24<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Name("Chained24Murmur")->DenseRange(0, 2)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<Linearprobing<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("LPMultAdd")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Linearprobing<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("LPMultAdd")->DenseRange(3, 4)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Linearprobing<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("LPMultAdd")->DenseRange(5, 5)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<Linearprobing<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Name("LPTab")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Linearprobing<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Name("LPTab")->DenseRange(3, 4)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Linearprobing<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Name("LPTab")->DenseRange(5, 5)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<Quadraticprobing<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("QPMultAdd")->DenseRange(3, 4)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Quadraticprobing<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("QPMultAdd")->DenseRange(5, 5)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<Quadraticprobing<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Name("QPTab")->DenseRange(3, 4)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Quadraticprobing<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Name("QPTab")->DenseRange(5, 5)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<RH<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("RHMultAdd")->DenseRange(3, 4)->Unit(benchmark::kSecond);
//BENCHMARK(insertionBench<RH<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("RHMultAdd")->DenseRange(5, 5)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<RH<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Name("RHTab")->DenseRange(3, 4)->Unit(benchmark::kSecond);
//BENCHMARK(insertionBench<RH<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Name("RHTab")->DenseRange(5, 5)->Unit(benchmark::kSecond);

//BENCHMARK(insertionBench<Chained8<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("Chained8MultAdd")->DenseRange(0, 2)->Unit(benchmark::kSecond);

//BENCHMARK(insertionBench<Chained8<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Name("Chained8Tab")->DenseRange(0, 2)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<Chained24<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("Chained24MultAdd")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained24<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Name("Chained24Tab")->DenseRange(0, 2)->Unit(benchmark::kSecond);


BENCHMARK(insertionBench<Chained8<uint64_t, uint64_t, MultiplyShift, Allocator<uint64_t, uint64_t>>>)->Name("AChained8Mult")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained8<uint64_t, uint64_t, Murmur64, Allocator<uint64_t, uint64_t>>>)->Name("AChained8Murmur")->DenseRange(0, 2)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<Chained24<uint64_t, uint64_t, MultiplyShift, Allocator<uint64_t, uint64_t>>>)->Name("AChained24Mult")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained24<uint64_t, uint64_t, Murmur64, Allocator<uint64_t, uint64_t>>>)->Name("AChained24Murmur")->DenseRange(0, 2)->Unit(benchmark::kSecond);
//BENCHMARK(insertionBench<MCuckoo<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("CuckooMult")->DenseRange(3, 4)->Unit(benchmark::kSecond);

//BENCHMARK(insertionBench<MCuckoo<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("CuckooMultAdd")->DenseRange(3, 4)->Unit(benchmark::kSecond);
//BENCHMARK(insertionBench<MCuckoo<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Name("CuckooTab")->DenseRange(3, 4)->Unit(benchmark::kSecond);
*/

BENCHMARK(insertionBench<RH<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("RHMult")->DenseRange(5, 5)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<RH<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("RHMurmur")->DenseRange(5, 5)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<RH<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("RHMAdd")->DenseRange(5, 5)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<RH<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("RHTab")->DenseRange(5, 5)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<Chained8<uint64_t, uint64_t, MultiplyShift, Allocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained8Mult")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained8<uint64_t, uint64_t, Murmur64, Allocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained8Murmur")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained24<uint64_t, uint64_t, MultiplyShift, Allocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained24Mult")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained24<uint64_t, uint64_t, Murmur64, Allocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained24Murmur")->DenseRange(0, 2)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<Chained8<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained8MAdd")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained8<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained8Tab")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained24<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained24MAdd")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained24<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained24Tab")->DenseRange(0, 2)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<Chained8<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("BulkCh8Mult")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained8<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("BulkCh8Murmur")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained24<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("BulkCh4Mult")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained24<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("BulkCh24Murmur")->DenseRange(0, 2)->Unit(benchmark::kSecond);

//BENCHMARK(insertionBench<MCuckoo<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("CuckooMult")->DenseRange(3, 5)->Unit(benchmark::kSecond);
//BENCHMARK(insertionBench<MCuckoo<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("CuckooMAdd")->DenseRange(3, 5)->Unit(benchmark::kSecond);
//BENCHMARK(insertionBench<MCuckoo<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("CuckooTab")->DenseRange(3, 5)->Unit(benchmark::kSecond);

BENCHMARK_MAIN();

