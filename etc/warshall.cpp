#include "Matrix.h"

using namespace std;
using namespace DataStructure;

int main()
{
	size_t k;
	cout << "Informe o tamanho do conjunto: ";
	cin >> k;
	Matrix<bool> R(k,k);
	
	cout << "Informe a existência de relações:\n";
	for(int i=0; i<k; i++)
		for(int j=0; j<k; j++)
			cin >> R[i][j];
	
	cout << "[R1]:\n" << std::string(R);
	Matrix<bool> Rx = R;
	
	for(int x=2; x<=k; x++)
	{
		Rx = Rx*R;
		cout << "[R" << x << "]:\n" << std::string(Rx);
	}
	
	return 0;
}

//	clear && g++ warshall.cpp -o md -std=c++11 && ./md < input.txt
