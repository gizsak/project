#include <bits/stdc++.h>
#include "../src/Linearprobing.h"
#include "../src/HashFunction.h"
#include "../src/Allocator.h"

#include <gtest/gtest.h>
#include "mytests.h"

using namespace std;

template <typename T>
class LP : public ::testing::Test {
    public:
        using Table = Linearprobing<uint64_t ,uint64_t,T,BulkAllocator<uint64_t,uint64_t>>;

};
using HashFunctions = ::testing::Types<MultiplyShift, MultiplyAddShift,Murmur64,TabulationHashing>;
TYPED_TEST_SUITE(LP, HashFunctions);

TYPED_TEST(LP, Insertion)
{   
    typename TestFixture::Table table(capacity,capacity);
    table.clear();
    cleardata();
    populate();
    for (auto const &pair : elements)
    {
        key = pair.first;
        value = pair.second;
        returnpair = table.emplace(key, value);
        GTEST_ASSERT_EQ(returnpair.second, true);
    }
}


TYPED_TEST(LP, CorrectSize)
{
   typename TestFixture::Table table(capacity,capacity);
    table.clear();
    cleardata();
    populate();
    for (auto const &pair : elements)
    {
        key = pair.first;
        value = pair.second;
        table.emplace(key, value);
    }
    GTEST_ASSERT_EQ(table.getSize() == elements.size(), true);
}
TYPED_TEST(LP, Lookup)
{
    typename TestFixture::Table table(capacity,capacity);
    table.clear();
    populate();
    for (auto const &pair : elements)
    {
        key = pair.first;
        value = pair.second;
        table.emplace(key, value);
    }
    for (auto const &pair : elements)
    {
        key = pair.first;
        returnpair = table.find(key);
        GTEST_ASSERT_EQ(returnpair.second, true);
    }
}
TYPED_TEST(LP,LookupValue)
{
    typename TestFixture::Table table(capacity,capacity);
    table.clear();
    populate();
    for (auto const &pair : elements)
    {
        key = pair.first;
        value = pair.second;
        table.emplace(key, value);
    }
    for (auto const &pair : elements)
    {
        key = pair.first;
        value = pair.second;
        returnpair = table.find(key);
        GTEST_ASSERT_EQ(returnpair.first->getValue() == value, true);
    }
}
TYPED_TEST(LP, Deletion)
{
    typename TestFixture::Table table(capacity,capacity);
    table.clear();
    populate();
    for (auto const &pair : elements)
    {
        key = pair.first;
        value = pair.second;
        table.emplace(key, value);
    }
    bool deleted;
    populate2();
    for (auto const &pair : deletedelements)
    {
        key = pair.first;
        deleted = table.erase(key);
        GTEST_ASSERT_EQ(deleted, true);
    }
}

TYPED_TEST(LP, LookupDeletedElements)
{
    typename TestFixture::Table table(capacity,capacity);
    table.clear();
    populate();
    for (auto const &pair : elements)
    {
        key = pair.first;
        value = pair.second;
        table.emplace(key, value);
    }
    bool deleted;
    populate2();
    for (auto const &pair : deletedelements)
    {
        key = pair.first;
        deleted = table.erase(key);
    }
    for (auto const &pair : deletedelements)
    {
        key = pair.first;
        returnpair = table.find(key);
        GTEST_ASSERT_EQ(returnpair.second, false);
    }
}
TYPED_TEST(LP, LookupRemainingElements)
{
    typename TestFixture::Table table(capacity,capacity);
    table.clear();
    populate();

    for (auto const &pair : elements)
    {
        key = pair.first;
        value = pair.second;
        table.emplace(key, value);
    }
    bool deleted;
    populate2();
    cout<<remainedelements.size()<<" "<<table.getSize()<<endl;

    for (auto const &pair : deletedelements)
    {
        key = pair.first;
        deleted = table.erase(key);
    }
    for (auto const &pair : remainedelements)
    {
        key = pair.first;
        value = pair.second;
        returnpair = table.find(key);
        GTEST_ASSERT_EQ(returnpair.second, true);
    }
    cout<<remainedelements.size()<<" "<<table.getSize()<<endl;
}
TYPED_TEST(LP, SizeAfterDeletion)
{
    typename TestFixture::Table table(capacity,capacity);
    table.clear();
    populate();
    for (auto const &pair : elements)
    {
        key = pair.first;
        value = pair.second;
        table.emplace(key, value);
    }
    bool deleted;
    populate2();
    for (auto const &pair : deletedelements)
    {
        key = pair.first;
        deleted = table.erase(key);
    }
    GTEST_ASSERT_EQ(table.getSize() == remainedelements.size(), true);
}
TYPED_TEST(LP, LookupValueAfterDeletion)
{
    typename TestFixture::Table table(capacity,capacity);
    table.clear();
    populate();
    for (auto const &pair : elements)
    {
        key = pair.first;
        value = pair.second;
        table.emplace(key, value);
    }
    bool deleted;
    populate2();
    for (auto const &pair : deletedelements)
    {
        key = pair.first;
        deleted = table.erase(key);
    }
    for (auto const &pair : remainedelements)
    {
        key = pair.first;
        value = pair.second;
        returnpair = table.find(key);

        GTEST_ASSERT_EQ(returnpair.first->getValue() == value, true);
    }
}
