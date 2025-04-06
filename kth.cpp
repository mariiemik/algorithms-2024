/*K-ые порядковые статистики
В массиве из n элементов нужно найти k1-ое, (k1 + 1)-ое, …, k2-ое в порядке возрастания в этом массиве.*/

#include <fstream>
#include <iostream>
using std::cout;
using std::cin;
int f_part(int arr[], int low, int high) {
	std::swap(arr[low], arr[(low + high) / 2]);
	int support_element = arr[low];
	int i = low + 1;
	int j = high;
	while (true) {
		while (i < high && arr[i] < support_element) {
			++i;
		}
		while (arr[j] > support_element)
		{
			--j;
		}
		if (i >= j) {
			break;
		}
		std::swap(arr[i], arr[j]);
		++i;
		--j;
	}
	std::swap(arr[low], arr[j]);
	return j;
}
void f_kth_element(int arr[], int low, int high, const int& k1, const int& k2) {
	int piv = f_part(arr, low, high);
	if (piv > k1 - 1 && low < piv -1) {
		f_kth_element(arr, low, piv - 1, k1, k2);
	}
	if (piv < k2 - 1 && piv + 1 < high) {
		f_kth_element(arr, piv + 1, high, k1, k2);
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

	int* array = new int[kol];

	array[0] = x1;
	array[1] = x2;

	for (int i = 2; i < kol; ++i) {
		array[i] = A * array[i - 2] + B * array[i - 1] + C;
	}

	srand(time(0));
	f_kth_element(array, 0, kol - 1, k1, k2);
	cout << "runtime = " << clock() / 1000.0 << " \n";

	outp << array[k1 - 1];
	for (int i = k1; i < k2; ++i) {
		outp << ' ';
		outp << array[i];
	}

	delete[] array;
	inp.close();
	outp.close();
	
	return 0;
}
