#include <bits/stdc++.h>
#include "../src/Linearprobing.h"
#include "../src/Quadraticprobing.h"
#include "../src/Chained8.h"
#include "../src/Chained24.h"
#include "../src/MCuckoo.h"
#include "../src/RH.h"
#include <array>
#include "../src/HashFunction.h"
#include "../src/Allocator.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <string>
#include <benchmark/benchmark.h>

using namespace std;

uint64_t capacity = pow(2, 27);
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
    while (succkeys.size() < insertkeys.size())
    {
        succkeys.push_back(insertkeys[index]);
        index++;
    }
    std::shuffle(insertkeys.begin(), insertkeys.end(), engvalue);
    std::shuffle(succkeys.begin(), succkeys.end(), engvalue);

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
    capacity = pow(2, 27);


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
    s.counters["UnsuccesRatio"] = (double)unsucclookup/index;

}

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
BENCHMARK(insertionBench<RH<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("RHMultAdd")->DenseRange(5, 5)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<RH<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Name("RHTab")->DenseRange(3, 4)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<RH<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Name("RHTab")->DenseRange(5, 5)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<Chained8<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("Chained8MultAdd")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained8<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Name("Chained8Tab")->DenseRange(0, 2)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<Chained24<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("Chained24MultAdd")->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained24<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Name("Chained24Tab")->DenseRange(0, 2)->Unit(benchmark::kSecond);


BENCHMARK(insertionBench<MCuckoo<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("CuckooMultAdd")->DenseRange(3, 4)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<MCuckoo<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("CuckooMultAdd")->DenseRange(5, 5)->Unit(benchmark::kSecond);

BENCHMARK(insertionBench<MCuckoo<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Name("CuckooTab")->DenseRange(3, 4)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<MCuckoo<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Name("CuckooTab")->DenseRange(5, 5)->Unit(benchmark::kSecond);


BENCHMARK_MAIN();
