/*Реализуйте работу очереди. На вход программе подаются строки, содержащие команды. Каждая строка содержит одну команду.
Команда «+ X» означает добавление в очередь числа X (число по модулю не превышает 109).
Команда «−» означает изъятие элемента из очереди.
Команда «?» означает поиск максимальной разности (разности между максимальным и минимальным элементами) в очереди.*/

#include <iostream>
#include <fstream>
using std::cout;
using std::cin;


struct StackNode {
	int value;
	StackNode* before_added; // указатель на элемент который был добавлен первее
	StackNode* after_added; // указатель на элемент который был после данного
	StackNode(int val = 0) : value(val), before_added(nullptr), after_added(nullptr) {}
};
struct stackList {
	StackNode* first;
	StackNode* last;

	stackList() : first(nullptr), last(nullptr) {}

	void push(int val) {
		StackNode* add = new StackNode(val);
		if (first == nullptr) {
			first = add;
		}
		else {
			add->before_added = last;
			last->after_added = add;
		}
		last = add;
	}
	void push(StackNode* val) {
		if (first == nullptr) {
			first = val;
		}
		last = val;		
	}

	StackNode* pop_left() {
		// так как элементы после перемещения из левого в правый стек
		// используется, не удаляем указатель на элемент, он будет удален после того, как
		// будет сделан pop() из всей очереди

		if (last == first){//(ret->before_added == first->before_added) { //(ret == first) {
			first == nullptr; //wwhhhyyyyy 
		}
		if (last->before_added == nullptr) {
			first = nullptr;
		}

		StackNode* ret = last;
		
		last = last->before_added;
		return ret;
	}
	void pop_right() {
		if (last == first) {//(ret->before_added == first->before_added) { //(ret == first) {
			first == nullptr; //wwhhhyyyyy 
		}
		if (last->after_added == nullptr) {
			first = nullptr;
		}

		StackNode* ret = last;

		last = last->after_added;
		if (last != nullptr) {
			last->before_added = nullptr;
		}
		delete ret;
	}

	bool if_empty() {
		return (last == nullptr);
	}

};
struct MinMaxNode {
	int Min;
	int Max;
	MinMaxNode* prev;
	MinMaxNode() : Min(0), Max(0), prev(nullptr) {}
};
struct MinMaxStackList {
	MinMaxNode* first;
	MinMaxNode* last;
	MinMaxStackList() : first(nullptr), last(nullptr) {}

	void push(int min, int max) {
		MinMaxNode* add = new MinMaxNode;
		add->Min = min;
		add->Max = max;
		add->prev = last;
		last = add;
		if (first == nullptr) {
			first = add;
		}
	}
	void pop() {
		MinMaxNode* del = last;
		if (last == first) {
			first = nullptr;
		}
		last = last->prev;
		delete del;
	}
	int last_min() {
		return last->Min;
	}
	int last_max() {
		return last->Max;
	}
};
struct queueList {
	stackList left; // for push
	// храним макс и мин на весь массив для левого стека, т.к они меняться не будут после удаления элемента
	int min_in_left;
	int max_in_left;
	stackList right; // for pop
	//для правого стека используем персистентный стек
	MinMaxStackList max_min_in_right;

	queueList() : left(),min_in_left(0), max_in_left(0), right(), max_min_in_right() {}

	void push(int val) {
		if (left.if_empty()) {
			min_in_left = val;
			max_in_left = val;
		}
		else {
			if (min_in_left > val) {
				min_in_left = val;
			}
			else if (max_in_left < val) {
				max_in_left = val;
			}
		}
		left.push(val);
	}

	void pop() { // здесь нужно сделать delete узла StackNode
		if (right.if_empty()) { // тогда перемещаем из левого в правый стек

			right.push(left.pop_left());
			int CurMax = right.last->value;
			int CurMin = CurMax; 
			max_min_in_right.push(CurMin, CurMax);

			while (left.last != nullptr)
			{
				right.push(left.pop_left());
				if (right.last->value < max_min_in_right.last_min()) {
					max_min_in_right.push(right.last->value, max_min_in_right.last_max());
				}
				else if (right.last->value > max_min_in_right.last_max()) {
					max_min_in_right.push(max_min_in_right.last_min(), right.last->value);
				}
				else {
					max_min_in_right.push(max_min_in_right.last_min(), max_min_in_right.last_max());
				}
			}
		}
		right.pop_right();
		max_min_in_right.pop();
	}

	int what_difference(){	
		int min;
		int max;
		if (left.if_empty()) {
			min = max_min_in_right.last_min();
			max = max_min_in_right.last_max();
			return max - min;
		}
		else if (right.if_empty()) {
			max = max_in_left;
			min = min_in_left;
			return max - min;
		}
		
		if (max_min_in_right.last_min() < min_in_left) {
			min = max_min_in_right.last_min();
		}
		else{
			min = min_in_left;
		}
		if (max_min_in_right.last_max() < max_in_left) {
			max = max_min_in_right.last_max();
		}
		else {
			max = max_in_left;
		}

		return max - min;
	}
};

int main(int argc, char* argv[])
{
	std::ifstream inp;
	inp.open(argv[1]);
	std::ofstream out;
	out.open(argv[2]);

	int kol;
	inp >> kol;
	char sign;

	queueList numbers;
	for (int i = 0; i < kol; ++i) {
		inp >> sign;
		if (sign == '+') {
			int num;
			inp >> num;
			numbers.push(num);
		}
		else if (sign == '-') {
			numbers.pop();
		}
		else if (sign == '?') {
			out << numbers.what_difference() << '\n';
		}
	}
	while (!numbers.right.if_empty()) {
		numbers.pop();
	}
	inp.close();
	out.close();
	return 0;
}
