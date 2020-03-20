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
		
		//função-hash padrão
		template<typename T=Type, isFundamental<T>* = nullptr>
		static size_t defaultgetHash(Type value, size_t size)
		{
			return size_t(value) % size;
		}
		
		Type* at(size_t index) const { return nullptr; }
		
	public:
		size_t (*getHash)(Type, size_t);
		
		//construtor e destrutor		
		template<typename T=Type, isFundamental<T>* = nullptr>
		HashSet<Type>(size_t init_cap = 16)
		{
			this->data = Vector<List<Type>>(init_cap);
			this->getHash = defaultgetHash;
			for(size_t i=0; i<init_cap; i++)
				this->data.pushBack(List<Type>());
		}
		
		template<typename T=Type, isntFundamental<T>* = nullptr>
		HashSet<Type>(size_t init_cap = 16)
		{
			this->data = Vector<List<Type>>(init_cap);
			this->getHash = nullptr;
			for(size_t i=0; i<init_cap; i++)
				this->data.pushBack(List<Type>());
		}
		
		/*HashSet<Type>(const HashSet<Type>& hs) : HashSet<Type>(hs.getHeigh())
		{
			List<Type> l = hs.toList();
			this->clear();
			this->data.resize(hs.getHeight());
			while(!l.isEmpty())
				this->add(l.popFront());
			return *this;
		}*/
		
		HashSet<Type>& operator=(const HashSet<Type>& hs)
		{
			List<Type> l = hs.toList();
			this->clear();
			this->data.resize(hs.getHeight());
			while(!l.isEmpty())
				this->add(l.popFront());
			return *this;
		}

		//busca e verificação
		bool isAllocated() const { return this->data.isAllocated(); }
		
		bool contains(Type value) const
		{
			size_t hash = getHash(value,this->getHeight());
			return (this->data[hash].indexOf(value) != ~0);
		}
		
		size_t getSize() const 
		{
			size_t size = 0;
			for(size_t i=0; i < this->data.getSize(); i++)
				size += this->data.get(i).getSize();
			return size;
		}
		
		size_t getHeight() const { return this->data.getSize(); }
		float avgCharge() const { return this->getSize() / (float)this->getHeight(); }
		
		template<typename T=Type>
		decltype(auto) operator==(HashSet<T>& hs)
		{
			Vector<Type> v1 = this->toVector();
			Vector<Type> v2 = hs.toVector();
			size_t sz = v1.getSize();
			
			if(sz != v2.getSize())
				return false;
			for(size_t i = 0; i < sz; i++)
				if(!v2.contains(v1[i]))
					return false;
			return true;
		}
		
		template<typename T=Type>
		decltype(auto) operator!=(HashSet<T>& hs)
		{ return !(this->operator==(hs)); }
		
		/*template<class T>
		friend bool operator==(HashSet<T>& hs1, HashSet<T>& hs2);
		
		template<class T>
		friend bool operator!=(HashSet<T>& hs1, HashSet<T>& hs2);*/
		
		
		/*template<typename T=Type>
		decltype(auto) operator==(HashSet<T>& hs)
		{
			size_t size = this->getSize();
			if(size != v.getSize())
				return false;
			for(size_t i=0; i<size; i++)
				if(this->data[i] != v[i])
					return false;
			return true;
		}*/	
		
		//acesso e manipulação
		void add(Type value)
		{
			size_t hash = this->getHash(value, this->getHeight());
			if(!this->data[hash].contains(value))
				this->data[hash].pushBack(value);
		}
		
		void remove(Type value)
		{
			size_t hash = this->getHash(value, this->getHeight());
			size_t index = this->data[hash].indexOf(value);
			if(index != ~0)
				this->data[hash].erase(index);
		}
		
		void resize(size_t new_height)
		{
			if(this->isEmpty() || !new_height)
				return;
			List<Type> l = this->toList();
			this->clear();
			this->data.resize(new_height);
			while(this->getHeight() < new_height)
				this->data.pushBack(List<Type>());
			while(!l.isEmpty())
				this->add(l.popFront());
		}
		
		void rebalance(){ this->resize(this->getSize() * 1.5); }
		
		void clear()
		{
			for(size_t i=0; i < this->data.getSize(); i++)
				this->data[i].clear();
		}
		
		//conversão para outros containers
		Vector<Type> toVector() const 
		{
			Vector<Type> v = Vector<Type>(this->getSize());
			for(size_t i=0; i < this->getHeight(); i++)
				for(size_t j=0; j < this->data[i].getSize(); j++)
					v.pushBack(this->data[i][j]);
			return v;
		}
		
		List<Type> toList() const 
		{
			List<Type> l = List<Type>();
			for(size_t i=0; i < this->getHeight(); i++)
				for(size_t j=0; j < this->data[i].getSize(); j++)
					l.pushBack(this->data[i][j]);
			return l;
		}
		
		//conversão para texto
		template<typename T=Type, isPrintable<T>* = nullptr>
		std::string strFormat(char c=' ') const 
		{
			std::stringstream ss;
			size_t size = this->getHeight();
			for(size_t i=0; i < size; i++)
			{
				if(c=' ')
					ss << (i==0 ? 'T' : '|') << ": ";
				ss << this->data[i].strFormat(c);
				if(i != size-1)
					ss << '\n';
			}
			return ss.str();
		}
		
		template<typename T=Type, isntPrintable<T>* = nullptr>
		std::string strFormat(char c=' ') const { return "[]"; }
		
		operator std::string() const { return this->strFormat(); }
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
