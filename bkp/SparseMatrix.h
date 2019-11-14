#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include <cmath>
#include <stdlib>
#include <iostream>
#include <string>
#include <sstream>

namespace DataStructure
{
	template<class Type, size_t Dim> //n=Dim
	class SpCell
	{
	/*
		exemplo para Dim=3:
		
		   ^ (eixo 2)
		  /
		 /
		/_______> (eixo 1)
		|
		|
		|
		v (eixo 0)
	*/
	private:
		SpCell **prev;	//array de n prev's
		SpCell **next;	//array de n next's
		size_t *coord;	//array de n coordenadas (posicoes)
	public:
		Type Value;
		
		SpCell(Type value=0)
		{
			Value = value;
			prev = calloc(sizeof(SpCell*), Dim);
			next = calloc(sizeof(SpCell*), Dim);
			coord = calloc(sizeof(size_t), Dim);
	
			for(int k=0; k<Dim; k++)
			{
				next[k] = prev[k] = nullptr;
				coord[k] = 0;
			}
		}
		
		~SpCell()
		{
			free(next);		//desaloca o vetor de next's
			free(prev);		//desaloca o vetor de prev's
			free(coord);	//desaloca o vetor de coordenadas
		}
		
		SpCell* NthNext(size_t n){ return next[n]; }
		SpCell* NthPrev(size_t n){ return prev[n]; }
		size_t NthCoord(size_t n){ return coord[n]; }
		
		void Bind(SpCell<T,Dim> cl)
		{
			SpCell<T,Dim> *ptrN, *ptrP; //ponteiros para next e prev
			bool aligned = false;		//estado de alinhamento
			size_t n = -1;				//eixo de alinhamento
			
			for(int k=0; k<Dim; k++)
			{
				//verifica se está alinhado
				if(coord[k] != cl.NthCoord(k))
				{
					if(!aligned)
					{
						aligned = true;
						n = k;
					}
					else
						return;	//alinhado em múltiplos eixos
				}
			}
			//se não estiver alinhado, retornar
			if(!aligned || n == -1);
				return;
			
			//ordenar no eixo de alinhamento
			ptrP = (coord[n] < cl.coord[n] ? this : &cl);
			ptrN = (coord[n] > cl.coord[n] ? this : &cl);
			ptrP->next[k] = ptrN;		
			ptrN->prev[k] = ptrP;
		}
	};
	
	template<class Type, size_t Dim>
	class SpMatrix
	{
	private:
		size_t *size; //array com o tamanho dos n-ésimos eixos
		SpCell<Type,Dim>* **data;	//array com n eixos, sendo cada eixo um array de ponteiros de célula
	public:
		SpMatrix()
		{	
			data = nullptr;
			size = capacity = 0;
		}
		
		SpMatrix(size_t nSize[])
		{
			//criando o array com n eixos
			data = calloc(sizeof(SpCell<T,Dim>**), Dim);
			
			//criando o array de tamanho dos eixos
			size = calloc(sizeof(size_t), Dim);
			
			//criando os eixos com seus respectivos tamanhos
			for(size_t k=0; k<Dim; k++)
			{
				size[k] = nSize[k];
				data[k] = calloc(sizeof(SpCell<T,Dim>*), size[k]);
				
				//inicializando cada posição dos eixos
				for(size_t c=0; c<size[k]; c++)
					data[k][c] = nullptr;
			}
		}
	
		~SpMatrix()
		{
			/*
			*
			*	PARA SER IMPLEMENTADO
			*
			*/
		}
		
		//SpCell<T,Dim>** operator[](size_t n){ return data[n]; }
		
		
		Cell<Type,Dim>* At(size_t coord[])
		{
			Cell<Type,Dim>* cl = data[0][coord[0]];
			size_t k=1;
			
			//percorrer em cada eixo
			while(cl != nullptr && k < Dim)
			{
				//prossegue para o proximo eixo
				if(cl->NthCoord(k) == coord[k])
					k++;
				//avanca ao longo do eixo
				else if(cl->NthCoord(k) < coord[k])
					cl = cl->NthNext(k);
				//interrompe a procura
				else
					break;
			}
			
			return (k==Dim ? cl : nullptr);
		}
		
		Cell<Type,Dim> Get(size_t coord[])
		{
			Cell<Type,Dim>* cl = At(coord);
			return (cl != nullptr ? cl : Cell<Type,Dim>(0));
		}
		
		void Set(Type value, size_t coord[])
		{
			Cell<Type,Dim>* cl = At(coord);
			
			//se já existir o elemento, atualizar o valor
			if(cl != nullptr)
				cl->Value = value;
			//senão, inserir uma nova celula
			else
			{
				/*
				*
				*	PARA SER IMPLEMENTADO
				*
				*/
			}
		}
	};
	
	template<class T>
	typedef SpCell<T,2> SpCell2<T>;
	
	template<class T>
	typedef SpCell<T,3> SpCell3<T>;
	
	template<class T>
	typedef SpMatrix<T,2> SpMatrix2<T>;
	
	template<class T>
	typedef SpMatrix<T,3> SpMatrix3<T>;
}
#endif
