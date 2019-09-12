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
//#include "Tree.h"
#include "Graph.h"		//generalizar as estruturas de dados usadas
#include "SimpleGraph.h"	//escrever em função de Graph
//utilidades
#include "Sorting.h"

using namespace std;
using namespace DataStructure;

void firstTest();
void secondTest();

Tuple<int,3> tpl;
Vector<int> vct;
List<int> lst;
Matrix<int> mtx;

Stack<int> stk;
Queue<int> que;
Set<char,Vector> st1;
Set<char,List> st2;

HashSet<double> hst;
/*Graph<char,int> gph;
SimpleGraph<char> sgh;*/

int main()
{
	//firstTest();
	secondTest();
	
	cout << "\nOkay!\n";
	return 0;
}

void firstTest()
{
	int k=1;
	int array[] = {1,2,3};
	
	tpl = Tuple<int,3>(array);
	vct = Vector<int>(16);
	lst = List<int>();
	mtx = Matrix<int>(3,3);
	
	stk = Stack<int>(4);
	que = Queue<int>();
	st1 = Set<char,Vector>(9);
	st2 = Set<char,List>();
	
	hst = HashSet<double>(5);
	/*gph = Graph<char,int>(4);
	sgh = SimpleGraph<char>(4);*/
	
	for(int i=0; i<9; i++)
	{
		k *= 2;
		
		vct.pushBack(k);
		lst.pushBack(k);
		mtx.set(i+1,i);
		
		stk.push(k);
		que.push(k);
		st1.insert('a'+i);
		st2.insert('a'+2*i);
		
		hst.insert(i);
		/*hst.insert(7*i);
		hst.insert(12*i);*/
		
		/*gph.insertVertex('A'+i);
		sgh.insertVertex('A'+i);*/
	}
	
	/*cout << "\n<<TUPLE>>\n";
	tpl.reverse();
	tpl.print();
	
	cout << "\n<<VECTOR>>\n";
	vct.reverse();
	vct.swap(1,4);
	vct.swap(3,6);
	bubbleSort(&vct);
	shuffle(&vct);
	vct.print();
	
	cout << "\n<<LIST>>\n";
	lst.swap(1,7);
	lst.print();

	cout << "\n<<MATRIX>>\n";
	cout << mtx.strFormat() << "\n\n";
	
	cout << "\n<<STACK>>\n";	
	stk.pop();
	stk.push(0);
	cout << std::string(stk) << "\n";
	
	cout << "\n<<QUEUE>>\n";
	que.pop();
	que.push(0);
	cout << std::string(que) << "\n";
	
	cout << "\n<<SET>>\n";
	st1.print();
	st2.print();
	(st1+st2).print();
	(st1-st2).print();
	(st1^st2).print();
	st1 = st2;*/
	
	cout << "\n<<HASH-SET>>\n";
	//hst.print();
	cout << "\n";
	
	/*gph.insertEdge('F','B',1);
	gph.insertEdge('C','B',3);
	gph.insertEdge('D','H',2);
	//gph.print();
	
	sgh.insertEdge('F','B');
	sgh.insertEdge('C','B');
	sgh.insertEdge('D','H');
	sgh.print();
	sgh.removeVertex('B');
	sgh.print();*/
}

void secondTest()
{
	auto vx = Vector<string>(4);
	vx.pushBack("str_0");
	vx.pushBack("str_1");
	vx.pushBack("str_2");
	vx.pushBack("str_3");
	vx.print();
	
	auto vy = Vector<int>(4);
	vy.pushBack(0);
	vy.pushBack(1);
	vy.pushBack(0);
	vy.pushBack(1);
	vy.print();
	
	auto vz = Vector<Vector<int>>(2);
	vz.pushBack(Vector<int>(2));
	vz[0].pushBack(0);
	vz[0].pushBack(1);
	vz.pushBack(Vector<int>(2));
	vz[1].pushBack(10);
	vz[1].pushBack(11);
	vz.pushBack(Vector<int>(2));
	vz[2].pushBack(20);
	vz[2].pushBack(21);
	vz.print();
	
	auto sx = Set<int,Vector>(4);
	sx = vy;
	sx.print();
	
	Set<int,Vector> sy = vy;
	sy.print();
	
	auto vt = Vector<Tuple<int,2>*>(3);
	vt.pushBack(new Tuple<int,2>(0));
	vt.pushBack(new Tuple<int,2>(1));
	vt.pushBack(new Tuple<int,2>(2));
	for(int i=0; i<vt.getSize(); i++)
		vt[i]->print();
}
//		clear && g++ 0_sample.cpp -o 0_exe -std=c++17 && valgrind ./0_exe
