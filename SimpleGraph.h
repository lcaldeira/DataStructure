#ifndef SIMPLEGRAPH_H
#define SIMPLEGRAPH_H

#include "Vector.h"
#include "List.h"

namespace DataStructure
{
	template<class Type>
	class SimpleGraph : public Container<Type>
	{
	protected:
		Vector<Type> *vertex;				//vetor de vértices
		Vector<List<Type>*> *relation;	//vetor de listas (relações)
	
		Type* at(size_t index) const { return &(*vertex)[index]; }
		
		void resize(size_t new_size)
		{
			size_t size = this->getSize();
			
			if(new_size == 0 || new_size == size)
				return;
			
			vertex->resize(new_size);
			relation->resize(new_size);
		}
	public:
		SimpleGraph()
		{
			vertex = nullptr;
			relation = nullptr;
		}
		
		SimpleGraph(size_t init_cap)
		{
			if(init_cap==0)
				throw;
			
			vertex = new Vector<Type>(init_cap);
			relation = new Vector<List<Type>*>(init_cap);
		}
		
		SimpleGraph(const SimpleGraph<Type>& g) : SimpleGraph<Type>(g.countVertex())
		{
			size_t size = g.countVertex();
			Vector<Type> v;
			//inserindo vértices
			for(int i=0; i < size; i++)
			{
				vertex->pushBack(g.getVertex(i));
				relation->pushBack(new List<Type>());
			}
			//inserindo arestas
			for(int i=0; i < size; i++)
			{
				v = g.edgesFrom((*vertex)[i]);
				while(!v.isEmpty())
					(*relation)[i]->pushBack(v.popBack());
			}
		}
		
		~SimpleGraph<Type>()
		{
			if(vertex != nullptr)
			{
				delete vertex;
				vertex = nullptr;
			}
			if(relation != nullptr)
			{
				for(int i=0; i < relation->getSize(); i++)
					delete (*relation)[i];
				delete relation;
				relation = nullptr;
			}
		}
		
		SimpleGraph<Type> operator=(SimpleGraph<Type> g)
		{
			this->~SimpleGraph<Type>();
			
			size_t size = g.countVertex();
			vertex = new Vector<Type>(size);
			relation = new Vector<List<size_t>>(size);
			Vector<Type> v;
			
			//inserindo vértices
			for(int i=0; i < size; i++)
			{
				vertex->pushBack(g.getVertex(i));
				relation->pushBack(new List<Type>());
			}
			//inserindo arestas
			for(int i=0; i < size; i++)
			{
				v = g.edgesFrom((*vertex)[i]);
				while(!v.isEmpty())
					(*relation)[i]->pushBack(v.popBack());
			}
			return *this;
		}
		
		//busca e verificação
		bool isAllocated() const { return (this->vertex != nullptr && this->relation != nullptr); }
		long int indexOf(Type v) const { return vertex->indexOf(v); }
		bool contains(Type v) const { return (indexOf(v) >= 0); }
		bool contains(Type v1, Type v2) const 
		{
			long int idx1 = indexOf(v1);
			return (idx1 >= 0 ? (*relation)[idx1]->contains(v2) : false);
		}
		
		size_t getSize() const { return countVertex() + countEdge(); }
		size_t countVertex() const { return vertex->getSize(); }
		size_t countEdge() const 
		{
			size_t qt = 0;
			size_t size = vertex->getSize();
			
			for(size_t i=0; i<size; i++)
				for(size_t j=0; j<size; j++)
					qt += (*relation)[i]->getSize();
			return qt/2;
		}
		
		size_t degreeOf(Type v) const 
		{
			int idx = vertex->indexOf(v);
			return ((idx >= 0) ? (*relation)[idx]->getSize() : 0);
		}
		
		bool operator==(SimpleGraph<Type>& g)
		{
			size_t qtV = this->countVertex();
			size_t qtE = this->countEdge();
			
			if(qtV != g.countVertex() || qtE != g.countEdge())
				return false;
			
			for(size_t i=0; i<qtV; i++)
				if(!g.contains(vertex->get(i)))
					return false;
			
			/*for(size_t i=0; i<qtV; i++)
				for(size_t j=0; j<qtV; j++)
					if(relation->get(i,j) != g.getEdge(vertex->get(i), vertex->get(j)))	
						return false;*/
			
			return true;
		}
		
		//acesso e manipulação
		Type getVertex(size_t index) const { return vertex->get(index); }
		
		void insertVertex(Type v)
		{
			if(!vertex->contains(v))
			{
				vertex->pushBack(v);
				relation->pushBack(new List<Type>());
			}
		}
		
		void removeVertex(Type v)
		{
			long int index = vertex->indexOf(v);
			if(index >= 0)
			{
				vertex->erase(index);
				delete (*relation)[index];
				relation->erase(index);
				
				long int idx0;
				for(size_t i=0; i < vertex->getSize(); i++)
				{
					idx0 = (*relation)[i]->indexOf(v);
					if(idx0 >= 0)
						(*relation)[i]->erase(idx0);
				}
			}
		}
		
		bool getEdge(Type v1, Type v2) const { return this->contains(v1,v2); }
		
		void insertEdge(Type v1, Type v2)
		{
			long int idx1 = indexOf(v1);
			if(idx1 >= 0)
				if(!(*relation)[idx1]->contains(v2))
				{
					List<Type> *l = (*relation)[idx1];
					int idx0 = l->getSize();
					for(int i=0; i < l->getSize(); i++)
						if(l->get(i) > v2)
						{
							idx0 = i;
							break;
						}
					l->insert(v2,idx0);
				}
		}
		
		void removeEdge(Type v1, Type v2)
		{
			long int idx1 = indexOf(v1);
			if(idx1 >= 0)
			{
				long int idx0 = (*relation)[idx1]->indexOf(v2);
				if(idx0 >= 0)
					(*relation)[idx1]->erase(idx0);
			}
		}
		
		void clear()
		{
			this->vertex->clear();
			for(int i=0; i < relation->getSize(); i++)
				delete (*relation)[i];
			this->relation->clear();
		}
		
		Vector<Type> edgesFrom(Type v) const 
		{
			long int idx = this->indexOf(v);
			if(idx >= 0)
			{
				List<Type> rel = *(*relation)[idx];
				Vector<Type> vec = Vector<Type>(rel.getSize());
				while(!rel.isEmpty())
					vec.pushBack(rel.popFront());
				return vec;
			}
			else
				return Vector<Type>(1);
		}
		
		template<typename T=Type, isFundamental<T>* = nullptr>
		std::string strFormat(char c=' ')
		{
			std::stringstream ss;
			Vector<size_t> vec;
			size_t size = vertex->getSize();
			
			for(int i=0; i<size; i++)
			{
				ss << (*vertex)[i] << ": ";
				ss << edgesFrom((*vertex)[i]).strFormat();
				ss << '\n';
			}
			return ss.str();
		}
		
		template<typename T=Type, isntFundamental<T>* = nullptr>
		std::string strFormat(char c=' '){ return "[]"; }
		
		operator std::string(){ return this->strFormat(); }
		virtual void print(){ std::cout << (this->strFormat()) << '\n'; }
	};
}
#endif
