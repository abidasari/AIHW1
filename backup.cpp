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
    string name, ontimesstr;
    int cost;
    vector<bool> onTimes; 
    vertex(string s, int c, string t)//t is just the in times without the count
    {
		name = s;
		string temp;
		cost = c;
		ontimesstr = t;
		int j, i;
		stringstream ss9(t);
		for ( i=0; i < 24; i++)
			onTimes.push_back(1);
		while(ss9 >> temp)
		{
			stringstream ss10(temp);
			ss10 >> i;
			ss10.ignore();
			ss10 >> j;
			i-=1;
			for(i; i<j; i++)
				onTimes[i] = 0;
		}
		//onTimes = onTimes2;
	}
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

bool operator==(const vertex & lhs, const vertex & rhs)
{
	if(lhs.name == rhs.name)
		return true;
	else
		return false;
}


class Graph
{
	public:
	string source, output; //output is for DFS output
	list<string> vertices;
	list<string> destinations;
    int vNumber, onTime;
    map<string, list<struct vertex> > adjList;
    int costDFS;
    //methods
    Graph(string sourceNode, string dest, string vert);
    void addEdge(string s);  
    string BFS();
    string DFS(vertex v);
    bool DFSUtil(vertex v, map<string, bool> &visitedList);
    string UCS();
    
    //debugging methods
    void viewGraph();
    void printMap();
};
 
Graph::Graph(string s, string d, string vert)
{
	source = s;
	vNumber =  0;
	vertices.push_back(s);//add source node to source and veritces list
	vNumber++;
	string temp;
	costDFS = 0;
	stringstream ss(d); stringstream ss1(vert);	
	while(ss >> temp)//add destination nodes to destinations and vertices lists
	{
		destinations.push_back(temp);
		vertices.push_back(temp);
		vNumber++;
	}
	while(ss1 >> temp)//add the rest of the vertices to the vertices list
	{
		vertices.push_back(temp);
		vNumber++;
	}
}


void Graph::addEdge(string s)
{
	int c = 0, intCount;
	string sourceNode, destNode, ontimes ;
	stringstream ss2(s);
	ss2 >> sourceNode;
	ss2 >> destNode;
	ss2 >> c;
	ss2 >> intCount;
	if(intCount != 0)
	{
		//cout << intCount << endl;
		ss2 >> ontimes;
		adjList[sourceNode].push_back(vertex(destNode, c, ontimes));
	}
	else
	{
		adjList[sourceNode].push_back(vertex(destNode, c, ""));//deal with ontimes later DONE!!
	}
}


void Graph :: viewGraph()
{
	cout << "\nSource Vertex: "<< source << endl;
	cout << "All the vertices: " ;
	list<string> :: iterator p, t;
	for(p = vertices.begin(); p != vertices.end(); ++p)
	{
		cout << *p << " ";
	} 
	cout << "\nDestination vertices: " ;
	for(t = destinations.begin(); t != destinations.end(); ++t)
	{
		cout << *t << " ";
	} 
	cout << "\nNumber of vertices: " << vNumber << endl;
	cout << "On time of source: " << onTime << endl;	
}

void Graph :: printMap()
{
	map<string, list<vertex> > :: iterator p1;
	list<vertex> :: iterator p2;
	for(p1 = adjList.begin(); p1 != adjList.end(); ++p1)
	{
		cout << "\nMapping of " << p1->first<< " --> ";
		for(p2 = p1->second.begin(); p2 != p1->second.end(); ++p2)
			{
				cout << p2->name << "( "<< p2->cost << " ) ";
			}
	}
}

string Graph :: BFS()
{
	string output; 
	int cost = onTime;
	queue<vertex> frontier;
	list<vertex> explored, frontier2;////////frontier 2  == frontier using frontier 2 coz no iterator for frontier can optimize code here by making frontier2 a list<string>
	ostringstream out;
	map<string, int> newFrontier;
	vertex current(source, cost, "");
	frontier.push(current); frontier2.push_back(current);
	while(!frontier.empty())
	{
		current = frontier.front();
		explored.push_back(current);
		frontier.pop(); frontier2.pop_front();
		cost = current.cost;
		list<vertex> :: iterator p3;
		map<string, int> :: iterator s3;
		//for getting the Adj List and sorting it in Ascending order
		for(p3 = adjList[current.name].begin(); p3 != adjList[current.name].end(); ++p3)
		{
			newFrontier.insert(pair<string, int>(p3->name, cost + 1));//this will automatically put it in asc order	HERE PUT IN COST+1 IN TO THE COST PART		
		}
		//for checking sorted list's members and putting them on the frontier
		for(s3 = newFrontier.begin(); s3 != newFrontier.end(); s3++)
		{
			if(find(destinations.begin(), destinations.end(), s3->first) != destinations.end())
			{
				//Yay!! found a destination!!!
				out << s3->second;
				output = s3->first + " " + out.str() + "\n";
				return output;
			}
			//add element to frontier iff it is not present in frontier or explored

			list<vertex>::iterator listit, listit2;			
			bool flip = true, flip2 = true;
			for (listit = explored.begin(); listit != explored.end(); ++listit) 
			{
    			if(listit->name == s3->first)
    				flip = false;
			}
			for (listit2 = frontier2.begin(); listit2 != frontier2.end(); ++listit2 )
			{
				if(listit2->name == s3->first)
					flip2 = false;
			}
			
			if(flip && flip2)//element not in explored
			{				
				vertex temp(s3->first, s3->second, "");
				frontier.push(temp); frontier2.push_back(temp);
				newFrontier.erase(s3->first);
			}
		}
		newFrontier.clear();
	}
	// no destination found!!!
	return "NULL";
}

string Graph :: DFS(vertex v)
{
	map<string, bool> visited;
	list<string> :: iterator iter1;
	costDFS = onTime;
	for (iter1 = vertices.begin(); iter1 != vertices.end(); iter1++)//first initialize all to not visited
	{
		visited.insert(pair<string, bool>(*iter1, false));
	}
	map<string, bool> :: iterator i;
	// for (i=visited.begin(); i != visited.end(); i++)
	// {
	// 	cout << i->first << "  " << i->second << "\n";
	// }
	// cout << "\nabout to call the recursive part: cost(start time) = " << costDFS << endl; 
	visited[v.name] = 1;
	bool flip = DFSUtil(v, visited);
	if(flip == false)
		return "NULL";
	else
		return output;
} 

bool Graph :: DFSUtil(vertex ver, map<string, bool> &visited)
{
	if(find(destinations.begin(), destinations.end(), ver.name) != destinations.end())
	{
		ostringstream out;
		out << costDFS;
		output = ver.name + " " + out.str() +"\n";
		return true;
	}
	map<string, bool> :: iterator i;
	// cout << "At " << ver.name <<endl;
	// for (i=visited.begin(); i != visited.end(); i++)
	// {
	// 	cout << i->first << "  " << i->second << "\n";
	// }
	// cout << "------------------------------------------\n";
	list<vertex> :: iterator iter2;
	map<string, int> newFrontier;
	map<string, int> :: iterator iter3;

	for(iter2 = adjList[ver.name].begin(); iter2 != adjList[ver.name].end(); ++iter2)
	{
		if(visited[iter2->name] == false)
		{
			// cout << "inserting " <<  iter2->name << " into newFrontier\n";		
			newFrontier.insert(pair<string, int>(iter2->name, costDFS));//this will automatically put it in asc order		
		}
	}
	for(iter3 = newFrontier.begin(); iter3 != newFrontier.end(); iter3++)
	{
		if(visited[iter3->first] == false)//node not yet visited
		{
			visited[ver.name] = true;
			vertex temp(iter3->first, iter3->second, "");
			costDFS++;

			bool flop = DFSUtil(temp, visited);
			if (flop)
			{
				return true;
			}			
			// else
			// {
			// 	return false;
			// }
		}
	}
	// cout<< "HBHJBB------" << ver.name << "  "<< newFrontier.empty() << endl;
	costDFS--;
	return false;
	// if(newFrontier.empty())//leaf node
	// {
	// 	costDFS--;
	// 	return false;
	// }
}


string Graph :: UCS()
{
	string output;
	int cost = onTime;
	priority_queue<vertex, vector<vertex>, compareTime > frontier;//previously it was a queue of strings
	list<vertex> frontier2;///possible optimization location
	list<vertex> explored;//previously a list of strings
	ostringstream out;
	vertex current(source, cost, "");//previously current was a string
	frontier.push(current); frontier2.push_back(current);
	//int i=0;
	while(!frontier.empty())
	{
		current = frontier.top();
		frontier.pop(); frontier2.remove(current);	
		list<vertex> :: iterator iter0;
		// cout << "adding " << current.name << "  " << current.cost<< " to explored\n"; 
		explored.push_back(current);
		if(find(destinations.begin(), destinations.end(), current.name) != destinations.end())
		{
			//Yay!! found a destination!!!
			out << current.cost;
			output = current.name + " " + out.str() + "\n";
			return output;
		}

		cost = cost + current.cost;////////////////////////////////check once. Probably wrong
		list<vertex> :: iterator p3;//for iterating through adjList
		list<vertex> :: iterator s3;//for iterating through explored
		list<vertex> :: iterator s4;//for iterating through frontier2
		bool flip = false, flop = false, fff = false;
		for(p3 = adjList[current.name].begin(); p3 != adjList[current.name].end(); ++p3)
		{
			//have to check on times and add costs here AND also check if the node name exists  in the explored list if it does remove the one with the longer path
			//adding costs
			for(s4=frontier2.begin(); s4 != frontier2.end(); s4++)//checking if in frontier
			{
				if(s4->name == p3->name)
				{
					flip = true;
					if(s4->cost > p3->cost)
					{
						fff = true;//some serious optimization can be done here//this bool can be used to remove the higher cost node from the frontier
					}
					break;
				}
			}
			for(s3 = explored.begin(); s3!= explored.end(); s3++)//cheching if in explored
			{
				if(s3->name == p3->name)
				{
					flop = true;
					break;
				}
			}
			if(flop)//already in explored
				continue;
			// cout<< "currently checking " << p3->name<< " current cost: " << current.cost << " when checking for costs: " << cost << endl;
			// if(p3->onTimes[current.cost]==0)
			// {
				vertex temp(p3->name, p3->cost + current.cost, p3->ontimesstr);
				frontier.push(temp);
				frontier2.push_back(temp);
			// }									
		}
	}
	//no destination found!!!
	return "NULL";
}

int main ()
{
	ifstream myfile;
	string Alg, src, dest, ver, temp;
	string STRING, a, b, c, d;
	myfile.open("test.txt");
	getline(myfile, STRING); //STRING contains the number of instances in the problem
	int edgeCount = 0;	
	while(!myfile.eof())
	{
		getline(myfile, Alg);//type of algorithm to run
		getline(myfile, src);//source vertex string
		getline(myfile, dest);//destination vertex string
		getline(myfile, ver);//other vertices string
		getline(myfile, temp);//number of edges
		stringstream ss(temp);
		ss >> edgeCount;
		Graph newGraph(src, dest, ver);
		for (int i=0; i< edgeCount; i++)
		{
			getline(myfile, temp);
			newGraph.addEdge(temp);
		}
		//newGraph.printMap();
		getline(myfile, temp);
		stringstream ss3(temp);
		ss3 >> newGraph.onTime;
		//newGraph.viewGraph();
		getline(myfile, temp);
		
		if (Alg == "BFS" || Alg == "bfs")
		{
			cout << "Output of Running BFS: " << newGraph.BFS() << endl;
		}
		if (Alg == "DFS" || Alg == "dfs")
		{
			vertex src(newGraph.source, newGraph.onTime, "");
			cout << "Output of Running DFS: " << newGraph.DFS(src) << endl;
		}
		if (Alg == "UCS" || Alg == "ucs")
		{
			cout << "Output of Running UCS: " << newGraph.UCS() << endl;
		}
	}
	myfile.close();
	return 0;
}
