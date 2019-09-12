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
	void shuffle(Sequence<Type> *s, int intensity = 50)
	{
		randomSeed();
		intensity = abs(intensity) % 100;
		int qt_swaps = s->getSize() * intensity/100.0;
		unsigned int i, j;
		for(int cont = 0; cont < qt_swaps; cont++)
		{
			i = (unsigned int) randomNum(0, (s->getSize()-1));
			j = (unsigned int) randomNum(0, (s->getSize()-1));
			if(i != j)
				s->swap(i,j);
			else
				cont--;
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
}
#endif
