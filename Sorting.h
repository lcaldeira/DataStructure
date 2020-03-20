#ifndef SORTING_H
#define SORTING_H

#include "Sequence.h"
#include <time.h>

namespace DataStructure
{
	void randomSeed()
	{
		static unsigned int i = 0;
		srand(time(NULL)+i);
		i++;
	}
	
	int randomNum(int min, int max)
	{
		int div = (max - min) + 1;
		return rand()%(div == 0 ? 1 : div) + min;
	}
	
	template<typename Type>
	void shuffle(Sequence<Type> *s, size_t intensity = 50)
	{
		intensity = abs(intensity) % 100;
		size_t i, j, size = s->getSize();
		int qt_swaps = (size * intensity)/100.0;
		
		for(int cont = 0; cont < qt_swaps; cont++)
		{
			do
			{
				i = randomNum(0, size-1);
				j = randomNum(0, size-1);
			}
			while(i == j);
			s->swap(i,j);
		}
	}
	
	template<typename Type>
	void bubbleSort(Sequence<Type> *s)
	{
		int aux;
		bool flag;
		
		for(unsigned int i = s->getSize()-1; i > 0; i--)
		{
			flag = true;
			for(unsigned int j = 0; j < i; j++)
				if(s->get(j) > s->get(j+1))
				{
					s->swap(j,j+1);
					if(j > 0 && s->get(j) < s->get(j-1))
						flag = false;
				}
			if(flag)
				break;
		}
	}
	
	template<typename Type>
	void quickSort(Sequence<Type> *s, size_t a = 0, size_t b = ~0)
	{
		if(s->getSize() <= 1 || b <= a)
			return;
		else if(b == ~0)
			b = s->getSize()-1;
		
		size_t pivot=b, i=a, j=b-1;
		
		//particionamento do vetor
		while(i < j)
		{
			while(s->get(i) < s->get(pivot) && i<j)
				i++;
			while(s->get(j) > s->get(pivot) && j>i)
				j--;
			if(i==j)
				break;
			s->swap(i,j);
			i++;
		}
		//reposicionamento do pivô
		if(s->get(j) < s->get(pivot))
			j++;
		s->swap(j,pivot);
		pivot = j;
		//chamadas recursiva nos particionamentos
		quickSort(s,a,pivot-1);
		quickSort(s,pivot+1,b);
	}
}
#endif
