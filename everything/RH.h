/*
 * Linearprobing.h
 *
 *  Created on: Oct 23, 2021
 *      Author: Gabriel
 */
#ifndef RH_H_
#define RH_H_

#include <bits/stdc++.h>
#include "Node.h"

using namespace std;

template<class Key, class T, class Hash,class Allocator>
class RH{

	Key size;
	Key capacity;

	Node<Key,T>* table;
	Node<Key,T>* b;
	Node<Key,T>* e;
	Hash function = Hash();
	uint64_t key2;
	uint64_t value2;
	Key dnew;
	Key dold;	
public:

	RH(Key capacity, Key bulkcapacity ){
		this->capacity = capacity;
		function.initialize(capacity);
		size = 0;
		table = new Node<Key,T>[capacity];
		b = table;
		e = table + (capacity-1);

	}
	~RH(){
		delete[] table;
	}
	pair<Node<Key,T>*,bool> emplace(Key key, T value){

		Key hashindex = function.hashcode(key,capacity) % capacity;
		Key counter = 0;
		Key test = (function.hashcode(key,capacity) % capacity);

		while (table[hashindex].getKey() != 0 && table[hashindex].getKey() != key && counter < capacity ){
			
			dnew = (hashindex - test );
			dold = (hashindex - (function.hashcode(table[hashindex].getKey(),capacity) % capacity) );

			if(dnew>dold){
				key2 = table[hashindex].getKey();
                                value2 = table[hashindex].getValue();
				table[hashindex] = Node<Key,T>(key,value);
				pair<Node<Key,T>*,bool>returnpair(b+hashindex,true);
				emplace(key2, value2);
				return returnpair;
			}
			counter++;
			hashindex++;
//			hashindex %= capacity;
		}
		if(table[hashindex].getKey() == 0){
			table[hashindex] = Node<Key,T>(key,value);
			size++;
			pair<Node<Key,T>*,bool>returnpair(b+hashindex,true);
			return returnpair;
		}
		
		else{
			pair<Node<Key,T>*,bool>returnpair(b+hashindex,false);
			return returnpair;
		}
	}
	bool erase(Key key)
	{
		Key hashindex = function.hashcode(key,capacity) % capacity;
		Key counter = 0;
		bool deleted = false;
		
		while (table[hashindex].getKey() != 0 && counter < capacity && !deleted ) {
			
			if (table[hashindex].getKey() == key){
				table[hashindex] = Node<Key,T>();
				deleted = true;
				size--;
				hashindex++;
				while(table[hashindex].getKey() != 0){//counter
					Key key = table[hashindex].getKey();
					T value = table[hashindex].getValue();
					table[hashindex] = Node<Key,T>();
					size--;
					emplace(key,value);
					hashindex++;
					hashindex %= capacity;
				}
			}
			counter++;
			hashindex++;
			hashindex %= capacity;
		}
		
		return deleted;
	}
	
	pair<Node<Key,T>*,bool> find(Key key)
	{
		Key hashindex = function.hashcode(key,capacity) % capacity;
		Key counter = 0;
		Node<Key,T>* it = e;
		Key dold = 0;
//		bool found;

		for(;;){
			
			if(table[hashindex].getKey() == 0){
				pair<Node<Key,T>*,bool> returnpair(it,false);
                		return returnpair;
			}
			if (table[hashindex].getKey() == key){
				it = b+(hashindex);
				pair<Node<Key,T>*,bool> returnpair(it,true);
				return returnpair;
			}
			/*counter++;
			hashindex++;
			hashindex %= capacity;*/
			if((counter%32)==0){
			dold = (hashindex - (function.hashcode(table[hashindex].getKey(),capacity)%capacity ) );
				if (counter>dold){
                                pair<Node<Key,T>*,bool> returnpair(it,false);
                                return returnpair;
                        }

			}
			counter++;
                        hashindex++;
  //                      hashindex %= capacity;
		}
	//	pair<Node<Key,T>*,bool> returnpair(it,false);
	//	return returnpair;
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
		return "RH";
	}
	
	
};

#endif /* RH_H_ */



