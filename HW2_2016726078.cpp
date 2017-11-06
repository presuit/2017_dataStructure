#include <iostream>
#include <cstring>
#include <stack>
#include <cctype>
#include <string>
#include <limits>

using namespace std;

const char LEFT_PERENTHESIS = '(';
const char RIGHT_PERENTHESIS = ')';

string toPostfix(istream& ins);
int calculate(string str, int& errCode);

int main() {
	string postfix;
	int result;
	int errCode = 0;

	while (true) {
		cout << "Input data " << endl;
		postfix = toPostfix(cin);
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if (postfix == "0,") {
			cout << "Program Terminated" << endl;
			break;
		}
		else {
			cout << "postfix : " << postfix << endl;
			result = calculate(postfix, errCode);
			if (result == 0 && errCode > 0) {
				cout << "===================" << endl;
				cout << "#Calculate Error#" << endl;
				cout << "===================" << endl;
			}
			else {
				cout << "===================" << endl;
				cout << "result : \t" << result << endl;
				cout << "===================" << endl;
			}
		}
	}
	

	return 0;
}

string toPostfix(istream& ins) {
	//precondition : expression should be insfix style, should not be unvalided
	//postcondition : infix -> postfix.
	string result;
	stack<char>operations;
	int number;
	char symbol;

	while (ins && ins.peek() != '\n') {
		if (isdigit(ins.peek())) {
			//test
			cout << "--NUMBER_PRINT--" << endl;
			//number
			ins >> number;
			result += to_string(number);
			result += ",";
		}
		else if (strchr("+-*/^%", ins.peek()) != NULL) {
			//operations
			if (operations.empty()) {
				//test
				cout << "--OPERATIONS EMPTY INSERT--" << endl;
				//stack is empty
				ins >> symbol;
				operations.push(symbol);
			}
			else {
				//stack is not empty
				if (ins.peek() == '^') {
					//input value is ^
					while (operations.top() == '^') {
						symbol = operations.top();
						operations.pop();
						result += symbol;

						if (operations.empty()) {
							break;
						}
					}
					//test
					cout << "--OPERATIONS ^ INSERT--" << endl;
					ins >> symbol;
					operations.push(symbol);
				}
				else if (ins.peek() == '*' || ins.peek() == '/' || ins.peek() == '%') {
					switch (operations.top()) {
					case '^':
						while (operations.top() == '^') {
							symbol = operations.top();
							operations.pop();
							result += symbol;

							if (operations.empty()) {
								break;
							}
						}
						//test
						cout << "--OPERATIONS * / % INSERT--" << endl;
						ins >> symbol;
						operations.push(symbol);
						break;
					case '*':
					case '%':
					case '/':
						while (operations.top() == '*' || operations.top() == '/' || operations.top() == '%')
						{
							symbol = operations.top();
							operations.pop();
							result += symbol;
							
							if (operations.empty()) {
								break;
							}
						}
						//test
						cout << "--OPERATIONS * / % INSERT--" << endl;
						ins >> symbol;
						operations.push(symbol);
						break;
					case '+':
					case '-':
						//test
						cout << "--OPERATIONS * / % INSERT--" << endl;
						ins >> symbol;
						operations.push(symbol);
						break;
					case LEFT_PERENTHESIS:
						//test
						cout << "--OPERATIONS * / % INSERT--" << endl;
						ins >> symbol;
						operations.push(symbol);
						break;
					}
				}
				else if (ins.peek() == '+' || ins.peek() == '-') {
					while (operations.top() != LEFT_PERENTHESIS) {
						symbol = operations.top();
						operations.pop();
						result += symbol;
						
						if (operations.empty()) {
							break;
						}
					}
					//test
					cout << "--OPERATIONS + - INSERT--" << endl;
					ins >> symbol;
					operations.push(symbol);
				}
			}
		}
		else if (ins.peek() == LEFT_PERENTHESIS) {
			//test
			cout << "--LEFT_PARENTHESIS--" << endl;
			ins >> symbol;
			operations.push(symbol);
		}
		else if (ins.peek() == RIGHT_PERENTHESIS) {
			ins.ignore();
			//test
			cout << "--RIGHT_PARENTHESIS--" << endl;
			while (!operations.empty() && operations.top() != LEFT_PERENTHESIS) {
				symbol = operations.top();
				operations.pop();
				result += symbol;

				if (operations.empty()) {
					break;
				}
			}
			if (operations.empty()) {
				//there is no (
				cout << "Error : < Omitted ( >" << endl;
				return "";
			}
			else {
				// there is a ( , erase it.
				operations.pop();
			}
		}
		else {
			ins.ignore();
		}
	}
	while (!operations.empty() && operations.top() != LEFT_PERENTHESIS) {
		symbol = operations.top();
		operations.pop();
		result += symbol;

		if (operations.empty()) {
			break;
		}
	}

	if(!operations.empty()){
		cout << "Error : < Still remains ( in stack >" << endl;
		return "";
	}

	return result;
}

int calculate(string str, int& errCode) {
	//precondition : valid postfix string should be inserted.
	//postcondition : calculate postfix, return result which is integer type value
	string::iterator iter;
	stack<int>numbers;
	string number;
	int new_number;
	int result;
	int tmp;

	int operand1;
	int operand2;

	for (iter = str.begin(); iter != str.end(); ++iter) {
		if ( isdigit(*iter) ) {
			while (*iter != ',' ) {
				number += *iter;
				++iter;
			}
			//test
			cout << "number : " << number << endl;
			new_number = stoi(number);
			numbers.push(new_number);
			number = "";
		}
		else if ( strchr("+-*/^%", *iter) != NULL ) {
			if (!numbers.empty()) {
				operand2 = numbers.top();
				numbers.pop();
			}
			else {
				cout << "Error : < There is more operator, but there is no more number. >" << endl;
				errCode = 1;
				return 0;
			}
			if (!numbers.empty()) {
				operand1 = numbers.top();
				numbers.pop();
			}
			else {
				cout << "Error : < There is more operator, but there is no more number. >" << endl;
				errCode = 1;
				return 0;
			}
			
			switch (*iter) {
			case '^':
				tmp = operand1;
				for (int i = 0; i < operand2 - 1; i++) {
					operand1 *= tmp;
				}
				cout << "calculated [^] : " << operand1 << endl;
				numbers.push(operand1);
				break;
			case '*':
				numbers.push(operand1 * operand2);
				cout << "calculated [*] : " << operand1 * operand2 << endl;
				break;
			case '/':
				if (operand2 == 0) {
					cout << "Error" << endl;
					errCode = 2;
					return 0;
				}
				else {
					numbers.push(operand1 / operand2);
					cout << "calculated [/] : " << operand1 / operand2 << endl;
				}
				break;
			case '%':
				if (operand2 == 0) {
					cout << "Error" << endl;
					errCode = 2;
					return 0;
				}
				else {
					numbers.push(operand1 % operand2);
					cout << "calculated [%] : " << operand1 % operand2 << endl;
				}
				break;
			case '+':
				numbers.push(operand1 + operand2);
				cout << "calculated [+] : " << operand1 + operand2 << endl;
				break;
			case '-':
				numbers.push(operand1 - operand2);
				cout << "calculated [-] : " << operand1 - operand2 << endl;
				break;
			}
		}
	}
	
	if (numbers.empty()) {
		cout << "Error : < There is no entry in stack >" << endl;
		errCode = 3;
		return 0;
	}
	else {
		result = numbers.top();
		numbers.pop();
	}

	if (!numbers.empty()) {
		cout << "Error : < Uncalculated entry remains in stack>" << endl;
		errCode = 4;
		return 0;
	}

	return result;
}

