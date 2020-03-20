#ifndef LIST_H
#define LIST_H

#include "Sequence.h"

namespace DataStructure
{
	template<class Type>
	class List;
	
	template<class Type>
	class Node
	{
	protected:
		Node<Type> *nxt, *prv;
	public:
		Type value;
		
		Node<Type>(){ nxt = prv = nullptr; }
		Node<Type>(Type val) : Node<Type>(){ value = val; }
		
		Node<Type>* next(){ return nxt; }
		Node<Type>* prev(){ return prv; }
		
		friend class List<Type>;
	};

	template<class Type>
	class List : public Sequence<Type>
	{
	protected:
		Node<Type> *head = nullptr;
		Node<Type> *tail = nullptr;
		size_t size = 0;

		Type* at(size_t index) const { return &(this->nthNode(index)->value); }
	public:
		//construtores e destrutores
		List<Type>(size_t init_cap = 0)
		{
			this->head = new Node<Type>(); //nó inicial(cabeça de lista)
			this->head->nxt = this->head;
			this->tail = this->head;
			this->size = 0;
		}
		
		List<Type>(const List<Type>& l) : List<Type>()
		{
			for(int i=0; i < l.getSize(); i++)
				this->pushBack(l.get(i));
		}
		
		~List<Type>()
		{
			clear();
			if(this->head != nullptr)
				delete this->head;
		}
		
		List<Type>& operator=(const List<Type>& l)
		{
			this->clear();
			for(int i=0; i < l.getSize(); i++)
				this->pushBack(l.get(i));
			return *this;
		}

		//busca e verificação
		bool isAllocated() const { return (this->head != nullptr || this->tail != nullptr); }
		size_t getSize() const { return this->size; }
		
		size_t findNext(Type value, size_t idx) const 
		{
			Node<Type> *n = this->nthNode(idx);
			for(int i=0; i < this->size; i++)
			{
				if(n->value == value)
					return i;
				n = n->nxt;
			}
			return ~0;
		}
		
		bool isBaseNode(Node<Type>* n) const { return (n == this->head); }
		
		Node<Type>* nthNode(size_t idx) const 
		{
			Node<Type> *n = this->head; //ponteiro para o nó-cabeça (anterior ao nó 0)
			while(idx != ~0)
			{
				n = n->nxt;
				idx--;
			}
			return n;
		}
		
		template<typename T=Type>
		decltype(auto) operator==(List<T>& l)
		{
			if(this->size != l.getSize())
				return false;
			Node<Type> *n = this->head->nxt;
			for(int i=0; i < this->size; i++)
			{
				if(n->value != l.get(i))
					return false;
				n = n->nxt;
			}
			return true;
		}
		
		template<typename T=Type>
		decltype(auto) operator!=(List<Type>& l)
		{ return !(this->operator==(l)); }

		//acesso e manipulação
		Type& operator[](size_t index) const { return this->nthNode(index)->value; }

		void insert(Type value, size_t index)
		{
			Node<Type> *n0 = this->nthNode(index-1);
			Node<Type> *n1 = new Node<Type>(value);
			
			n1->nxt = n0->nxt;
			n1->prv = n0;
			n0->nxt->prv = n1;
			n0->nxt = n1;

			this->size++;
		}

		Type erase(size_t index)
		{
			Node<Type> *n0 = this->nthNode(index-1);
			Node<Type> *n1 = n0->nxt;
			
			n0->nxt = n1->nxt;
			n1->nxt->prv = n0;

			Type value = n1->value;
			this->size--;
			delete n1;

			return value;
		}

		void pushFront(Type value){ insert(value, 0); }
		void pushBack(Type value){ insert(value, this->size); }
		Type popFront(){ return this->erase(0); }
		Type popBack(){ return this->erase(this->size-1); }

		void swap(size_t index1, size_t index2)
		{
			if(index1 >= this->size || index2 >= this->size || index1 == index2)
				return;
			//next e prev do primeiro nó
			Node<Type> *node1 = this->nthNode(index1);
			Node<Type> *n1 = node1->next();
			Node<Type> *p1 = node1->prev();
			//next e prev do segundo nó
			Node<Type> *node2 = this->nthNode(index2);
			Node<Type> *n2 = node2->next();
			Node<Type> *p2 = node2->prev();
			//troca de endereços (parte 1)
			node1->nxt = n2;
			node1->prv = p2;
			node2->nxt = n1;
			node2->prv = p1;
			//troca de endereços (parte 2)
			n1->prv = node2;
			p1->nxt = node2;	
			n2->prv = node1;
			p2->nxt = node1;
		}
		
		void clear()
		{
			Node<Type> *n0 = this->head->nxt, *n1;
			while(n0 != this->head)
			{
				n1 = n0->nxt;
				delete n0;
				n0 = n1;
			}
			this->head->nxt = this->head;
			this->tail = this->head;
			this->size = 0;
		}
		
		//conversão para texto
		template<typename T=Type, isPrintable<T>* = nullptr>
		std::string strFormat(char c=' ') const 
		{
			Node<Type> *n = this->nthNode(0);
			std::stringstream ss;
			ss << "";
			if(c == ' ')
				while(n != this->head)
				{
					ss << n->value << " ";
					n = n->nxt;
				}
			else if(c == 's')
				while(n != this->head)
				{
					ss << n->value;
					n = n->nxt;
				}
			else if(c == '{')
				ss << "{ " << this->strFormat() << "}";
			else if(c == '[')
				ss << "[" << this->size << "] " << this->strFormat();
			else if(c == '>')
			{
				ss << "|-";
				while(n != this->head)
				{
					ss << "<" << n->value << ">-";
					n = n->nxt;
				}
				ss << "|";
			}
			return ss.str();
		}
		
		template<typename T=Type, isntPrintable<T>* = nullptr>
		std::string strFormat(char c=' ') const { return Container<Type>::strFormat(c); }
		
		operator std::string() const { return this->strFormat(); }
		virtual void print(){ std::cout << (this->strFormat()) << '\n'; }
	};
}
#endif
