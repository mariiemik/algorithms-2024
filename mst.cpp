/*Минимальное остовное дерево
Вычислить вес минимального остова обыкновенного графа.*/
#include <fstream>
#include <iostream>
using std::cout;
using std::cin;


struct Edge {
	int weight;
	int start;
	int end;
	Edge(int w = 0, int s = 0, int e = 0): weight(w), start(s), end(e) {}
};
struct Graph {
	int kol_of_nodes;
	int kol_of_edges;
	
	Edge* edges;
	int ind_of_edges;
	int* nodes;
	Graph(int n, int e) : kol_of_nodes(n), kol_of_edges(e), ind_of_edges(0) {
		edges = new Edge[e];
	}
	void add_edge(int w, int s, int e) {
		edges[ind_of_edges++] = Edge(w, s, e);
	}
	~Graph() {
		delete[] edges;
	}
	
};

void make_heap(Edge M[], int n) {
	for (int i = 1; i < n; ++i) {
		int pos = i;
		while (pos != 0) {
			if (M[(pos - 1) / 2].weight < M[pos].weight) {
				std::swap(M[pos], M[(pos - 1) / 2]);
				pos = (pos - 1) / 2;
			}
			else {
				break;
			}
		}
	}
}

void shift_down(int pos, Edge M[], int size) {
	while (2 * pos + 1 < size) {
		int posMaxChild = 2 * pos + 1;
		if (2 * pos + 2 < size) {
			if (M[posMaxChild].weight < M[posMaxChild + 1].weight)
				++posMaxChild;
		}
		if (M[pos].weight < M[posMaxChild].weight) {
			std::swap(M[pos], M[posMaxChild]);
			pos = posMaxChild;
		}
		else
			break;
	}
}

void heap_sort(Edge M[], int n) {
	make_heap(M, n);
	int size = n;
	for (int i = 1; i < n; ++i) {
		std::swap(M[0], M[size - 1]);
		--size;
		shift_down(0, M, size);
	}
}

struct DSU {
	int size;
	int* array;
	int sum_of_edges;
	DSU(int s) : size(s), sum_of_edges(0) {
		array = new int[size];
		for (int i = 0; i < size; ++i) {
			// каждый корень для себя
			array[i] = i;
		}
	}
	int find(int x) {
		if (array[x] == x)
			return x;
		return array[x] = find(array[x]);
	}
	void un(Edge ed) {
		int rx = find(ed.start);
		int ry = find(ed.end);
		if (rx == ry) {
			return;
		}
		sum_of_edges += ed.weight;
		// если оба не корень
		if (ed.start != rx && ed.end != ry) {
			for (int i = 0; i < size; ++i) {
				if (array[i] == rx) {
					array[i] = ry;
				}
			}
		}
		else if (ed.start != rx) {
			array[ry] = rx;
		}
		else {
			array[rx] = ry;
		}
	}
	~DSU() {
		delete[] array;
	}
};

int main(int argc, char* argv[])
{
	std::ifstream inp(argv[1]);
	srand(time(0));
	int kol_of_nodes;
	int kol_of_edges;
	inp >> kol_of_nodes >> kol_of_edges;
	int start;
	int end;
	int weight;
	Graph graph(kol_of_nodes, kol_of_edges);
	for (int i = 0; i < kol_of_edges; ++i) {
		inp >> start >> end >> weight;
		graph.add_edge(weight, start, end);
	}
	// сортируем ребра по весу
	heap_sort(graph.edges, kol_of_edges);
	DSU dsu(kol_of_nodes);

	for (int i = 0; i < kol_of_edges; ++i) {
		dsu.un(graph.edges[i]);
	}
	cout << dsu.sum_of_edges;
	inp.close();
	return 0;
}
