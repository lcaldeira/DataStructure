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
		
		virtual std::string strFormat(char c=' '){ return "{}"; }
		operator std::string(){ return this->strFormat(); }
		virtual void print(){ std::cout << (this->strFormat()) << '\n'; }
	};

	template<class Type>
	std::ostream& operator<<(std::ostream& ost, Container<Type>& c)
	{
		ost << c.strFormat();
		return ost;
	}
}
#endif
