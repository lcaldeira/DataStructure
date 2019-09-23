#ifndef MATRIX_H
#define MATRIX_H

#include "Container.h"

namespace DataStructure
{
	template<class Type>
	class Matrix : public Container<Type>
	{
	protected:
		Type **data;
		size_t m, n;
		
		Type* at(size_t index) const 
		{
			size_t n0 = index % this->n;
			size_t m0 = (index - n0)/this->n;
			return &(this->data[m0][n0]);
		}
	public:
		static size_t CellWidth;
		
		//construtor e destrutor
		Matrix<Type>()
		{
			data = nullptr;
			m = n = 0;
		}
		
		Matrix<Type>(size_t qt_ln, size_t qt_col)
		{
			this->m = qt_ln;
			this->n = qt_col;
			this->data = new Type* [this->m];
			for(size_t i=0; i< this->m; i++)
				this->data[i] = new Type[this->n];
		}
		
		Matrix<Type>(const Matrix<Type>& m) : Matrix<Type>(m.qtLines(), m.qtColumns())
		{
			for(int i=0; i < this->m; i++)
				for(int j=0; j < this->n; j++)
					data[i][j] = m.get(i,j);
		}
		
		~Matrix<Type>()
		{
			if(this->data != nullptr)
			{
				for(size_t i=0; i< this->m; i++)
					if(this->data[i] != nullptr)
						delete[] this->data[i];
				
				delete[] this->data;
				this->data = nullptr;
			}
		}
		
		Matrix<Type> operator=(Matrix<Type> M)
		{
			this->~Matrix<Type>();
			this->m = M.qtLines();
			this->n = M.qtColumns();
			this->data = new Type* [this->m];
			
			for(size_t i=0; i < this->m; i++)
			{
				this->data[i] = new Type[this->n];
				for(int j=0; j < this->n; j++)
					data[i][j] = M.get(i,j);
			}
			return *this;
		}
		
		//busca e verificação
		bool isAllocated() const { return this->data != nullptr; }
		size_t getSize() const { return this->m * this->n; }
		size_t qtLines() const { return this->m; }
		size_t qtColumns() const { return this->n; }
		
		bool contains(Type value, bool(*eqFunc)(Type&,Type&)) const
		{
			for(int i=0; i<m; i++)
				for(int j=0; j<n; j++)
					if(eqFunc(this->data[i][j],value))
						return true;
			return false;
		}
		
		bool isSimetric() const 
		{
			if(m != n || m*n == 0)
				return false;
			
			for(int i=0; i<m; i++)
				for(int j=i+1; j<n; j++)
					if(this->data[i][j] != this->data[j][i])
						return false;
			return true;
		}
		
		template<typename T=Type>
		decltype(auto) operator==(Matrix<T>& M)
		{
			if(m != M.qtLines() || n != M.qtColumns())
				return false;
			for(size_t i=0; i<m; i++)
				for(size_t j=0; j<n; j++)
					if(this->data[i][j] != M[i][j])
						return false;
			return true;
		}
		
		template<typename T=Type>
		decltype(auto) operator!=(Matrix<T>& M)
		{ return !(this->operator==(M)); }

		//acesso e manipulação
		Type* operator[](size_t ln) const { return this->data[ln]; }
		
		Type get(size_t index) const { return *this->at(index); }
		Type get(size_t i, size_t j) const { return this->data[i][j]; }
		void set(Type value, size_t index){ *this->at(index) = value; }
		void set(Type value, size_t i, size_t j){ this->data[i][j] = value; }
		
		void fill(Type value)
		{
			for(size_t i=0; i < this->m; i++)
				for(size_t j=0; j < this->n; j++)
					this->data[i][j] = value;
		}
		
		//conversão para texto
		template<typename T=Type, isPrintable<T>* = nullptr>
		std::string strFormat(char c=' ')
		{
			std::stringstream ss;
			switch(c)
			{
			case '[':
				for(size_t i=0; i < this->m; i++)
				{
					ss << "[";
					for(size_t j=0; j < this->n; j++)
						ss << std::setw(CellWidth) << this->data[i][j];
					ss << "  ]";
					if(i != this->m-1)
						ss << "\n";
				}
				break;
			case 'x':
				ss << strFormat('[') << this->m << 'x' << this->n;
				break;
			case ' ':
			default:
				for(size_t i=0; i < this->m; i++)
				{
					for(size_t j=0; j < this->n; j++)
						ss << std::setw(CellWidth) << this->data[i][j];
					if(i != this->m-1)
						ss << "\n";
				}
			}
			return ss.str();
		}
		
		template<typename T=Type, isntPrintable<T>* = nullptr>
		std::string strFormat(char c=' '){ return "[]"; }
		
		operator std::string(){ return this->strFormat(); }
		virtual void print(){ std::cout << (this->strFormat()) << '\n'; }
		
		//matemático
		template<class T>
		friend Matrix<T> operator*(double k, Matrix<T> A);
		
		template<class T>
		friend Matrix<T> operator*(Matrix<T> A, double k);
		
		template<class T>
		friend Matrix<T> operator+(Matrix<T> A, Matrix<T> B);
		
		template<class T>
		friend Matrix<T> operator-(Matrix<T> A, Matrix<T> B);
		
		template<class T>
		friend Matrix<T> operator*(Matrix<T> A, Matrix<T> B);
		
		Matrix<Type> pow(unsigned int n)
		{
			static_assert(std::is_arithmetic<Type>::value, "Arithmetic type required.");
			
			if(n==1)
				return (*this);
			else if(n==2)
				return (*this)*(*this);
			else
				return this->pow(n/2) * this->pow(n - n/2);
		}
	};
	
	template<class Type>
	size_t Matrix<Type>::CellWidth = 6;
	
	template<class Type>
	Matrix<Type> operator*(double k, Matrix<Type> A)
	{
		static_assert(std::is_arithmetic<Type>::value, "Arithmetic type required.");
		
		size_t m = A.qtLines();
		size_t n = A.qtColumns();
		Matrix<Type> B = Matrix<Type>(m,n);
		
		for(size_t i=0; i<m; i++)
			for(size_t j=0; j<n; j++)
				B[i][j] = k*A[i][j];
		return B;
	}
	
	template<class Type>
	Matrix<Type> operator*(Matrix<Type> A, double k)
	{
		return k*A;
	}
	
	template<class Type>
	Matrix<Type> operator+(Matrix<Type> A, Matrix<Type> B)
	{
		static_assert(std::is_arithmetic<Type>::value, "Arithmetic type required.");
		
		size_t m = A.qtLines();
		size_t n = B.qtColumns();
		Matrix<Type> C = Matrix<Type>(m,n);
		
		for(size_t i=0; i<m; i++)
			for(size_t j=0; j<n; j++)
				C[i][j] = A[i][j] + B[i][j];
		return C;
	}
	
	template<class Type>
	Matrix<Type> operator-(Matrix<Type> A, Matrix<Type> B)
	{
		static_assert(std::is_arithmetic<Type>::value, "Arithmetic type required.");
		
		size_t m = A.qtLines();
		size_t n = B.qtColumns();
		Matrix<Type> C = Matrix<Type>(m,n);
		
		for(size_t i=0; i<m; i++)
			for(size_t j=0; j<n; j++)
				C[i][j] = A[i][j] - B[i][j];
		return C;
	}
	
	template<class Type>
	Matrix<Type> operator*(Matrix<Type> A, Matrix<Type> B)
	{
		static_assert(std::is_arithmetic<Type>::value, "Arithmetic type required.");
		
		size_t m = A.qtLines();
		size_t n = B.qtColumns();
		size_t r = A.qtColumns();
		Matrix<Type> C = Matrix<Type>(m,n);
		
		for(size_t i=0; i<m; i++)
			for(size_t j=0; j<n; j++)
			{
				C[i][j] = 0;
				for(size_t k=0; k<r; k++)
					C[i][j] += (A[i][k] * B[k][j]);
			}
		return C;
	}
}
#endif
