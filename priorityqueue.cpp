/*Реализуйте очередь с приоритетами. Очередь должна поддерживать следующие операции:
добавить элемент,
извлечь минимальный элемент,
уменьшить элемент, добавленный во время одной из операций.*/

#include <fstream>
#include <iostream>
using std::cout;
using std::cin;
struct Node {
	int key;
	int number;
	Node* parent;
	Node* child; //  указатель на левого ребенка узла
	Node* sibling; // указатель на правого брата узла
	int degree; // степень узла (количество дочерних узлов данного узла)

	Node() : parent(nullptr), child(nullptr), sibling(nullptr) {}
	Node(int k = 0, int n = 0) : key(k), parent(nullptr), child(nullptr), sibling(nullptr), degree(0), number(n) {}
};

struct BinomialHeap {
	Node* head;
	BinomialHeap() : head(nullptr) {}
	BinomialHeap(Node* h) : head(h) {}

	void BinomialTreeLink(Node* child, Node* parent) {
		child->parent = parent;
		child->sibling = parent->child;
		parent->child = child;
		parent->degree = child->degree + 1;
	}
	bool is_empty() {
		return head == nullptr;
	}
	void unite(Node* add) {
		if (head == nullptr) {
			head = add;
			return;
		}
		if (add == nullptr) {
			return;
		}
		Node* temp_head = nullptr;

		Node* curH = nullptr;
		Node* curH1 = head;
		Node* curH2 = add;
		if (curH1->degree < curH2->degree) {
			temp_head = curH1;
			curH1 = curH1->sibling;
		}
		else {
			temp_head = curH2;
			curH2 = curH2->sibling;
		}
		curH = temp_head;
		while (curH1 != nullptr && curH2 != nullptr) {
			// добавляем узел с меньшей степенью в новый список
			if (curH1->degree < curH2->degree) {
				curH->sibling = curH1;
				curH1 = curH1->sibling;
			}
			else {
				curH->sibling = curH2;
				curH2 = curH2->sibling;
			}
			curH = curH->sibling;
		}
		// добавляем все оставшиеся узлы
		if (curH1 != nullptr) {
			curH->sibling = curH1;
		}
		else {
			curH->sibling = curH2;
		}

		Node* prev = nullptr;
		curH = temp_head;
		Node* next = curH->sibling;
		// объединение деревьев одной степени 
		while (next != nullptr) {
			if (curH->degree != next->degree ||
				next->sibling != nullptr && (next->sibling->degree == curH->degree)) {
				prev = curH;
				curH = next;
			}
			else if (curH->key <= next->key) {
				curH->sibling = next->sibling;
				BinomialTreeLink(next, curH);
			}
			else {
				if (prev == nullptr) {
					temp_head = next;
				}
				else {
					prev->sibling = next;
				}
				BinomialTreeLink(curH, next);
				curH = next;
			}
			next = curH->sibling;
		}
		head = temp_head;
	}


	Node* insert(int& x, int& n) {
		Node* add = new Node(x, n);
		unite(add);
		return add;
	}

	int extractMin(Node** all) {
		Node* cur_node = head;
		Node* prev = nullptr;
		Node* min_node = nullptr;
		Node* min_node_prev = nullptr;

		int min = std::numeric_limits<int>::max();
		while (cur_node != nullptr) {
			if (cur_node->key < min) {
				min = cur_node->key;
				min_node = cur_node;
				min_node_prev = prev;
			}
			prev = cur_node;
			cur_node = cur_node->sibling;

		}
		if (min_node_prev == nullptr) { // первый узел
			head = min_node->sibling;
		}
		else {
			min_node_prev->sibling = min_node->sibling;
		}

		Node* add = nullptr;
		if (min_node->child != nullptr) {
			cur_node = min_node->child;
			Node* next = cur_node->sibling;
			cur_node->sibling = nullptr;
			cur_node->parent = nullptr;
			while (next != nullptr) {
				Node* tmp = next->sibling;
				next->sibling = cur_node;
				cur_node = next;
				next = tmp;
				cur_node->parent = nullptr;
			}
			add = cur_node;
		}
		int d = min_node->number;
		all[d - 1] = nullptr;
		delete min_node;
		min_node = nullptr;
		unite(add);
		return min;
	}


	void decrease(int& y, int& k, Node** all) {
		Node* node_to_decr = all[k - 1];
		node_to_decr->key = y;
		Node* par = node_to_decr->parent;
		Node* child = node_to_decr;
		int tmp1;
		int tmp2;
		Node* tmp;

		while (par != nullptr) {
			std::swap(all[child->number - 1], all[par->number - 1]);

			std::swap(child->key, par->key);

			std::swap(child->number, par->number);

			child = par;
			par = par->parent;
		}
	}
	void delete_node(Node* node) {
		if (node == nullptr) {
			return;
		}

		delete_node(node->child);
		delete_node(node->sibling);
		delete node;
		node = nullptr;

	}
	~BinomialHeap() {
		delete_node(head);
	}
};

int main(int argc, char* argv[])
{
	std::ifstream inp(argv[1]);
	std::ofstream outp(argv[2]);
	srand(time(0));

	char operation;
	int x;
	int y;
	BinomialHeap heap;
	int n = 100000;
	Node** all = new Node * [n];
	int ind_of_push = 0;
	while (inp >> operation) {
		if (operation == 'p') {

			inp.ignore(3);

			inp >> x;
			if (ind_of_push >= n) {
				while (n <= ind_of_push) {
					n *= 2;
				}
				Node** tmp2 = new Node * [n];

				for (int i = 0; i < ind_of_push; ++i) {
					tmp2[i] = all[i];

				}

				delete[] all;
				all = tmp2;
			}

			++ind_of_push;
			all[ind_of_push - 1] = heap.insert(x, ind_of_push);

		}
		else if (operation == 'e') {
			++ind_of_push;

			inp.ignore(10);
			if (!heap.is_empty()) {
				int r = heap.extractMin(all);
				outp << r;
			}
			else {
				outp << '*';
			}
			outp << '\n';
		}
		else if (operation == 'd') {
			++ind_of_push;
			inp.ignore(11);
			inp >> x >> y;
			heap.decrease(y, x, all);

		}
	}

	delete[] all;
	cout << "\nruntime = " << clock() / 1000.0 << " \n";
	outp.close();
	inp.close();
	system("pause");
	return 0;
}
