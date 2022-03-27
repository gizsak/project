/*
 * Allocator.h
 *
 *  Created on: Oct 27, 2021
 *      Author: Gabriel
 */
#ifndef TEST_H_
#define TEST_H_

#include <string>
#include <map>
#include <bits/stdc++.h>
using namespace std;
extern string name;
extern map<uint64_t, uint64_t> elements;
extern map<uint64_t, uint64_t> deletedelements;
extern map<uint64_t, uint64_t> remainedelements;
extern std::uniform_int_distribution<uint64_t> distr;
extern std::random_device rd;
extern std::mt19937_64 eng;
extern uint64_t capacity;
extern uint64_t key;
extern uint64_t value;
extern pair<Node<uint64_t, uint64_t> *, bool> returnpair;
extern pair<Cnode<uint64_t, uint64_t> *, bool> chainedreturnpair;

void populate();
void populate2();
void cleardata();


#endif /* TEST_H_ */
