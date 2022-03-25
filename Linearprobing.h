/*
 * Linearprobing.h
 *
 *  Created on: Oct 23, 2021
 *      Author: Gabriel
 */
#ifndef LINEARPROBING_H_
#define LINEARPROBING_H_

#include <bits/stdc++.h>
#include "Node.h"

using namespace std;

template<class Key, class T, class Hash, class Allocator>
class Linearprobing{

	Key size;
	Key capacity;

	Node<Key,T>* table;
	Node<Key,T> tombstone;
	Node<Key,T>* b;
	Node<Key,T>* e;
	Hash function = Hash();
	string hashname = function.getName(); 

public:

	Linearprobing(Key capacity, Key bulkcapacity ){
		this->capacity = capacity;
		function.initialize(capacity);
		size = 0;
		table = new Node<Key,T>[capacity];
		b = table;
		e = table + (capacity-1);
		tombstone = Node<Key,T>(-1,-1);

	}
	~Linearprobing(){
		delete[] table;
	}
	pair<Node<Key,T>*,bool> emplace(Key key, T value){

		Key hashindex = function.hashcode(key,capacity) % capacity;
		
		Key counter = 0;
		pair<Node<Key,T>*,bool> returnpair;

		while (table[hashindex].getKey() != 0 && table[hashindex].getKey() != key && table[hashindex].getKey() != -1 && counter < capacity) {
			counter++;
			hashindex++;
//			hashindex %= capacity;
		}
		if (table[hashindex].getKey() == 0 || table[hashindex].getKey() == -1){
			size++;
			table[hashindex] = Node<Key,T>(key,value);
			returnpair = pair<Node<Key,T>*,bool>(b+hashindex,true);
		}
		else
			returnpair = pair<Node<Key,T>*,bool>(b+hashindex,false);

		return returnpair;
	}
	bool erase(Key key)
		{
			Key hashindex = function.hashcode(key,capacity) % capacity;
			bool deleted = false;
			Key counter = 0;

			while (table[hashindex].getKey() != 0 && counter < capacity && !deleted) {
				if (table[hashindex].getKey() == key) {
					if(table[hashindex+1].getKey() != 0 ){
						table[hashindex] = tombstone;
					}
					else{
						table[hashindex] = Node<Key,T>();
					}
					size--;
					deleted = true;
				}
				counter++;
				hashindex++;
			//	hashindex %= capacity;
			}
			

			return deleted;
		}
	Node<Key,T>* erase(Node<Key,T>* it)
			{
				*it = tombstone;
				it++;
				return it;
			}
	pair<Node<Key,T>*,bool> find(Key key)
	{
                Key hashindex = function.hashcode(key,capacity) % capacity;

		Key counter = 0;
		Node<Key,T>* it;
		bool found = false;

		while (table[hashindex].getKey() != 0 && counter < capacity && !found ) {

			if (table[hashindex].getKey() == key){
				it = b+(hashindex);
				found = true;
			}
			counter++;
			hashindex++;
//			hashindex %= capacity;
		}
		if(!found){
			it = e;
		}
		pair<Node<Key,T>*,bool> returnpair(it,found);

		return returnpair;
	}

	Node<Key,T>* begin(){
		return &(*b);
	}
	Node<Key,T>* end(){
			return &(*e);
		}
	T & operator[](Key key){

		pair<Node<Key,T>*,bool>p;
		Node<Key,T>* it;
		it = find(key);
		if(it == e){
			p = emplace(key,0);
			it = p.first;
		}
		//table[key].getValue();
		return it->getValue();
	}
	T operator = (T second){
		return second;
	}
	Key getSize(){
		return size;
	}
	void clear(){
		for(Key i = 0;i<capacity;++i)
			table[i] = Node<Key,T>();
			size = 0;
	}
	static string getName(){
		return "LP";
	}
	
};

#endif /* LINEARPROBING_H_ */



