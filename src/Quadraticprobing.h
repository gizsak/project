/*
 * Quadraticprobing.h
 *
 *  Created on: Oct 24, 2021
 *      Author: Gabriel
 */

#ifndef QUADRATICPROBING_H_
#define QUADRATICPROBING_H_

#include <bits/stdc++.h>
#include "Node.h"

using namespace std;

template<class Key, class T, class Hash,class Allocator>
class Quadraticprobing{

	Key size;
	Key capacity;

	Node<Key,T>* table;
	Node<Key,T> tombstone;
	Node<Key,T>* b;
	Node<Key,T>* e;
	Hash function = Hash();
	string hashname = function.getName();

public:

	Quadraticprobing(Key capacity, Key bulkcapacity ){
		this->capacity = capacity;
		function.initialize(capacity);
		size = 0;
		table = new Node<Key,T>[capacity];
		b = table;
		e = table + (capacity-1);
		tombstone = Node<Key,T>(-1,-1);

	}
	~Quadraticprobing(){
		delete[] table;
	}
	pair<Node<Key,T>*,bool> emplace(Key key, T value){

                Key hashindex = function.hashcode(key,capacity) % capacity;
                

		pair<Node<Key,T>*,bool> returnpair;
		Key counter = 0;
		//const uint64_t a = 10;
		while (table[hashindex].getKey() != 0 && table[hashindex].getKey() != key && table[hashindex].getKey() != -1  && counter < capacity) {
			counter++;
//			hashindex = hashindex + ( (0*counter) + (2*counter*counter) );
			hashindex = hashindex + (counter*counter);
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
			Key i = 0;

			while (table[hashindex].getKey() != 0 && counter < capacity && !deleted) {
				if (table[hashindex].getKey() == key) {
					table[hashindex] = tombstone;// can be same logic as LP, next is (0.5*i) + (0.5*i*i)
					size--;
					deleted = true;
				}
				counter++;
				i++;
			//	hashindex = hashindex + ( (0.5*i) + (0.5*i*i) );
				
				hashindex %= capacity;//do we need mod?
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
		//const uint64_t a = 10;
		while (table[hashindex].getKey() != 0 && counter < capacity && !found  ) {

			if (table[hashindex].getKey() == key){
				it = b+(hashindex);
				found = true;
			}
			counter++;
//			hashindex = hashindex + ( (0*counter) + (2*counter*counter) );
			hashindex = hashindex + (counter*counter);
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
		return "QP";
	}

};

#endif /* QUADRATICPROBING_H_ */



