#include <bits/stdc++.h>
#include "../src/Chained8.h"
#include "../src/HashFunction.h"
#include "../src/Allocator.h"

#include <gtest/gtest.h>
#include "mytests.h"

using namespace std;
template <typename T>
class CH8 : public ::testing::Test {
    public:
        using Table = Chained8<uint64_t ,uint64_t,T,BulkAllocator<uint64_t,uint64_t>>;

};
using HashFunctions = ::testing::Types<MultiplyShift, MultiplyAddShift,Murmur64,TabulationHashing>;
TYPED_TEST_SUITE(CH8, HashFunctions);

TYPED_TEST(CH8, Insertion)
{
    typename TestFixture::Table table(capacity,capacity);
    table.clear();
    cleardata();
    populate();
    for (auto const &pair : elements)
    {
        key = pair.first;
        value = pair.second;
        chainedreturnpair = table.emplace(key, value);
        GTEST_ASSERT_EQ(chainedreturnpair.second, true);
    }
}
TYPED_TEST(CH8, CorrectSize)
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
TYPED_TEST(CH8, Lookup)
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
    for (auto const &pair : elements)
    {
        key = pair.first;
        chainedreturnpair = table.find(key);
        GTEST_ASSERT_EQ(chainedreturnpair.second, true);
    }
}
TYPED_TEST(CH8,LookupValue)
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
    for (auto const &pair : elements)
    {
        key = pair.first;
        value = pair.second;
        chainedreturnpair = table.find(key);
        GTEST_ASSERT_EQ(chainedreturnpair.first->getValue() == value, true);
    }
}
TYPED_TEST(CH8, Deletion)
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
    bool deleted;
    populate2();
    for (auto const &pair : deletedelements)
    {
        key = pair.first;
        deleted = table.erase(key);
        GTEST_ASSERT_EQ(deleted, true);
    }
}

TYPED_TEST(CH8, LookupDeletedElements)
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
        chainedreturnpair = table.find(key);
        GTEST_ASSERT_EQ(chainedreturnpair.second, false);
    }
}
TYPED_TEST(CH8, LookupRemainingElements)
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
        chainedreturnpair = table.find(key);
        GTEST_ASSERT_EQ(chainedreturnpair.second, true);
    }
    cout<<remainedelements.size()<<" "<<table.getSize()<<endl;
}
TYPED_TEST(CH8, SizeAfterDeletion)
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
    bool deleted;
    populate2();
    for (auto const &pair : deletedelements)
    {
        key = pair.first;
        deleted = table.erase(key);
    }
    GTEST_ASSERT_EQ(table.getSize() == remainedelements.size(), true);
}
TYPED_TEST(CH8, LookupValueAfterDeletion)
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
        chainedreturnpair = table.find(key);

        GTEST_ASSERT_EQ(chainedreturnpair.first->getValue() == value, true);
    }
}
