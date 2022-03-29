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
static void lookupBenchUnsucces(benchmark::State &s)
{
    std::uniform_int_distribution<uint64_t> distrsparse;
    std::random_device rd;
    std::mt19937_64 engkey(rd());
    std::mt19937_64 engvalue;

    std::vector<uint64_t> insertkeys;
    std::vector<uint64_t> unsucckeys;

    pair<Node<uint64_t, uint64_t> *, bool> returnpair;
    uint64_t key = 1;
    uint64_t value = 1;
    double tableloadfactor = 0;
    double loadfactor;
    int i;
    i = s.range(0);
    int loadfactors[6] = {25, 35, 45, 50, 70, 90};
    loadfactor = (double)loadfactors[i] / 100;

    while (insertkeys.size() < (capacity * loadfactor))
    {
        key = distrsparse(engkey);
        insertkeys.push_back(key);
    }

    while (unsucckeys.size() < pow(2, 24))
    {
        key = distrsparse(engkey);
        unsucckeys.push_back(key);
    }

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

    uint64_t d = 0;
    while (tableloadfactor < loadfactor)
    {
        key = insertkeys[d];
        d++;
        returnpair = table.emplace(key, key);
        if (returnpair.second == true)
        {
            tableloadfactor = (double)table.getSize() / capacity;
        }
    }

    insertkeys.clear();
    d = 0;
    uint64_t *myvector = new uint64_t[unsucckeys.size()];
    while (d < unsucckeys.size())
    {
        myvector[d] = unsucckeys[d];
        d++;
    }
    uint64_t size = d;
    d = 0;
    uint64_t k=0;
    for (auto _ : s)
    {
        while (d < size)
        {
            key = myvector[d];
            d++;
            returnpair = table.find(key);
		if(returnpair.second==false){
		size--;}
        }
    }
cout<<"finish"<<endl;
    delete[] myvector;
    s.counters["Totlookup"] = d;
    s.counters["Loadfactor"] = loadfactor;
    s.counters["UnsuccesRatio"] = 100;
}

BENCHMARK(lookupBenchUnsucces<Linearprobing<uint64_t, uint64_t, MultiplyShift, BulkAllocator<uint64_t, uint64_t>>>)->DenseRange(0, 2)->Unit(benchmark::kSecond);
BENCHMARK(lookupBenchUnsucces<Linearprobing<uint64_t, uint64_t, Murmur64, BulkAllocator<uint64_t, uint64_t>>>)->DenseRange(0, 2)->Unit(benchmark::kSecond); //
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
