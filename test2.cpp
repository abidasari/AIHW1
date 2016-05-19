#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <list>
#include <algorithm>
#include <queue>
#include <map>
#include <iomanip>
using namespace std;

struct vertex
{
	string name;
	int cost;
	std::vector<int> v;
};

class compareTime
{
	public:
		bool operator()(vertex& t1, vertex&t2)
		{
			if (t1.cost < t2.cost) return false;
      		else if (t1.cost == t2.cost && t1.name < t2.name) return false;
     	 	else if (t1.cost == t2.cost && t1.name > t2.name) return true;
      	 	else if (t1.cost > t2.cost) return true;
		}
};

int main()
{
	priority_queue<vertex, vector<vertex>, compareTime> vertices;
	vertex X[4] = { { "AA", 2 }, { "AB", 0 }, { "BB" , 6 }, { "AA", 1 } };
	for (int i=0 ; i<4 ;i++)
		vertices.push(X[i]);
	while(!vertices.empty())
	{
		vertex vert = vertices.top();
		cout << vert.name << " " << vert.cost << endl;
		vertices.pop();
	}
	return 0;
}