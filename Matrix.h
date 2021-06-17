#ifndef MATRIX_H
#define MATRIX_H

#include "Container.h"
#include "Vector.h"

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
		static size_t Precision;
		
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
		
		Matrix<Type>(const Matrix<Type>& m) : Matrix<Type>(m.qtLines(), m.qtCols())
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
		
		Matrix<Type>& operator=(const Matrix<Type>& M)
		{
			this->~Matrix<Type>();
			this->m = M.qtLines();
			this->n = M.qtCols();
			this->data = new Type* [this->m];
			
			for(size_t i=0; i < this->m; i++)
			{
				this->data[i] = new Type[this->n];
				for(int j=0; j < this->n; j++)
					data[i][j] = M.get(i,j);
			}
			return *this;
		}
		
		friend Matrix<Type>& operator&=(Matrix<Type>& A, Matrix<Type>& B)
		{
			A.~Matrix<Type>();
			A.m &= B.m;
			A.n &= B.n;
			A.data = B.data;
			return A;
		}
		
		//busca e verificação
		bool isAllocated() const { return this->data != nullptr; }
		size_t getSize() const { return this->m * this->n; }
		size_t qtLines() const { return this->m; }
		size_t qtCols() const { return this->n; }
		size_t getDim(bool d) const { return (d ? this->n : this->m); }
		
		bool contains(Type value) const
		{
			for(int i=0; i<m; i++)
				for(int j=0; j<n; j++)
					if(this->data[i][j] == value)
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
			if(m != M.qtLines() || n != M.qtCols())
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
		Type& operator()(size_t ln, size_t col) const { return this->data[ln][col]; }
		
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
		
		void resize(size_t new_m, size_t new_n)
		{
			Type **old_data = this->data;
			this->data = new Type* [new_m];

			for(size_t i=0; i < new_m; i++)
			{
				data[i] = new Type[new_n];
				
				for(size_t j=0; j < this->n; j++)
					data[i][j] = old_data[i][j];
				
				delete[] old_data[i];
			}
			this->m = new_m;
			this->n = new_n;
			delete[] old_data;
		}
		
		void resize(size_t new_m, size_t new_n, Type zero)
		{
			size_t old_m = this->m;
			size_t old_n = this->n;
			
			resize(new_m, new_n);

			for(size_t i=0; i < new_m; i++)
			{
				if(i == old_m)
					old_n = 0;
				for(size_t j=old_n; j < new_n; j++)
					data[i][j] = zero;
			}
		}
		
		Matrix<Type> t()
		{
			Matrix<Type> temp = Matrix<Type>(this->n, this->m);
			for(size_t i = 0; i < this->m; i++)
				for(size_t j = 0; j < this->n; j++)
					temp[j][i] = this->data[i][j];
			return temp;
		}
		
		//conversão para texto
		template<typename T=Type, isPrintable<T>* = nullptr>
		std::string strFormat(char c=' ') const 
		{
			std::stringstream ss;
			ss << std::setprecision(Precision);
			switch(c)
			{
			case '[':
				for(size_t i=0; i < this->m; i++)
				{
					ss << "[";
					for(size_t j=0; j < this->n; j++)
						ss << this->data[i][j] << ' ';
					ss << "]";
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
						ss << std::setw(Precision+4) << this->data[i][j] << ' ';
					if(i != this->m-1)
						ss << "\n";
				}
			}
			return ss.str();
		}
		
		template<typename T=Type, isntPrintable<T>* = nullptr>
		std::string strFormat(char c=' ') const { return "[]"; }
		
		operator std::string() const { return this->strFormat(); }
		virtual void print(){ std::cout << (this->strFormat()) << '\n'; }
		
		//operações matemáticas
		template<class T, void*>
		friend Matrix<T> operator*(double k, Matrix<T> A);
		
		template<class T, void*>
		friend Matrix<T> operator*(Matrix<T> A, double k);
		
		template<class T, void*>
		friend Matrix<T> operator+(Matrix<T> A, Matrix<T> B);
		
		template<class T, void*>
		friend Matrix<T> operator-(Matrix<T> A, Matrix<T> B);
		
		template<class T, void*>
		friend Matrix<T> operator*(Matrix<T> A, Matrix<T> B);
		
		template<class T, void*>
		friend Vector<T> operator*(Vector<T> x, Matrix<T> A);
		
		template<class T, void*>
		friend Vector<T> operator*(Matrix<T> A, Vector<T> x);
		
		template<class T=Type, isFundamental<T>* = nullptr>
		Matrix<T> pow(unsigned int k)
		{	
			if(k==1)		return (*this);
			else if(k==2)	return (*this) * (*this);
			else			return this->pow(k/2) * this->pow(k-k/2);
		}
	};
	
	template<class Type>
	size_t Matrix<Type>::Precision = 3*sizeof(Type)/2;
	
	template<class Type, isFundamental<Type>* = nullptr>
	Matrix<Type> operator*(double k, Matrix<Type> A)
	{
		size_t m = A.qtLines();
		size_t n = A.qtCols();
		Matrix<Type> B = Matrix<Type>(m,n);
		
		for(size_t i=0; i<m; i++)
			for(size_t j=0; j<n; j++)
				B[i][j] = k*A[i][j];
		return B;
	}
	
	template<class Type, isFundamental<Type>* = nullptr>
	Matrix<Type> operator*(Matrix<Type> A, double k)
	{ return k*A; }
	
	template<class Type, isFundamental<Type>* = nullptr>
	Matrix<Type> operator+(Matrix<Type> A, Matrix<Type> B)
	{
		size_t m = A.qtLines();
		size_t n = B.qtCols();
		Matrix<Type> C = Matrix<Type>(m,n);
		
		for(size_t i=0; i<m; i++)
			for(size_t j=0; j<n; j++)
				C[i][j] = A[i][j] + B[i][j];
		return C;
	}
	
	template<class Type, isFundamental<Type>* = nullptr>
	Matrix<Type> operator-(Matrix<Type> A, Matrix<Type> B)
	{
		size_t m = A.qtLines();
		size_t n = B.qtCols();
		Matrix<Type> C = Matrix<Type>(m,n);
		
		for(size_t i=0; i<m; i++)
			for(size_t j=0; j<n; j++)
				C[i][j] = A[i][j] - B[i][j];
		return C;
	}
	
	template<class Type, isFundamental<Type>* = nullptr>
	Matrix<Type> operator*(Matrix<Type> A, Matrix<Type> B)
	{
		size_t m = A.qtLines();
		size_t n = B.qtCols();
		size_t r = A.qtCols();
		Matrix<Type> C = Matrix<Type>(m,n);
		
		for(size_t i=0; i<m; i++)
			for(size_t j=0; j<n; j++)
			{
				C[i][j] = 0;
				for(size_t k=0; k<r; k++)
					C[i][j] += A[i][k] * B[k][j];
			}
		return C;
	}
	
	template<class Type, isFundamental<Type>* = nullptr>
	Vector<Type> operator*(Vector<Type> x, Matrix<Type> A)
	{
		size_t m = x.getSize();
		size_t n = A.qtCols();
		Vector<Type> y = Vector<Type>(n,n);
		
		for(size_t j=0; j<n; j++)
		{
			y[j] = 0;
			for(size_t i=0; i<m; i++)
				y[j] += x[i] * A[i][j];
		}
		return y;
	}
	
	template<class Type, isFundamental<Type>* = nullptr>
	Vector<Type> operator*(Matrix<Type> A, Vector<Type> x)
	{
		size_t m = A.qtLines();
		size_t n = x.getSize();
		Vector<Type> y = Vector<Type>(m,m);
		
		for(size_t i=0; i<m; i++)
		{
			y[i] = 0;
			for(size_t j=0; j<n; j++)
				y[i] += A[i][j] * x[j];
		}
		return y;
	}
}
#endif
