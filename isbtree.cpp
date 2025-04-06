/*Проверка индекса
Для построения индекса использовалось B-дерево. В результате ошибки индекс мог быть поврежден.
В файл выгрузили дамп B-дерева. Нужно проверить по выгруженной информации правильно ли построен индекс (выполняются ли все свойства B-дерева).*/
#include <fstream>
#include <iostream>
using std::cout;
using std::cin;
int min_int = std::numeric_limits<int>::min();
int max_int = std::numeric_limits<int>::max();
bool is_sorted(const int* ar, const int n) {
	if (ar == nullptr) {
		return false;
	}
	for (int i = 1; i < n; ++i) {
		if (ar[i - 1] > ar[i]) {
			return false;
		}
	}
	return true;
}
struct Node {
	int id;
	bool leaf;    // является ли узел листом
	int kol_of_keys; //n;      // количество ключей узла
	int* keys;   // ключи узла
	int kol_of_children;
	int* children;    // c; указатели на детей узла
	int level; //  высота
	Node(int i = 0, bool l = true, int kk = 0, int* k = nullptr, int kc = 0, int* c = nullptr, int lev = 0) :
		id(i), leaf(l), kol_of_keys(kk), keys(k), kol_of_children(kc), children(c), level(l) {}
	~Node() {
		delete[] keys;
		delete[] children;
	}
};

int index_node(Node** arr, const int& kol_of_nodes, const int& num_of_node) {
	for (int i = 0; i < kol_of_nodes; ++i) {
		if (arr[i]->id == num_of_node) {
			return i;
		}
	}
	return -1;
}

bool is_node_correct(Node** arr, const int& kol_of_nodes, const int& index_of_node, int height, int left, int right) {
	arr[index_of_node][0].level = height;
	// проверяем отрезок значений ключей
	bool result = left < arr[index_of_node][0].keys[0] && arr[index_of_node][0].keys[0] < right;
	if (!result) {
		return false;
	}

	for (int i = 0; i < arr[index_of_node][0].kol_of_children; ++i) {
		int l = (i == 0 ? left : arr[index_of_node][0].keys[i - 1]);
		int r = (i == arr[index_of_node][0].kol_of_children - 1 ? right : arr[index_of_node][0].keys[i]);
		int child = index_node(arr, kol_of_nodes, arr[index_of_node][0].children[i]); // id ребенка // возможно проверить существует ли он
		result = is_node_correct(arr, kol_of_nodes, child, height + 1, l, r);
		if (!result) {
			return false;
		}
	}
	return true;
}
bool isBTree(Node** arr, const int& kol_of_nodes, const int& root_numb) {
	// проверяем интерваы
	int root_ind = index_node(arr, kol_of_nodes, root_numb);
	if (root_ind == -1) {
		return false;
	}
	arr[root_ind][0].level = 1;
	bool result = true;
	// проверяем детей на правльный значения ключей
	for (int i = 0; i < arr[root_ind][0].kol_of_children; ++i) {
		int child =index_node(arr, kol_of_nodes, arr[root_ind][0].children[i]); // id ребенка // возможно проверить существует ли он
		if (child == -1) {
			return false;
		}
		int left = (i == 0 ? min_int : arr[root_ind][0].keys[i - 1]);
		int right = (i == arr[root_ind][0].kol_of_children - 1 ? max_int : arr[root_ind][0].keys[i]);
		result = is_node_correct(arr, kol_of_nodes, child, 2, left, right);
		if (!result) {
			return false;
		}
	}
	// проверка на то, что листья на одинаковой глубине
	
	int level = 0;
	for (int i = 0; i < kol_of_nodes; ++i) {
		if (arr[i][0].leaf) {
			if (level != 0) {
				result = (level == arr[i][0].level);
				if (!result) {
					return false;
				}
			}
			else {
				level = arr[i][0].level;
			}
		}
	}
	return true;
}

int main(int argc, char* argv[])
{
	std::ifstream inp(argv[1]);
	srand(time(0));
	int n;  // число узлов
	int t;
	int root_number;
	inp >> n >> t >> root_number;
	char leaf_or_branch;
	int num_of_node;
	int kol_of_keys;
	int kol_of_child;
	int key_child;
	 Node** array = new Node*[n];
	for (int i = 0; i < n; ++i) {
		array[i] = nullptr;
	}
	bool result = true;
	for (int i = 0; i < n; ++i) {
		inp >> leaf_or_branch;
		if (leaf_or_branch == 'l') {
			// leaf: 0xномер_узла (количество_ключей: ключ1 ключ2 ...)
			for (int j = 0; j < 6; ++j) {
				inp >> leaf_or_branch;
			}
			inp >> num_of_node;
			inp >> leaf_or_branch; // (
			inp >> kol_of_keys;
			inp >> leaf_or_branch; // :
			// t-1 < количество ключей < 2t-1
			if (num_of_node != root_number && (kol_of_keys < t - 1 || kol_of_keys > 2 * t - 1) ||
				num_of_node == root_number && kol_of_keys > 2 * t - 1) {
				result = false;
				break;
			}
			int* keys = new int[kol_of_keys];
			for (int j = 0; j < kol_of_keys; ++j) {
				inp >> key_child;
				keys[j] = key_child;
			}
			//ключи в узле должны быть остортированы
			if (!is_sorted(keys, kol_of_keys)) {
				result = false;
				delete[] keys;
				break;
			}
			inp >> leaf_or_branch; // )

			Node* cur = new Node(num_of_node, true, kol_of_keys, keys, 0, nullptr, 0);
			array[i] = cur;
		}
		else if (leaf_or_branch == 'b') {
			// branch: 0xномер_узла (количество_ключей: ключ_1 ключ_2 ...) (количество_потомков: номер_узла_1 номер_узла_2 номер_узла_3 ...)
			for (int j = 0; j < 8; ++j) {
				inp >> leaf_or_branch;
			}
			inp >> num_of_node;
			inp >> leaf_or_branch; // (
			inp >> kol_of_keys;
			inp >> leaf_or_branch; // :
			// t-1 < количество ключей < 2t-1
			if (num_of_node != root_number && (kol_of_keys < t - 1 || kol_of_keys > 2 * t - 1) ||
				num_of_node == root_number && kol_of_keys > 2 * t - 1) {

				result = false;
				break;
			}

			int* keys = new int[kol_of_keys];
			for (int j = 0; j < kol_of_keys; ++j) {
				inp >> key_child;
				keys[j] = key_child;
			}
			//ключи в узле должны быть остортированы
			if (!is_sorted(keys, kol_of_keys)) {
				result = false;
				delete[] keys;
				break;
			}
			inp >> leaf_or_branch >> leaf_or_branch; // ) (
			inp >> kol_of_child;
			inp >> leaf_or_branch; // :

			// если  к ключей -> к+1 ребенок
			if (kol_of_child != kol_of_keys + 1) {
				result = false;
				delete[] keys;
				break;
			}
			int* children = new int[kol_of_child];
			for (int j = 0; j < kol_of_child; ++j) {
				inp >> key_child;
				children[j] = key_child;
			}
			inp >> leaf_or_branch; // )
			
			Node* cur = new Node(num_of_node, false, kol_of_keys, keys, kol_of_child, children, 0);
			array[i] = cur;
		}
		else {
			result = false;
			break;
		}
	}
	// осталось проверить интервалы и то, что листья на одной глубине
	if (result) { 
		result = isBTree(array, n, root_number); 
	}
	if (result) {
		cout << "yes";
	}
	else {
		cout << "no";
	}
	for (int i = 0; i < n; ++i) {
		delete array[i];
	}
	delete[] array;
	inp.close();
	return 0;
}
