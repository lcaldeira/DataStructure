#ifndef CONTAINER_H
#define CONTAINER_H

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <type_traits>

namespace TypeTraits
{
	template<typename Type>
	using isFundamental = std::enable_if_t<std::is_fundamental<Type>::value>;
	
	template<typename Type>
	using isntFundamental = std::enable_if_t<!std::is_fundamental<Type>::value>;
	
	template<class Base, class Derived>
	using isBaseOf = std::enable_if_t<std::is_base_of<Base, Derived>::value>;
	
	template<class TypeA, class TypeB>
	using isConvertible = std::enable_if_t<std::is_convertible<TypeA,TypeB>::value>;
	
	template<class TypeA, class TypeB>
	using isntConvertible = std::enable_if_t<!std::is_convertible<TypeA,TypeB>::value>;
	
	template<class Type>
	using isPrintable = std::enable_if_t<
		std::is_fundamental<Type>::value || 
		std::is_convertible<Type,std::string>::value || 
		std::is_assignable<Type,std::string>::value>;
	
	template<class Type>
	using isntPrintable = std::enable_if_t<
		!std::is_fundamental<Type>::value && 
		!std::is_convertible<Type,std::string>::value && 
		!std::is_assignable<Type,std::string>::value>;
}

namespace DataStructure
{
	using namespace TypeTraits;
	
	template<typename Type>
	class Container
	{
	protected:
		virtual Type* at(size_t index) const = 0;
	public:
		//busca e verificação
		virtual size_t getSize() const = 0;
		virtual bool isEmpty() const { return !this->getSize(); }
		virtual bool isAllocated() const = 0;

		bool operator==(Container<Type>& c)
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
		
		bool operator!=(Container<Type>& c){ return !(this->operator==(c)); }
		
		//conversão para texto
		template<class T>
		friend std::ostream& operator<<(std::ostream& ost, Container<T>& c);
		
		virtual std::string strFormat(char c=' ') const { return "{-}"; }
		operator std::string() const { return this->strFormat(); }
		virtual void print(){ std::cout << (this->strFormat()) << '\n'; }
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
