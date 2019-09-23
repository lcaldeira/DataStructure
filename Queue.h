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
	public:
		//construtor e destrutor
		Queue<Type>() : List<Type>(){}
		Queue<Type>(size_t init_cap) : List<Type>(init_cap){}

		//acesso e manipulação
		void operator<<(Type value){ this->push(value); }
		void operator>>(Type& ref){ ref = this->pop(); }
		
		void push(Type value){ this->pushBack(value); }
		Type pop(){ return this->popFront(); }

		Type* first(){ return (this->size > 0 ? &this->head->next()->value : nullptr); }
		Type* last(){ return (this->size > 0 ? &this->tail->prev()->value : nullptr); }
	};
}
#endif
