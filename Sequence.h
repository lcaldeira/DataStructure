#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "Container.h"

namespace DataStructure
{
	template<class Type>
	class Sequence : public Container<Type>
	{
	public:
		//busca e verificação
		virtual long int findNext(Type value, size_t idx, bool(*eqFunc)(Type&,Type&)) const = 0;
		
		virtual long int indexOf(Type value, bool(*eqFunc)(Type&,Type&)) const { return findNext(value,0,eqFunc); }
		
		virtual bool contains(Type value, bool(*eqFunc)(Type&,Type&)) const
		{ return (indexOf(value,eqFunc) >= 0); }
		
		//acesso e manipulação
		virtual Type& operator[](size_t index) const = 0;
		
		virtual Type get(size_t index) const { return *this->at(index); }
		virtual void set(Type value, size_t index){ *this->at(index) = value; }
		
		virtual void swap(size_t index1, size_t index2)
		{
			Type value = this->get(index1);
			this->set(this->get(index2), index1);
			this->set(value, index2);
		}

		virtual void reverse()
		{
			size_t size = this->getSize();
			for(size_t i=0; i < (size-1)/2; i++)
				this->swap(i, size-i-1);
		}
		
		virtual void insert(Type value, size_t index) = 0;
		virtual Type erase(size_t index) = 0;
		virtual void clear() = 0;
		
		//virtual void remove(Type value) = 0;
		//virtual void splice() = 0;
		//virtual void count() = 0;
		//virtual void sort() = 0;
		//virtual void shuffle() = 0;
	};
}
#endif
