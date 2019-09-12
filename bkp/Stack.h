#ifndef STACK_H
#define STACK_H

#include "Vector.h"

namespace DataStructure
{
	template<class Type>
	class Stack : public Vector<Type>
	{
	private:
		void insert(Type value, size_t index){ Vector<Type>::insert(value, index); }
		Type erase(size_t index){ return Vector<Type>::erase(index); }
		void pushFront(Type value){ Vector<Type>::pushFront(value); }
		void pushBack(Type value){ Vector<Type>::pushBack(value);  }
		Type popFront(){ return Vector<Type>::popFront();  }
		Type popBack(){ return Vector<Type>::popBack();  }
		long int indexOf(Type value){ return this->findNext(value,0); }
		long int findNext(Type value, size_t i0){ return Vector<Type>::findNext(value, i0); }
	public:
		//construtor e destrutor
		Stack<Type>(size_t init_cap) : Vector<Type>(init_cap){}

		//acesso e manipulação
		void operator<<(Type value){ this->push(value); }
		void operator>>(Type& ref){ ref = this->pop(); }
		
		Type* first(){ return (this->size > 0 ? &this->data[0] : nullptr); }
		Type* last(){ return (this->size > 0 ? &this->data[this->size-1] : nullptr); }
		
		void push(Type value){ this->pushBack(value); }
		Type pop(){ return this->popBack(); }
	};
	
	
}
#endif
