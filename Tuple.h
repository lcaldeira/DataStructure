#ifndef TUPLE_H
#define TUPLE_H

#include "Sequence.h"

namespace DataStructure
{
	template<typename Type, size_t N>
	class Tuple : public Sequence<Type>
	{
	protected:
		Type data[N];
		
		Type* at(size_t index) const { return (Type*) &(this->data[index]); }
		
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
		
		Tuple(const Type array[])
		{
			for(int i=0; i<N; i++)
				this->data[i] = array[i];
		}
		
		/*Tuple(const Tuple<Type,N>& t)
		{
			for(int i=0; i<N; i++)
				this->set(t.get(i), i);
		}*/
		
		/*Tuple<Type,N> operator=(Tuple<Type,N> t)
		{
			for(int i=0; i<N; i++)
				this->set(t.get(i), i);
			return *this;
		}*/

		//busca e verificação
		bool isAllocated() const { return true; }
		size_t getSize() const { return N; }
		
		long int findNext(Type value, size_t idx, bool(*eqFunc)(Type&,Type&)) const 
		{
			for(size_t i=0; i<N; i++)
				if(eqFunc((Type&) this->data[(i+idx) % N], value))
					return ((i+idx) % N);
			return -1;
		}
		
		template<typename T=Type>
		auto contains(T value) const -> decltype(value == value, bool())
		{ return (this->indexOf(value) >= 0); }
		
		template<typename T=Type>
		auto indexOf(T value) const -> decltype(value == value, long())
		{ return this->findNext(value,0); }
		
		template<typename T=Type>
		auto findNext(T value, size_t idx) const -> decltype(value == value, long())
		{
			for(size_t i=0; i < N; i++)
				if(this->data[(i+idx) % N] == value)
					return ((i+idx) % N);
			return -1;
		}
		
		template<typename T=Type, size_t M>
		decltype(auto) operator==(Tuple<T,M>& t)
		{
			if(N != M)
				return false;
			for(size_t i=0; i<N; i++)
				if(this->data[i] != t[i])
					return false;
			return true;
		}
		
		template<typename T=Type, size_t M>
		decltype(auto) operator!=(Tuple<T,M>& t)
		{ return !(this->operator==(t)); }

		//acesso e manipulação
		Type& operator[](size_t index) const { return (Type&) this->data[index]; }
		
		//conversão para texto
		template<typename T=Type, isPrintable<T>* = nullptr>
		std::string strFormat(char c=' ') const
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
		std::string strFormat(char c=' ') const { return "()"; }
		
		operator std::string() const { return this->strFormat(); }
		virtual void print(){ std::cout << (this->strFormat()) << '\n'; }
	};
}
#endif
