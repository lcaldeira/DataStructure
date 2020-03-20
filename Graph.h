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
		virtual TypeV operator[](size_t index) const = 0;
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
	class SimpleGraph : public ProtoGraph<TypeV,bool,List<size_t>*,Vector>
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
		SimpleGraph<TypeV>(size_t init_cap = 0)
		{
			if(init_cap == ~0 || !init_cap)
				init_cap = 16;
			
			this->vertex = Vector<TypeV>(init_cap);
			this->relation = Vector< List<size_t>* >(init_cap);
		}
		
		//SimpleGraph<TypeV>& operator=(const SimpleGraph<TypeV>& v)
		
		~SimpleGraph<TypeV>()
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
		decltype(auto) operator==(SimpleGraph<T>& g)
		{
			size_t sz = g.getSize();
			
			if(sz != this->getSize())
				return false;
			
			Node<size_t>* n;
			
			for(size_t i=0; i < sz; i++)
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
		decltype(auto) operator!=(SimpleGraph<T>& g)
		{ return !(this->operator==(g)); }
		
		//acesso e manipulação
		TypeV operator[](size_t idx) const { return this->vertex[idx]; }
		
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
	
	/*template<typename TypeV, typename TypeR>
	class Graph2 : public ProtoGraph<TypeV,TypeR,SparseMatrix>
	{
	};
	
	template<typename TypeV, typename TypeR>
	class CompleteGraph : public ProtoGraph<TypeV,TypeR,Matrix>
	{
	};*/
	
	/*template<typename TypeV, typename TypeR>
	class SparseGraph : public SparseMatrix<TypeV,TypeR,Matrix>
	{
	};*/

	/*template<class TypeV, class TypeR>
	class Graph : public Container<TypeV>
	{
	protected:
		Vector<TypeV> *vertex;		//vetor de vértices
		Matrix<TypeR> *relation;	//matriz de relações
	
		void resize(size_t new_size)
		{
			size_t size = this->getSize();
			
			if(new_size == 0 || new_size == size)
				return;
			
			Matrix<TypeR> m = *relation;
			
			delete relation;
			relation = new Matrix<TypeR>(new_size,new_size);
			relation->fill((TypeR) 0);
			vertex->resize(new_size);
			
			size = (size < new_size ? size : new_size);
			
			for(size_t i=0; i<size; i++)
				for(size_t j=0; j<size; j++)
					relation->set(m[i][j], i,j);
		}
		
		TypeV* at(size_t index) const { return &(*vertex)[index]; }
		TypeR* at(size_t idx1, size_t idx2) const { return &(*relation)[idx1][idx2]; }
		
	public:
		Graph<TypeV,TypeR>()
		{
			vertex = nullptr;
			relation = nullptr;
		}
		
		Graph<TypeV,TypeR>(size_t init_cap)
		{
			static_assert(std::is_arithmetic<TypeR>::value, "Integral type required.");
			
			if(init_cap==0)
				throw;
			
			vertex = new Vector<TypeV>(init_cap);
			relation = new Matrix<TypeR>(init_cap,init_cap);
			relation->fill((TypeR) 0);
		}
		
		Graph<TypeV,TypeR>(const Graph<TypeV,TypeR>& g) : Graph<TypeV,TypeR>(g.countVertex())
		{
			size_t size = g.countVertex();
			
			for(int i=0; i < size; i++)
			{
				vertex->pushBack(g.getVertex(i));
				for(int j=0; j < size; j++)
					relation->set(g.getNthEdge(i,j), i,j);
			}
		}
		
		~Graph<TypeV,TypeR>()
		{
			if(vertex != nullptr)
			{
				delete vertex;
				vertex = nullptr;
			}
			if(relation != nullptr)
			{
				delete relation;
				relation = nullptr;
			}
		}
		
		Graph<TypeV,TypeR> operator=(Graph<TypeV,TypeR> g)
		{
			this->~Graph<TypeV,TypeR>();
			
			size_t size = g.countVertex();
			this->vertex = new Vector<TypeV>(size);
			this->relation = new Matrix<TypeR>(size,size);
			
			for(int i=0; i < size; i++)
			{
				vertex->pushBack(g.getVertex(i));
				for(int j=0; j < size; j++)
					relation->set(g.getEdge(i,j), i,j);
			}
			return *this;
		}
		
		//busca e verificação
		bool isAllocated() const { return (this->vertex != nullptr && this->relation != nullptr); }
		long int indexOf(TypeV v) const { return vertex->indexOf(v); }
		bool contains(TypeV v) const { return (indexOf(v) >= 0); }
		bool contains(TypeV v1, TypeV v2) const 
		{
			long int idx1 = indexOf(v1);
			long int idx2 = indexOf(v2);
			
			if(v1 < 0 || v2 < 0)
				return false;
			return (at(idx1,idx2) != nullptr);
		}
		
		size_t getSize() const { return countVertex() + countEdge(); }
		size_t countVertex() const { return vertex->getSize(); }
		size_t countEdge() const 
		{
			size_t qt = 0;
			size_t size = vertex->getSize();
			
			for(size_t i=0; i<size; i++)
				for(size_t j=0; j<size; j++)
					qt += (relation->get(i,j) != (TypeR) 0);
			return qt;
		}
		
		size_t degreeOf(TypeV v) const 
		{
			int idx = vertex->indexOf(v);
			
			if(idx < 0)
				return 0;
		
			size_t deg = 0;
			for(size_t j=0; j<vertex->getSize(); j++)
				deg += (relation->get(idx,j) != (TypeR) 0);
			return deg;
		}
		
		bool operator==(Graph<TypeV,TypeR>& g)
		{
			size_t qtV = this->countVertex();
			size_t qtE = this->countEdge();
			
			if(qtV != g.countVertex() || qtE != g.countEdge())
				return false;
			
			for(size_t i=0; i<qtV; i++)
				if(vertex->get(i) != g.getVertex(i))
					return false;
			
			for(size_t i=0; i<qtV; i++)
				for(size_t j=0; j<qtV; j++)
					if(relation->get(i,j) != g.getEdge(vertex->get(i), vertex->get(j)))	
						return false;
			
			return true;
		}
		
		//acesso e manipulação
		TypeV getVertex(size_t index) const { return vertex->get(index); }
		
		void insertVertex(TypeV v)
		{
			if(!vertex->contains(v))
			{
				size_t size = this->getSize();
				if(size == vertex->getCapacity())
					this->resize(size*2);
				vertex->pushBack(v);
			}
		}
		
		void removeVertex(TypeV v)
		{
			long int index = vertex->indexOf(v);
			if(index >= 0)
			{
				size_t size = vertex->getSize();
				vertex->erase(index);
			
				for(size_t i=0; i < size; i++)
				{
					for(size_t k=index; k < size-1; k++)
						(*relation)[i][k] = (*relation)[i][k+1];
					(*relation)[i][size-1] = (TypeR) 0;
				}
			
				for(size_t j=0; j<size; j++)
				{
					for(size_t k=index; k<size-1; k++)
						(*relation)[k][j] = (*relation)[k+1][j];
					(*relation)[size-1][j] = (TypeR) 0;
				}
			
				//if(size <= vertex->getCapacity()/4)
					//resize(size/4);
			}
		}
		
		TypeR getEdge(TypeV v1, TypeV v2) const 
		{
			long int idx1 = this->indexOf(v1);
			long int idx2 = this->indexOf(v2);
			
			return ((idx1 >= 0 && idx2 >= 0) ? *at(idx1,idx2) : (TypeR) 0);
		}
		
		TypeR getNthEdge(size_t idx1, size_t idx2) const 
		{
			size_t size = vertex->getSize();
			return ((idx1 < size && idx2 < size) ? *at(idx1,idx2) : (TypeR) 0);
		}
		
		void insertEdge(TypeV v1, TypeV v2, TypeR value)
		{
			long int idx1 = indexOf(v1);
			long int idx2 = indexOf(v2);
			
			if(idx1 >= 0 && idx2 >= 0)
				*at(idx1,idx2) = value;
		}
		
		void removeEdge(TypeV v1, TypeV v2, TypeR value)
		{
			long int idx1 = indexOf(v1);
			long int idx2 = indexOf(v2);
			
			if(idx1 >= 0 && idx2 >= 0)
				*at(idx1,idx2) = (TypeR) 0;
		}
		
		void clear()
		{
			
		}
		
		//outros
		//Vector<TypeV> getArrowsTo(TypeV)
		//Vector<TypeV> getArrowsFrom(TypeV)
		
		template<typename T=TypeV, isFundamental<T>* = nullptr>
		std::string strFormat(char c=' ')
		{
			std::stringstream ss;
			size_t size = vertex->getSize();
			for(int i=0; i<size; i++)
			{
				ss << (*vertex)[i] << ": ";
				for(int j=0; j<size; j++)
				{
					if((*relation)[i][j] != (TypeR) 0)
						ss << (*relation)[i][j] << ' ';
					else
						ss << "_ ";
				}
				ss << '\n';
			}
			return ss.str();
		}
		
		template<typename T=TypeV, isntFundamental<T>* = nullptr>
		std::string strFormat(char c=' '){ return "[]"; }
		
		operator std::string(){ return this->strFormat(); }
		virtual void print(){ std::cout << (this->strFormat()) << '\n'; }
	};*/
}
#endif
