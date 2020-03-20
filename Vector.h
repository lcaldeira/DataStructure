#ifndef VECTOR_H
#define VECTOR_H

#include "Sequence.h"

namespace DataStructure
{
	template<class Type>
	class Vector : public Sequence<Type>
	{
	protected:
		Type* data;
		size_t size;
		size_t capacity;
		
		Type* at(size_t index) const { return &(this->data[index]); }
	public:
		//construtor e destrutor
		Vector<Type>()
		{
			data = nullptr;
			size = capacity = 0;
		}
		
		Vector<Type>(size_t init_cap)
		{
			this->capacity = (init_cap > 0 ? init_cap : 10);
			this->data = new Type[this->capacity];
			this->size = 0;
		}
		
		Vector<Type>(const Vector<Type>& v) : Vector<Type>(v.getCapacity())
		{
			this->size = v.getSize();
			for(int i=0; i < this->size; i++)
				data[i] = v.get(i);
		}

		~Vector<Type>()
		{
			if(this->data != nullptr)
			{
				delete[] this->data;
				this->data = nullptr;
			}
		}
		
		Vector<Type>& operator=(const Vector<Type>& v)
		{
			/*this->~Vector<Type>();
			this->size = v.getSize();
			this->capacity = v.getCapacity();
			this->data = new Type[this->capacity];
			
			for(int i=0; i < v.getSize(); i++)
				data[i] = v[i];
			return *this;*/
			
			if(this->capacity < v.getSize() || !isAllocated())
			{
				this->~Vector<Type>();
				this->capacity = v.getCapacity();
				this->data = new Type[this->capacity];
			}
			this->size = v.getSize();
			
			for(int i=0; i < this->size; i++)
				data[i] = v[i];
			return *this;
		}

		//busca e verificação
		bool isAllocated() const { return this->data != nullptr; }
		size_t getSize() const { return this->size; }
		size_t getCapacity() const { return this->capacity; }
		
		size_t findNext(Type value, size_t idx) const
		{
			for(size_t i=idx; i < this->size; i++)
				if(this->data[i] == value)
					return i;
			for(size_t i=0; i < idx; i++)
				if(this->data[i] == value)
					return i;
			return ~0;
		}
		
		template<typename T=Type>
		decltype(auto) operator==(Vector<T>& v)
		{
			size_t size = this->getSize();
			if(size != v.getSize())
				return false;
			for(size_t i=0; i<size; i++)
				if(this->data[i] != v[i])
					return false;
			return true;
		}
		
		template<typename T=Type>
		decltype(auto) operator!=(Vector<T>& v)
		{ return !(this->operator==(v)); }

		//acesso e manipulação
		Type& operator[](size_t index) const { return this->data[index]; }
		
		void insert(Type value, size_t index)
		{
			if(this->size == this->capacity)
				resize(2 * this->capacity);

			for(size_t i = this->size; i > index; i--)
				this->data[i] = this->data[i-1];

			this->data[index] = value;
			this->size++;
		}

		Type erase(size_t index)
		{
			Type t = this->data[index];

			if(index < this->size-1)
				for(size_t i=index; i < this->size-1; i++)
					this->data[i] = this->data[i+1];

			this->size--;
			return t;
		}

		void pushFront(Type value){ insert(value, 0); }
		void pushBack(Type value){ insert(value, this->size); }
		Type popFront(){ return erase(0); }
		Type popBack(){ return erase(this->size-1); }
		
		void resize(size_t new_cap)
		{
			this->capacity = new_cap;
			Type *old_data = this->data;
			this->data = new Type[this->capacity];

			if(this->size > this->capacity)
				this->size = this->capacity;
			
			for(size_t i=0; i < this->size; i++)
				this->data[i] = old_data[i];
			
			delete[] old_data;
		}
		
		void shrink(){ this->resize(this->size); }
		void clear(){ this->size = 0; }
		
		//conversão para texto
		template<typename T=Type, isPrintable<T>* = nullptr>
		std::string strFormat(char c=' ') const 
		{
			std::stringstream ss;
			if(c == ' ')
				for(size_t i=0; i < this->size; i++)
					ss << this->data[i] << " ";
			else if(c == 's')
				for(size_t i=0; i < this->size; i++)
					ss << this->data[i];
			else if(c == '{')
				ss << "{ " << strFormat() << "}";
			else if(c == '[')
			{
				ss << "[" << this->size;
				ss << "/" << this->capacity;
				ss << "] " << strFormat();
			}
			return ss.str();
		}
		
		template<typename T=Type, isntPrintable<T>* = nullptr>
		std::string strFormat(char c=' ') const { return Container<Type>::strFormat(c); }
		
		operator std::string() const { return this->strFormat(); }
		virtual void print(){ std::cout << (this->strFormat()) << '\n'; }
	};
}
#endif
