#include "Container.h"
#include "Sequence.h"
//basic containers
#include "Tuple.h"		//ok
#include "Vector.h"		//ok
#include "List.h"			//ok
#include "Matrix.h"		//ok
//derived containers
#include "Stack.h"		//ok
#include "Queue.h"		//ok
#include "Set.h"			//corrigir o operator=()
//compound containers
#include "HashSet.h"		//alterar o indexOf, at, get e set, adicionar o rehash
//#include "HashTable.h"
//#include "SparseMatrix.h"
#include "Graph.h"		//generalizar as estruturas de dados usadas
#include "SimpleGraph.h"	//escrever em função de Graph
//utilidades
#include "Sorting.h"

using namespace std;
using namespace DataStructure;

class Foo
{
public:
	int a;
	float b;
	double c;
	
	bool operator==(Foo& f)
	{
		return (a==f.a && b==f.b && c==f.c);
	}
};

int main()
{
	int k=1;
	int array[] = {1,2,3};
	
	Tuple<int,3> tpl = Tuple<int,3>(array);
	Vector<int> vct = Vector<int>(16);
	List<int> lst = List<int>();
	Matrix<int> mtx = Matrix<int>(3,3);
	
	Stack<int> stk = Stack<int>(4);
	Queue<int> que = Queue<int>();
	Set<char,Vector> st1 = Set<char,Vector>(9);
	Set<char,List> st2 = Set<char,List>();
	
	HashSet<double> hst = HashSet<double>(5);
	Graph<char,int> gph = Graph<char,int>(4);
	SimpleGraph<char> sgh = SimpleGraph<char>(4);
	
	for(int i=0; i<9; i++)
	{
		k *= 2;
		
		vct.pushBack(k);
		lst.pushBack(k);
		mtx.set(i+1,i);
		
		stk.push(k);
		que.push(k);
		st1.insert('a'+2*i);
		st2.insert('a'+i);
		
		hst.insert(i);
		hst.insert(7*i);
		hst.insert(12*i);
		
		gph.insertVertex('A'+i);
		sgh.insertVertex('A'+i);
	}
	
	tpl.reverse();
	tpl.print();
	
	vct.reverse();
	vct.swap(1,4);
	vct.swap(3,6);
	Sorting::bubbleSort(&vct);
	Sorting::shuffle(&vct);
	vct.print();
	
	return 0;
	
	lst.swap(1,7);
	lst.print();
	
	mtx.transpose();
	cout << "\n" << mtx.strFormat() << "\n\n";
	
	stk.pop();
	stk.push(0);
	cout << std::string(stk) << "\n";
	
	que.pop();
	que.push(0);
	cout << std::string(que) << "\n";
	
	cout << "\n";
	st1.print();
	st2.print();
	(st1+st2).print();
	(st1-st2).print();
	(st1^st2).print();
	cout << "\n";
	
	hst.print();
	cout << "\n";
	
	gph.insertEdge('F','B',1);
	gph.insertEdge('C','B',3);
	gph.insertEdge('D','H',2);
	//gph.print();
	
	sgh.insertEdge('F','B');
	sgh.insertEdge('C','B');
	sgh.insertEdge('D','H');
	sgh.print();
	sgh.removeVertex('B');
	sgh.print();
	
	cout << "\nOkay!\n";
	return 0;
}
//	clear && g++ sample.cpp -o exe -std=c++17 && valgrind ./exe
