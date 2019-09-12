#include "SparseMatrix.h"

cell* new_cell2d(Type value)
{
	int dim = 2;
	
	cell *cl = malloc(sizeof(cell));	
	cl->prev = calloc(sizeof(cell*), dim);
	cl->next = calloc(sizeof(cell*), dim);
	cl->coord = calloc(sizeof(unsigned int), dim);
	cl->value = value;
	
	for(int k=0; k<dim; k++)
	{
		cl->coord[k] = 0;
		cl->next[k] = NULL;
		cl->prev[k] = NULL;
	}
	
	return cl;
}

void free_cell2d(cell* cl)
{
	free(cl->coord);	//desaloca o vetor de coordenadas
	free(cl->next);		//desaloca o vetor de next's
	free(cl->prev);		//desaloca o vetor de prev's
	free(cl);			//desaloca a célula
}

void cell2d_bind(cell* cl1, cell* cl2)
{
	if(cl1 != NULL && cl2 != NULL)
	{
		cell *prev, *next;
		
		//se estiver alinhado em linha
		if((cl1->coord[0] == cl2->coord[0]) && (cl1->coord[1] != cl2->coord[1]))
		{
			//ordenar ao longo do eixo 1
			prev = (cl1->coord[1] < cl2->coord[1] ? cl1 : cl2);
			next = (cl1->coord[1] > cl2->coord[1] ? cl1 : cl2);
			//vincular ao longo do eixo 0
			prev->next[1] = next;		
			next->prev[1] = prev;
		}
		//se estiver alinhado em coluna
		else if((cl1->coord[1] == cl2->coord[1]) && (cl1->coord[0] != cl2->coord[0]))
		{
			//ordenar ao longo do eixo 0
			prev = (cl1->coord[0] < cl2->coord[0] ? cl1 : cl2);
			next = (cl1->coord[0] > cl2->coord[0] ? cl1 : cl2);
			//vincular ao longo do eixo 1
			prev->next[0] = next;			
			next->prev[0] = prev;
		}
		//senão, não vincular
	}
}

unsigned int matrix_nth_size(matrix *mx, unsigned int coord)
{
	return (coord < mx->dim ? mx->size[coord] : -1);
}

matrix* new_matrix2d(unsigned int m, unsigned int n)
{
	//alocando espaço para uma matriz de dimensão 2
	matrix *mx = malloc(sizeof(matrix));
	mx->dim = 2;
	
	//definindo o tamanho de cada eixo
	mx->size = malloc(sizeof(unsigned int) * mx->dim);
	mx->size[0] = m;
	mx->size[1] = n;
	
	//criando o array com eixos de células (a quantidade de eixos é a dimensão da matriz) 
	mx->data = malloc(sizeof(cell**) * mx->dim);
	
	//inicializando cada posição dos eixos com o ponteiro nulo
	for(int d=0; d < mx->dim; d++)
	{
		mx->data[d] = malloc(sizeof(cell*) * mx->size[d]);
		
		for(int c=0; c < mx->size[d]; c++)
			mx->data[d][c] = NULL;
	}
	
	return mx;
}

void free_matrix2d(matrix *mx)
{
	cell *ptr, *aux;
	
	//liberando as células linha a linha
	for(int i=0; i < mx->size[0]; i++)
	{
		ptr = mx->data[0][i];	//i-ésima linha (eixo 0 é o eixo das linhas)
		while(ptr != NULL)
		{
			aux = ptr;
			ptr = ptr->next[1];
			free_cell2d(aux);
		}
	}
	
	for(int d=0; d < mx->dim; d++)
		free(mx->data[d]);
	
	free(mx->size);
	free(mx->data);
	free(mx);
}

cell* matrix2d_at(matrix *mx, unsigned int i, unsigned int j)
{
	// [0] é o eixo x (onde se está percorrendo) e [i] corresponde ao y (onde se está comparando)
	cell *ptr = mx->data[0][i];
	
	while(ptr != NULL)
	{
		if(ptr->coord[1] == j)
			return ptr;
		else if(ptr->coord[1] > j)
			break;
		ptr = ptr->next[1];
	}
	
	return NULL;
}

Type matrix2d_get(matrix *mx, unsigned int i, unsigned int j)
{
	cell *ptr = matrix2d_at(mx, i, j);
	return (ptr != NULL ? ptr->value : EMPTY_VALUE);
}

void matrix2d_set(matrix *mx, unsigned int i, unsigned int j, Type value)
{
	cell *ptr0 = mx->data[0][i];	//i-ésima linha (eixo 0)
	cell *ptr1 = mx->data[1][j];	//j-ésima coluna (eixo 1)
	cell *a_ij = NULL;
	cell *temp;
	
	//análise no eixo 0
	temp = NULL;
	
	//busca pela célula ao longo da i-ésima linha
	while(ptr0 != NULL)
	{
		if(ptr0->coord[1] >= j)
			break;
		
		temp = ptr0;
		ptr0 = ptr0->next[1];
	}
	
	//se já existir, atualizar e sair
	if(ptr0 != NULL && ptr0->coord[1] == j)
	{
		ptr0->value = value;
		return;
	}
	//senão, criar uma nova célula e vincular
	else
	{
		a_ij = new_cell2d(value);
		a_ij->coord[0] = i;
		a_ij->coord[1] = j;
		
		//se existir célula anterior e posterior, vincular as 3
		if(ptr0 != NULL)
		{
			temp = ptr0->prev[1];
		
			cell2d_bind(temp,a_ij);	//anterior e atual
			cell2d_bind(a_ij,ptr0);	//atual e posterior
			
			if(mx->data[0][i] == ptr0)
				mx->data[0][i] = a_ij;
		}
		//se existir apenas célula anterior, vincular as 2
		else if(ptr0 == NULL && temp != NULL)
			cell2d_bind(temp,a_ij);
		//senão, vincular a nova ao início da linha
		else
			mx->data[0][i] = a_ij;
	}
	
	//análise no eixo 1
	temp = NULL;

	//busca pelo ponto anterior à célula ao longo da j-ésima coluna
	while(ptr1 != NULL)
	{
		if(ptr1->coord[0] >= i)
			break;
		
		temp = ptr1;
		ptr1 = ptr1->next[0];
	}
	
	//se existir célula anterior e posterior, vincular as 3
	if(ptr1 != NULL)
	{
		temp = ptr1->prev[0];
		
		cell2d_bind(temp,a_ij);
		cell2d_bind(a_ij,ptr1);
		
		if(mx->data[1][j] == ptr1)
			mx->data[1][j] = a_ij;
	}
	//se existir apenas célula anterior, vincular as 2
	else if(ptr1 == NULL && temp != NULL)
		cell2d_bind(temp,a_ij);
	//senão, vincular a nova ao início da linha
	else
		mx->data[1][j] = a_ij;
}

void matrix2d_unset(matrix *mx, unsigned int i, unsigned int j)
{
	//obtém a célula
	cell *ptr = matrix2d_at(mx,i,j);	
	if(ptr == NULL)
		return;
	
	//reajuste ao longo da i-ésima linha (i fixo, j variando)
	if(ptr->prev[1] != NULL && ptr->next[1] != NULL)
		cell2d_bind(ptr->prev[1], ptr->next[1]); //vincula as células adjacentes
	else if(ptr->prev[1] != NULL)
		ptr->prev[1]->next[1] = NULL;	//vincula apenas a célula anterior
	else
	{
		mx->data[0][i] = ptr->next[1];	//atualiza o início da linha
		
		if(ptr->next[1] != NULL)
			ptr->next[1]->prev[1] = NULL;	//desvincula a célula seguinte
	}
	
	//reajuste ao longo da j-ésima coluna (i variando, j fixo)
	if(ptr->prev[0] != NULL && ptr->next[0] != NULL)
		cell2d_bind(ptr->prev[0], ptr->next[0]); //vincula as células adjacentes
	else if(ptr->prev[0] != NULL)
		ptr->prev[0]->next[0] = NULL;	//vincula apenas a célula anterior
	else
	{
		mx->data[1][j] = ptr->next[0];	//atualiza o início da coluna
		
		if(ptr->next[0] != NULL)
			ptr->next[0]->prev[0] = NULL;	//desvincula a célula seguinte
	}
	
	free_cell2d(ptr);
}

cell* matrix2d_get_ln(matrix *mx, unsigned int i)
{
	return (i < mx->size[0] ? mx->data[0][i] : NULL);
}

cell* matrix2d_get_col(matrix *mx, unsigned int j)
{
	return (j < mx->size[1] ? mx->data[1][j] : NULL);
}

void matrix2d_add_ln(matrix *mx, unsigned int ln)
{
	//aumenta o tamanho do vetor-linha
	assert(ln <= mx->size[0]);
	mx->size[0]++;
	mx->data[0] = realloc(mx->data[0], sizeof(cell*) * mx->size[0]);
	cell *ptr;
	
	//atualiza o valor das coordenadas de todas as linhas deslocadas (linhas abaixo da adicionada)
	for(int i = mx->size[0]-2; i >= ln; i--)
	{
		//obtém e move a linha atual para baixo
		ptr = matrix2d_get_ln(mx,i);
		mx->data[0][i+1] = ptr;
		
		//se houver células nessa linha, percorrer e atualizar uma a uma
		while(ptr != NULL)
		{
			ptr->coord[0]++;
			ptr = ptr->next[1];
		}
	}
	
	mx->data[0][ln] = NULL;
}

void matrix2d_add_col(matrix *mx, unsigned int col)
{	
	//aumenta o tamanho do vetor-coluna
	assert(col <= mx->size[1]);
	mx->size[1]++;
	mx->data[1] = realloc(mx->data[1], sizeof(cell*) * mx->size[1]);
	cell *ptr;
	
	//atualiza o valor das coordenadas de todas as colunas deslocadas (colunas à frente da adicionada)
	for(int j = mx->size[1]-2; j >= col; j--)
	{
		//obtém e move a coluna atual para frente
		ptr = matrix2d_get_col(mx,j);
		mx->data[1][j+1] = ptr;
		
		//se houver células nessa coluna, percorrer e atualizar uma a uma
		while(ptr != NULL)
		{
			ptr->coord[1]++;
			ptr = ptr->next[0];
		}
	}
	
	mx->data[1][col] = NULL;
}

void matrix2d_rmv_ln(matrix *mx, unsigned int ln)
{
	//obtém a linha a ser excluída
	assert(ln < mx->size[0]);
	cell *aux, *ptr = mx->data[0][ln];
	
	while(ptr != NULL)
	{
		//obtém a célula atual e a próxima na coluna
		aux = ptr;
		ptr = ptr->next[1];
		//revincula a linha e exclui a célula atual
		matrix2d_unset(mx, aux->coord[0], aux->coord[1]);
	}
	
	//para cada linha abaixo da removida: deslocar e atualizar a coordenada
	for(int i = ln+1; i < mx->size[0]; i++)
	{
		//obtém e move a linha atual para cima
		ptr = matrix2d_get_ln(mx,i);
		mx->data[0][i-1] = ptr;		
		//se houver células nessa linha, percorrer e atualizar uma a uma
		while(ptr != NULL)
		{
			ptr->coord[0]--;
			ptr = ptr->next[1];
		}
	}
	
	//diminui o tamanho do vetor-linha
	mx->size[0]--;
	mx->data[0] = realloc(mx->data[0], sizeof(cell*) * mx->size[0]);
}

void matrix2d_rmv_col(matrix *mx, unsigned int col)
{
	//obtém a coluna a ser excluída
	assert(col < mx->size[1]);
	cell *aux, *ptr = mx->data[1][col];
	
	while(ptr != NULL)
	{
		//obtém a célula atual e a próxima na coluna
		aux = ptr;
		ptr = ptr->next[0];
		//revincula a linha e exclui a célula atual
		matrix2d_unset(mx, aux->coord[0], aux->coord[1]);
	}
		
	//para cada coluna à frente da removida: deslocar e atualizar a coordenada
	for(int j = col+1; j < mx->size[1]; j++)
	{
		//obtém e move a coluna atual para trás
		ptr = matrix2d_get_col(mx,j);
		mx->data[1][j-1] = ptr;		
		//se houver células nessa coluna, percorrer e atualizar uma a uma
		while(ptr != NULL)
		{
			ptr->coord[1]--;
			ptr = ptr->next[0];
		}
	}
	
	//diminui o tamanho do vetor-coluna
	mx->size[1]--;
	mx->data[1] = realloc(mx->data[1], sizeof(cell*) * mx->size[1]);
}

void matrix2d_print_ln(matrix *mx, unsigned int i, char c)
{
	
	cell *ptr = matrix2d_get_ln(mx,i);
	int cont =0;
	
	if(c == ' ')
	{
		for(int cont=0; cont < mx->size[1]; cont++)
		{
			if(ptr != NULL && ptr->coord[1] == cont)
			{
				printf("%4i ", ptr->value);
				ptr = ptr->next[1];
			}
			else
				printf("%4i ", EMPTY_VALUE);
		}
	}
	else if(c == 'c')
	{
		for(int cont=0; cont < mx->size[1]; cont++)
		{
			if(ptr != NULL && ptr->coord[1] == cont)
			{
				printf("%c", ptr->value);
				ptr = ptr->next[1];
			}
			else
				printf(" ");
		}
	}
	else if(c == '_')
	{
		printf("ln %i: ", i+1);
		
		for(int cont=0; cont < mx->size[1]; cont++)
		{
			if(ptr != NULL && ptr->coord[1] == cont)
			{
				printf("%i ", ptr->value);
				ptr = ptr->next[1];
			}
			else
				printf("_ ");
		}
	}
	
	printf("\n");
}

void matrix2d_print_col(matrix *mx, unsigned int j, char c)
{
	
	cell *ptr = matrix2d_get_col(mx,j);
	int cont =0;
	
	if(c == ' ')
	{
		for(int cont=0; cont < mx->size[0]; cont++)
		{
			if(ptr != NULL && ptr->coord[0] == cont)
			{
				printf("%4i ", ptr->value);
				ptr = ptr->next[0];
			}
			else
				printf("%4i ", EMPTY_VALUE);
		}
	}
	else if(c == 'c')
	{
		for(int cont=0; cont < mx->size[0]; cont++)
		{
			if(ptr != NULL && ptr->coord[0] == cont)
			{
				printf("%c", ptr->value);
				ptr = ptr->next[0];
			}
			else
				printf(" ");
		}
	}
	else if(c == '_')
	{
		printf("col %i: ", j+1);
		
		for(int cont=0; cont < mx->size[0]; cont++)
		{
			if(ptr != NULL && ptr->coord[0] == cont)
			{
				printf("%i ", ptr->value);
				ptr = ptr->next[0];
			}
			else
				printf("_ ");
		}
	}
	
	printf("\n");
}

void matrix2d_print(matrix *mx, char c)
{
	if(c == ' ')
	{
		for(int i=0; i < mx->size[0]; i++)
			matrix2d_print_ln(mx,i,c);
	}
	else if(c == 't') //imprime a transposta
	{
		for(int j=0; j < mx->size[1]; j++)
			matrix2d_print_col(mx,j,' ');
	}
	else if(c == 'c')
	{
		for(int i=0; i < mx->size[0]; i++)
			matrix2d_print_ln(mx,i,c);
	}
	else if(c == '[')
	{
		int cont = 0;
		cell *ptr = NULL;
		printf("     "); // 5 espaços para [---]
		
		for(int j = 0; j < mx->size[1]; j++)
			printf("[%3i]", j+1);
		
		for(int i=0; i < mx->size[0]; i++)
		{
			printf("\n[%3i]", i+1);
			ptr = mx->data[0][i];
			
			while(ptr != NULL)
			{
				if(ptr->coord[1] == cont)
				{
					printf(" %3i ", ptr->value);
					ptr = ptr->next[1];
				}
				else
					printf(" --- ");
				cont++;
			}
			cont = mx->size[1] - cont;
			
			while(cont > 0)
			{
				printf(" --- ");
				cont--;
			}
		}
		printf("\n");
	}
}

void matrix2d_resize(matrix *mx, unsigned int m, unsigned int n)
{
	//resize no vetor-linha
	if(m > mx->size[0])
	{
		//aumenta o tamanho do vetor-linha para m unidades
		unsigned int old_size = mx->size[0];
		mx->size[0] = m;
		mx->data[0] = realloc(mx->data[0], sizeof(cell*) * mx->size[0]);
		
		//atribuir valor inicial para as recém criadas linhas
		for(int i = old_size; i < mx->size[0]; i++)
			mx->data[0][i] = NULL;
	}
	else if(m < mx->size[0] && m > 0)
	{
		//remove cada linha excedente e revicula as células adjacentes
		cell *ptr, *aux;
		
		for(int i = m; i < mx->size[0]; i++)
		{
			ptr = matrix2d_get_ln(mx,i);
			
			while(ptr != NULL)
			{
				aux = ptr;
				ptr = ptr->next[0];
				
				if(aux->prev[0] != NULL)
					aux->prev[0] = NULL;
				else
					mx->data[1][aux->coord[1]] = NULL;
				
				free_cell2d(aux);
			}
		}
		
		//diminui o tamanho do vetor-linha para m unidades
		mx->size[0] = m;
		mx->data[0] = realloc(mx->data[0], sizeof(cell*) * mx->size[0]);
	}
	
	//resize no vetor-coluna
	if(n > mx->size[1])
	{
		//aumenta o tamanho do vetor-coluna para n unidades
		unsigned int old_size = mx->size[1];
		mx->size[1] = n;
		mx->data[1] = realloc(mx->data[1], sizeof(cell*) * mx->size[1]);
		
		//atribuir valor inicial para as recém criadas colunas
		for(int j = old_size; j < mx->size[1]; j++)
			mx->data[1][j] = NULL;
	}
	else if(n < mx->size[1] && n > 0)
	{
		//remove cada coluna excedente e revicula as células adjacentes
		cell *ptr, *aux;
		
		for(int j = m; j < mx->size[1]; j++)
		{
			ptr = matrix2d_get_ln(mx,j);
			
			while(ptr != NULL)
			{
				aux = ptr;
				ptr = ptr->next[1];
				
				if(aux->prev[1] != NULL)
					aux->prev[1] = NULL;
				else
					mx->data[0][aux->coord[0]] = NULL;
				
				free_cell2d(aux);
			}
		}
		
		//diminui o tamanho do vetor-coluna para n unidades
		mx->size[1] = n;
		mx->data[1] = realloc(mx->data[1], sizeof(cell*) * mx->size[1]);
	}
}

matrix* matrix2d_multiply(matrix *mx1, matrix *mx2)
{
	if(mx1->size[1] != mx2->size[0])
		return NULL;
	
	//	A[a_ij]mn x B[b_ij]nr = C[c_ij]mr
	matrix *mx3 = new_matrix2d(mx1->size[0], mx2->size[1]);
	cell *ln = NULL, *col = NULL;
	int qt = mx1->size[1];	//quantidade de operações
	int sum;
	
	for(int i=0; i < mx1->size[0]; i++)
	{
		for(int j=0; j < mx2->size[1]; j++)
		{
			ln = matrix2d_get_ln(mx1,i);
			col = matrix2d_get_col(mx2,j);
			sum = 0;
			
			for(int cont=0; cont < qt && ln != NULL && col != NULL; cont++)
			{
				//acrescentando à soma o produto das células correspondentes
				if(ln->coord[1] == cont && col->coord[0] == cont)
					sum += ln->value * col->value;
					
				//a linha é porcorrida 'cont' vezes
				if(ln->coord[1] <= cont)
					ln = ln->next[0];
					
				//a coluna é percorrida 'cont' vezes
				if(col->coord[0] <= cont)
					col = col->next[1];
			}
			
			if(sum != 0)
				matrix2d_set(mx3,i,j,sum);
		}
	}
	
	return mx3;
}
