/*
 * MCuckoo.h
 *
 *  Created on: Oct 23, 2021
 *      Author: Gabriel
 */
#ifndef MCUCKOO_H_
#define MCUCKOO_H_

#include <bits/stdc++.h>
#include "Node.h"

using namespace std;

template <class Key, class T, class Hash, class Allocator>
class MCuckoo
{
	Key size;
	Key capacity;
	Key singlecapacity;
	Node<Key, T> *table;
	Node<Key, T> tombstone;
	Node<Key, T> *b;
	Node<Key, T> *e;
	Key pos[4];
	Key position = 0;
	Key tempkey = 0;

	Hash function1 = Hash();
	Hash function2 = Hash();
	Hash function3 = Hash();
	Hash function4 = Hash();

	// Key k;
	// T v;
	// Key one;
	// Key two;
	// Key three;
	// Key four;
	// std::uniform_int_distribution<uint64_t> distrsparse;
	// std::random_device rd;
	// std::mt19937_64 eng;

public:
	MCuckoo(Key capacity, Key bulkcapacity)
	{
		this->capacity = capacity;
		this->singlecapacity = capacity / 4;
		function1.initialize(singlecapacity);
		function2.initialize(singlecapacity);
		function3.initialize(singlecapacity);
		function4.initialize(singlecapacity);
		size = 0;
		// one = distrsparse(eng);
		// two = distrsparse(eng);
		// three = distrsparse(eng);
		// four = distrsparse(eng);

		table = new Node<Key, T>[capacity];
	}
	~MCuckoo()
	{

		delete[] table;
	}
	Key hashCode(int function, Key key)
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
	pair<Node<Key, T> *, bool> emplace(Key key, T value, int tableID = 0, Key cnt = 0, Key n = pow(2, 22))
	{

		pair<Node<Key, T> *, bool> returnpair;
		if (cnt == n)
		{
			printf("REHASH.\n");
			returnpair = pair<Node<Key, T> *, bool>(b, false);
			return returnpair;
		}
		Key position = 0;
		for (int i = 0; i < 4; i++)
		{
			pos[i] = hashCode(i + 1, key);
			position = (i*singlecapacity) + pos[i];
			if (table[position].getKey() == key)
			{
				b = b+position;
				returnpair = pair<Node<Key, T> *, bool>(b, false);
				return returnpair;
			}
		}
		position = (tableID*singlecapacity)+pos[tableID];
		
		if (table[position].getKey() != 0)

		{
			tempkey = table[position].getKey();
			b = b + position;
			table[position] = Node<Key, T>(key, value);
			returnpair = pair<Node<Key, T> *, bool>(b, true);
			emplace(tempkey, tempkey, (tableID + 1) % 4, cnt + 1, n);
			//delete temp;
			//return pair<Node<Key, T> *, bool>(b, true);;
		}
		else
		{
			table[position] = Node<Key, T>(key, value);
			b = b + position;
			size++;
			returnpair = pair<Node<Key, T> *, bool>(b, true);
			 //return pair<Node<Key, T> *, bool>(b, true);;
		}

		return returnpair;
	}
	/*
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
	}*/

	pair<Node<Key, T> *, bool> find(Key key)
	{
		pair<Node<Key, T> *, bool> returnpair;
		returnpair = pair<Node<Key, T> *, bool>(e, false);

		Key position = 0;
		for (int i = 0; i < 4; i++)
		{
			pos[i] = hashCode(i + 1, key);
			position = (i*singlecapacity) + pos[i];
			if (table[position].getKey() == key)
			{
				return returnpair = pair<Node<Key, T> *, bool>(b+position, true);
			}
		}
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
	static string getName()
	{
		return "MCuckoo";
	}
};

#endif /* MCUCKOO__H_ */
