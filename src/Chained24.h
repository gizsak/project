/*
 * Chained8.h
 *
 *  Created on: Oct 27, 2021
 *      Author: Gabriel
 */

#include <bits/stdc++.h>
#include "Allocator.h"
#include "Cnode.h"
//#include <chrono>
//#include <thread>
using namespace std;

#ifndef CHAINED24_H_
#define CHAINED24_H_
using namespace std;

template<class Key, class T, class Hash, class Allocator>
class Chained24 {

	Key size;
	Key capacity;
	Cnode<Key,T>* table;

	Cnode<Key,T>* b;
	Cnode<Key,T>* e;
	Hash function;
	Allocator allocator;
public:
	Chained24(Key capacity,Key bulkcapacity){
		size = 0;
		this->capacity = capacity;
		function.initialize(capacity);
		allocator.Allocate(bulkcapacity);
		table = new Cnode<Key,T>[this->capacity];
		b = &table[0];
		e = &table[capacity-1];
	}
	~Chained24(){
		
		delete[] table;

	}
	pair<Cnode<Key,T>*,bool> emplace(Key key, T value){

		Key hashindex = function.hashcode(key,capacity) % capacity;
		pair<Cnode<Key,T>*,bool> returnpair;
		Cnode<Key,T>* entry = &table[hashindex];
		Cnode<Key,T>* prev = NULL;

		while (entry != NULL && entry->key != key)
		{
			prev = entry;
			entry = entry->next;
		}
		if (entry == NULL)
		{
			if(prev->key == -1){//before while because of erase -1
				table[hashindex].key = key;
				table[hashindex].value = value;
				entry = &table[hashindex];
				returnpair = pair<Cnode<Key,T>*,bool>(entry,true);
			}
			else
			{
				entry = allocator.makenode(key,value);
				prev->next = entry;
				returnpair = pair<Cnode<Key,T>*,bool>(entry,true);
			}
			size++;	
		}
		else
		{
			returnpair = pair<Cnode<Key,T>*,bool>(entry->next,false);//just entry?
		}
		return returnpair;
	}

	bool erase(Key key){

		Key hashindex = function.hashcode(key,capacity) % capacity;
		bool deleted = false;
		Cnode<Key,T>* entry = &table[hashindex];
		Cnode<Key,T>* prev = NULL;
		
		while (entry->next != NULL && entry->key != key )
		{
			prev = entry;
			entry = entry->next;
		}
		if(entry->key == key){
			if (prev != NULL)
			{
				prev->next = entry->next;
				if(allocator.isbulk()){
					*entry = Cnode<Key,T>();
				}
				else{
					delete entry;
				}
				deleted = true;
				size--;
			}

			else{
				if(entry->next == NULL){
					table[hashindex] = Cnode<Key,T>();
				}
				else{
					table[hashindex].key = -1;
					table[hashindex].next = entry->next;
				}
				deleted = true;
				size--;
			}
		}

		return deleted;

	}
	
	pair<Cnode<Key,T>*,bool> find(Key key){
		Key hashindex = function.hashcode(key,capacity) % capacity;
		Cnode<Key,T>* entry = &table[hashindex];
//		if(entry->next == NULL && entry->key != key){
//		pair<Cnode<Key,T>*,bool> returnpair(entry,false);
//		return returnpair;
//		}
		while(entry->next != NULL && entry->key != key){
			
			entry = entry->next;
		}

		if(entry->next == NULL && entry->key != key){
                      pair<Cnode<Key,T>*,bool> returnpair(entry,false);
                      return returnpair;
               }
/*		if(entry->next == NULL && entry->key == key){
	              pair<Cnode<Key,T>*,bool> returnpair(entry,true);
              	      return returnpair;
               }
		if(entry->key == key){
			pair<Cnode<Key,T>*,bool> returnpair(entry,true);
                        return returnpair;
		}
*/
			pair<Cnode<Key,T>*,bool> returnpair(entry,true);
                        return returnpair;		
	}
	T & operator[](Key key){

		pair<Cnode<Key,T>*,bool>p;
		Cnode<Key,T>* it;
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
		allocator.clear();
		for (Key i = 0; i < capacity; i++){
			table[i] = Cnode<Key,T>();
		}
		size = 0;
	}
	static string getName(){
		return "Chained24";
	}

};

#endif /* CHAINED24_H_ */
