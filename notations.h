#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>

using namespace std;

namespace nts {
	void stream_cleaning() {
		cin.clear();
		cin.ignore(cin.rdbuf()->in_avail());
	}

	char* int_to_char(int i) {
		char* str = new char[15];
		str[0] = '\0';
		while (i > 0) {
			for (int i = 14; i > 0; i--)//����� ������
				str[i] = str[i - 1];
			str[0] = (unsigned char)(48 + i % 10);
			i = i / 10;
		}
		return str;
	}

	int insert_variable(char*& expression, int i, int value) {
		char* c_value = int_to_char(value);
		for (int i = 14; i > 0; i--)
			c_value[i] = c_value[i - 1];
		c_value[0] = '(';//���������� ������ ��� ���������� ����������� � ������������ �����
		for (int i = 0; i < 15; i++)
			if (c_value[i] == '\0') {
				c_value[i] = ')';
				c_value[i + 1] = '\0';
				break;
			}
		int c_value_lenght = strlen(c_value);
		char* c_value_ptr = c_value;
		for (int j = i; j < i + c_value_lenght; j++) {//������� �������� �� ����� ����������
			if (j != i)
				for (int k = 99; k > j; k--)
					expression[k] = expression[k - 1];
			expression[j] = *c_value_ptr;
			c_value_ptr++;
		}
		delete[] c_value;
		return c_value_lenght - 1;
	}

	void filling_variables(char*& expression) {
		bool exit = 1;
		for (int i = 0; i < 100; i++) {//����� ����������
			if ((unsigned char)expression[i] > 96 and (unsigned char)expression[i] < 123)//������� ����� ����. ��������
				exit = 0;
			if (expression[i] == '\0')
				break;
		}
		char name;
		int value;
		while (!exit) {
			exit = 1;
			cout << "������� ��� ���������� � �� �������� ����� ������\n";
			cin >> name;
			if (cin >> value);//���� ��� �������, ���� ����������, ���� ����, ���������� �������, ����� ���� ����������
			else {
				cin.clear();
				cin.get();
				cin >> value;
			}
			stream_cleaning();

			for (int i = 0; i < 100; i++) {//����� � ������� ���� ���������� � ������ name
				if (expression[i] == '\0')
					break;

				if (expression[i] == name)
					i += insert_variable(expression, i, value);

				if ((unsigned char)expression[i] > 96 and (unsigned char)expression[i] < 123 and expression[i] != name)
					exit = 0;
			}
			cout << expression << endl;
		}
	}

	bool check_reverse_polish_notation(char* expression) {
		int counter_operands = 0, counter_operations = 0;
		unsigned char c;//������� � ����������� ������ ��� ����� �������� ��������� � �������� ����
		for (int i = 0; i < 100; i++) {
			c = expression[i];
			if (c >= 48 and c <= 57 or c >= 97 and c <= 122)//��������
				counter_operands++;
			else if (c == 42 or c == 43 or c == 45 or c == 47)//��������
				counter_operations++;
			else if (c != '\0')
				return 0;
			else
				break;
			if (counter_operands <= counter_operations)//���������� ��������� ������ �������� ���������� ��������� � ��������
				return 0;
		}
		if (counter_operands != counter_operations + 1)
			return 0;
		else
			return 1;
	}

	bool check_polish_notation(char* expression) {
		int counter_operands = 0, counter_operations = 0;
		int i = 0;
		while (expression[i] != '\0')//�������� ��� ��, ��� �������� ��������, �� � �����
			i++;
		unsigned char c;
		for (int j = i - 1; j >= 0; j--) {
			c = expression[j];
			if (c >= 48 and c <= 57 or c >= 97 and c <= 122)//��������
				counter_operands++;
			else if (c == 42 or c == 43 or c == 45 or c == 47)//��������
				counter_operations++;
			else
				return 0;
			if (counter_operands <= counter_operations)
				return 0;
		}
		if (counter_operands != counter_operations + 1)
			return 0;
		else
			return 1;
	}

	bool service_check_simple_expression(char*& expression, bool first) {
		bool b = 0;
		int counter_operands = 0, counter_operations = 0;
		unsigned char c = *expression;
		while (!(c == ')' or c == '\0')) {
			b = 1;
			if (c >= 48 and c <= 57 or c >= 97 and c <= 122)//����� � ����� ����. ��������
				counter_operands++;
			else if ((c == 42 or c == 43 or c == 45 or c == 47) and counter_operands)//��������
				counter_operations++;
			else if (c != '(')
				return 0;

			if (c == '(') {
				if (!service_check_simple_expression(++expression, 0))//�������� ������ �������� ����������� ����� �������
					return 0;
				counter_operands++;//��, ��� � �������, ���� �������
				c = *expression;//�������� ���� �������� � ������� �������
			}
			else {
				expression++;
				c = *expression;//������� � ���������� �������
			}
			if (counter_operands >= 2)//�������� ��������� ��������� � ��������
				if (counter_operations != 1)
					return 0;
				else {
					counter_operands--;
					counter_operations--;
				}
			if (counter_operations >= 2)
				return 0;
		}
		if (!first)//���� ����� ������ ������ ')', �� �� ��������
			if (c != '\0')
				expression++;
		return b;
	}

	bool check_simple_expression(char* expression) {//��� ����, ����� �� ��������� � main ��������� �� ���������, ������������ ��� �������
		if (service_check_simple_expression(expression, 1) and *expression == '\0')
			return 1;
		else
			return 0;
	}

	template<class T>
	struct Stack {
		T info = 0;
		Stack* previous = nullptr;
	};

	int char_to_int(char*& str) {//������� �� char(����� ����� �������) � int
		int in = 0;
		unsigned char c = *str;
		while (c >= 48 and c <= 57) {
			in *= 10;
			in += c - 48;
			str++;
			c = *str;
		}
		return in;
	}

	int calculating_reverse_polish_notation(char* expression) {
		Stack<int>* head = nullptr;
		Stack<int>* new_s = nullptr;
		int summ = 0;
		unsigned char c;
		for (c = *expression; c != '\0'; c = *(++expression)) {
			if (c >= 48 and c <= 57 or c == '(') {//��������
				new_s = new Stack<int>;//�������� ������ ������� ����� ��� ��������
				new_s->previous = head;
				head = new_s;
				if (c != '(')//� ������� ����� ������ 9
					head->info = c - 48;
				else {
					head->info = char_to_int(++expression);
				}
			}
			else {//���������
				int operand1 = head->info;//������� 2 ��������� ��������
				new_s = head->previous;
				delete head;//��������������, 1 �������
				head = new_s;
				int operand2 = head->info;
				switch (c) {
				case '*':
					summ = operand2 * operand1;
					break;
				case '+':
					summ = operand2 + operand1;
					break;
				case '-':
					summ = operand2 - operand1;
					break;
				case '/':
					summ = operand2 / operand1;
					break;
				}
				head->info = summ;//�������� ��������� �� ����� 2 ��������
			}
		}
		delete head;
		return summ;
	}

	int reverse_char_to_int(char*& str, int& i) {//������� �� char(����� ������ ������) � int
		int in = 0, counter = 0;
		unsigned char c = *str;
		while (c >= 48 and c <= 57) {
			in += (c - 48) * pow(10, counter);
			str--;
			c = *str;
			i--;
			counter++;
		}
		return in;
	}

	int calculating_polish_notation(char* expression) {
		int i = strlen(expression);
		while (*expression != '\0')//������� � ����� ���������
			expression++;
		Stack<int>* head = nullptr;
		Stack<int>* new_s = nullptr;
		int summ = 0;
		unsigned char c;
		for (c = *(--expression); i > 0; c = *(--expression)) {//���������� ������ ������
			if (c >= 48 and c <= 57 or c == ')') {//��������
				new_s = new Stack<int>;
				new_s->previous = head;
				head = new_s;
				if (c != ')')
					head->info = c - 48;
				else {
					head->info = reverse_char_to_int(--expression, --i);
				}
			}
			else {//���������
				int operand1 = head->info;
				new_s = head->previous;
				delete head;
				head = new_s;
				int operand2 = head->info;
				switch (c) {
				case '*':
					summ = operand1 * operand2;
					break;
				case '+':
					summ = operand1 + operand2;
					break;
				case '-':
					summ = operand1 - operand2;
					break;
				case '/':
					summ = operand1 / operand2;
					break;
				}
				head->info = summ;
			}
			i--;
		}
		delete head;
		return summ;
	}

	int normal_record_calculation(char*& expression) {
		Stack<int>* operands = nullptr;//����� ��� ��������� � �������������� ��������
		Stack<char>* operators = nullptr;
		Stack<int>* newi;
		Stack<char>* newc;
		unsigned char c = *expression;
		while (!(c == ')' or c == '\0')) {
			if (c == '*' or c == '/' or c == '-' or c == '+') {//��������� �������� � ����
				newc = new Stack<char>;
				newc->previous = operators;
				operators = newc;
				operators->info = c;
				expression++;
				c = *expression;
			}
			if (c == '(') {//��������� �������� � ����
				newi = new Stack<int>;
				newi->previous = operands;
				operands = newi;
				operands->info = normal_record_calculation(++expression);//����������� �������� ��������� � �������
				c = *expression;
			}
			else if (c >= 48 and c <= 57) {//��������� �������� � ����
				newi = new Stack<int>;
				newi->previous = operands;
				operands = newi;
				operands->info = char_to_int(expression);
				c = *expression;
			}
			if (operators != nullptr and (operators->info == '*' or operators->info == '/')) {//������� ������� ������������ ��������
				if (operators->info == '*')
					operands->previous->info *= operands->info;
				if (operators->info == '/')
					operands->previous->info /= operands->info;
				newi = operands->previous;
				delete operands;
				operands = newi;
				newc = operators->previous;
				delete operators;
				operators = newc;
			}
		}

		if (c == ')')
			expression++;
		while (operands->previous != nullptr) {//������� �������������� ��������
			switch (operators->info) {
			case '+':
				operands->previous->info += operands->info;
				break;
			case '-':
				operands->previous->info -= operands->info;
				break;
			}
			newi = operands->previous;
			delete operands;
			operands = newi;
			newc = operators->previous;
			delete operators;
			operators = newc;
		}
		int summ = operands->info;
		delete operands;
		return summ;
	}

	//int normal_record_calculation(char*& expression) {//���������� �������� ��������� ��� ����� � ���������� ��������
	//	char operat = 0;
	//	int summ = 0;
	//	unsigned char c = *expression;
	//	while (!(c == ')' or c == '\0')) {
	//		if (c == '(') {
	//			switch (operat) {
	//			case '*':
	//				summ *= normal_record_calculation(++expression);
	//				operat = 1;
	//				break;
	//			case '/':
	//				summ /= normal_record_calculation(++expression);
	//				operat = 1;
	//				break;
	//			case '-':
	//				summ -= normal_record_calculation(++expression);
	//				operat = 1;
	//				break;
	//			case '+':
	//				summ += normal_record_calculation(++expression);
	//				operat = 1;
	//				break;
	//			case 0:
	//				summ = normal_record_calculation(++expression);
	//				operat = 1;
	//				break;
	//			case 1:
	//				summ *= normal_record_calculation(++expression);
	//				break;
	//			}
	//			c = *expression;
	//		}
	//
	//		if (c == '*' or c == '/' or c == '-' or c == '+')
	//			operat = c;
	//		if (c >= 48 and c <= 57) {
	//			switch (operat) {
	//			case '*':
	//				summ *= reverse_char_to_int(expression);
	//				break;
	//			case '/':
	//				summ /= reverse_char_to_int(expression);
	//				break;
	//			case '-':
	//				summ -= reverse_char_to_int(expression);
	//				break;
	//			case '+':
	//				summ += reverse_char_to_int(expression);
	//				break;
	//			case 0:
	//				summ = reverse_char_to_int(expression);
	//				break;
	//			case '1':
	//				summ *= reverse_char_to_int(expression);
	//				break;
	//			}
	//			operat = 1;
	//			c = *expression;
	//		}
	//		else if (c != '\0') {
	//			expression++;
	//			c = *expression;
	//		}
	//	}
	//	if (c == ')')
	//		expression++;
	//	return summ;
	//}

	void test_generation(char* filename, int quantity) {
		int q_operands, q_operations, cout_operands;
		ofstream file(filename);
		char* expression = new char[50];
		for (int j = 0; j < quantity; j++) {
			char* c;
			/*file.write("������� ", strlen("������� "));
			c = int_to_char(j + 1);
			file.write(c, strlen(c));
			delete[] c;
			file.write("\n������ ���������:\n1) ", strlen("\n������ ���������:\n1) "));*/
			cout_operands = q_operands = 5 + rand() % 4;//��������� ���������� ��������� � ��������� 1, ��� ������ �����, ��� ������ ����� � ��������� 2
			q_operations = q_operands - 1;
			for (int i = 0; i < 50; i++)
				expression[i] = '\0';
			char* expression_ptr = expression;
			for (expression_ptr; q_operands or q_operations; 1) {//�������� ��������
				if (rand() % 2) {//����������� �������� ������� - 50%, �������� - 50%, ���� ���������� ��������
					if (q_operands) {//���� ��� �������� ��������� ��������
						c = int_to_char(rand() % 15 + 1);//����� �������� ��������
						strcat(expression_ptr, c);
						delete[] c;
						while (*expression_ptr != '\0')
							expression_ptr++;
						*expression_ptr = ' ';
						q_operands--;
						expression_ptr++;
					}
				}
				else {
					if (q_operands < q_operations) {		//���� ���������� ��������� ������ ���������� ��������
						*expression_ptr = rand() % 3 + 1;	//��������������, ������ ��� ������� ��������� ����� �������� (������ �������� ������ 9 � ������� �� ����),
															//� ��������� ����� ���������
						switch (*expression_ptr) {
						case 1:
							*expression_ptr = '+';
							break;
						case 2:
							*expression_ptr = '*';
							break;
						case 3:
							*expression_ptr = '-';
							break;
						}
						q_operations--;
						expression_ptr++;
					}
				}
			}
			file.write(expression, strlen(expression));//������ ������ � ����

			file << "\n";
			//file.write("\n2) ", strlen("\n2) "));
			q_operands = 5 + 8 - cout_operands;//���������� ��������� � ������������ � 1 ����������
			q_operations = q_operands - 1;
			for (int i = 0; i < 50; i++)
				expression[i] = '\0';
			expression_ptr = expression;
			for (expression_ptr; q_operands or q_operations; 1) {//�������� ��������
				if (rand() % 2) {
					if (q_operands) {
						for (int i = 49; i > 0; i--)//����� ��������� ������
							expression_ptr[i] = expression_ptr[i - 1];
						*expression_ptr = ' ';
						c = int_to_char(rand() % 15 + 1);
						for (int j = strlen(c); j > 0; j--) {//����� ������ �� ���������� ����� ����� � ��� ������
							for (int i = 49; i > 0; i--)
								expression_ptr[i] = expression_ptr[i - 1];
							*expression = c[j - 1];//��������������, ������ ����� � �����
						}
						q_operands--;
					}
				}
				else {
					if (q_operands < q_operations) {
						for (int i = 49; i > 0; i--)
							expression_ptr[i] = expression_ptr[i - 1];
						*expression_ptr = rand() % 3 + 1;//����� ��������
						switch (*expression_ptr) {
						case 1:
							*expression_ptr = '+';
							break;
						case 2:
							*expression_ptr = '*';
							break;
						case 3:
							*expression_ptr = '-';
							break;
						}
						q_operations--;
					}
				}
			}
			file.write(expression, strlen(expression));
			file.write("\n\n", strlen("\n\n"));
		}
		file.close();
		delete[] expression;
	}

	void stack_output(Stack<int>* head, ofstream* file) {//����� � ����
		while (head != nullptr) {
			*file << head->info << " ";
			head = head->previous;
		}
	}

	void stack_output(Stack<int>* head) {//����� � �������
		while (head != nullptr) {
			cout << head->info << " ";
			head = head->previous;
		}
	}

	void stack_delete(Stack<int>* head) {
		Stack<int>* next;
		while (head != nullptr) {
			next = head->previous;
			delete head;
			head = next;
		}
	}

	void test_solution(ofstream& solution, char* Testfilename) {
		ifstream test(Testfilename);
		char* line = new char[100];
		char* line_ptr;

		while (test.getline(line, 100, '\n')) {
			//if (*line == '�')
			//	solution << line << endl;//������� ������ � ������� ��������
			if (*line != ' ' and *line != '\n' and *line != '\0') {
				line_ptr = line;
				//line_ptr += 3;//����� ��������� � ������ ���������
				for (int i = 0; i < strlen(line_ptr); i++) {//������� ������ ��������� � ���������� ����� ��� ����������
					if (line_ptr[i] == ' ')
						line_ptr[i] = ')';
					if ((unsigned char)line_ptr[i] >= 48 and (unsigned char)line_ptr[i] <= 57) {
						for (int j = 49; j > i; j--)
							line_ptr[j] = line_ptr[j - 1];
						line_ptr[i] = '(';
						do {
							i++;
						} while ((unsigned char)line_ptr[i] >= 48 and (unsigned char)line_ptr[i] <= 57);
						i--;
					}
				}
				if (*line_ptr != '(') {//������� �� �������� � �������� ��������
					char* new_line_ptr = new char[strlen(line_ptr)];
					for (int i = strlen(line_ptr) - 1; i >= 0; i--) {
						if (line_ptr[i] == '+' or line_ptr[i] == '*' or line_ptr[i] == '-' or line_ptr[i] == '/') {
							*new_line_ptr = line_ptr[i];
							new_line_ptr++;
						}
						if (line_ptr[i] == '(') {
							int j = i;
							while (line_ptr[j] != ')') {
								*new_line_ptr = line_ptr[j];
								new_line_ptr++;
								j++;
							}
							*new_line_ptr = line_ptr[j];
							new_line_ptr++;
						}
					}
					*new_line_ptr = '\0';
					for (int i = strlen(line_ptr) - 1; i >= 0; i--)
						new_line_ptr--;
					//line_ptr -= 3;
					line_ptr = new_line_ptr;
				}
				Stack<int>* head = nullptr;
				Stack<int>* new_s = nullptr;
				int summ = 0;
				unsigned char c;
				for (c = *line_ptr; c != '\0'; c = *(++line_ptr)) {//������� ��������� � ����� �������� � ����
					if (c >= 48 and c <= 57 or c == '(') {//��������
						new_s = new Stack<int>;
						new_s->previous = head;
						head = new_s;
						if (c != '(')
							head->info = c - 48;
						else {
							head->info = char_to_int(++line_ptr);
						}
						solution << "������� � ���� ����� " << head->info << endl;
						solution << "����: ";
						stack_output(head, &solution);
						solution << endl;
					}
					else {//���������
						int operand1 = head->info;
						new_s = head->previous;
						delete head;
						head = new_s;
						int operand2 = head->info;
						solution << "������� �� ����� ����� " << operand2 << " � " << operand1 << endl;
						switch (c) {
						case '*':
							summ = operand2 * operand1;
							solution << "���������� �������� " << summ << " ������� ��� ����� � ����" << endl;
							break;
						case '+':
							summ = operand2 + operand1;
							solution << "��������� �������� " << summ << " ������� ��� ����� � ����" << endl;
							break;
						case '-':
							summ = operand2 - operand1;
							solution << "������� �������� " << summ << " ������� ��� ����� � ����" << endl;
							break;
						case '/':
							summ = operand2 / operand1;
							solution << "� ���������� �������� " << summ << " ������� ��� ����� � ����" << endl;
							break;
						}
						head->info = summ;
						solution << "����: ";
						stack_output(head, &solution);
						solution << endl;
					}
				}
				delete head;
				solution << "�����: " << summ << endl << endl;
			}
		}

		test.close();
		delete[] line;
	}
}