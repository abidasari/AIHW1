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
using namespace std;


int main (int argc, char** argv) 
{
	string temp, temp2 = "A N  M    ", s= "0-4 4-8 ";
	stringstream sse(temp2);
	while(sse >> temp)
	{
		cout << temp <<"."<<endl;
	}

	temp2 = s;
	vector<bool> v(12);
	stringstream ss0(temp2);
	int i,j;
	for(i=0; i<12; i++)
		v[i] = true;
	while(ss0 >> temp)
	{
		stringstream ss(temp);
		ss >> i;
		ss.ignore();
		ss >> j;
		//i-=1;
		for (i; i<=j ;i++)
		{
			v[i] = false;
		}
	}
	for(i=0; i<12; i++)
		cout << v[i] << " ";
}