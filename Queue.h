#ifndef QUEUE_H
#define QUEUE_H

#include "List.h"

namespace DataStructure
{
	template<class Type>
	class Queue : public List<Type>
	{
	private:
		void insert(Type value, size_t index){ List<Type>::insert(value, index); }
		Type erase(size_t index){ return List<Type>::erase(index); }
		void pushFront(Type value){ List<Type>::pushFront(value); }
		void pushBack(Type value){ List<Type>::pushBack(value);  }
		Type popFront(){ return List<Type>::popFront();  }
		Type popBack(){ return List<Type>::popBack();  }
		long int indexOf(Type value) const { return this->findNext(value,0); }
		long int findNext(Type value, size_t i0) const { return List<Type>::findNext(value, i0); }
	public:
		//construtor e destrutor
		Queue<Type>() : List<Type>(){}

		//acesso e manipulação
		void operator<<(Type value){ this->push(value); }
		void operator>>(Type& ref){ ref = this->pop(); }

		Type* first(){ return (this->size > 0 ? &this->head->next()->value : nullptr); }
		Type* last(){ return (this->size > 0 ? &this->tail->prev()->value : nullptr); }
		
		void push(Type value){ this->pushBack(value); }
		Type pop(){ return this->popFront(); }
	};
}
#endif
