/* K-ые порядковые статистики в больших массивах
Но в этот раз массив такой большой, что не влезает в оперативную память!*/
#include <fstream>
#include <iostream>
using std::cout;
using std::cin;

void make_heap(int M[], int n) {
	for (int i = 1; i < n; ++i) {
		int pos = i;
		while (pos != 0) {
			if (M[(pos - 1) / 2] < M[pos]) {
				std::swap(M[pos], M[(pos - 1) / 2]);
				pos = (pos - 1) / 2;
			}
			else {
				break;
			}
		}
	}
}

void shift_down(int pos, int M[], int size) {
	while (2 * pos + 1 < size) {
		int posMaxChild = 2 * pos + 1;
		if (2 * pos + 2 < size) {
			if (M[posMaxChild] < M[posMaxChild + 1])
				++posMaxChild;
		}
		if (M[pos] < M[posMaxChild]) {
			std::swap(M[pos], M[posMaxChild]);
			pos = posMaxChild;
		}
		else
			break;
	}
}

int main(int argc, char* argv[])
{
	std::ifstream inp(argv[1]);
	std::ofstream outp(argv[2]);

	int kol;
	int k1;
	int k2;
	inp >> kol >> k1 >> k2;

	int A;
	int B;
	int C;
	int x1;
	int x2;

	inp >> A >> B >> C >> x1 >> x2;

	int* heap = new int[k2];
	heap[0] = x1;
	heap[1] = x2;

	srand(time(0));

	// добавляем первые к2 элементов в массив и делаем из них max-heap
	for (int i = 2; i < k2; ++i) {
		heap[i] = A * heap[i - 2] + B * heap[i - 1] + C;
	}

	int x_i_minus_one = heap[k2 - 1];
	int x_i_minus_two = heap[k2 - 2];
	make_heap(heap, k2);

  //проверяем меньше ли элемент вершины, если да то заменяем вершину на данный элемент, выкидываем вершину так как есть к2 элементов меньше ее 
  //и запускаем процедуру опускания элемента
	int x_i;
	for (int i = k2; i < kol; ++i) {
		x_i = A * x_i_minus_two + B * x_i_minus_one + C;
		if (x_i < heap[0]) {
			heap[0] = x_i;
			shift_down(0, heap, k2);
		}
		x_i_minus_two = x_i_minus_one;
		x_i_minus_one = x_i;
	}

  //найдем порядковые статистики, начиная с самой больше идя к самой маленькой, так как элементы хранятся в max-heap
  //значит в вершине хранится k2 по счету больший элемент
	int* statistics = new int[k2 - k1 + 1];
	statistics[k2 - k1] = heap[0];
	int size = k2;

	for (int i = 1; i <= k2 - k1; ++i) {
		std::swap(heap[0], heap[size - 1]);
		--size;
		shift_down(0, heap, size);
		statistics[k2 - k1 - i] = heap[0];
	}

	outp << statistics[0];
	for (int i = 1; i < k2 - k1 + 1; ++i) {
		outp << ' ';
		outp << statistics[i];
	}
	cout << "runtime = " << clock() / 1000.0 << " \n";
	delete[] statistics;
	delete[] heap;
	inp.close();
	outp.close();
	return 0;
}
