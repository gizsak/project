/*
 * Cuckoo.h
 *
 *  Created on: Oct 23, 2021
 *      Author: Gabriel
 */
#ifndef CUCKOO_H_
#define CUCKOO_H_

#include <bits/stdc++.h>
#include "Node.h"

using namespace std;

template <class Key, class T, class Hash,class Allocator>
class Cuckoo
{
	Key size;
	Key capacity;
	Key singlecapacity;
	Node<Key, T> **table;
	Node<Key, T> tombstone;
	Node<Key, T> *b;
	Node<Key, T> *e;
	Key pos[4];
	pair<Node<Key, T> *, bool> returnpair;	
	Hash function1 = Hash();
	Hash function2 = Hash();
	Hash function3 = Hash();
	Hash function4 = Hash();
	
	Key k;
	T v;
	Key one;
	Key two;
	Key three;
	Key four;
	std::uniform_int_distribution<uint64_t> distrsparse;
	std::random_device rd;
	std::mt19937_64 eng;

public:
	Cuckoo(Key capacity,Key bulkcapacity)
	{
		this->capacity = capacity;
		singlecapacity = capacity / 4;
		function1.initialize(singlecapacity);
		function2.initialize(singlecapacity);
		function3.initialize(singlecapacity);
		function4.initialize(singlecapacity);
		size = 0;
		one = distrsparse(eng);
		two = distrsparse(eng);
		three = distrsparse(eng);
		four = distrsparse(eng);

		table = new Node<Key, T> *[4];
		for (int i = 0; i < 4; i++)
			table[i] = new Node<Key, T>[singlecapacity];
	}
	~Cuckoo()
	{
		for (int i = 0; i < 4; i++)
			delete[] table[i];
		delete[] table;
	}
	Key hashCode(int function, Key key)
	{

		if (function1.getName() == "Mult")
		{
			switch (function)
			{
			case 1:
				return (function1.hashcode(key, singlecapacity) % (singlecapacity));
			case 2:
				return (function2.hashcode(key, singlecapacity) % (singlecapacity));
			case 3:
				return (function3.hashcode(key, singlecapacity) % (singlecapacity));
			case 4:
				return (function4.hashcode(key, singlecapacity) % (singlecapacity));
			default:
				return 0;
			}
		}
		else
		{
			switch (function)
			{
			case 1:
				return (function1.hashcode(key, singlecapacity) % (singlecapacity));
			case 2:
				return ((function2.hashcode(key, singlecapacity) / singlecapacity) % (singlecapacity));
			case 3:
				return (((function3.hashcode(key, singlecapacity) / singlecapacity) % singlecapacity) % (singlecapacity));
			case 4:
				return ((((function4.hashcode(key, singlecapacity) / singlecapacity) % singlecapacity/2) % singlecapacity) % (singlecapacity));
			default:
				return 0;
			}
		}
	}
	pair<Node<Key, T> *, bool> emplace(Key key, T value, int tableID=0, Key cnt=0, Key n=pow(2,17))
	
	{
		uint64_t key2;
		//uint64_t value2;
		//pair<Node<Key, T> *, bool> returnpair;
		if (cnt == n)
		{
			printf("Cycle present. REHASH.\n");
			return returnpair;
		}
		for (int i = 0; i < 4; i++)
		{
			pos[i] = hashCode(i + 1, key);
			if (table[i][pos[i]].getKey() == key)
			{
				b = (table[i] + pos[i]);
				returnpair = pair<Node<Key, T> *, bool>(b, false);
				return returnpair;
			}
		}
		if (table[tableID][pos[tableID]].getKey() != 0)

		{
		//	Node<Key, T> *temp = (new Node<Key, T>(table[tableID][pos[tableID]].getKey(), table[tableID][pos[tableID]].getValue()));
			key2 = table[tableID][pos[tableID]].getKey();
//			value2 = table[tableID][pos[tableID]].getValue();
//			b = (table[tableID] + pos[tableID]);
			table[tableID][pos[tableID]] = Node<Key, T>(key, value);
			returnpair = pair<Node<Key, T> *, bool>(b, true);
			emplace(key2,key2, (tableID + 1) % 4, cnt + 1, n);
			//delete temp;
			// return returnpair;
		}
		else
		{
			table[tableID][pos[tableID]] = Node<Key, T>(key, value);
//			b = (table[tableID] + pos[tableID]);
			size++;
			returnpair = pair<Node<Key, T> *, bool>(b, true);
			// return returnpair;
		}

		return returnpair;
	}
	bool erase(Key key)
	{

		bool deleted = false;
		for (int i = 0; i < 4; i++)
		{
			pos[i] = hashCode(i + 1, key);
			if (table[i][pos[i]].getKey() == key)
			{
				table[i][pos[i]] = Node<Key, T>();
				deleted = true;
				size--;
				return deleted;
			}
		}
		return deleted;
	}

	pair<Node<Key, T> *, bool> find(Key key)
	{
//		pair<Node<Key, T> *, bool> returnpair;
//		b = table[0];
//		returnpair = pair<Node<Key, T> *, bool>(b, false);

		for (int i = 0; i < 4; i++)
		{
			pos[i] = hashCode(i + 1, key);
			if (table[i][pos[i]].getKey() == key)
			{
				b = table[i] + pos[i];
				pair<Node<Key,T>*,bool> returnpair(b,true);
				return returnpair;
			}
		}
		pair<Node<Key,T>*,bool> returnpair(table[0],false);
		return returnpair;
	}

	T &operator[](Key key)
	{

		pair<Node<Key, T> *, bool> p;
		Node<Key, T> *it;
		it = find(key);
		if (it == e)
		{
			p = emplace(key, 0);
			it = p.first;
		}
		// table[key].getValue();
		return it->getValue();
	}
	T operator=(T second)
	{
		return second;
	}
	Key getSize()
	{
		return size;
	}
	void clear()
	{
		for (int i = 0; i < 4; i++)
		{
			for (Key j = 0; j < singlecapacity; j++)
				table[i][j] = Node<Key, T>();
		}
		size = 0;
	}
	static string getName(){
		return "Cuckoo";
	}
};

#endif /* CUCKOO__H_ */
