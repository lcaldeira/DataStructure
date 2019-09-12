#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include <cmath>
#include <iostream>
#include <string>
#include <sstream>

//célula de dimensão n
typedef struct stc_cell
{
	struct stc_cell **prev;	//array de n prev's
	struct stc_cell **next;	//array de n next's
	unsigned int *coord;	//array de n coordenadas
	Type value;				//valor da célula
} cell;

//criação e exclusão de célula bidimensional
cell* new_cell2d(Type value);
void free_cell2d(cell* cl);
//vínculo entre duas células possivelmente alinhadas ao longo de algum eixo
void cell2d_bind(cell* cl1, cell* cl2);

//matriz de adjacência
typedef struct
{
	unsigned int dim;	//dimensão n da matriz
	unsigned int *size; //um array com o tamanho dos n-ésimos eixos
	cell* **data;		//um array com n eixos, sendo cada eixo um array de ponteiros de célula
} matrix;

//obtém o tamanho do n-ésimo eixo da matriz
unsigned int matrix_nth_size(matrix *mx, unsigned int coord);

//criação e exclusão da matriz bidimensional
matrix* new_matrix2d(unsigned int m, unsigned int n);
void free_matrix2d(matrix *mx);

//manipulação básica de células dentro da matriz
cell* matrix2d_at(matrix *mx, unsigned int i, unsigned int j);
Type matrix2d_get(matrix *mx, unsigned int i, unsigned int j);
void matrix2d_set(matrix *mx, unsigned int i, unsigned int j, Type value);
void matrix2d_unset(matrix *mx, unsigned int i, unsigned int j);

//obtém a i-ésima linha ou j-ésima coluna
cell* matrix2d_get_ln(matrix *mx, unsigned int i);
cell* matrix2d_get_col(matrix *mx, unsigned int j);

//adiciona/remove uma linha ou coluna
void matrix2d_add_ln(matrix *mx, unsigned int ln);
void matrix2d_add_col(matrix *mx, unsigned int col);
void matrix2d_rmv_ln(matrix *mx, unsigned int ln);
void matrix2d_rmv_col(matrix *mx, unsigned int col);

//imprime uma linha/coluna específica ou a matriz inteira
void matrix2d_print_ln(matrix *mx, unsigned int i, char c);
void matrix2d_print_col(matrix *mx, unsigned int j, char c);
void matrix2d_print(matrix *mx, char c);

//funções extras: redimensionamento de matriz e mutuplicação de matriz de inteiros
void matrix2d_resize(matrix *mx, unsigned int m, unsigned int n);
matrix* matrix2d_multiply(matrix *mx1, matrix *mx2);

#endif
