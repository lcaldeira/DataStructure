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
		static size_t getHash(Type value, size_t size)
		{
			return size_t(value) % size;
		}
		
		/*template<typename T=Type, isntFundamental<T>* = nullptr>
		size_t getHash(Type value)
		{
			size_t size = sizeof(Type) * 1;
			char *ptr = (char*) &value;
			size_t hash = 0;
			
			for(int i=0; i < size; i++)
				hash += (size_t) *(ptr+i);
			return (hash) % this->data.getSize();
		}*/
		
		Type* at(size_t index) const { return nullptr; }
		
	public:
		size_t (*hashFunc)(Type, size_t);
		
		//construtor e destrutor		
		template<typename T=Type, isFundamental<T>* = nullptr>
		HashSet<Type>(size_t init_cap = 16)
		{
			this->data = Vector<List<Type>>(init_cap);
			this->hashFunc = getHash;
			for(size_t i=0; i<init_cap; i++)
				this->data.pushBack(List<Type>());
		}
		
		template<typename T=Type, isntFundamental<T>* = nullptr>
		HashSet<Type>(size_t init_cap = 16)
		{
			this->data = Vector<List<Type>>(init_cap);
			this->hashFunc = nullptr;
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
		
		HashSet<Type> operator=(HashSet<Type> hs)
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
			size_t hash = hashFunc(value,this->getHeight());
			return this->data[hash].contains(value);
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

		/*template<class T>
		friend bool operator==(HashSet<T>& hs1, HashSet<T>& hs2);
		
		template<class T>
		friend bool operator!=(HashSet<T>& hs1, HashSet<T>& hs2);*/
		
		//acesso e manipulação
		void add(Type value)
		{
			size_t hash = this->hashFunc(value, this->getHeight());
			if(!this->data[hash].contains(value))
				this->data[hash].pushBack(value);
		}
		
		void remove(Type value)
		{
			size_t hash = this->hashFunc(value, this->getHeight());
			long int index = this->data[hash].indexOf(value);
			if(index >= 0)
				this->data[hash].erase(index);
		}
		
		void reHash()
		{
			if(this->isEmpty())
				return;
			List<Type> l = this->toList();
			this->clear();
			this->data.resize(l.getSize());
			while(this->getHeight() < l.getSize())
				this->data.pushBack(List<Type>());
			while(!l.isEmpty())
				this->add(l.popFront());
		}
		
		void clear()
		{
			for(size_t i=0; i<this->data.getSize(); i++)
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
