/*В организации решили хранить все пароли в текстовом файле, а чтобы определить конкретный одноразовый пароль пользователю надо будет просто сделать несколько фаз LSD-сортровки.
Вам необходимо написать генератор одноразовых паролей.*/
#include <fstream>
#include <iostream>
using std::cout;
using std::cin;


unsigned char** radixSort(unsigned char** ar, const int& n, const int& m, const int& phase) {
	// n - размер массива
	// m - колво разрядов
	// B - вспомогательный массив, нужный порядок элементов исходного массива
	// C - вспомогательный массив, для объектов где одинаковый разряд

	int C[256]; // для подсчета количества элементов с данным значением символав разряде
	unsigned char ch;
	for (int i = m - 1; i >= m - phase; --i) { // проходимся по каждому разряду
		for (int j = 0; j < 256; ++j) {
			C[j] = 0;
		}
		for (int j = 0; j < n; ++j) { // подсчет колва элементов с определенным сиволом для текущего разряда
			ch = ar[j][i]; //сивол в iй разряд слова //digit(A[j], i)
			++C[static_cast<int>(ch)];
		}
		int count = 0;
		int tmp;
		for (int j = 0; j < 256; ++j) {
			tmp = C[j];
			C[j] = count; // присваиваем элементу начальный индекс в массивe B
			count += tmp;
		}
		unsigned char** B = new unsigned char* [n];
		
		for (int j = 0; j < n; ++j) {
			ch = ar[j][i]; //iй разряд слова
			B[C[static_cast<int>(ch)]] = ar[j];
			++C[static_cast<int>(ch)];
		
		}
		unsigned char** del = ar;
		ar = B;
		delete[] del;	
	}
	return ar;
}

int main(int argc, char* argv[])
{
	std::ifstream inp(argv[1]);
	std::ofstream outp(argv[2]);

	int n; // число строк
	int m; // длина
	int k; // число фаз цифровой сортировки
	inp >> n >> m >> k;

	
	unsigned char** passwords_transp = new unsigned char* [n];
	for (int i = 0; i < n; ++i) {
		passwords_transp[i] = new unsigned char[m];
	}
	unsigned char el;
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			inp >> el;
			passwords_transp[j][i] = el;
		}
	}
	srand(time(0));

	passwords_transp = radixSort(passwords_transp, n, m, k);
	cout << "runtime = " << clock() / 1000.0 << " \n";

	for (int i = 0; i < n; ++i) {
		outp << passwords_transp[i][0];
	}

	for (int i = 0; i < n; i++) {
		delete[] passwords_transp[i];
	}
	delete[] passwords_transp;
	inp.close();
	outp.close();
	return 0;
}
