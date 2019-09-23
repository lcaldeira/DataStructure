#ifndef SET_H
#define SET_H

#include "Vector.h"
#include "List.h"

namespace DataStructure
{
	template<typename Type, template<typename> class TypeC = List, 
				isBaseOf<Sequence<Type>,TypeC<Type>>* = nullptr>
	class Set : protected TypeC<Type>
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
				this->add(s.get(i));
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
				this->add(s.get(i));
			return (*this);
		}
		
		//~Set(){ this->~TypeC<Type>(); }
		
		//busca e verificação
		bool isAllocated() const { return TypeC<Type>::isAllocated(); }
		bool isEmpty() const { return TypeC<Type>::isEmpty(); }
		size_t getSize() const { return TypeC<Type>::getSize(); }
		
		long int indexOf(Type value, bool(*eqFunc)(Type&,Type&)) const 
		{ return Sequence<Type>::indexOf(value,eqFunc); }
		
		bool contains(Type value, bool(*eqFunc)(Type&,Type&)) const 
		{ return Sequence<Type>::contains(value,eqFunc); }
		
		bool compare(Set<Type>& s, bool(*eqFunc)(Type&,Type&)) const 
		{
			size_t size = this->getSize();
			if(size != s.getSize())
				return false;
			for(size_t i=0; i<size; i++)
				if(!s.contains(this->get(i),eqFunc))
					return false;
			return true;
		}
		
		template<typename T=Type>
		auto indexOf(T value) const -> decltype(value == value, long())
		{ return TypeC<Type>::indexOf(value); }
		
		template<typename T=Type>
		auto contains(T value) const -> decltype(value == value, bool())
		{ return TypeC<Type>::contains(value); }

		template<typename T=Type>
		decltype(auto) compare(Set<T>& s) const 
		{
			size_t size = this->getSize();
			if(size != s.getSize())
				return false;
			for(size_t i=0; i<size; i++)
				if(!s.contains(this->get(i)))
					return false;
			return true;
		}
		
		template<typename T=Type>
		decltype(auto) operator==(Set<T>& s){ return compare(s); }
		
		template<typename T=Type>
		decltype(auto) operator!=(Set<T>& s){ return !compare(s); }
		
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
		
		void add(Type value)
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
			for(int i=0; i < s.getSize(); i++)
				this->add(s.get(i));
		}
		template<template<typename> class Tc>
		void diffWith(Set<Type,Tc> s)
		{
			for(int i=0; i < s.getSize(); i++)
				this->remove(s.get(i));
		}
		template<template<typename> class Tc>
		void intersecWith(Set<Type,Tc> s)
		{
			for(int i=0; i < this->getSize(); i++)
				if(!s.contains(this->get(i)))
				{
					TypeC<Type>::erase(i);
					i--;
				}
		}
		
		//conversão para texto
		std::string strFormat(char c='{') const { return TypeC<Type>::strFormat(c); }
		operator std::string() const { return TypeC<Type>::strFormat('{'); }
		void print(){ std::cout << TypeC<Type>::strFormat('{') << '\n'; }
		
		template<class T>
		friend std::ostream& operator<<(std::ostream& ost, Set<T>& c);
	};
	
	template<typename T, template<typename> class Tc1, template<typename> class Tc2>
	Set<T> operator+(Set<T,Tc1> s1, Set<T,Tc2> s2)
	{
		Set<T> s = Set<T>(8);
		for(int i=0; i < s1.getSize(); i++)
			s.add(s1.get(i));
		for(int i=0; i < s2.getSize(); i++)
			s.add(s2.get(i));
		return s;
	}
	
	template<typename T, template<typename> class Tc1, template<typename> class Tc2>
	Set<T> operator-(Set<T,Tc1> s1, Set<T,Tc2> s2)
	{
		Set<T> s = Set<T>(8);
		for(int i=0; i < s1.getSize(); i++)
			if(!s2.contains(s1.get(i)))
				s.add(s1.get(i));
		return s;
	}
	
	template<typename T, template<typename> class Tc1, template<typename> class Tc2>
	Set<T> operator^(Set<T,Tc1> s1, Set<T,Tc2> s2)
	{
		Set<T> s = Set<T>(8);
		for(int i=0; i < s1.getSize(); i++)
			if(s2.contains(s1.get(i)))
				s.add(s1.get(i));
		return s;
	}

	template<typename Type>
	std::ostream& operator<<(std::ostream& ost, Set<Type>& s)
	{
		ost << s.strFormat();
		return ost;
	}
}
#endif
