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

#ifndef CHAINED8_H_
#define CHAINED8_H_
using namespace std;

template<class Key, class T, class Hash, class Allocator>
class Chained8 {

	Key size;
	Key capacity;
	Cnode<Key,T>** table;

	Cnode<Key,T>* b;
	Cnode<Key,T>* e;
	Hash function;
	Allocator allocator;

public:
	Chained8(Key capacity,Key bulkcapacity){

		size = 0;
		this->capacity = capacity;
		function.initialize(capacity);
		allocator.Allocate(bulkcapacity);
		table = new Cnode<Key,T>*[this->capacity];
		b = table[0];
		e = table[capacity-1];
		for (Key i = 0; i < capacity; i++)
			table[i] = NULL;
	}
	~Chained8(){

		delete[] table;

	}
	pair<Cnode<Key,T>*,bool> emplace(Key key, T value){

		Key hashindex = function.hashcode(key,capacity) % capacity;
		pair<Cnode<Key,T>*,bool> returnpair;
		Cnode<Key,T>* entry = table[hashindex];
		Cnode<Key,T>* prev = NULL;
		
		while ( entry != NULL && entry->key != key)
		{
			prev = entry;
			entry = entry->next;
		}
	
		if (entry == NULL)
		{
			entry = allocator.makenode(key,value);
			if (prev == NULL)
			{
				table[hashindex] = entry;
			}
			else
			{
				prev->next = entry;
			}
			returnpair = pair<Cnode<Key,T>*,bool>(entry,true);
			size++;
		}
		else
		{
			returnpair = pair<Cnode<Key,T>*,bool>(entry,false);//not entry->next just entry
		}

		return returnpair;
	}
	bool erase(Key key){

		Key hashindex = function.hashcode(key,capacity) % capacity;
		bool deleted = false;
		Cnode<Key,T>* entry = table[hashindex];
		Cnode<Key,T>* prev = NULL;

		if (entry == NULL)
		{
			return false;

		}
		while (entry->key != key && entry->next != NULL)
		{
			prev = entry;
			entry = entry->next;
		}
		if(entry->key == key){
			if (prev != NULL){
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
				table[hashindex] = entry->next;
				if(allocator.isbulk()){
					*entry = Cnode<Key,T>();
				}
				else{
					delete entry;
				}
				deleted = true;
				size--;
			}
		}

		return deleted;

	}
	pair<Cnode<Key,T>*,bool> find(Key key){

		Key hashindex = function.hashcode(key,capacity) % capacity;
		Cnode<Key,T>* entry = table[hashindex];
		bool found = false;
//		Cnode<Key,T>* it;
/*		if(entry == NULL){
		   pair<Cnode<Key,T>*,bool> returnpair(entry,false);
		    return returnpair;
		}
*/
		while(entry != NULL && !found){
//			entry = entry->next;
			if(entry->key==key){
			//pair<Cnode<Key,T>*,bool> returnpair(entry,true);
                        //return returnpair;
			found = true;
			break;
			}
			entry = entry->next;
		}
	/*	if(entry->key==key){
                        pair<Cnode<Key,T>*,bool> returnpair(entry,true);
                        return returnpair;
		}
/*		if(entry == NULL){
		pair<Cnode<Key,T>*,bool> returnpair(e,false);
		return returnpair;
		}
*/		
		if(!found){
                        pair<Cnode<Key,T>*,bool> returnpair(e,false);
			return returnpair;
		}
                else{
                        pair<Cnode<Key,T>*,bool> returnpair(entry,true);
                        return returnpair;
		}

       
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
	Node<Key,T>* begin(){
		return &(*b);
	}
	Node<Key,T>* end(){
		return &(*e);
	}
	Node<Key,T>* begin(Key key){
		Key hashindex = function.hashcode(key,capacity) % capacity;
		Cnode<Key,T>* begin = table[hashindex];
		return begin;
	}
	Node<Key,T>* end(Key key){
		Key hashindex = function.hashcode(key,capacity) % capacity;
		Cnode<Key,T>* begin = table[hashindex];
		if(begin == NULL){
			return begin;
		}
		while(begin->next != NULL){
			begin = begin->next;
		}
		return begin;
	}
	Key getSize(){
		return size;
	}
	void clear(){
		allocator.clear();
		for (Key i = 0; i < capacity; i++){
			table[i] = NULL;
		}
		size = 0;
	}
	static string getName(){
		return "Chained8";
	}


};

#endif /* CHAINED8_H_ */
