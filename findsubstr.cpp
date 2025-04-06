/*Написать программу, которая подсчитывает число вхождений заданной строки в строки файла в качестве подстроки.

Искомая строка и имя файла заданы в командной строке (соответственно первый и второй аргументы командной строки).

Ответ (количество вхождений) вывести в консоль.*/

#include <fstream>
#include <iostream>
using std::cout;
using std::cin;
void buildP(char* t, int t_length, int* p) {
	p[0] = 0;
	int i = 1;
	int j = 0;
	while (i < t_length) {
		if (t[i] == t[j]) {
			p[i] = j + 1;
			++i;
			++j;
		}
		else {
			if (j > 0) {
				j = p[j-1];
			}
			else {
				p[i] = 0;
				++i;
			}
		}
	}
}

int main(int argc, char* argv[])
{
	std::ifstream inp(argv[2]);
	srand(time(0));

	char* substr = argv[1];
	int substr_length = 0;
	while (substr[substr_length] != '\0') {
		++substr_length;
	}
	int* prefix = new int[substr_length];
	buildP(substr, substr_length, prefix);
	
	char ch;
	int count = 0;
	int pref_ind = 0; 

	const int bufferSize = 1024;
	char buffer[bufferSize]; // Массив для хранения считанных символов
	int i = 0;
	while (inp.getline(buffer, bufferSize)) {
		int length = inp.gcount(); // Количество считанных символов
		while (i<length) {
			if (buffer[i] == substr[pref_ind]) {
				++pref_ind;
				if (pref_ind == substr_length) {
					++count;
					pref_ind = prefix[pref_ind - 1];
				}
			}
			else if (pref_ind != 0) {
				pref_ind = prefix[pref_ind - 1];
			}
			++i;
		}
		i = 0;
		pref_ind = 0;
	}
	cout << count;
	delete[] prefix;
	inp.close();
	return 0;
}
