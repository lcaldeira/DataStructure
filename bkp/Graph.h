#ifndef GRAPH_H
#define GRAPH_H

#include "Vector.h"
#include "Matrix.h"

namespace DataStructure
{
	template<class TypeV, class TypeR>
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
	};
}
#endif
