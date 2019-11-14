//		clear && g++ [2]_test.cpp -o exe2 -std=c++17 && ./exe2

#include <iostream>
#include "Container.h"
#include "Sequence.h"
#include "Vector.h"
#include "List.h"
#include "Set.h"

using namespace std;
using namespace DataStructure;

template<class T>
decltype(auto) cmp(T t1)
{
	t1.compare(t1);
	return 2;
}

template<class T>
auto cmp(T t1, T t2) -> decltype(t1==t2, int())
{
  return 1;
}

template<class T>
int cmp(T t1, T t2, int func)
{
  return 0;
}

struct A{ int x; };

int main()
{
	int i1, i2;
	string s1, s2;
	A a1, a2;
	
	s1 == s2;
	
	//decltype((&string::compare(const string&))) t = false;
	
	/*cout << "(" << cmp(i1,i2)<< ")\n";
	cout << "(" << cmp(s1) << ")\n";
	cout << "(" << cmp(a1,a2,0) << ")\n";*/
	
	auto v = Vector<List<int>>(2);
	
	for(int i=0; i<2; i++)
	{
		v.pushBack(List<int>());
		for(int j=0; j<5; j++)
			v[i].pushBack(j + 10*i);
	}
	v.print();
	
	auto l1 = v[0];
	auto l2 = v[0];
	l2.reverse();
	
	//cout << "[" << v.indexOf(l1) << "," << v.indexOf(l2) << "]\n";
	
	auto l = List<string>();
	l.pushBack("a");
	l.pushBack("b");
	
	cout << "[" << l.indexOf("a") << "," << l.indexOf("c") << "]\n";
	
	auto vs = Vector<Set<string>>(2);
	auto s = Set<string>();
	s.add("A");
	s.add("A");
	s.add("B");
	s.add("C");
	vs.pushBack(s);
	vs.pushBack(s);
	vs.pushBack(s);
	cout << "[" << s.indexOf("A") << "," << s.contains("E") << "]\n";
	vs.print();
	
	return 0;
}
