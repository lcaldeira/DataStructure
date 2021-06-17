#ifndef GRAPH_H
#define GRAPH_H

#include "Vector.h"
#include "List.h"
#include "Matrix.h"

namespace DataStructure
{
	/*
	 * TypeV: type of vertices
	 * TypeR: type of relations
	 * TypeS: stored type in container
	 * TypeC: container type
	 */
	template<typename TypeV, typename TypeR, typename TypeS, 
				template<typename> typename TypeC, 
				isBaseOf<Container<TypeS>,TypeC<TypeS>>* = nullptr>
	class ProtoGraph : public Container<TypeV>
	{
	protected:	
		Vector<TypeV> vertex;
		TypeC<TypeS> relation;
		
		virtual void resize(size_t new_size) = 0;
		virtual TypeV* at(size_t index) const { return nullptr; }
		virtual void addRelation(size_t idx1, size_t idx2, TypeR weight) = 0;
		virtual void removeRelation(size_t idx1, size_t idx2) = 0;
	public:
		//busca e verificação
		bool isAllocated() const { return (vertex.isAllocated() && relation.isAllocated()); }
		bool isEmpty() const { return vertex.isEmpty(); }
		size_t getSize() const { return vertex.getSize(); }
		size_t getCapacity() const { return vertex.getCapacity(); }
		size_t indexOf(TypeV vtx) const { return vertex.indexOf(vtx); }
		bool contains(TypeV vtx) const { return vertex.contains(vtx); }
		virtual bool contains(TypeV vtx1, TypeV vtx2) const = 0;
		
		template<typename T=TypeV>
		decltype(auto) operator==(ProtoGraph<TypeV,TypeR,TypeS,TypeC>& g)
		{
			size_t sz = g.getSize();
			
			if(sz != this->getSize())
				return false;
			
			for(size_t i=0; i < sz; i++)
			{
				if(!g.contains(vertex[i]))
					return false;
				
				for(size_t j=0; j < sz; j++)
					if(g.contains(vertex[i], vertex[j]) != contains(vertex[i], vertex[j]))
						return false;
			}
			return true;
		}
		
		template<typename T=TypeV>
		decltype(auto) operator!=(ProtoGraph<TypeV,TypeR,TypeS,TypeC>& g)
		{	return !(this->operator==(g)); }
		
		//acesso e manipulação
		virtual TypeV operator[](size_t idx) const { return this->vertex[idx]; }
		virtual TypeR operator()(size_t index1, size_t index2) const = 0;
		
		virtual void addVertex(TypeV v) = 0;
		virtual void removeVertex(TypeV v) = 0;
		
		void shrink(){ resize(this->getSize()); }
		void clear(){ vertex.clear(); relation.clear(); }
		
		//conversão para texto
		std::string strFormat(char c=' ') const { return vertex.strFormat(c) + "\n" + relation.strFormat(c); }
		operator std::string() const { return this->strFormat(); }
		void print(){ std::cout << this->strFormat() << '\n'; }
	};
	
	template<typename TypeV>
	class Graph : public ProtoGraph<TypeV,bool,List<size_t>*,Vector>
	{
	protected:
		void resize(size_t new_cap)
		{
			size_t sz = this->getSize();
			size_t cap = this->getCapacity();
			
			if(new_cap == 0 || new_cap == ~0 || new_cap == cap)
				return;
			
			for(size_t i = new_cap; i < sz; i++)
				this->removeVertex(this->vertex[i]);
			
			this->vertex.resize(new_cap);
			this->relation.resize(new_cap);
		}
		
		void addRelation(size_t idx1, size_t idx2, bool weight)
		{
			size_t i = 0;
			Node<size_t>* n = this->relation[idx1]->nthNode(0);
			
			while(n->value < idx2)
			{
				n = n->next();
				i++;
			}
			
			if(n->value != idx2)
				this->relation[idx1]->insert(idx2,i);
		}
		
		void removeRelation(size_t idx1, size_t idx2)
		{
			size_t i = this->relation[idx1]->indexOf(idx2);
			if(i != ~0)
				this->relation[idx1]->erase(i);
		}
	public:
		//construtor e destrutor
		Graph(size_t init_cap = 0)
		{
			if(init_cap == ~0 || !init_cap)
				init_cap = 16;
			
			this->vertex = Vector<TypeV>(init_cap);
			this->relation = Vector< List<size_t>* >(init_cap);
		}
		
		//Graph& operator=(const Graph<TypeV>& v)
		
		~Graph()
		{
			if(this->isAllocated())
				this->clear();
		}
		
		//busca e verificação
		bool contains(TypeV vtx1, TypeV vtx2) const 
		{
			size_t idx1 = this->vertex.indexOf(vtx1);
			size_t idx2 = this->vertex.indexOf(vtx2);
			
			return this->operator()(idx1,idx2);
		}
		
		template<typename T=TypeV>
		decltype(auto) operator==(Graph<T>& g)
		{
			size_t i, sz = g.getSize();
			Node<size_t>* n;
			
			if(sz != this->getSize())
				return false;
			
			for(i=0; i < sz; i++)
			{
				if(!g.contains(this->vertex[i]))
					return false;
				
				n = this->relation[i]->nthNode(0);
				
				while(n->value != ~0)
				{
					if(!g.contains(this->vertex[i], this->vertex[n->value]))
						return false;
					n = n->next();
				}
			}
			return true;
		}
		
		template<typename T=TypeV>
		decltype(auto) operator!=(Graph<T>& g)
		{ return !(this->operator==(g)); }
		
		//acesso e manipulação		
		bool operator()(size_t idx1, size_t idx2) const 
		{
			if(idx1 < this->getSize() && idx2 < this->getSize())
			{
				Node<size_t>* n = this->relation[idx1]->nthNode(0);
				while(n->value < idx2)
					n = n->next();
				return (n->value == idx2);
			}
			return false;
		}
		
		void addVertex(TypeV v)
		{
			if(this->vertex.contains(v))
				return;
			
			size_t sz = this->getSize();
			
			if(sz == this->getCapacity())
				this->resize(sz * 1.5);
			
			this->vertex.pushBack(v);
			this->relation.pushBack(new List<size_t>);
			this->relation[sz]->nthNode(~0)->value = ~0;
		}
		
		void removeVertex(TypeV v)
		{
			size_t idx = this->indexOf(v);
			
			if(idx != ~0)
			{
				Node<size_t>* n;
				
				for(size_t i=0; i < this->getSize(); i++)
				{
					if(i == idx)
						continue;
					
					this->removeRelation(i,idx);
					n = this->relation[i]->nthNode(~0)->prev();
					
					while(n->value > idx && n->value != ~0)
					{
						n->value--;
						n = n->prev();
					}
				}
				delete this->relation[idx];
				this->relation.erase(idx);
				this->vertex.erase(idx);
			}
		}
		
		void addEdge(TypeV v1, TypeV v2)
		{
			size_t idx1 = this->vertex.indexOf(v1);
			size_t idx2 = this->vertex.indexOf(v2);
			
			if(idx1 != ~0 && idx2 != ~0)
			{
				this->addRelation(idx1,idx2,true);
				this->addRelation(idx2,idx1,true);
			}
		}
		
		void addArrow(TypeV v1, TypeV v2)
		{
			size_t idx1 = this->vertex.indexOf(v1);
			size_t idx2 = this->vertex.indexOf(v2);
			
			if(idx1 != ~0 && idx2 != ~0)
				this->addRelation(idx1,idx2,true);
		}
		
		void removeEdge(TypeV v1, TypeV v2)
		{
			size_t idx1 = this->vertex.indexOf(v1);
			size_t idx2 = this->vertex.indexOf(v2);
			
			if(idx1 != ~0 && idx2 != ~0)
			{
				this->removeRelation(idx1,idx2);
				this->removeRelation(idx2,idx1);
			}
		}
		
		void removeArrow(TypeV v1, TypeV v2)
		{
			size_t idx1 = this->vertex.indexOf(v1);
			size_t idx2 = this->vertex.indexOf(v2);
			
			if(idx1 != ~0 && idx2 != ~0)
				this->removeRelation(idx1,idx2);
		}
		
		void clear()
		{
			for(size_t i=0; i < this->getSize(); i++)
				delete this->relation[i];
			this->vertex.clear();
			this->relation.clear();
		}
		
		//conversão para texto
		std::string strFormat(char c=' ') const 
		{
			std::stringstream ss;
			Node<size_t>* n;
			
			for(size_t i=0; i < this->getSize(); i++)
			{
				ss << this->vertex[i] << ": ";
				n = this->relation[i]->nthNode(0);
				
				while(n->value != ~0)
				{
					ss << this->vertex[n->value] << ' ';
					n = n->next();
				}
				ss << '\n';
			}
			return ss.str();
		}
	};
}
#endif
