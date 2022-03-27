/*
 * Cnode.h
 *
 *  Created on: Oct 27, 2021
 *      Author: Gabriel
 */
#include <bits/stdc++.h>
#include "Node.h"

#ifndef CNODE_H_
#define CNODE_H_

template<class Key, class T>
class Cnode :public Node<Key,T>{
public:
	Key key = -1;
	T value = 0;
	Cnode* next = NULL;
	
	Cnode(Key key, T value){
		this->key = key;
		this->value = value;
		next = NULL;
	}
	Cnode(){
		key = -1;
		next = NULL;
	}
	Key getKey(){
		//T * k = &key;
		return key;
	}
	T& getValue(){
		T * v = &value;
		return *v;
	}
};

#endif /* CNODE_H_ */
