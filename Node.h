/*
 * Node.h
 *
 *  Created on: Oct 23, 2021
 *      Author: Gabriel
 */

#ifndef NODE_H_
#define NODE_H_

template<class Key, class T>

class Node {
public:
	T value = 0;
	Key key = 0;

	Node(Key key, T value){
		this->value = value;
	    this->key = key;
	 }
	Node(){
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

#endif /* NODE_H_ */
