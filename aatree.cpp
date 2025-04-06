/*AA-Tree
Реализовать контейнер множество с помощью самобалансирующегося AA-дерева.
На вход программе подаются строки, содержащие команды. Каждая строка содержит одну команду.
Команда «+ X» означает добавление в множество числа X, по модулю не превышающего 109.
Команда «− X» означает изъятие элемента из множества.
Команда «? X» означает проверку наличия числа X в множестве.*/
#include <fstream>
#include <iostream>
using std::cout;
using std::cin;

struct AATree {
	struct Node {
		int value;                  // значение вершины
		int level;               // высота вершины
		Node* left;                // указатель на левого потомка
		Node* right;            // указатель на правого потомка
		Node(int val, int lev, Node* l, Node* r) : value(val), level(lev), left(l), right(r) {}
	};
	Node* root;
	AATree() : root(nullptr) {}
	void remove(int value) {
		root = del(value, root);

	}
	void insert(int value) {
		root = insert_node(value, root);
	}
	Node* skew(Node* t) {
		if (t == nullptr) {
			return nullptr;
		}
		if (t->left == nullptr) {
			return t;
		}

		if (t->left->level == t->level) {
			Node* left = t->left;
			t->left = left->right;
			left->right = t;
			return left;
		}
		return t;
	}
	Node* split(Node* t) {
		if (t == nullptr) {
			return nullptr;
		}

		if (t->right == nullptr || t->right->right == nullptr)
			return t;
		if (t->level == t->right->right->level) {

			Node* tmp = t->right;

			Node* left_child = tmp->left;
			++(tmp->level);
			tmp->left = t;
			t->right = left_child;
			return tmp;

		}
		return t;
	}
	Node* insert_node(int x, Node* t) {
		if (root == nullptr) {
			root = new Node(x, 1, nullptr, nullptr);
			return root;
		}
		if (t == nullptr) {
			return new Node(x, 1, nullptr, nullptr);
		}
		else if (x < t->value) {
			t->left = insert_node(x, t->left);
		}
		else {
			t->right = insert_node(x, t->right);
		}
		t = skew(t);
		t = split(t);
		return t;
	}
	Node* decreaseLevel(Node* t) {
		int shouldBe;
		shouldBe = std::min(t->left ? t->left->level : 0,
			t->right ? t->right->level : 0) + 1;
		if (shouldBe < t->level) {
			t->level = shouldBe;
			if (t->right != nullptr && shouldBe < t->right->level) {
				t->right->level = shouldBe;
			}
		}
		return t;

	}
	Node* del(int x, Node* t) {
		if (t == nullptr) {
			cout << "didn't find\n";
			return t;
		}
		else if (x > t->value) {
			t->right = del(x, t->right);
		}
		else if (x < t->value) {
			t->left = del(x, t->left);
		}
				else {
			if (t->right == nullptr && t->left == nullptr) {
				delete t;
				return nullptr;
			}
			
			if (t->left == nullptr) {
				Node* tmp = t->right;
				t->right = nullptr;
				delete t;
				return tmp;
			}
			if (t->right == nullptr) {
				Node* tmp = t->left;
				t->left = nullptr;
				delete t;
				return tmp;
			}

			Node* successor = t->right;
			while (successor->left != nullptr) {
				successor = successor->left;
			}
			t->value = successor->value;
			t->right = del(successor->value, t->right);
			
		}


		t = decreaseLevel(t);
		t = skew(t);
		if (t) { // можно мб убрать
			t->right = skew(t->right);
		}
		if (t && t->right != nullptr) {
			t->right->right = skew(t->right->right);
		}
		t = split(t);
		if (t) { // можно мб убрать
			t->right = split(t->right);
		}
		return t;
	}
	bool is_in_tree(int x, Node* t) {
		if (t == nullptr) {
			return false;
		}
		if (t->value == x) {
			return true;
		}
		if (x < t->value) {
			return is_in_tree(x, t->left);
		}
		else {
			return is_in_tree(x, t->right);
		}
	}
	void deleteTree(Node* node) {
		if (node != nullptr) {
			if (node->left != nullptr) {
				deleteTree(node->left);
			}
			if (node->right != nullptr) {
				deleteTree(node->right);
			}
			delete node;
		}
	}
	int height() {
		return (root == nullptr ? 0 : root->level);
	}
	~AATree() {
		deleteTree(root);
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
	AATree tree;
	bool f;
	for (int i = 0; i < n; ++i) {
		inp >> sign >> el;
		if (sign == '+') {
			tree.insert(el);
			outp << tree.root->level << '\n';
		}
		else if (sign == '-') {
			tree.remove(el);
			outp << tree.height() << '\n';
		}
		else if (sign == '?') {
			f = tree.is_in_tree(el, tree.root);
			if (f) {
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
