#ifndef SET_H
#define SET_H

#include "Vector.h"
#include "List.h"

namespace DataStructure
{
	template<typename Type, template<typename> class TypeC = List, 
		isBaseOf<Sequence<Type>,TypeC<Type>>* = nullptr>
	class Set : public TypeC<Type>
	{
	public:
		//construtor e destrutor
		Set() : TypeC<Type>(){}
		
		Set(size_t init_cap) : TypeC<Type>(init_cap){}
		
		template<template<typename> class Tc>
		Set(const Set<Type,Tc>& s) : Set(s.getSize())
		{
			for(int i=0; i < s.getSize(); i++)
				TypeC<Type>::pushBack(s.get(i));
		}
		
		Set(const TypeC<Type>& s) : Set(s.getSize())
		{
			for(int i=0; i < s.getSize(); i++)
				this->insert(s.get(i));
		}
		
		template<template<typename> class Tc>
		Set<Type,TypeC>& operator=(Set<Type,Tc> s)
		{
			this->clear();
			for(int i=0; i < s.getSize(); i++)
				TypeC<Type>::pushBack(s.get(i));
			return (*this);
		}
		
		Set<Type,TypeC>& operator=(TypeC<Type> s)
		{
			this->clear();
			for(int i=0; i < s.getSize(); i++)
				this->insert(s.get(i));
			return (*this);
		}
		
		//~Set(){ this->~TypeC<Type>(); }
		
		//busca e verificação
		bool isAllocated() const { return TypeC<Type>::isAllocated(); }
		bool isEmpty() const { return TypeC<Type>::isEmpty(); }
		size_t getSize() const { return TypeC<Type>::getSize(); }
		long int indexOf(Type value) const { return TypeC<Type>::indexOf(value); }
		bool contains(Type value) const { return TypeC<Type>::contains(value); }
		
		bool operator==(Set<Type>& s)
		{
			size_t size = this->getSize();
			if(size != s.getSize())
				return false;
			for(size_t i=0; i<size; i++)
				if(!s.contains(this->get(i)))
					return false;
			return true;
		}
		
		bool operator!=(Set<Type>& s){ return !(this->operator==(s)); }
		
		//acesso e manipulação
		Type get(size_t index) const { return TypeC<Type>::get(index); }
		
		void set(Type value, size_t index)
		{
			long int idx = this->indexOf(value);
			if(idx < 0)
				TypeC<Type>::insert(value,index);
			else
				TypeC<Type>::swap(index,idx);
		}
		
		void insert(Type value)
		{
			if(!this->contains(value))
				TypeC<Type>::pushBack(value);
		}
		
		void remove(Type value)
		{
			long int idx = TypeC<Type>::indexOf(value);
			if(idx >= 0)
				TypeC<Type>::erase(idx);
		}
		
		void clear(){ TypeC<Type>::clear(); }
		
		//conversão para texto
		std::string strFormat(char c='{') const { return TypeC<Type>::strFormat(c); }
		operator std::string() const { return TypeC<Type>::strFormat('{'); }
		void print(){ std::cout << TypeC<Type>::strFormat('{') << '\n'; }
		
		//operações de conjunto
		template<typename T, template<typename> class Tc1, template<typename> class Tc2>
		friend Set<T> operator+(Set<T,Tc1> s1, Set<T,Tc2> s2);
		
		template<typename T, template<typename> class Tc1, template<typename> class Tc2>
		friend Set<T> operator-(Set<T,Tc1> s1, Set<T,Tc2> s2);
		
		template<typename T, template<typename> class Tc1, template<typename> class Tc2>
		friend Set<T> operator^(Set<T,Tc1> s1, Set<T,Tc2> s2);
		
		template<template<typename> class Tc>
		void unionWith(Set<Type,Tc> s)
		{
			Set<Type> s0 = ((*this)+s);
			clear();
			for(int i=0; i < s0.getSize(); i++)
				TypeC<Type>::pushBack(s0.get(i));
		}
		template<template<typename> class Tc>
		void intersecWith(Set<Type,Tc> s)
		{
			Set<Type> s0 = ((*this)^s);
			clear();
			for(int i=0; i < s0.getSize(); i++)
				TypeC<Type>::pushBack(s0.get(i));
		}
	};
	
	template<typename T, template<typename> class Tc1, template<typename> class Tc2>
	Set<T> operator+(Set<T,Tc1> s1, Set<T,Tc2> s2)
	{
		Set<T> s = Set<T>(8);
		for(int i=0; i < s1.getSize(); i++)
			s.insert(s1.get(i));
		for(int i=0; i < s2.getSize(); i++)
			s.insert(s2.get(i));
		return s;
	}
	
	template<typename T, template<typename> class Tc1, template<typename> class Tc2>
	Set<T> operator-(Set<T,Tc1> s1, Set<T,Tc2> s2)
	{
		Set<T> s = Set<T>(8);
		for(int i=0; i < s1.getSize(); i++)
			s.insert(s1.get(i));
		for(int i=0; i < s2.getSize(); i++)
			s.remove(s2.get(i));
		return s;
	}
	
	template<typename T, template<typename> class Tc1, template<typename> class Tc2>
	Set<T> operator^(Set<T,Tc1> s1, Set<T,Tc2> s2)
	{
		Set<T> s = Set<T>(8);
		for(int i=0; i < s1.getSize(); i++)
			if(s2.contains(s1.get(i)))
				s.insert(s1.get(i));
		return s;
	}
}
#endif
