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
#include <benchmark/benchmark.h>

using namespace std;

uint64_t capacity = pow(2, 30);
std::uniform_int_distribution<uint64_t> distrsparse;
std::random_device rd;
std::mt19937_64 engkey(rd());
std::mt19937_64 engvalue(rd());

template <class Table>
static void lookupBenchUnsucces(benchmark::State &s)
{

    Table table(capacity, capacity);
    pair<Node<uint64_t, uint64_t> *, bool> returnpair;
    int i = s.range(0);
    int ranges[6] = {25, 35, 45, 50, 70, 90};
    double loadfactor = (double)ranges[i] / 100;
    std::uniform_int_distribution<uint64_t> distrdense(capacity * loadfactor, pow(2, 30));
    uint64_t key = 1;
    uint64_t value = 1;
    double tableloadfactor = 0;
    uint64_t size = 0;
    uint64_t totlookup = 0;
    uint64_t unsucclookup = 0;
    uint64_t c = 0;
    double ratio = 0;

    while (tableloadfactor < loadfactor)
    {
        value = distrdense(engvalue);
        returnpair = table.emplace(key, value);
        if (returnpair.second == true)
        {
            tableloadfactor = (double)table.getSize() / capacity;
        }
        key++;
    }

    for (auto _ : s)
    {
        while (c < pow(2, 23))
        {
            key = distrdense(engkey);
            returnpair = table.find(key);
            c++;
            totlookup++;
            if (returnpair.second == false)
            {
                unsucclookup++;
                ratio = (double)unsucclookup / totlookup;
            }
        }
        s.counters["Totlookup"] = totlookup;
        s.counters["Loadfactor"] = loadfactor;
    }
    cout << totlookup << endl;
    cout << unsucclookup;
}

BENCHMARK(lookupBenchUnsucces<Linearprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->DenseRange(0, 5)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchUnsucces<Linearprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->DenseRange(0, 5)->Unit(benchmark::kSecond); //
BENCHMARK(lookupBenchUnsucces<Quadraticprobing<uint64_t ,uint64_t,MultiplyShift,BulkAllocator<uint64_t,uint64_t>>>)->DenseRange(3,5)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchUnsucces<Quadraticprobing<uint64_t ,uint64_t,Murmur64,BulkAllocator<uint64_t,uint64_t>>>)->DenseRange(3,5)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchUnsucces<RH<uint64_t ,uint64_t,MultiplyShift,BulkAllocator<uint64_t,uint64_t>>>)->DenseRange(3,5)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchUnsucces<RH<uint64_t ,uint64_t,Murmur64,BulkAllocator<uint64_t,uint64_t>>>)->DenseRange(3,5)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchUnsucces<Cuckoo<uint64_t ,uint64_t,MultiplyShift,BulkAllocator<uint64_t,uint64_t>>>)->DenseRange(3,5)->Unit(benchmark::kSecond);
//BENCHMARK(lookupBenchUnsucces<Cuckoo<uint64_t ,uint64_t,Murmur64,BulkAllocator<uint64_t,uint64_t>>>)->DenseRange(3,5)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchUnsucces<Chained8<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchUnsucces<Chained8<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchUnsucces<Chained24<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchUnsucces<Chained24<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->DenseRange(0, 2)->Unit(benchmark::kSecond);

BENCHMARK_MAIN();
