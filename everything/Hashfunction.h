/*
 * Hashfunction.h
 *
 *  Created on: Oct 23, 2021
 *      Author: Gabriel
 */

#ifndef MULTIPLYSHIFT_H_
#define MULTIPLYSHIFT_H_

#include <bits/stdc++.h>
#include<math.h>
#include <random>
#include <limits>
using namespace std;


class MultiplyShift {

string name = "Mult";
uint64_t d = 0;
public:
uint64_t  a;
	MultiplyShift(){
		std::uniform_int_distribution< uint64_t> distr;
//		std::random_device rd;
//		std::mt19937_64 eng(rd());
		std::mt19937_64 eng;
		a = distr(eng);
		if (a%2==0)
			a = a-1;
		
	}
	void initialize(uint64_t capacity){
		d = log2(capacity);

	}

	std::size_t hashcode(uint64_t key,uint64_t capacity){

		//uint64_t d = log2(capacity);
		return  ((a*key) >> (64-27) );
	}
	string getName(){
		return name;
	}
};
class MultiplyAddShift {

	std::uniform_int_distribution<uint64_t> distr;

	uint64_t a;
	uint64_t b;

public:
	MultiplyAddShift(){
		std::random_device rd;
		std::mt19937_64 eng;
		a = distr(eng);
		b = distr(eng);
		if (a%2==0)
			a = a-1;
	}

	uint32_t hashcode(uint32_t key,uint32_t capacity){

		uint64_t d = log2(capacity);
		return ( ((a*key)+b) >> 34 );
	}
};
class Murmur64{

	string name = "Murmur";
public:
	Murmur64(){}

	uint64_t hashcode(uint64_t key,uint64_t capacity){
		key = key ^ ( (key) >> (33) );
		key = key * 0xff51afd7ed558ccdL;
		key = key ^ ( (key) >> (33) );
		key *= 0xc4ceb9fe1a85ec53L;
		key = key ^ ( (key) >> (33) );
		return  key;
	}
	void initialize(uint64_t capacity){
               ;
        }
	string getName(){
		return name;
	}

};
class TabulationHashing{
public:
	std::random_device rd;
	std::mt19937_64 eng;
	std::uniform_int_distribution<unsigned long long> distr;
	unsigned long long T[8] [256];
	TabulationHashing(){

		for (int i = 0; i < 8; i++){
			for (int j = 0; j < 256; j++){
				T[i][j] = distr(eng);
			}
		}
	}

	unsigned long long hashcode(unsigned long key,unsigned long capacity){

		// Simple Tabulation Hash
		unsigned long long res = 0;
		for (int i = 0; i < 8; i++)
			res ^= T[i][(key >> 8*i) & 0xFF];

		return res;
	}
};


#endif /* MULTIPLYSHIFT_H_ */
