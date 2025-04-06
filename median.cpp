/*Во время аккредитации эксперту необходимо провести собеседование с обучающимися по образовательной программе. Но студентов достаточно много и побеседовать со всеми у эксперта нет возможности, поэтому он попросил учебный отдел выбрать троих представителей:
«слабый студент» — студент с самым низким «средним баллом зачетки»,
«средний студент» — студент с медианным «средним баллом зачетки»,
«сильный студент» — студент с самым высоким «средним баллом зачётки».
Считается, что у студента на образовательной программе медианный «средний балл зачётки», если при сортировке студентов по «среднему баллу зачётки» он оказывается ровно посередине списка.
Выведите в консоль три целых положительных числа, разделенных пробелами — идентификационные номера самого слабого, среднего и самого сильного студента.*/

#include <fstream>
#include <iostream>
using std::cout;
using std::cin;
using std::pair;

void make_heap(pair<int, float> M[], int n) {
	for (int i = 1; i < n; ++i) {
		int pos = i;
		while (pos != 0) {
			if (M[(pos - 1) / 2].second < M[pos].second) {
				std::swap(M[pos], M[(pos - 1) / 2]);
				pos = (pos - 1) / 2;
			}
			else {
				break;
			}
		}
	}
}

void shift_down(int pos, pair<int, float> M[], int size) {
	while (2 * pos + 1 < size) {
		int posMaxChild = 2 * pos + 1;
		if (2 * pos + 2 < size) {
			if (M[posMaxChild].second < M[posMaxChild + 1].second)
				++posMaxChild;
		}
		if (M[pos].second < M[posMaxChild].second) {
			std::swap(M[pos], M[posMaxChild]);
			pos = posMaxChild;
		}
		else
			break;
	}
}

void heap_sort(pair<int, float> M[], int n) {
	make_heap(M, n);
	int size = n; 
	for (int i = 1; i < n; ++i) {
		std::swap(M[0], M[size - 1]);
		--size;
		shift_down(0, M, size);
	}
}

int main(int argc, char* argv[])
{
	std::ifstream inp;
	inp.open(argv[1]);

	int kol;
	inp >> kol;
	pair<int, float>* array = new pair<int, float>[kol];
	float grade;
	pair<int, float> el;
	for (int i = 0; i < kol; ++i) {
		inp >> grade;
		el.first = i + 1;
		el.second = grade;
		array[i] = el;
	}
	heap_sort(array, kol);

	cout << array[0].first << " " << array[kol / 2].first  << " " << array[kol - 1].first;

	delete[] array;
	inp.close();
	return 0;
}
