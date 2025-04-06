/* Напишите программу, которая моделирует игру в пьяницу и определяет, кто выигрывает.
В игре в пьяницу карточная колода (52 карты) раздается поровну двум игрокам. Игроки не смотрят в свои карты, а кладут их в стопку рядом с собой. 
Далее они вскрывают по одной верхней карте, и тот, чья карта старше, забирает себе обе вскрытые карты, которые кладутся под низ его колоды. Тот, кто остается без карт – проигрывает.*/
#include <iostream>
#include <fstream>
using std::cout;
using std::endl;
using std::cin;

const int STACK_SZ = 52;
struct stack {
	int stack_[STACK_SZ];
	int stack_size = 0;
	int pop() {
		return stack_[--stack_size];
	}
	void push(int x) {
		stack_[stack_size++] = x;
	}
	bool is_empty() const {
		return (stack_size == 0);
	}
};
struct queue {
	stack right; // for push
	stack left; // for pop

	int pop() {
		if (left.is_empty()) {
			while (!right.is_empty()) {
				left.push(right.pop());
			}
		}
		return left.pop();
	}
	void push(int x) {
		right.push(x);
	}
	bool is_empty() const {
		return right.is_empty() && left.is_empty();
	}
};
int numberOFcard(char s) {
	if ('2' <= s && s <= '9') {
		return (static_cast<int>(s) - 48);
	}
	if (s == '1') {
		return 10;
	}
	if (s == 'J') {
		return 11;
	}
	if (s == 'Q') {
		return 12;
	}
	if (s == 'K') {
		return 13;
	}
	if (s == 'A') {
		return 14;
	}
	return 0;
}
int main(int argc, char* argv[])
{
	std::ifstream inp;
	inp.open(argv[1]);

	queue first;
	queue second;

	char s = static_cast<char>(inp.get());

	for (int i = 0; i < 52; ++i) {
		if (i < 26) {
			first.push(numberOFcard(s));
		}
		else {
			second.push(numberOFcard(s));
		}
		if (s == '1') {
			inp.get();
		}
		s = static_cast<char>(inp.get());
		while (numberOFcard(s) == 0 && !inp.eof()) { // read symbols before suit(масть) and suit and after suit
			s = static_cast<char>(inp.get());
		}
	}

	inp.close();

	//GAME START
	int numberOfMoves = 0;
	while (!first.is_empty() && !second.is_empty()) {
		int cardOFfirst = first.pop();
		int cardOFsecond = second.pop();
		++numberOfMoves;
		if (cardOFfirst == cardOFsecond) {
			queue tableCardsOfFirst;
			queue tableCardsOfSecond;

			tableCardsOfFirst.push(cardOFfirst); // cards that player put on table
			tableCardsOfSecond.push(cardOFsecond);
			while (cardOFfirst == cardOFsecond && (!first.is_empty() && !second.is_empty())) { // case of arguement
				cardOFfirst = first.pop();
				cardOFsecond = second.pop();
				tableCardsOfFirst.push(cardOFfirst);
				tableCardsOfSecond.push(cardOFsecond);
				++numberOfMoves;
			}
			if (cardOFfirst < cardOFsecond || cardOFfirst == 14 && cardOFsecond == 2) {
				while (!tableCardsOfFirst.is_empty()) {
					second.push(tableCardsOfFirst.pop());
					second.push(tableCardsOfSecond.pop());
				}
			}
			else if (cardOFfirst > cardOFsecond || cardOFfirst == 2 && cardOFsecond == 14) {
				while (!tableCardsOfFirst.is_empty()) {
					first.push(tableCardsOfFirst.pop());
					first.push(tableCardsOfSecond.pop());
				}
			}
		}
		else if ((cardOFfirst < cardOFsecond) && !(cardOFfirst == 2 && cardOFsecond == 14 )|| cardOFfirst == 14 && cardOFsecond == 2) {
			second.push(cardOFfirst);
			second.push(cardOFsecond);
		}
		else if ((cardOFfirst > cardOFsecond) && !(cardOFfirst == 14 && cardOFsecond == 2) || cardOFfirst == 2 && cardOFsecond == 14) {
			first.push(cardOFfirst);
			first.push(cardOFsecond);
		}
		if (numberOfMoves > 1000000) {
			cout << "unknown";
			return 0;
		}
	}
	
	if (first.is_empty() && second.is_empty()) {
		cout  << "draw";
	}
	else if (!first.is_empty()) {
		cout << "first";
	}
	else {
		cout << "second";
	}
	return 0;

}
