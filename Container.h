#ifndef CONTAINER_H
#define CONTAINER_H

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdint>
#include <initializer_list>

#include "../Utilities/TypeTraits.h"

namespace DataStructure
{
	using namespace TypeTraits;
	
	template<typename Type>
	using init_list = std::initializer_list<Type>;
	
	template<typename Type>
	class Container
	{
	protected:
		virtual Type* at(size_t index) const = 0;
	public:
		//construtor e destrutor (modelos)
		/*
			Class() {}	// default constructor
			Class(Args ...) {}	// parameterized constructor
			Class(const Class& C) {} // copy constructor
			Class& operator=(const Class& C) {} // copy assignment
			friend Class& operator&=(Class& C1, Class& C2) {} // reference assignment
			~Class() {} // destructor
		*/
		
		//busca e verificação
		virtual size_t getSize() const = 0;
		virtual bool isEmpty() const { return !this->getSize(); }
		virtual bool isAllocated() const = 0;
		virtual bool contains(Type value) const = 0;
		
		template<typename T=Type>
		decltype(auto) operator==(Container<T>& c)
		{
			size_t size = this->getSize();
			if(size != c.getSize())
				return false;
			for(size_t i=0; i<size; i++)
				if(*this->at(i) != *c.at(i))
					return false;
			return true;
		}
		
		template<typename T=Type>
		decltype(auto) operator!=(Container<T>& c)
		{ return !(this->operator==(c)); }
		
		/*auto operator==(Container<T>& c) -> decltype(*this == 
		{
			//static_assert(std::is_fundamental<Type>::value, "Fundamental type required.");
			size_t size = this->getSize();
			if(size != c.getSize())
				return false;
			for(size_t i=0; i<size; i++)
				if(*this->at(i) != *c.at(i))
					return false;
			return true;
		}
		
		bool operator!=(Container<Type>& c){ return !(this->operator==(c)); }*/
		
		//conversão para texto		
		virtual std::string strFormat(char c=' ') const { return "{-}"; }
		operator std::string() const { return this->strFormat(); }
		virtual void print(){ std::cout << (this->strFormat()) << '\n'; }
		
		template<class T>
		friend std::ostream& operator<<(std::ostream& ost, Container<T>& c);
	};

	template<typename Type, template<typename> class TypeC, 
				isBaseOf<Container<Type>,TypeC<Type>>* = nullptr>
	std::ostream& operator<<(std::ostream& ost, TypeC<Type>& c)
	{
		ost << c.strFormat();
		return ost;
	}
}
#endif
