#ifndef HASHSET_H
#define HASHSET_H

#include "Vector.h"
#include "List.h"

namespace DataStructure
{
	template<class Type>
	class HashSet : public Container<Type>
	{
	protected:
		Vector<List<Type>> data;
		
		template<typename T=Type, isFundamental<T>* = nullptr>
		size_t getHash(Type value)
		{
			size_t hash = (size_t)(value);
			return (hash) % this->data.getSize();
		}
		
		template<typename T=Type, isntFundamental<T>* = nullptr>
		size_t getHash(Type value)
		{
			size_t size = sizeof(Type) * 1;
			char *ptr = (char*) &value;
			size_t hash = 0;
			
			for(int i=0; i < size; i++)
				hash += (size_t) *(ptr+i);
			return (hash) % this->data.getSize();
		}
		
		Type* at(size_t index) const { return nullptr; }
		
	public:
		//construtor e destrutor		
		HashSet<Type>(size_t init_cap = 16)
		{
			this->data = Vector<List<Type>>(init_cap);
			
			for(size_t i=0; i<init_cap; i++)
				this->data.pushBack(List<Type>());
		}
		
		HashSet<Type>(const HashSet<Type>& hs) : HashSet<Type>(hs.getHeigh())
		{
			List<Type> l = hs.toList();
			while(!l.isEmpty())
				this->insert(l.popFront());
		}
		
		HashSet<Type> operator=(HashSet<Type> hs)
		{
			this->~HashSet<Type>();
			List<Type> l = hs.toList();
			while(!l.isEmpty())
				this->insert(l.popFront());
			return *this;
		}

		//busca e verificação
		size_t getSize() const 
		{
			size_t size = 0;
			for(size_t i=0; i < this->data.getSize(); i++)
				size += this->data.get(i).getSize();
			return size;
		}
		
		size_t getHeigh() const { return this->data.getSize(); }
		
		float avgCharge() const 
		{
			float sizeV = this->data.getSize();
			return (sizeV == 0 ? 0 : this->getSize() / sizeV);
		}
		
		/*long int indexOf(Type value)
		{
			long int hash = (long int) this->getHash(value);
			long int idx0 = this->data[hash].indexOf(value);
			return (idx0 < 0 ? -1 : hash);
		}*/

		/*template<class T>
		friend bool operator==(HashSet<T>& hs1, HashSet<T>& hs2);
		
		template<class T>
		friend bool operator!=(HashSet<T>& hs1, HashSet<T>& hs2);*/
		
		//acesso e manipulação
		void insert(Type value)
		{
			size_t hash = getHash(value);
			if(!this->data[hash].contains(value))
				this->data[hash].pushBack(value);
		}
		
		void remove(Type value)
		{
			size_t hash = getHash(value);
			long int index = this->data[hash].indexOf(value);
			if(index >= 0)
				this->data[hash].remove(index);
		}
		
		//conversão para outros containers
		Vector<Type> toVector() const 
		{
			Vector<Type> v = Vector<Type>(this->getSize());
			for(size_t i=0; i < this->getHeigh(); i++)
				for(size_t j=0; j < this->data[i].getSize(); i++)
					v.pushBack(this->data[i][j]);
			return v;
		}
		
		List<Type> toList() const 
		{
			List<Type> l = List<Type>();
			for(size_t i=0; i < this->getHeigh(); i++)
				for(size_t j=0; j < this->data.get(i).getSize(); i++)
					l.pushBack(this->data.get(i).get(j));
			return l;
		}
		
		//conversão para texto
		template<typename T=Type, isFundamental<T>* = nullptr>
		std::string strFormat(char c=' ')
		{
			std::stringstream ss;
			size_t size = this->data.getSize();
			for(size_t i=0; i < size; i++)
			{
				if(c=' ')
					ss << "|: ";
				ss << this->data[i].strFormat(c);
				if(i != size-1)
					ss << '\n';
			}
			return ss.str();
		}
		
		template<typename T=Type, isntFundamental<T>* = nullptr>
		std::string strFormat(char c=' '){ return "[]"; }
		
		operator std::string(){ return this->strFormat(); }
		virtual void print(){ std::cout << (this->strFormat()) << '\n'; }
	};
	
	/*template<class T>
	bool operator==(HashSet<T>& hs1, HashSet<T>& hs2)
	{
		size_t size = hs1.data.getSize();
		if(size != hs2.data.getSize())
			return false;
		for(int i=0; i<size; i++)
			if(hs1.data[i] != hs2.data[i])
				return false;
		return true;
	}

	template<class T>
	bool operator!=(HashSet<T>& hs1, HashSet<T>& hs2)
	{
		return !(hs1 == hs2);
	}*/
}
#endif
