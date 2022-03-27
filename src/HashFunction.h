/*
 * Hashfunction.h
 *
 *  Created on: Oct 23, 2021
 *      Author: Gabriel
 */

#ifndef MULTIPLYSHIFT_H_
#define MULTIPLYSHIFT_H_

#include <bits/stdc++.h>
#include <math.h>
#include <random>
#include <limits>
using namespace std;

class MultiplyShift
{

	string name = "Mult";
	uint64_t d = 0;

public:
	uint64_t a;
	MultiplyShift()
	{
		std::uniform_int_distribution<uint64_t> distr;
		std::random_device rd;
		//std::mt19937_64 eng(rd());
		std::mt19937_64 eng;
		a = distr(eng);
		if (a % 2 == 0){
			a = a - 1;
		}
	}
	void initialize(uint64_t capacity)
	{
		d = log2(capacity);
	}

	uint64_t hashcode(uint64_t key, uint64_t capacity)
	{

		// uint64_t d = log2(capacity);
		return ((a * key) >> (64 - d));
	}
	string getName()
	{
		return name;
	}
};

class MultiplyAddShift
{
	string name = "Mult";
	uint64_t a;
	uint32_t b;
	uint64_t d = 0;

public:
	MultiplyAddShift()
	{

		std::uniform_int_distribution<uint64_t> distr;
		std::uniform_int_distribution<uint32_t> distrb;
		std::random_device rd;
		//std::mt19937_64 eng;
		std::mt19937_64 eng(rd());
		a = distr(eng);
		b = distrb(eng);
		if (a % 2 == 0)
			a = a - 1;
	}
	void initialize(uint64_t capacity)
	{
		d = log2(capacity);
	}
	uint64_t hashcode(uint64_t key, uint64_t capacity)
	{

		return ((a * key + b) >> (64 - d));
	}
	string getName()
	{
		return name;
	}
};

class Murmur64
{

	string name = "Murmur";

public:
	Murmur64() {}

	uint64_t hashcode(uint64_t key, uint64_t capacity)
	{
		key = key ^ ((key) >> (33));
		key = key * 0xff51afd7ed558ccdL;
		key = key ^ ((key) >> (33));
		key *= 0xc4ceb9fe1a85ec53L;
		key = key ^ ((key) >> (33));
		return key;
	}
	void initialize(uint64_t capacity)
	{
		;
	}
	string getName()
	{
		return name;
	}
};
class TabulationHashing
{
public:

	string name = "Tab";
	uint64_t T[8][256];
	TabulationHashing()
	{
		std::uniform_int_distribution<uint64_t> distr;
		std::random_device rd;
		//std::mt19937_64 eng(rd());
		std::mt19937_64 eng;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 256; j++)
			{
				T[i][j] = distr(eng);
			}
		}
	}

	uint64_t hashcode(uint64_t key, uint64_t capacity)
	{
		uint64_t result = 0;
		for (int i = 0; i < 8; i++)
			result ^= T[i][(key >> 8 * i) & 0xFF];

		return result;
	}
	void initialize(uint64_t capacity)
	{
		;
	}
	string getName()
	{
		return name;
	}
};

#endif /* MULTIPLYSHIFT_H_ */
