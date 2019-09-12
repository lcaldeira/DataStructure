#ifndef TUPLE_H
#define TUPLE_H

#include "Sequence.h"

namespace DataStructure
{
	template<class Type, size_t N>
	class Tuple : public Sequence<Type>
	{
	protected:
		Type data[N];
		
		Type* at(size_t index) const { return (int*) &(this->data[index]); }
		
		void insert(Type value, size_t index){};
		Type erase(size_t index){ return this->data[0]; };
		void clear(){};
	public:
		//construtor e destrutor
		Tuple(){}
		
		Tuple(Type default_value)
		{
			for(int i=0; i<N; i++)
				this->data[i] = default_value;
		}
		
		Tuple(const int array[])
		{
			for(int i=0; i<N; i++)
				this->data[i] = array[i];
		}
		
		Tuple(const Tuple<Type,N>& t)
		{
			for(int i=0; i<N; i++)
				this->set(t.get(i), i);
		}
		
		Tuple<Type,N> operator=(Tuple<Type,N> t)
		{
			for(int i=0; i<N; i++)
				this->set(t.get(i), i);
			return *this;
		}

		//busca e verificação
		bool isAllocated() const { return true; }
		size_t getSize() const { return N; }
		
		long int indexOf(Type value) const { return findNext(value,0); }
		
		long int findNext(Type value, size_t i0) const 
		{
			for(size_t i=0; i < N; i++)
				if(this->data[(i+i0) % N] == value)
					return ((i+i0) % N);
			return -1;
		}
		
		template<size_t M>
		bool operator==(Tuple<Type,M>& t)
		{
			size_t size = this->getSize();
			if(size != t.getSize())
				return false;
			for(size_t i=0; i<size; i++)
				if(this->data[i] != t.get(i))
					return false;
			return true;
		}
		
		template<size_t M>
		bool operator!=(Tuple<Type,M>& t){ return !(this->operator==(t)); }

		//acesso e manipulação
		Type& operator[](size_t index) const { return (Type&) this->data[index]; }

		void reverse()
		{
			for(size_t i=0; i < (N-1)/2; i++)
				this->swap(i, N-i-1);
		}
		
		//conversão para texto
		template<typename T=Type, isPrintable<T>* = nullptr>
		std::string strFormat(char c=' ')
		{
			std::stringstream ss;
			if(c == ' ')
			{
				ss << "(";
				for(size_t i=0; i < N; i++)
					ss << this->data[i] << (N-i!=1 ? "," : ")");
			}
			else if(c == 's')
				for(size_t i=0; i < N; i++)
					ss << this->data[i];
			else if(c == '[')
			{
				ss << "[ ";
				for(size_t i=0; i < N; i++)
					ss << this->data[i] << " ";
				ss << "]";
			}
			return ss.str();
		}
		
		template<typename T=Type, isntPrintable<T>* = nullptr>
		std::string strFormat(char c=' '){ return "()"; }
		
		operator std::string(){ return this->strFormat(); }
		virtual void print(){ std::cout << (this->strFormat()) << '\n'; }
	};
}
#endif
