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
#include "Node.h"
#include "Cnode.h"
#include <benchmark/benchmark.h>

using namespace std;

uint64_t capacity = pow(2, 30);
uint64_t bulkcapacity;
std::uniform_int_distribution<uint64_t> distrdense(1, pow(2, 30));
std::random_device rd;
std::mt19937_64 engkey(rd());
std::mt19937_64 engvalue(rd());

std::default_random_engine gen(rd());

pair<Node<uint64_t, uint64_t> *, bool> returnpair;

template <class Table>
static void insertionBench(benchmark::State &s)
{

    uint64_t key = 1;
    uint64_t value = 1;
    uint64_t totinsert = 0;
    double tableloadfactor = 0;

    int i = s.range(0);
    int ranges[6] = {25, 35, 45, 50, 70, 90};
    double loadfactor = (double)ranges[i] / 100;

    if (Table::getName() == "Chained8")
    {
        if (loadfactor == 0.25)
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

    while (tableloadfactor < loadfactor)
    {
        key = distrdense(engkey);
        value = distrdense(engvalue);
        returnpair = table.emplace(key, value);
        if (returnpair.second == true)
        {
            tableloadfactor = (double)table.getSize() / capacity;
        }
    }
    while (myvector.size() < 10737420)
    {
        key = distrdense(engkey);
        // value = distrsparse(engvalue);
        returnpair = table.find(key);
        if (returnpair.second == false)
        {
            if (find(myvector.begin(), myvector.end(), key) == myvector.end())
            {
                myvector.push_back(key);
                totinsert++;
            }
        }
    }

    std::shuffle(myvector.begin(), myvector.end(), gen);

    for (auto _ : s)
    {
        for (uint64_t k : myvector)
        {
            returnpair = table.emplace(k, k);
        }
    }
    // myvector.clear();
    s.counters["Totinsert"] = totinsert;
    s.counters["Loadfactor"] = loadfactor;

    cout << myvector.size() << endl;
    cout << totinsert << endl;
}

BENCHMARK(insertionBench<Linearprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->DenseRange(0, 5)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Linearprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->DenseRange(0, 5)->Unit(benchmark::kSecond);
/*BENCHMARK(insertionBench<Quadraticprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->DenseRange(3, 5)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Quadraticprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->DenseRange(3, 5)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<RH<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->DenseRange(3, 5)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<RH<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->DenseRange(3, 5)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Cuckoo<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->DenseRange(3, 5)->Unit(benchmark::kSecond);
// BENCHMARK(insertionBench<Cuckoo<uint64_t ,uint64_t,Murmur64,BulkAllocator<uint64_t,uint64_t>>>)->DenseRange(3,5)->Unit(benchmark::kSecond);
*/BENCHMARK(insertionBench<Chained8<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained8<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained24<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(insertionBench<Chained24<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->DenseRange(0, 2)->Unit(benchmark::kSecond);

BENCHMARK_MAIN();
