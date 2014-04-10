#include <iostream>
#include <math.h>
#include <string>
#include <vector>

 
using namespace std;
 
struct vertex{
	double x, y;
	int visited, prev, oi;
	vector<int> edge_number;
	vertex(string tmpx, string tmpy){
		x = atof(tmpx.c_str());
		y = atof(tmpy.c_str());
		visited = oi = prev =  -1;
	}
};
 
struct edge{
	int bgn, end;
	double weight;
	edge(string tmpbgn, string tmpend, string tmpweight){
		bgn = atoi(tmpbgn.c_str());
		end = atoi(tmpend.c_str());
		weight = atof(tmpweight.c_str());
	}
};

vector<edge> edge_tab;
vector<vertex> vertex_tab; 
int vertex_count = 0, edge_count = 0, ii;
 
void go_visit(int vv, int father){
	vertex_tab[vv].prev = father;
	vertex_tab[vv].visited = ii;
	++ii;
	vertex_tab[vv].oi = vertex_tab[vv].visited;
	for (int i = 0; i < vertex_tab[vv].edge_number.size(); ++i){
		int vvv = (edge_tab[vertex_tab[vv].edge_number[i]].bgn==vv?edge_tab[vertex_tab[vv].edge_number[i]].end:edge_tab[vertex_tab[vv].edge_number[i]].bgn);
		if (vvv != father){
			if (vertex_tab[vvv].visited == -1){
				go_visit(vvv,vv);
				if (vertex_tab[vv].oi > vertex_tab[vvv].oi)
					vertex_tab[vv].oi = vertex_tab[vvv].oi;
			}
			else
				if (vertex_tab[vv].oi > vertex_tab[vvv].visited)
					vertex_tab[vv].oi = vertex_tab[vvv].visited;
		}
	}
}

 void read_it(void){
	while (!cin.eof()){
		string tmp, l1, l2, l3;
		string::iterator it;
		getline(cin, tmp);
		if (tmp.empty())
			continue;
		it = tmp.begin();
		while (*it != ' '){
			l1.push_back(*it);
			++it;
		}
		++it;
		while (it != tmp.end() && *it != ' '){
			l2.push_back(*it);
			++it;
		}
		if (it != tmp.end()){
			++it;
			while (it != tmp.end()){
				l3.push_back(*it);
				++it;
			}
		}
		if (l3.empty()){
			vertex_tab.push_back(vertex(l1, l2));
			++vertex_count;
		}
		else{
			edge_tab.push_back(edge(l1, l2, l3));
			vertex_tab[atoi(l1.c_str())].edge_number.push_back(edge_count);
			vertex_tab[atoi(l2.c_str())].edge_number.push_back(edge_count);
			++edge_count;
		}
	}
}

void do_it(void){
	for (int i = 0; i < vertex_tab.size(); ++i){
		ii = 1;
		for (int j = 0; j < vertex_tab.size(); ++j)
			if (vertex_tab[i].visited == -1)
		go_visit(i, -1);
	}
}

void print_it(void){
	for (int i = 0; i < vertex_tab.size(); ++i){
		if (vertex_tab[i].oi == vertex_tab[i].visited && vertex_tab[i].prev != -1){
			int m = vertex_tab[i].prev, j=0, n;
			cout << i << " " << m << " ";
			while (edge_tab[vertex_tab[i].edge_number[j]].bgn != m && edge_tab[vertex_tab[i].edge_number[j]].end != m)
				++j;
			n = vertex_tab[i].edge_number[j];
			cout << edge_tab[n].weight << " " << sqrt(pow(vertex_tab[i].x-vertex_tab[m].x, 2) + pow(vertex_tab[i].y - vertex_tab[m].y, 2)) << endl;
		}
	}
}

int main(void){
	read_it();
	do_it();
	print_it();
	return 0;
}
