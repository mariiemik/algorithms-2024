/*Хэш-таблица
Реализовать контейнер множество с помощью хэш-таблицы, поддерживающий операции «добавление ключа», «удаление ключа», «проверка существования ключа».*/
#include <iostream>
#include <fstream>
using std::cout;
using std::cin;

class HashTable {
	int m = 50000;
	int p = 54121;
	int a = 7;
	int b = 93;
	struct Node {
		int value;
		Node* next;
		Node(int val, Node* n = nullptr) : value(val), next(n) {}
	};
	Node** table;
	int hash(int x) {
		return std::abs((a * x + b) % p % p);
	}
public:	
	HashTable() {
		table = new Node*[m];
		for (int i = 0; i < m; ++i) {
			table[i] = nullptr;
		}
	}
	void add(int x) {
		int ind = hash(x);
		if (table[ind] == nullptr) {
			table[ind] = new Node(x);
		}
		else {
			Node* node_to_add = new Node(x, table[ind]);
			table[ind] = node_to_add;
		}
	}
	void remove(int x) {
		int ind = hash(x);
		if (table[ind] == nullptr) {
			return;
		}
		Node* cur = table[ind];
		
		if (cur->value == x) { // если удаляемый элемент первый
			table[ind] = cur->next;
			delete cur;
			cur = nullptr;
		}
		else {
			// ищем предыдущий узел нужного узла в списке
			while (cur->next!=nullptr && cur->next->value != x) {
				cur = cur->next;
			}
			if (cur->next != nullptr) { // иначе элемента нет
				Node* remember = cur->next;
				cur->next = remember->next;
				delete remember;
				remember = nullptr;

			}
		}
	}
	bool is_in_table(int x) {
		int ind = hash(x);
		if (table[ind] == nullptr) {
			return false;
		}
		Node* cur = table[ind];
		while (cur!=nullptr && cur->value != x) {
			cur = cur->next;
		}
		if (cur == nullptr) {
			return false;
		}
		return true;
	}
	~HashTable() {
		for (int i = 0; i < m; ++i) {
			Node* cur_node = table[i];
			while (cur_node != nullptr) {
				Node* next_node = cur_node->next;
				delete cur_node;
				cur_node = next_node;
			}
		}
		delete[] table;
	}

};

int main(int argc, char* argv[])
{
	std::ifstream inp(argv[1]);
	std::ofstream outp(argv[2]);
	srand(time(0));
	int n;
	inp >> n;
	char sign;
	int el;
	HashTable hash_table;
	for (int i = 0; i < n; ++i) {
		inp >> sign >> el;
		if (sign == '+') {
			hash_table.add(el);
		}
		else if (sign == '-') {
			hash_table.remove(el);
		}
		else if (sign == '?') {
			if (hash_table.is_in_table(el)) {
				outp << "true\n";
			}
			else {
				outp << "false\n";
			}
		}
	}

	cout << "runtime = " << clock() / 1000.0 << " \n";
	inp.close();
	outp.close();
	return 0;
}
