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

template <class Table>
static void lookupBenchUnSucces(benchmark::State &s)
{   
    std::uniform_int_distribution<uint64_t> distrsparse;
    std::mt19937_64 engkey;
    std::mt19937_64 engvalue;

    std::vector<uint64_t> insertkeys;
    std::vector<uint64_t> unsucckeys;

    pair<Node<uint64_t, uint64_t> *, bool> returnpair;
    int i = s.range(0);
    int ranges[6] = {25, 35, 45, 50, 70, 90};
    double loadfactor = (double)ranges[i] / 100;

    uint64_t key = 1;
    uint64_t value = 1;
    double tableloadfactor = 0;
    uint64_t unsucclookup = 0;

    uint64_t index = 0;
    uint64_t unsucckeysnumber = pow(2, 22);

     while (insertkeys.size() < (capacity * loadfactor))
    {
        key = distrsparse(engkey);
        insertkeys.push_back(key);
//	key++;
    }
    index = 0;
    while (unsucckeys.size() < unsucckeysnumber)
    {
        key = distrsparse(engkey);
        unsucckeys.push_back(key);
    }
    std::shuffle(insertkeys.begin(), insertkeys.end(), engvalue);
//    std::shuffle(unsucckeys.begin(), unsucckeys.end(), engvalue);

     if (Table::getName() == "Chained8")
    {
        if (loadfactor == 0.45)
        {
            bulkcapacity = capacity;
            capacity = capacity/2;
        }
        else
        {
            bulkcapacity = capacity;
            capacity = capacity*4 ;
        }
    }
    if (Table::getName() == "Chained24")
    {
        if (loadfactor == 0.50 || loadfactor == 0.45)
        {
            bulkcapacity = capacity / 4;
            capacity = capacity /2;
        }
        else
        {
            bulkcapacity = capacity / 4;
            capacity = capacity /2;
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
    uint64_t *lookuparray = new uint64_t[unsucckeys.size()];
    while (index < unsucckeys.size())
    {
        lookuparray[index] = unsucckeys[index];
        index++;
    }
    uint64_t size = index;
    unsucckeys.clear();
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
    double ratio = (double)unsucclookup/index;
    ratio = ratio*100;
    s.counters["Totlookup"] = index;
    s.counters["Loadfactor"] = loadfactor;
    s.counters["UnsuccesRatio"] = ratio;

}

BENCHMARK(lookupBenchUnSucces<Linearprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("LPMultAdd")->DenseRange(0, 2)->Unit(benchmark::kSecond);
/*BENCHMARK(lookupBenchSucces<Linearprobing<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Name("LPTab")->DenseRange(0, 5)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchSucces<Quadraticprobing<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("QPMultAdd")->DenseRange(0, 5)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchSucces<Quadraticprobing<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Name("QPTab")->DenseRange(0, 5)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchSucces<RH<uint64_t, uint64_t, MultiplyAddShift, BulkAllocator<uint64_t, uint64_t>>>)->Name("RHMultAdd")->DenseRange(0, 5)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchSucces<RH<uint64_t, uint64_t, TabulationHashing, BulkAllocator<uint64_t, uint64_t>>>)->Name("RHTab")->DenseRange(0, 5)->Unit(benchmark::kSecond);
*/
BENCHMARK(lookupBenchUnSucces<Chained8<uint64_t, uint64_t, MultiplyShift, Allocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained8MultAdd")->DenseRange(0, 2)->Unit(benchmark::kSecond);
//BENCHMARK(insertionBench<Chained8<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Name("Chained8Tab")->DenseRange(0, 2)->Unit(benchmark::kSecond);

BENCHMARK(lookupBenchUnSucces<Chained24<uint64_t, uint64_t, MultiplyShift, Allocator<uint64_t, uint64_t>>>)->Iterations(1)->Name("Chained24MultAdd")->DenseRange(0, 3)->Unit(benchmark::kSecond);
//BENCHMARK(insertionBench<Chained24<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->Name("Chained24Tab")->DenseRange(0, 3)->Unit(benchmark::kSecond);
BENCHMARK_MAIN();

