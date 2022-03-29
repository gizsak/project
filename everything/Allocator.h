/*
 * Allocator.h
 *
 *  Created on: Oct 27, 2021
 *      Author: Gabriel
 */
#include "Cnode.h"
#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

template<class Key, class T>
class BulkAllocator {
    Cnode<Key,T>* bulktable;
    Key bulkcapacity = 0;
    Key nextnode = 0;
    Key currentnode = 0;
public:
	BulkAllocator(){
		//this->bulkcapacity = pow(2,26);
		//bulktable = new Cnode<Key,T>[bulkcapacity];
	}
	~BulkAllocator(){
//		cout<<nextnode<<endl;
		delete[] bulktable;
	}
	Cnode<Key,T>* makenode(Key key, T value){
		if(nextnode<bulkcapacity){
			bulktable[nextnode] = Cnode<Key,T>(key,value);
			nextnode++;
		}
		return &bulktable[nextnode-1];
	}
	void Allocate(Key capacity){
		this->bulkcapacity = capacity;
		bulktable = new Cnode<Key,T>[bulkcapacity];
	}
	void clear(){
		bulktable = new Cnode<Key,T>[bulkcapacity];
		nextnode = 0;
	}

	bool isbulk(){
		return true;
	}
};
template<class Key, class T>
class Allocator {

public:
	Allocator(){}

	Cnode<Key,T>* makenode(Key key, T value){

		return new Cnode<Key,T>(key,value);
	}
	bool isbulk(){
			return false;
		}
	void Allocate(Key capacity){
	;
	}
};

#endif /* ALLOCATOR_H_ */
